//  Copyright (c) 2016-2017 The STAR Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "record/schduler_engine.h"
#include "basic/template.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "record/errno.h"
#include "record/operator_code.h"
#include "record/record_proto_buf.h"

namespace record_logic {

RecordManager* RecordEngine::schduler_mgr_ = NULL;
RecordEngine* RecordEngine::schduler_engine_ = NULL;

RecordManager::RecordManager() {
    record_cache_ = new RecordCache();
    Init();
}

RecordManager::~RecordManager() {
    DeinitThreadrw(lock_);

}

void RecordManager::Init() {
    InitThreadrw(&lock_);

    manager_schduler::SchdulerEngine* (*schduler_engine)(void);
    std::string schduler_library = "./data_share.so";
    std::string schduler_func = "GetManagerSchdulerEngine";
    schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                      logic::SomeUtils::GetLibraryFunction(
                          schduler_library, schduler_func);
    manager_schduler_engine_ = (*schduler_engine)();
    if (manager_schduler_engine_ == NULL)
        assert(0);
}

void RecordManager::InitDB(record_logic::RecordDB* record_db) {
    record_db_ = record_db;
}

void RecordManager::InitKafka(record_logic::RecordKafka* record_kafka) {
    record_kafka_ = record_kafka;
}


void RecordManager::InitData() {
    InitDataHisOrder();
    InitUserInfo();
    InitDataHisPosition();
//    InitStarInfo();
}


void RecordManager::InitStarInfo() {
    record_db_->OnGetStarInfo(record_cache_->star_info_map_);

    LOG_DEBUG2("star_info %d",record_cache_->star_info_map_.size());
}


void RecordManager::InitDataHisOrder() {
    std::map<int64,star_logic::TradesOrder> trades_order_map;
    record_db_->OnGetAllOrderInfo(trades_order_map);

    LOG_DEBUG2("trades_order %d",trades_order_map.size());
    std::map<int64, star_logic::TradesOrder>::iterator it = trades_order_map.begin();
    for(;it != trades_order_map.end(); it++) {
        star_logic::TradesOrder order = it->second;
        SetTradesOrder(order);
    }
}

void RecordManager::InitDataHisPosition() {
    std::map<int64, star_logic::TradesPosition> trades_position_map;
    record_db_->OnGetAllPositionInfo(trades_position_map);
    LOG_DEBUG2("trades_order %d",trades_position_map.size());

    std::map<int64, star_logic::TradesPosition>::iterator it = trades_position_map.begin();
    for(;it != trades_position_map.end(); it++) {
        star_logic::TradesPosition position = it->second;
        SetTradesPosition(position,true);
    }
}

void RecordManager::InitUserInfo() {
    record_db_->OnGetAllUserInfo(record_cache_->user_info_map_);
}

void RecordManager::SendOrder(const int socket, const int64 session, const int32 reserved,
        const int operator_code, const int32 start, const int32 count,const int64 uid,
        std::list<star_logic::TradesOrder>& trades_order_list) {
    int32 base_num = 50;
    base_num = base_num < count ? base_num : count;
    int32 t_start = 0;
    int32 t_count = 0;
    trades_order_list.sort(star_logic::TradesOrder::open_after);
    record_logic::net_reply::AllOrder net_allorder;
    while (trades_order_list.size() > 0 && t_count < count) {
        star_logic::TradesOrder trades_order = trades_order_list.front();
        trades_order_list.pop_front();
        t_start++;
        if (t_start < start)
            continue;
        net_reply::TradesUnit* net_trades_unit =
            new net_reply::TradesUnit;
        net_trades_unit->set_order_id(trades_order.order_id());
        net_trades_unit->set_symbol(trades_order.symbol());
        net_trades_unit->set_amount(trades_order.amount());
        net_trades_unit->set_open_price(trades_order.open_price());
        net_trades_unit->set_open_time(trades_order.open_position_time());
        net_trades_unit->set_open_charge(trades_order.open_charge());
        net_trades_unit->set_handle(trades_order.handle_type());
        net_trades_unit->set_close_time(trades_order.close_position_time());
        net_trades_unit->set_gross_profit(trades_order.gross_profit());
        net_trades_unit->set_buy_uid(trades_order.buy_uid());
        net_trades_unit->set_sell_uid(trades_order.sell_uid());
        net_trades_unit->set_buy_handle(trades_order.buy_handle_type());
        net_trades_unit->set_sell_handle(trades_order.sell_handle_type());
        if (trades_order.buy_uid()==uid)
            net_trades_unit->set_position_id(trades_order.buy_position_id());
        else 
            net_trades_unit->set_position_id(trades_order.sell_position_id());
        net_allorder.set_unit(net_trades_unit->get());
        t_count++;
        if (net_allorder.Size() % base_num == 0
                && net_allorder.Size() != 0) {
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, operator_code, HISTORY_TYPE, 0, session, 0);
            packet_control.body_ = net_allorder.get();
            send_message(socket, &packet_control);
            net_allorder.Reset();
        }
    }

    if (net_allorder.Size() > 0) {
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, operator_code, HISTORY_TYPE, 0, session, 0);
        packet_control.body_ = net_allorder.get();
        send_message(socket, &packet_control);
    }
}


void RecordManager::SendPosition(const int socket, const int64 session, const int32 reserved,
        const int operator_code, const int32 start, const int32 count,
        std::list<star_logic::TradesPosition>& trades_position_list) {
    int32 base_num = 50;
    base_num = base_num < count ? base_num : count;
    int32 t_start = 0;
    int32 t_count = 0;
    trades_position_list.sort(star_logic::TradesPosition::open_after);
    record_logic::net_reply::AllPosition net_allposition;
    while (trades_position_list.size() > 0 && t_count < count) {
        star_logic::TradesPosition trades_position = trades_position_list.front();
        trades_position_list.pop_front();
        t_start++;
        if (t_start < start)
            continue;
        net_reply::PositionUnit* net_position_unit =
            new net_reply::PositionUnit;
        net_position_unit->set_amount(trades_position.amount());
        net_position_unit->set_buy_sell(trades_position.buy_sell());
        net_position_unit->set_id(trades_position.uid());
        net_position_unit->set_open_charge(trades_position.open_charge());
        net_position_unit->set_open_price(trades_position.open_price());
        net_position_unit->set_position_time(trades_position.open_position_time());
        net_position_unit->set_symbol(trades_position.symbol());
        net_position_unit->set_position_id(trades_position.position_id());
        //net_position_unit->set_result(trades_position.result());
        net_position_unit->set_r_amount(trades_position.r_amount());
        net_position_unit->set_handle(trades_position.handle());
        net_allposition.set_unit(net_position_unit->get());
        t_count++;
        if (net_allposition.Size() % base_num == 0
                && net_allposition.Size() != 0) {
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, operator_code, HISTORY_TYPE, 0, session, 0);
            packet_control.body_ = net_allposition.get();
            send_message(socket, &packet_control);
            net_allposition.Reset();
        }
    }

    if (net_allposition.Size() > 0) {
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, operator_code, HISTORY_TYPE, 0, session, 0);
        packet_control.body_ = net_allposition.get();
        send_message(socket, &packet_control);
    }
}

void RecordManager::SendPositionCount(const int socket, const int64 session, const int32 reserved,
                        const std::string& symbol) {
    int32 buy_count = 0;
    int32 sell_count = 0;
    int64 buy_time = 0;
    int64 sell_time = 0;
    star_logic::Auction auction;
    bool r = base::MapGet<AUCTION_INFO_MAP,AUCTION_INFO_MAP::iterator,std::string,star_logic::Auction>
                (record_cache_->symbol_auction_map_, symbol, auction);
    /*GetSymbolPositionCount(record_cache_->symbol_sell_trades_position_,symbol,sell_count);
    GetSymbolPositionCount(record_cache_->symbol_buy_trades_position_, symbol, buy_count);
    base::MapGet<SYMBOL_POSITION_TIME_MAP,SYMBOL_POSITION_TIME_MAP::iterator,std::string,int64>
                            (record_cache_->symbol_sell_trades_time_position_,symbol,sell_time);
    
    base::MapGet<SYMBOL_POSITION_TIME_MAP,SYMBOL_POSITION_TIME_MAP::iterator,std::string,int64>
                            (record_cache_->symbol_buy_trades_time_position_,symbol,buy_time);*/
    
    if (!r)
        send_error(socket, ERROR_TYPE, NO_HAVE_POSITION_DATA, session);
    net_reply::PositionCount net_position_count;
    net_position_count.set_buy_count(auction.buy_count());
    net_position_count.set_sell_count(auction.sell_count());
    net_position_count.set_buy_time(auction.buy_time());
    net_position_count.set_sell_time(auction.sell_time());
    net_position_count.set_symbol(symbol);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_POSITION_COUNT, HISTORY_TYPE, 0, session, 0);
    packet_control.body_ = net_position_count.get();
    send_message(socket, &packet_control);
}

void RecordManager::SendFansOrder(const int socket, const int64 session, const int32 reserved,
        const int32 start, const int32 count,
        std::list<star_logic::TradesOrder>& trades_order_list) {
    int32 base_num = 50;
    base_num = base_num < count ? base_num : count;
    int32 t_start = 0;
    int32 t_count = 0;
    trades_order_list.sort(star_logic::TradesOrder::open_price_after);
    record_logic::net_reply::AllOrder net_allorder;
    while (trades_order_list.size() > 0 && t_count < count) {
        star_logic::TradesOrder trades_order = trades_order_list.front();
        trades_order_list.pop_front();
        t_start++;
        if (t_start < start)
            continue;
        net_reply::TradesUnit* net_trades_unit =
            new net_reply::TradesUnit;

        net_reply::UserInfoUnit* net_buy_user_unit = 
            new net_reply::UserInfoUnit;
        
        net_reply::UserInfoUnit* net_sell_user_unit = 
            new net_reply::UserInfoUnit;

        net_reply::UserOrder* net_user_order = 
            new net_reply::UserOrder;
        
        net_trades_unit->set_order_id(trades_order.order_id());
        net_trades_unit->set_symbol(trades_order.symbol());
        net_trades_unit->set_amount(trades_order.amount());
        net_trades_unit->set_open_time(trades_order.open_position_time());
        net_trades_unit->set_close_time(trades_order.open_position_time());
        net_trades_unit->set_open_charge(trades_order.open_charge());
        net_trades_unit->set_handle(trades_order.handle_type());
        net_trades_unit->set_close_time(trades_order.close_position_time());
        net_trades_unit->set_gross_profit(trades_order.gross_profit());
        net_trades_unit->set_open_price(trades_order.open_price());
        net_trades_unit->set_buy_uid(trades_order.buy_uid());
        net_trades_unit->set_sell_uid(trades_order.sell_uid());
        
        star_logic::UserInfo sell_user_info;
        {
            base_logic::RLockGd lk(lock_);
            if(!base::MapGet<USER_INFO_MAP,USER_INFO_MAP::iterator,int64>
                (record_cache_->user_info_map_,trades_order.sell_uid(),sell_user_info)){
                if(net_trades_unit){
                    delete net_trades_unit;
                    net_trades_unit = NULL;
                }
                if(net_buy_user_unit){
                    delete net_buy_user_unit;
                    net_buy_user_unit = NULL;
                }
                if(net_sell_user_unit){
                    delete net_sell_user_unit;
                    net_sell_user_unit = NULL;
                }
                if(net_user_order){
                    delete net_user_order;
                    net_user_order = NULL;
                }
                continue;
            }
        }

        star_logic::UserInfo buy_user_info;

        {
            base_logic::RLockGd lk(lock_);
            base::MapGet<USER_INFO_MAP,USER_INFO_MAP::iterator,int64>
                (record_cache_->user_info_map_,trades_order.buy_uid(),buy_user_info);
        }
        net_buy_user_unit->set_uid(buy_user_info.uid());
        net_buy_user_unit->set_gender(buy_user_info.gender());
        net_buy_user_unit->set_nickname(buy_user_info.nickname());
        net_buy_user_unit->set_head_url(buy_user_info.head_url());



        net_sell_user_unit->set_uid(sell_user_info.uid());
        net_sell_user_unit->set_gender(sell_user_info.gender());
        net_sell_user_unit->set_nickname(sell_user_info.nickname());
        net_sell_user_unit->set_head_url(sell_user_info.head_url());
        
        
        net_user_order->set_buy_user(net_buy_user_unit->get());
        net_user_order->set_sell_user(net_sell_user_unit->get());
        net_user_order->set_trades(net_trades_unit->get());
        
        net_allorder.set_unit(net_user_order->get());
        t_count++;
        if (net_allorder.Size() % base_num == 0
                && net_allorder.Size() != 0) {
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, S_FANS_ORDER, HISTORY_TYPE, 0, session, 0);
            packet_control.body_ = net_allorder.get();
            send_message(socket, &packet_control);
            net_allorder.Reset();
        }
    }

    if (net_allorder.Size() > 0) {
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, S_FANS_ORDER, HISTORY_TYPE, 0, session, 0);
        packet_control.body_ = net_allorder.get();
        send_message(socket, &packet_control);
    }
}
void RecordManager::SendFansPosition(const int socket, const int64 session, const int32 reserved,
                                    const int32 start, const int32 count,
                                    std::list<star_logic::TradesPosition>& trades_position_list) {
    int32 base_num = 50;
    base_num = base_num < count ? base_num : count;
    int32 t_start = 0;
    int32 t_count = 0;
    trades_position_list.sort(star_logic::TradesPosition::price_after);
    record_logic::net_reply::AllPosition net_allposition;
    while (trades_position_list.size() > 0 && t_count < count) {
        star_logic::TradesPosition trades_position = trades_position_list.front();
        trades_position_list.pop_front();
        t_start++;
        if (t_start < start)
            continue;
        net_reply::UserInfoUnit* net_user_unit = 
            new net_reply::UserInfoUnit;
        net_reply::PositionUnit* net_position_unit =
            new net_reply::PositionUnit;
        net_reply::UserTrades* net_user_trades = 
            new net_reply::UserTrades;
        net_position_unit->set_amount(trades_position.amount());
        net_position_unit->set_buy_sell(trades_position.buy_sell());
        net_position_unit->set_id(trades_position.uid());
        net_position_unit->set_open_charge(trades_position.open_charge());
        net_position_unit->set_open_price(trades_position.open_price());
        net_position_unit->set_position_time(trades_position.open_position_time());
        net_position_unit->set_symbol(trades_position.symbol());
        net_position_unit->set_position_id(trades_position.position_id());
        net_position_unit->set_handle(trades_position.handle());
        net_position_unit->set_r_amount(trades_position.r_amount());
        star_logic::UserInfo user_info;
        {
            base_logic::RLockGd lk(lock_);
            if(!base::MapGet<USER_INFO_MAP,USER_INFO_MAP::iterator,int64>
                    (record_cache_->user_info_map_,trades_position.uid(),user_info)){
                if(net_user_unit){
                    delete net_user_unit;
                    net_user_unit = NULL;
                }
                if(net_position_unit){
                    delete net_position_unit;
                    net_position_unit = NULL;
                }
                if(net_user_trades){
                    delete net_user_trades;
                    net_user_trades = NULL;
                }
                continue;
            }
        }
        net_user_unit->set_uid(user_info.uid());
        net_user_unit->set_gender(user_info.gender());
        net_user_unit->set_nickname(user_info.nickname());
        net_user_unit->set_head_url(user_info.head_url());

        net_user_trades->set_user(net_user_unit->get());
        net_user_trades->set_trades(net_position_unit->get());
        net_allposition.set_unit(net_user_trades->get());
        t_count++;
        if (net_allposition.Size() % base_num == 0
                && net_allposition.Size() != 0) {
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, S_FANS_POSITION, HISTORY_TYPE, 0, session, 0);
            packet_control.body_ = net_allposition.get();
            send_message(socket, &packet_control);
            net_allposition.Reset();
        }
    }

    if (net_allposition.Size() > 0) {
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, S_FANS_POSITION, HISTORY_TYPE, 0, session, 0);
        packet_control.body_ = net_allposition.get();
        send_message(socket, &packet_control);
    }
}

void RecordManager::FansOrder(const int socket, const int64 session, const int32 reserved,
                              const std::string& symbol, const int32 start, const int32 count) {
    std::list<star_logic::TradesOrder> trades_order_list;
    GetSymbolOrder(symbol, start, count, trades_order_list);
    if(trades_order_list.size() <= 0){
        send_error(socket, ERROR_TYPE, NO_HAVE_ORDER_DATA,session);
        return;
    }
    SendFansOrder(socket, session, reserved, start, count, trades_order_list);
}


void RecordManager::FansPosition(const int socket, const int64 session, const int32 reserved,
                                const std::string& symbol, const int32 buy_sell, const int32 start,
                                const int32 count) {
    std::list<star_logic::TradesPosition> trades_position_list;
    if (buy_sell == SELL_TYPE)
        GetSymbolPosition(record_cache_->symbol_sell_trades_position_,symbol,
                          start, count, trades_position_list);
    else 
        GetSymbolPosition(record_cache_->symbol_buy_trades_position_,symbol, 
                          start, count, trades_position_list);

    if (trades_position_list.size() <= 0) {
        send_error(socket, ERROR_TYPE,NO_HAVE_POSITION_DATA,session);
        return;
    }
    SendFansPosition(socket, session, reserved, start, count,
                    trades_position_list);
}

void RecordManager::HisOrder(const int socket, const int64 session, const int32 reserved,
                                const int64 uid, const int32 status,
                                const int32 start,const int32 count) {
    std::list<star_logic::TradesOrder> trades_order_list;
    int64 start_pan = 0;
    GetUserOrder(uid,start_pan,status,start,count,trades_order_list);
    if (trades_order_list.size()<=0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA,session);
        return;
    }
    SendOrder(socket, session, reserved, S_ALL_ORDER_RECORD, 
                 start, count, uid, trades_order_list);
}

void RecordManager::TodayOrder(const int socket, const int64 session, const int32 reserved,
                                const int64 uid, const int32 start,const int32 count) {
    std::list<star_logic::TradesOrder> trades_order_list;
    int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60) - (8 * 60 * 60);
    GetUserOrder(uid,start_pan,COMPLETE_ORDER,start,count,trades_order_list);
    if (trades_order_list.size()<=0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_TODAY_DATA, session);
        return;
    }
    SendOrder(socket, session, reserved, S_TODAY_ORDER_RECORD, 
                 start, count, uid, trades_order_list);
}

void RecordManager::TodayPosition(const int socket, const int64 session, const int32 reserved,
                                  const int64 uid, const int32 start, const int32 count) {
    std::list<star_logic::TradesPosition> trades_position_list;
    int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60) - (8 * 60 * 60);
    GetUserPosition(uid,start_pan,start,count,trades_position_list);
    if (trades_position_list.size()<=0) {
        send_error(socket,ERROR_TYPE, NO_HAVE_TODAY_DATA, session);
        return;
    }
    SendPosition(socket, session, reserved, S_TODAY_POSITION_RECORD, 
                 start, count,trades_position_list);
}


void RecordManager::HisPosition(const int socket, const int64 session, const int32 reserved,
                                  const int64 uid, const int32 start, const int32 count) {
    std::list<star_logic::TradesPosition> trades_position_list;
    int64 start_pan = 0;
    GetUserPosition(uid,start_pan,start,count,trades_position_list);
    if (trades_position_list.size()<=0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA,session);
        return;
    }
    SendPosition(socket, session, reserved, S_TODAY_POSITION_RECORD, 
                 start, count,trades_position_list);
}

void RecordManager::SetTradesPosition(base_logic::DictionaryValue* dict) {
    star_logic::TradesPosition trades_position;
    trades_position.ValueSerialization(dict);
    SetTradesPosition(trades_position);
}

void RecordManager::SetTradesOrder(base_logic::DictionaryValue* dict) {
    star_logic::TradesOrder trades_order;
    trades_order.ValueSerialization(dict);
    SetTradesOrder(trades_order);
}


void RecordManager::SetTradesPosition(SYMBOL_TRADES_POSITION_MAP& symbol_trades_position,
                                      USER_TRADES_POSITION_MAP& user_trades_position,
                                      star_logic::TradesPosition& trades_position,
                                        bool init) {
    bool r = false;
    star_logic::TradesPosition tmp_trades_position;
    TRADES_POSITION_LIST symbol_position_list;
    TRADES_POSITION_LIST user_position_list;
    //检查是否存在
    r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,int64,star_logic::TradesPosition>
        (record_cache_->trades_positions_,trades_position.position_id(),tmp_trades_position);
    if(r) { //存在，修改状态, 已经成功的状态不修改
        //tmp_trades_position = trades_position;
        if(tmp_trades_position.handle() != COMPLETE_HANDLE){
            tmp_trades_position.set_handle(trades_position.handle());
            //当前时间已过，清空列表
            base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator,std::string,TRADES_POSITION_LIST>
                        (symbol_trades_position,trades_position.symbol(),symbol_position_list);
            symbol_position_list.clear();
            LOG_DEBUG2("star[%s] current cycle end.%d", trades_position.symbol().c_str(), 99999999);
        }
        if (!init)
           record_db_->OnUpdateTradesPosition(tmp_trades_position);
           
        //交易成功或者失败，都更新统计数据
        //if (trades_position.handle() < 0) 
        SetSymbolAuction(trades_position.symbol(), trades_position.amount(), 
                        trades_position.buy_sell(),-1);
        /*if(trades_position.handle() < 0 && trades_position.buy_sell() == SELL_TYPE)
            SetSymbolPositionTime(record_cache_->symbol_buy_trades_time_position_,
                        trades_position.symbol(),-trades_position.amount());
        else if (trades_position.handle() < 0 && trades_position.buy_sell() == BUY_TYPE)
            SetSymbolPositionTime(record_cache_->symbol_sell_trades_time_position_,
                        trades_position.symbol(), -trades_position.amount());*/
        return;
    }

    r = base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator,std::string,TRADES_POSITION_LIST>
        (symbol_trades_position,trades_position.symbol(),symbol_position_list);
    symbol_position_list.push_back(trades_position);
    symbol_trades_position[trades_position.symbol()] = symbol_position_list;


    r = base::MapGet<USER_TRADES_POSITION_MAP,USER_TRADES_POSITION_MAP::iterator,int64,TRADES_POSITION_LIST>
        (user_trades_position,trades_position.uid(),user_position_list);
    user_position_list.push_back(trades_position);
    user_trades_position[trades_position.uid()] = user_position_list;
    record_cache_->trades_positions_[trades_position.position_id()] = trades_position;
    
    //SetSymbolAuction(trades_position.symbol(), trades_position.amount(), 
      //                  trades_position.buy_sell(),1);
    /*if(trades_position.handle() < 0 && trades_position.buy_sell() == SELL_TYPE)
        SetSymbolPositionTime(record_cache_->symbol_sell_trades_time_position_,
                       trades_position.symbol(),trades_position.amount());
    else if (trades_position.handle() < 0 && trades_position.buy_sell() == BUY_TYPE)
        SetSymbolPositionTime(record_cache_->symbol_buy_trades_time_position_,
                       trades_position.symbol(), trades_position.amount());*/
    if (!init){
        star_logic::StarInfo star;
        bool r = manager_schduler_engine_->GetStarInfoSchduler(trades_position.symbol(), &star);
        if (r) {
            if(trades_position.buy_sell() == SELL_TYPE)
                record_kafka_->SetSellPosition(star.weibo_index_id(),
                            trades_position.open_price(),trades_position.open_position_time());
            else
                record_kafka_->SetBuyPosition(star.weibo_index_id(),
                            trades_position.open_price(), trades_position.open_position_time());
        }
        SetSymbolAuction(trades_position.symbol(), trades_position.amount(), 
                        trades_position.buy_sell(),1);
        record_db_->OnCreateTradesPosition(trades_position);
    }
}


// -1 有人退单  1 有人进单
void RecordManager::SetSymbolAuction(const std::string& symbol, const int64 time, const int32 buy_sell,
                                     const int32 atype) {
    bool r = false;
    star_logic::Auction auction;
    auction.set_symbol(symbol);
    r = base::MapGet<AUCTION_INFO_MAP,AUCTION_INFO_MAP::iterator,std::string,star_logic::Auction>
            (record_cache_->symbol_auction_map_,symbol,auction);
    
    int32 auction_count = 0;
    int64 auction_time = 0;
    
    auction_count =  (buy_sell == SELL_TYPE)  ? auction.sell_count() : auction.buy_count();
    auction_time =  (buy_sell == SELL_TYPE) ? auction.sell_time() : auction.buy_time();

    auction_count = (atype == 1) ? auction_count += 1 : auction_count -= 1;
    auction_time = (atype == 1) ? auction_time += time : auction_time -= time;

    auction_count = (auction_count > 0) ? auction_count : 0;
    auction_time = (auction_time > 0) ? auction_time : 0;

    if (buy_sell == SELL_TYPE){
        auction.set_sell_time(auction_time);
        auction.set_sell_count(auction_count);
    }else{
        auction.set_buy_time(auction_time);
        auction.set_buy_count(auction_count);
    }
    record_cache_->symbol_auction_map_[symbol] = auction;
}

void RecordManager::SetSymbolPositionTime(SYMBOL_POSITION_TIME_MAP& position_map, 
                                const std::string symbol,const int64 time) {
    bool r = false;
    int64 time_s = 0;
    r = base::MapGet<SYMBOL_POSITION_TIME_MAP,SYMBOL_POSITION_TIME_MAP::iterator, std::string, int64>
            (position_map,symbol,time_s);
    int64 now_time = time_s + time;
    now_time > 0 ? now_time : 0;
    position_map[symbol] = now_time;
}

void RecordManager::SetTradesOrder(star_logic::TradesOrder& trades_order) {
    base_logic::WLockGd lk(lock_);
    star_logic::TradesOrder tmp_trades_order;
    TRADES_ORDER_LIST symbol_order_list;
    TRADES_ORDER_LIST buy_trades_order_list;
    TRADES_ORDER_LIST sell_trades_order_list;
    bool r = base::MapGet<TRADES_ORDER_MAP,TRADES_ORDER_MAP::iterator,int64,star_logic::TradesOrder>
             (record_cache_->trades_order_,trades_order.order_id(),tmp_trades_order);
    if(r) { //存在，修改状态
        tmp_trades_order.set_handle_type(trades_order.handle_type());
        tmp_trades_order.set_buy_handle_type(trades_order.buy_handle_type());
        tmp_trades_order.set_sell_handle_type(trades_order.sell_handle_type());
        //tmp_trades_order(trades_order);
        //tmp_trades_order = trades_order;
        //LOG_DEBUG("111111111111");
        if (trades_order.handle_type() == COMPLETE_ORDER ){
            base::MapGet<SYMBOL_TRADES_ORDER_MAP,SYMBOL_TRADES_ORDER_MAP::iterator,std::string,TRADES_ORDER_LIST>
                        (record_cache_->symbol_trades_order_,trades_order.symbol(),symbol_order_list);
            symbol_order_list.push_back(trades_order);
            record_cache_->symbol_trades_order_[trades_order.symbol()] = symbol_order_list;

            star_logic::StarInfo star;
            r = manager_schduler_engine_->GetStarInfoSchduler(trades_order.symbol(), &star);
            if(r){
                //更新热度
                star.set_hot_priority1(star.hot_priority1() + 1);
                star.set_hot_priority2(star.hot_priority2() + trades_order.open_price()*trades_order.amount());
                manager_schduler_engine_->SetStarInfoSchduler(star.symbol(), &star);
            }
        }
        return;
    }
    if (trades_order.handle_type() == COMPLETE_ORDER )
    {
        r = base::MapGet<SYMBOL_TRADES_ORDER_MAP,SYMBOL_TRADES_ORDER_MAP::iterator,std::string,TRADES_ORDER_LIST>
            (record_cache_->symbol_trades_order_,trades_order.symbol(),symbol_order_list);
        symbol_order_list.push_back(trades_order);
        record_cache_->symbol_trades_order_[trades_order.symbol()] = symbol_order_list;
    }



    r = base::MapGet<USER_TRADES_ORDER_MAP,USER_TRADES_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (record_cache_->user_trades_order_,trades_order.buy_uid(),buy_trades_order_list);
    buy_trades_order_list.push_back(trades_order);
    record_cache_->user_trades_order_[trades_order.buy_uid()] = buy_trades_order_list;



    r = base::MapGet<USER_TRADES_ORDER_MAP,USER_TRADES_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (record_cache_->user_trades_order_,trades_order.sell_uid(),sell_trades_order_list);
    sell_trades_order_list.push_back(trades_order);
    record_cache_->user_trades_order_[trades_order.sell_uid()] = sell_trades_order_list;

    record_cache_->trades_order_[trades_order.order_id()] = trades_order;
    star_logic::StarInfo star;
    r = manager_schduler_engine_->GetStarInfoSchduler(trades_order.symbol(), &star);
    if (r)
        record_kafka_->SetVolume(star.weibo_index_id(),trades_order.open_price(),
            trades_order.open_position_time());
}

void RecordManager::SetTradesPosition(star_logic::TradesPosition& trades_position,bool init) {
    base_logic::WLockGd lk(lock_);
    if(trades_position.buy_sell() == BUY_TYPE) {
        SetTradesPosition(record_cache_->symbol_buy_trades_position_,record_cache_->user_buy_trades_position_,
                          trades_position,init);
    } else if(trades_position.buy_sell() == SELL_TYPE) {
        SetTradesPosition(record_cache_->symbol_sell_trades_position_,record_cache_->user_sell_trades_position_,
                          trades_position,init);
    }
}


void RecordManager::GetUserOrder(const int64 uid, const int64 start_pan,
                                const int32 status, const int32 start, const int32 count,
                                std::list<star_logic::TradesOrder>& trades_order_list) {
    base_logic::RLockGd lk(lock_);
    TRADES_ORDER_LIST order_list;
    int32 t_start = 0;
    int32 t_count = 0;
    bool r = false;
    r = base::MapGet<USER_TRADES_ORDER_MAP,USER_TRADES_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (record_cache_->user_trades_order_,uid,order_list);
        //排i序
        order_list.sort(star_logic::TradesOrder::open_after);
        TRADES_ORDER_LIST::iterator it = order_list.begin();
        for(; it != order_list.end() && t_count < count; it++) {
            star_logic::TradesOrder order = (*it);
            t_start++;
            if (t_start < start)
                continue;
            if(order.open_position_time() > start_pan){
                if (status == COMPLETE_ORDER && order.handle_type() == COMPLETE_ORDER)
                    trades_order_list.push_back(order);
                else if (status == MATCHES_ORDER && 
                         (order.handle_type() == MATCHES_ORDER ||
                          order.handle_type() == CONFIRM_ORDER))
                    trades_order_list.push_back(order);
                else if (status == 3)
                    trades_order_list.push_back(order);
            }
        }
}


void RecordManager::GetSymbolOrder(const std::string& symbol, const int32 start, const int32 count,
                                   std::list<star_logic::TradesOrder>& trades_order_list) {
    
    bool r = false;
    {
        base_logic::RLockGd lk(lock_);
        int32 t_start = 0;
        int32 t_count = 0;
        TRADES_ORDER_LIST order_list;
        r = base::MapGet<SYMBOL_TRADES_ORDER_MAP,SYMBOL_TRADES_ORDER_MAP::iterator, std::string, TRADES_ORDER_LIST>
            (record_cache_->symbol_trades_order_, symbol, order_list);
        order_list.sort(star_logic::TradesOrder::price_after);
        TRADES_ORDER_LIST::iterator it = order_list.begin();
        for(; it != order_list.end() && t_count < count; it++) {
            star_logic::TradesOrder order = (*it);
            t_start++;
            trades_order_list.push_back(order);
        }
    }
}

void RecordManager::GetSymbolPosition(SYMBOL_TRADES_POSITION_MAP& symbol_trades_position, const std::string& symbol,
                    const int32 start,const int32 count, std::list<star_logic::TradesPosition>& trades_position_list){

    bool r = false;
    {
        base_logic::RLockGd lk(lock_);
        int32 t_start = 0;
        int32 t_count = 0;
        TRADES_POSITION_LIST position_list;
        r = base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator, std::string, TRADES_POSITION_LIST>
            (symbol_trades_position, symbol, position_list);
        position_list.sort(star_logic::TradesPosition::price_after);
        TRADES_POSITION_LIST::iterator it = position_list.begin();
        for(; it != position_list.end() && t_count < count; it++) {
            star_logic::TradesPosition position = (*it);
            //只返回委托中的数据
            if (position.handle() == POSITION_HANDLE || position.handle() == MATCHES_HANDLE) {
                t_start++;
                trades_position_list.push_back(position);
            }
        }
    }
}


void RecordManager::GetSymbolPositionCount(SYMBOL_TRADES_POSITION_MAP& symbol_trades_position, const std::string& symbol,
                    int32& count){

    bool r = false;
    {
        base_logic::RLockGd lk(lock_);
        int32 t_start = 0;
        int32 t_count = 0;
        count = 0;
        TRADES_POSITION_LIST position_list;
        r = base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator, std::string, TRADES_POSITION_LIST>
            (symbol_trades_position, symbol, position_list);
        TRADES_POSITION_LIST::iterator it = position_list.begin();
        //只返回匹配中的
        for(; it != position_list.end();++it){
            if(it->handle() == POSITION_HANDLE || it->handle() == MATCHES_HANDLE)count++;
        }
    }
}

void RecordManager::GetUserPosition(const int64 uid, const int64 start_pan,
                                    const int32 start,const int32 count,
                                    std::list<star_logic::TradesPosition>& trades_position_list) {
    bool r = false;
    {
        base_logic::RLockGd lk(lock_);
        TRADES_POSITION_LIST position_list;
        int32 t_start = 0;
        int32 t_count = 0;
        r = base::MapGet<USER_TRADES_POSITION_MAP,USER_TRADES_POSITION_MAP::iterator,int64,TRADES_POSITION_LIST>
            (record_cache_->user_buy_trades_position_,uid,position_list);
        //排i序
        position_list.sort(star_logic::TradesPosition::open_after);
        TRADES_POSITION_LIST::iterator it = position_list.begin();
        for(; it != position_list.end() && t_count < count; it++) {
            star_logic::TradesPosition position = (*it);
            t_start++;
            if (t_start < start)
                continue;
            if(position.open_position_time() > start_pan)
                trades_position_list.push_back(position);
        }
    }


    {
        base_logic::RLockGd lk(lock_);
        TRADES_POSITION_LIST position_list;
        int32 t_start = 0;
        int32 t_count = 0;
        r = base::MapGet<USER_TRADES_POSITION_MAP,USER_TRADES_POSITION_MAP::iterator,int64,TRADES_POSITION_LIST>
            (record_cache_->user_sell_trades_position_,uid,position_list);
        //排i序
        position_list.sort(star_logic::TradesPosition::open_after);
        TRADES_POSITION_LIST::iterator it = position_list.begin();
        for(; it != position_list.end() && t_count < count; it++) {
            star_logic::TradesPosition position = (*it);
            t_start++;
            if (t_start < start)
                continue;
            if(position.open_position_time() > start_pan)
                    trades_position_list.push_back(position);
        }
    }

}
//获取明星委托
void RecordManager::GetStarPosition(const int socket, const int64 session, const int32 reserved,
                                const std::string& symbol, const int32 count) {
    bool r = false;
    int32 t_count = 0;
    std::list<star_logic::TradesPosition> trades_position_list;
    {
        base_logic::RLockGd lk(lock_);
        t_count = 0;
        TRADES_POSITION_LIST position_list;
        r = base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator, std::string, TRADES_POSITION_LIST>
            (record_cache_->symbol_sell_trades_position_, symbol, position_list);
        TRADES_POSITION_LIST::iterator it = position_list.begin();
        for(; it != position_list.end() && t_count < count; ++it) {
            trades_position_list.push_back(*it);
            t_count++;
        }

        t_count = 0;
        TRADES_POSITION_LIST buy_position_list;
        r = base::MapGet<SYMBOL_TRADES_POSITION_MAP,SYMBOL_TRADES_POSITION_MAP::iterator, std::string, TRADES_POSITION_LIST>
            (record_cache_->symbol_buy_trades_position_, symbol, buy_position_list);
        it = buy_position_list.begin();
        for(; it != buy_position_list.end() && t_count < count; ++it) {
            trades_position_list.push_back(*it);
            t_count++;
        }
    }

    trades_position_list.sort(star_logic::TradesPosition::price_after);
    if (trades_position_list.size() <= 0) {
        send_error(socket, ERROR_TYPE,NO_HAVE_POSITION_DATA,session);
        return;
    }

    SendFansPosition(socket, session, reserved, 0, count,
                    trades_position_list);

}

//获取所有明星委托，用于弹幕
void RecordManager::GetAllPosition(const int socket, const int64 session, const int32 reserved,
                                const int32 count) {
    int32 t_count = 0;
    std::list<star_logic::TradesPosition> trades_position_list;
    TRADES_POSITION_MAP::iterator it = record_cache_->trades_positions_.begin();
    for(; it != record_cache_->trades_positions_.end() && t_count < count; ++it){
        trades_position_list.push_back(it->second);
        t_count++;
    }

    SendFansPosition(socket, session, reserved, 0, count,
                    trades_position_list);
}


void RecordManager::DistributionRecord() {
    std::list<base_logic::DictionaryValue*> list;
    record_kafka_->FectchBatchTempTask(&list);
    LOG_DEBUG2("list size : %d",list.size());
    if(list.size()<=0)
        return;

    bool r = false;
    while(list.size() > 0) {
        base_logic::DictionaryValue* dict_value = list.front();
        list.pop_front();
        int64 type = 0;
        r = dict_value->GetBigInteger(L"type", &type);
        if (r) {
            if (type == TRADES_POSITION_TYPE)
                SetTradesPosition(dict_value);
            else if (type == TRADES_ORDER_TYPE)
                SetTradesOrder(dict_value);
        }

        if (dict_value) {
            delete dict_value;
            dict_value = NULL;
        }
    }

}

}
