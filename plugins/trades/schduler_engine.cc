//  Copyright (c) 2016-2017 The star Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "trades/schduler_engine.h"
#include "trades/trades_proto_buf.h"
#include "trades/operator_code.h"
#include "trades/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "logic/time.h"
#include "basic/template.h"

namespace trades_logic {
TradesManager* TradesEngine::schduler_mgr_ = NULL;
TradesEngine* TradesEngine::schduler_engine_ = NULL;

TradesManager::TradesManager() {
    trades_cache_ = new TradesCache();
    Init();
}

TradesManager::~TradesManager() {
    DeinitThreadrw(lock_);
    if (trades_cache_) {
        delete trades_cache_;
        trades_cache_ = NULL;
    }

}

void TradesManager::InitDB(trades_logic::TradesDB* trades_db) {
    trades_db_ = trades_db;
}

void TradesManager::Init() {
    InitThreadrw(&lock_);


    //
    /*
    trades_logic::TimeTask task;
    task.set_earliest_time("9:45");
    task.set_latest_time("15:55");
    time_t current = time(NULL);
    ProcessTimeTask(current, task);
    */
}

void TradesManager::InitData() {
    trades_db_->OnFetchPlatformStar(trades_cache_->trades_star_map_);
    LOG_MSG2("trades map %lld", trades_cache_->trades_star_map_.size());
    CreateTimeTask();
}

void TradesManager::TimeStarEvent() {
    base_logic::RLockGd lk(lock_);
    time_t current_time = time(NULL);
    //LOG_MSG2("trades_task_list_ size %d", trades_cache_->trades_task_list_.size());
    if (trades_cache_->trades_task_list_.size() <= 0)
        return;

    trades_cache_->trades_task_list_.sort(trades_logic::TimeTask::cmp);
    while (trades_cache_->trades_task_list_.size() > 0) {
        //达到任务时间，则执行任务
        trades_logic::TimeTask time_task = trades_cache_->trades_task_list_.front();
        // LOG_MSG2("current_time %d, time_task %d", current_time, time_task.task_start_time());
        if (current_time < time_task.task_start_time())
            break;
        trades_cache_->trades_task_list_.pop_front();
        //修改状态
        if(time_task.task_type() == TASK_START_TYPE) //开始交易时间任务
            AlterTradesStarState(time_task.symbol(), true);
        else if (time_task.task_type() == TASK_STOP_TYPE)//结束交易时间任务
            AlterTradesStarState(time_task.symbol(), false);
        LOG_MSG("NEXT STAT");
        ProcessTimeTask(current_time, time_task);
        trades_cache_->trades_task_list_.push_back(time_task);
    }

}

void TradesManager::GetStarTradingInfo(const int socket, const int64 session,
                                       const int32 reserved, const std::string& symbol,
                                       const int32 atype,const int32 start, const int32 count) {
    base_logic::RLockGd lk(lock_);
    bool r = false;
    int32 base_num = 20;
    int32 t_start = 0;
    int32 t_count = 0;
    TRADES_POSITION_LIST trades_position_list;
    if (atype == BUY_TYPE)
        r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,std::string,TRADES_POSITION_LIST>
            (trades_cache_->buy_trades_position_,symbol,trades_position_list);
    else if (atype == SELL_TYPE)
        r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,std::string,TRADES_POSITION_LIST>
            (trades_cache_->sell_trades_position_,symbol,trades_position_list);
    if(!r||trades_position_list.size()) {
        //无挂单记录
        send_error(socket, ERROR_TYPE, NO_HAVE_POSITIONS_DATA,session);
        return;
    }

    trades_position_list.sort(star_logic::TradesPosition::close_after);

}


void TradesManager::CreateTradesPosition(const int socket, const int64 session, const int32 reserved,
                                        const int64 uid, const std::string& symbol, const std::string& wid,
                                        const int64 buy_sell,const int64 amount, const double price) {
    //判读该标的是否可以交易
    base_logic::WLockGd lk(lock_);
    int64 u = GetTradesStarStatus(symbol);
    if (u == -100) {
        return ;
    }
    //判断属于哪个类型和明星标的
    star_logic::TradesPosition position;
    position.set_uid(uid);
    position.create_position_id();
    position.set_symbol(symbol);
    position.set_buy_sell(buy_sell);
    position.set_amount(amount);
    position.set_open_price(price);
    position.set_open_position_time(time(NULL));

    //匹配价格
    if (position.buy_sell() == BUY_TYPE) {
        MatchTrades(socket, session, reserved, trades_cache_->sell_trading_position_,position);
        SetTradesPosition(trades_cache_->buy_trades_position_,trades_cache_->buy_trading_position_,position);
    } else if (position.buy_sell() ==SELL_TYPE) {
        MatchTrades(socket, session, reserved, trades_cache_->buy_trading_position_, position);
        SetTradesPosition(trades_cache_->sell_trades_position_,trades_cache_->sell_trading_position_,position);
    }
    //创建成功
    trades_logic::net_reply::TradesPosition net_trades_position;
    net_trades_position.set_position_id(position.position_id());
    net_trades_position.set_id(position.uid());
    net_trades_position.set_symbol(position.symbol());
    net_trades_position.set_buy_sell(position.buy_sell());
    net_trades_position.set_amount(position.amount());
    net_trades_position.set_open_price(position.open_price());
    net_trades_position.set_position_time(position.open_position_time());
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_TRADES_OPEN_POSITION, 1, 0, session, 0);
    packet_control.body_ = net_trades_position.get();
    send_message(socket, &packet_control);
}

int32 TradesManager::MatchTrades(const int socket, const int64 session, const int32 reserved,
                                TRADEING_POSITION_MAP& trading_position,star_logic::TradesPosition& trades) {
    bool r = false;
    PRICE_POSITION_MAP price_postion_map;
    TRADES_POSITION_LIST price_postion_list;
    r = base::MapGet<TRADEING_POSITION_MAP,TRADEING_POSITION_MAP::iterator,std::string,PRICE_POSITION_MAP>
        (trading_position,trades.symbol(),price_postion_map);
    if (!r)
        return NO_SYMBOL;
    r = base::MapGet<PRICE_POSITION_MAP,PRICE_POSITION_MAP::iterator,double,TRADES_POSITION_LIST>
        (price_postion_map,trades.open_price(),price_postion_list);
    if (!r)
        return NO_MATCH_PRICE;

    //排序取出最早的用户 循环找到最早的用户且有效的用户
    //op_trades = price_postion_list.front();
    TRADES_POSITION_LIST::iterator it = price_postion_list.begin();
    for(; it != price_postion_list.end(); it++) {
        star_logic::TradesPosition op_trades = (*it);
        if(op_trades.handle() == WAIT_HANDLE ) {
            //通知对方
            MatchNotice(trades.symbol(), socket, session,trades.uid(),
                trades.position_id(),op_trades.uid(),
                op_trades.position_id());
            break;
        }
    }
    return MATCHING;
}

void TradesManager::SetTradesPosition(TRADES_POSITION_MAP& trades_position,
                                      TRADEING_POSITION_MAP& trading_position,
                                      star_logic::TradesPosition& trades) {
    //存储排序表
    bool r = false;
    TRADES_POSITION_LIST  trades_position_list;
    r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,std::string,TRADES_POSITION_LIST>
        (trades_position,trades.symbol(),trades_position_list);

    trades_position_list.push_back(trades);
    trades_position[trades.symbol()] = trades_position_list;
    //存储价格匹配表
    //
    PRICE_POSITION_MAP price_position_map;
    TRADES_POSITION_LIST r_trades_position_list;
    r = base::MapGet<TRADEING_POSITION_MAP,TRADEING_POSITION_MAP::iterator,std::string,PRICE_POSITION_MAP>
        (trading_position,trades.symbol(),price_position_map);
    r = base::MapGet<PRICE_POSITION_MAP,PRICE_POSITION_MAP::iterator,double,TRADES_POSITION_LIST>
        (price_position_map,trades.open_price(),r_trades_position_list);
    r_trades_position_list.push_back(trades);
    price_position_map[trades.open_price()] = r_trades_position_list;
    trading_position[trades.symbol()] = price_position_map;
}

void TradesManager::AlterTradesStarState(const std::string& symbol, const bool state) {
    bool r = false;
    trades_logic::TradesStar star;
    r = base::MapGet<TRADES_STAR_MAP,TRADES_STAR_MAP::iterator,std::string,trades_logic::TradesStar>
        (trades_cache_->trades_star_map_,symbol, star);

    if (!r)
        return;
    star.set_is_trading(state);
    //star.data_->is_trading_ = state;
    LOG_MSG2("state %s ,is_trading %s,  name %s",
             state==true?"true":"false",star.is_trading()==true?"true":"false",
             star.name().c_str());
}

int64 TradesManager::ChangeTodayUnixTime(const base::Time::Exploded& exploded, const std::string& fromate_time) {

    struct tm time_info = { 0 };
    std::string price_time = base::BasicUtil::StringUtil::Int64ToString(exploded.year)
                             + "-"+ base::BasicUtil::StringUtil::Int64ToString(exploded.month)
                             + "-" + base::BasicUtil::StringUtil::Int64ToString(exploded.day_of_month)
                             + "T" + fromate_time;

    char* format = "%Y-%m-%dT%H:%M";
    strptime(price_time.c_str(), format, &time_info);
    return mktime(&time_info);
}

//设置开始时间
void TradesManager::ProcessTimeTask(const time_t current_time, trades_logic::TimeTask& task) {
    //转化时间
    base::Time t_time = base::Time::FromTimeT(current_time);
    base::Time::Exploded exploded;
    t_time.LocalExplode(&exploded);
    /*
     LOG_MSG2("year:%d  month:%d  day:%d hour:%d, minute:%d, second:%d",
             exploded.year, exploded.month, exploded.day_of_month, exploded.hour,
                      exploded.minute,exploded.second);*/
    int64 earliest_unix_time = ChangeTodayUnixTime(exploded, task.earliest_time());
    int64 laestset_unix_time = ChangeTodayUnixTime(exploded, task.latest_time());
    if (current_time < earliest_unix_time) { //今天开始时间还未到
        task.set_task_start_time(earliest_unix_time);
        task.set_task_type(TASK_START_TYPE);
    } else if (current_time > earliest_unix_time and current_time < laestset_unix_time) {
        if (TASK_START_TYPE == task.task_type()) { //当前任务是开始拍卖时间任务，需要创建一个结束拍卖的时间任务
            task.set_task_start_time(current_time + task.interval_time());
            task.set_task_type(TASK_STOP_TYPE);
        } else if (TASK_STOP_TYPE == task.task_type()) {//当前是结束拍卖时间任务，需要创建一个开始的时间任务
            task.set_task_start_time(current_time + task.task_time());
            task.set_task_type(TASK_START_TYPE);
        }
    } else if (current_time > laestset_unix_time) {//今天的任务时间范围已经执行完毕，需要调整到明天
        task.set_task_start_time(earliest_unix_time + 60 * 60 * 24);
        task.set_task_start_time(TASK_START_TYPE);
    }

    base::Time p_time = base::Time::FromTimeT(task.task_start_time());
    base::Time::Exploded p_exploed;
    p_time.LocalExplode(&p_exploed);
    LOG_MSG2("symbol %s  start_time: %d-%d-%d %d:%d:%d, type: %s--:%s~%s now:%d-%d-%d %d:%d:%d",
             task.symbol().c_str(), p_exploed.year,p_exploed.month,p_exploed.day_of_month,
             p_exploed.hour,p_exploed.minute,p_exploed.second,task.task_type()==TASK_STOP_TYPE?"TASK_STOP_TYPE":"TASK_START_TYPE",
             task.earliest_time().c_str(),task.latest_time().c_str(),
             exploded.year, exploded.month, exploded.day_of_month, exploded.hour,
             exploded.minute,exploded.second);
}

void TradesManager::CreateTimeTask() {
    base_logic::WLockGd lk(lock_);
    TRADES_STAR_MAP::iterator it = trades_cache_->trades_star_map_.begin();
    for (; it != trades_cache_->trades_star_map_.end(); it++) {
        trades_logic::TradesStar star = it->second;
        trades_logic::TimeTask time_task;
        time_task.set_symbol(star.symbol());
        time_task.set_earliest_time(star.start_time());
        time_task.set_latest_time(star.end_time());
        time_task.set_task_time(star.trades_time());
        time_task.set_interval_time(star.interval_time());
        time_task.set_task_type(TASK_STOP_TYPE);
        ProcessTimeTask(time(NULL), time_task);
        trades_cache_->trades_task_list_.push_back(time_task);
        trades_cache_->trades_task_map_[time_task.symbol()] = time_task;
    }
}

int64 TradesManager::GetTradesStarStatus(const std::string& symbol) {
    bool r = false;
    trades_logic::TradesStar star;
    r = base::MapGet<TRADES_STAR_MAP,TRADES_STAR_MAP::iterator,std::string,trades_logic::TradesStar>
        (trades_cache_->trades_star_map_,symbol,star);

    if (!r||star.is_trading()==false) //不存在或者未到交易时间
        return NO_STAR_TRADES;//不能交易
    trades_logic::TimeTask time_task;

    r = base::MapGet<TIME_TASK_MAP,TIME_TASK_MAP::iterator,std::string,trades_logic::TimeTask>(trades_cache_->trades_task_map_,symbol,time_task);
    if (!r)
        return NO_STAR_TRADES;

    return time_task.task_start_time() - time(NULL);
}


void TradesManager::MatchNotice(const std::string& symbol, const int socket, 
                                const int64 session, const int64 uid, 
                                const int64 u_position_id,
                                const int64 t_position_id,
                                const int64 tid) {
   //通知当时人
   SendMatch(socket,session,symbol,uid,u_position_id,tid,t_position_id);
   //通知对方
   int t_socket;
   SendMatch(t_socket,0,symbol,tid,t_position_id,uid,u_position_id);
}

void TradesManager::SendMatch(const int socket, const int64 session, const std::string& symbol,
                                const int64 uid, const int64 u_position_id,const int64 tid, 
                                const int64 t_position_id) {
    net_reply::MatchingNotice net_match_notice;
    net_match_notice.set_u_position_id(u_position_id);
    net_match_notice.set_t_position_id(t_position_id);
    net_match_notice.set_uid(uid);
    net_match_notice.set_tid(tid);
    net_match_notice.set_symbol(symbol);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_MATCH_NOTICE, 1, 0, session, 0);
    packet_control.body_ = net_match_notice.get();
    send_message(socket, &packet_control);
}
}
