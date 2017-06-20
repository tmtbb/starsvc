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

void TradesManager::InitKafka(trades_logic::TradesKafka* trades_kafka) {
    trades_kafka_ = trades_kafka;
}

void TradesManager::Init() {
    InitThreadrw(&lock_);
}

void TradesManager::InitData() {
    trades_db_->OnFetchPlatformStar(trades_cache_->trades_star_map_);
    LOG_MSG2("trades map %lld", trades_cache_->trades_star_map_.size());
    CreateTimeTask();
}

void TradesManager::InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine) {
   schduler_engine_ = schduler_engine;
}

void TradesManager::TimeStarEvent() {
    base_logic::RLockGd lk(lock_);
    time_t current_time = time(NULL);
    LOG_MSG2("trades_task_list_ size %d", trades_cache_->trades_task_list_.size());
    if (trades_cache_->trades_task_list_.size() <= 0)
        return;

    trades_cache_->trades_task_list_.sort(trades_logic::TimeTask::cmp);
    while (trades_cache_->trades_task_list_.size() > 0) {
        //达到任务时间，则执行任务
        trades_logic::TimeTask time_task = trades_cache_->trades_task_list_.front();
        LOG_MSG2("current_time %d, time_task %d", current_time, time_task.task_start_time());
        if (current_time < time_task.task_start_time())
            break;
        trades_cache_->trades_task_list_.pop_front();
        //修改状态
        if(time_task.task_type() == TASK_START_TYPE) //开始交易时间任务
            AlterTradesStarState(time_task.symbol(), true);
        else if (time_task.task_type() == TASK_STOP_TYPE){//结束交易时间任务
            AlterTradesStarState(time_task.symbol(), false);
            ClearSymbolTrades(time_task.symbol());
        }
        LOG_MSG("NEXT STAT");
        ProcessTimeTask(current_time, time_task);
        trades_cache_->trades_task_list_.push_back(time_task);
    }

}

//取消订单
void TradesManager::CancelOrder(const int socket, const int64 session, const int32 reserved,
                                const int64 uid, const int64 order_id) {

    
    star_logic::TradesOrder  trades_order;
    bool r = false;
    {
        base_logic::WLockGd lk(lock_);
        r = base::MapGet<TRADES_ORDER_MAP,TRADES_ORDER_MAP::iterator,int64,star_logic::TradesOrder>
                (trades_cache_->all_trades_order_,order_id,trades_order);
        if (!r){
            send_error(socket,ERROR_TYPE, NO_HAVE_ORDER,session);
            return;
        }
        if (trades_order.handle_type() != MATCHES_ORDER){//不存在，且已经不为匹配状态
            send_error(socket, ERROR_TYPE, NO_CANCEL_ERROR,session);
            return;
        }
        //验证是否双方发起
        if(trades_order.buy_uid() != uid && trades_order.sell_uid() != uid 
            && trades_order.order_id() != order_id){
            send_error(socket, ERROR_TYPE, NO_LOCK_USER_ORDER_VALID, session);
            return;
        }
        trades_order.set_handle_type(CANCEL_ORDER);
    }
    
    trades_db_->OnUpdateOrderState(order_id, uid, CANCEL_ORDER,0);
    trades_kafka_->SetTradesOrder(trades_order);
    //通知双方
    SendOrderResult(socket,session,reserved,trades_order.buy_uid(),
            trades_order.sell_uid(),CANCEL_ORDER,order_id);
}

//发送明星的交易状态
void TradesManager::TradesSymbolInfo(const int socket, const int64 session, const int32 reserved,
    const std::string& symbol) {
    bool r = false;
    trades_logic::TradesStar trades_star;
    trades_logic::TimeTask time_task;
    {
        base_logic::RLockGd lk(lock_);
        r = base::MapGet<TRADES_STAR_MAP, TRADES_STAR_MAP::iterator, std::string, trades_logic::TradesStar>
            (trades_cache_->trades_star_map_, symbol, trades_star);
        r = base::MapGet<TIME_TASK_MAP,TIME_TASK_MAP::iterator, std::string, trades_logic::TimeTask>
           (trades_cache_->trades_task_map_,symbol, time_task);
    }

    if(!r) {
        send_error(socket, ERROR_TYPE, NO_HAVE_TRADES_STAR, session);
    }
    trades_logic::net_reply::TradesSymbol trades_symbol;
    trades_symbol.set_symbol(symbol);
    trades_symbol.set_remaining_time(time_task.task_start_time() - time(NULL));
    trades_symbol.set_status(trades_star.is_trading());

    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_TRADES_SYMBOL_STATUS, 1, 0, session, 0);
    packet_control.body_ = trades_symbol.get();
    send_message(socket, &packet_control);
}


void TradesManager::CreateTradesPosition(const int socket, const int64 session, const int32 reserved,
        const int64 uid, const std::string& symbol, const std::string& wid,
        const int64 buy_sell,const int64 amount, const double price) {
    //判读该标的是否可以交易
    base_logic::WLockGd lk(lock_);
    int64 u = GetTradesStarStatus(symbol);
    if (u == -100) {
        send_error(socket, ERROR_TYPE, NO_HAVE_TRADES_TIME,session);
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
        SetTradesPosition(trades_cache_->buy_trades_position_,trades_cache_->buy_trading_position_,position);
        MatchTrades(socket, session, reserved, trades_cache_->sell_trading_position_,position);
    } else if (position.buy_sell() == SELL_TYPE) {
        SetTradesPosition(trades_cache_->sell_trades_position_,trades_cache_->sell_trading_position_,position);
        MatchTrades(socket, session, reserved, trades_cache_->buy_trading_position_, position);
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

void TradesManager::ConfirmOrder(const int socket, const int64 session, const int32 reserved,const int64 uid,const int64 order_id, const int64 position_id) {
    //查找订单是否存在，切状态是否正确
    bool r = false;
    int32 uid_type = 0;
    base_logic::WLockGd lk(lock_);
    star_logic::TradesOrder  trades_order;
    r = base::MapGet<TRADES_ORDER_MAP,TRADES_ORDER_MAP::iterator,int64,star_logic::TradesOrder>
                (trades_cache_->all_trades_order_,order_id,trades_order);

    if(!r || trades_order.handle_type() == NO_ORDER||
        trades_order.handle_type() == COMPLETE_HANDLE){
        send_error(socket, ERROR_TYPE,NO_HAVE_ORDER,session);
        return;
    }

    if (trades_order.handle_type() == CANCEL_ORDER){//不存在，且已经不为匹配状态
        send_error(socket, ERROR_TYPE, NO_CANCEL_ERROR, session);
        return;
    }
    //更新数据库
    if (trades_order.buy_uid() == uid)
        uid_type = BUY_TYPE;
    else if (trades_order.sell_uid() == uid)
        uid_type = SELL_TYPE;
    r = trades_db_->OnUpdateOrderState(trades_order.order_id(),uid,
                                       CONFIRM_ORDER,uid_type);
    if (!r){
        send_error(socket, ERROR_TYPE, NO_UPDATE_DB_ERROR, session);
        return;
    }

    if (trades_order.buy_position_id() == position_id && trades_order.buy_uid()){
        trades_order.set_buy_handle_type(CONFIRM_ORDER);
        trades_order.set_handle_type(CONFIRM_ORDER);
        uid_type = BUY_TYPE;
    }else if (trades_order.sell_position_id() == position_id 
            && trades_order.sell_uid()){
        trades_order.set_handle_type(CONFIRM_ORDER);
        trades_order.set_sell_handle_type(CONFIRM_ORDER);
        uid_type = SELL_TYPE;
    }else{
     //异常数据
     send_error(socket, ERROR_TYPE, NO_EXCEPTION_DATA, session);
     return;
    }

    trades_kafka_->SetTradesOrder(trades_order);
    //通知确认
    SendConfirmOrder(socket,session,reserved,uid,trades_order.order_id(),
                    trades_order.handle_type());
    //检测是否双方确认
    if(trades_order.is_complete()){
        //双方确认，开始扣费
        int32 result = 0;
        result = trades_db_->OnConfirmOrder(trades_order.order_id(),
                                            trades_order.buy_uid(),
                                            trades_order.sell_uid());
        SendOrderResult(socket, session, reserved, trades_order.buy_uid(),
                trades_order.sell_uid(), result, trades_order.order_id());
        if (result = 0)
            trades_order.set_handle_type(COMPLETE_ORDER);
        trades_kafka_->SetTradesOrder(trades_order);
    }
}

void TradesManager::SendConfirmOrder(const int socket, const int64 session,
                                const int32 reserved, const int64 uid,
                                const int64 order_id, const int32 status) {
    
    trades_logic::net_reply::OrderConfirm order_confirm;
    order_confirm.set_order_id(order_id);
    order_confirm.set_uid(uid);
    order_confirm.set_status(status);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_CONFIRM_ORDER, 1, 0, session, 0);
    packet_control.body_ = order_confirm.get();
    send_message(socket, &packet_control);
    //send_message(socket, &packet_control);
}

void TradesManager::SendOrderResult(const int socket, const int64 session, const int32 reserved,
        const int64 buy_uid, const int64 sell_uid, const int32 result, const int64 order_id) {
    //int t_socket = 0;
    trades_logic::net_reply::OrderResult order_result;
    order_result.set_order_id(order_id);
    order_result.set_result(result);
    SendNoiceMessage(buy_uid,S_ORDER_RESULT,session,order_result.get());
    SendNoiceMessage(sell_uid,S_ORDER_RESULT,session,order_result.get());
    /*struct PacketControl packet_control;
    SendNoiceMessage()
    struct PacketControl packet_control;
    send_message(socket, &packet_control);
    send_message(t_socket, &packet_control);*/
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
        if(op_trades.handle() == POSITION_HANDLE ) {
            //创建订单
            op_trades.set_handle(MATCHES_HANDLE);
            trades.set_handle(MATCHES_HANDLE);
            star_logic::TradesOrder  trades_order;
            if (op_trades.buy_sell() == BUY_TYPE)
                SetTradesOrder(op_trades,trades, trades_order);
            else
                SetTradesOrder(trades,op_trades,trades_order);
            //通知双对方
            MatchNotice(socket, session,reserved,trades_order);
            //数据库创建订单
            trades_db_->OnCreateTradesOrder(trades_order);
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

    trades.set_handle(POSITION_HANDLE);
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
    trades_kafka_->SetTradesPosition(trades);
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


void TradesManager::MatchNotice(const int socket, const int64 session, const int32 reserved, star_logic::TradesOrder& trades_order) {
    int t_socket;

    //獲取socket
    net_reply::MatchingNotice  match_notice;
    match_notice.set_order_id(trades_order.order_id());
    match_notice.set_buy_uid(trades_order.buy_uid());
    match_notice.set_sell_uid(trades_order.sell_uid());
    match_notice.set_open_position_time(trades_order.open_position_time());
    match_notice.set_open_price(trades_order.open_price());
    match_notice.set_symbol(trades_order.symbol());
    match_notice.set_amount(trades_order.amount());
    SendNoiceMessage(trades_order.buy_uid(),S_MATCH_NOTICE,session,match_notice.get());
    SendNoiceMessage(trades_order.sell_uid(), S_MATCH_NOTICE, session, match_notice.get());
}

void TradesManager::SetTradesOrder(star_logic::TradesPosition& buy_position,
                                   star_logic::TradesPosition& sell_position,
                                   star_logic::TradesOrder& trades_order) {
    bool r = false;
    trades_order.create_order_id();
    trades_order.set_buy_position_id(buy_position.position_id());
    trades_order.set_sell_position_id(sell_position.position_id());
    trades_order.set_buy_uid(buy_position.uid());
    trades_order.set_sell_uid(sell_position.uid());
    int64 amount = buy_position.amount() > sell_position.amount() ?
                   sell_position.amount():buy_position.amount();
    buy_position.set_r_amount(amount);
    sell_position.set_r_amount(amount);
    trades_order.set_amount(amount);
    trades_order.set_symbol(buy_position.symbol());
    trades_order.set_open_position_time(time(NULL));
    trades_order.set_close_position_time(0);
    trades_order.set_open_price(sell_position.open_price());
    trades_order.set_match_type();

    trades_cache_->all_trades_order_[trades_order.order_id()] = trades_order;

    TRADES_ORDER_LIST trades_order_list;
    r = base::MapGet<KEY_ORDER_MAP,KEY_ORDER_MAP::iterator,std::string,TRADES_ORDER_LIST>
        (trades_cache_->symbol_trades_order_,sell_position.symbol(),trades_order_list);
    trades_order_list.push_back(trades_order);
    trades_cache_->symbol_trades_order_[sell_position.symbol()] = trades_order_list;

    TRADES_ORDER_LIST buy_trades_order_list;
    r = base::MapGet<USER_ORDER_MAP,USER_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (trades_cache_->user_trades_order_,buy_position.uid(),buy_trades_order_list);
    buy_trades_order_list.push_back(trades_order);
    trades_cache_->user_trades_order_[buy_position.uid()] = buy_trades_order_list;

    TRADES_ORDER_LIST sell_trades_order_list;
    r = base::MapGet<USER_ORDER_MAP,USER_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (trades_cache_->user_trades_order_,sell_position.uid(),sell_trades_order_list);
    sell_trades_order_list.push_back(trades_order);
    trades_cache_->user_trades_order_[sell_position.uid()] = sell_trades_order_list;

    trades_kafka_->SetTradesPosition(buy_position);
    trades_kafka_->SetTradesPosition(sell_position);
    trades_kafka_->SetTradesOrder(trades_order);
}

void TradesManager::ClearSymbolTrades(const std::string& symbol) {
  //全部重置為無效
    //base_logic::WLockGd lk(lock_);
    ClearTradesPosition(trades_cache_->buy_trades_position_,symbol);
    ClearTradesPosition(trades_cache_->sell_trades_position_,symbol);
    ClearTradesOrder(trades_cache_->symbol_trades_order_,symbol);
}

void TradesManager::ClearTradesPosition(TRADES_POSITION_MAP& trades_position_map,
                            const std::string& symbol) {

    TRADES_POSITION_LIST trades_position_list;
    bool r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,
            std::string,TRADES_POSITION_LIST>(trades_position_map,symbol,
                                              trades_position_list);
   /* TRADES_POSITION_LIST::iterator it = buy_trades_position_list.begin();
    for(; it != buy_trades_position_list.end(); it++) {
        star_logic::TradesPosition position = (*it);
        position.set_handle(CANCEL_POSITION);
    }*/

    while(trades_position_list.size() > 0) {
        star_logic::TradesPosition position = trades_position_list.front();
        position.set_handle(CANCEL_POSITION);
        trades_position_list.pop_front();
        trades_kafka_->SetTradesPosition(position);
    }
}

void TradesManager::ClearTradesOrder(KEY_ORDER_MAP& symbol_trades_order,
                                     const std::string& symbol) {
    TRADES_ORDER_LIST trades_order_list;
    bool r = base::MapGet<KEY_ORDER_MAP,KEY_ORDER_MAP::iterator,std::string,
            TRADES_ORDER_LIST>(symbol_trades_order,symbol,trades_order_list);

    while(trades_order_list.size() > 0) {
        star_logic::TradesOrder order = trades_order_list.front();
        trades_order_list.pop_front();
        order.set_handle_type(CANCEL_ORDER);
        order.set_buy_handle_type(CANCEL_ORDER);
        order.set_sell_handle_type(CANCEL_ORDER);
    }
}

void TradesManager::SendNoiceMessage(const int64 uid, const int32 operator_code, const int64 session,
                                     base_logic::DictionaryValue* message) {
    star_logic::UserInfo user;
    schduler_engine_->GetUserInfoSchduler(uid, &user);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, operator_code, 1, 0, session, 0);
    packet_control.body_ = message;
    send_message(user.socket_fd(), &packet_control);

}

}
