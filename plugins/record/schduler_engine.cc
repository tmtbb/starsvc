//  Copyright (c) 2016-2017 The STAR Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "record/schduler_engine.h"
#include "record/record_proto_buf.h"
#include "record/operator_code.h"
#include "record/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

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
}

void RecordManager::InitDB(record_logic::RecordDB* record_db) {
    record_db_ = record_db;
}

void RecordManager::InitKafka(record_logic::RecordKafka* record_kafka) {
    record_kafka_ = record_kafka;
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
        net_trades_unit->set_open_time(trades_order.open_position_time());
        net_trades_unit->set_open_charge(trades_order.open_charge());
        net_trades_unit->set_handle(trades_order.handle_type());
        net_trades_unit->set_close_time(trades_order.close_position_time());
        net_trades_unit->set_gross_profit(trades_order.gross_profit());
        net_trades_unit->set_buy_uid(trades_order.buy_uid());
        net_trades_unit->set_sell_uid(trades_order.sell_uid());
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

void RecordManager::HisOrder(const int socket, const int64 session, const int32 reserved,
                                const int64 uid, const int32 start,const int32 count) {
    std::list<star_logic::TradesOrder> trades_order_list;
    int64 start_pan = 0;
    GetUserOrder(uid,start_pan,start,count,trades_order_list);
    if (trades_order_list.size()<=0) {
        return;
    }
    SendOrder(socket, session, reserved, S_ALL_ORDER_RECORD, 
                 start, count, uid, trades_order_list);
}

void RecordManager::TodayOrder(const int socket, const int64 session, const int32 reserved,
                                const int64 uid, const int32 start,const int32 count) {
    std::list<star_logic::TradesOrder> trades_order_list;
    int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60) - (8 * 60 * 60);
    GetUserOrder(uid,start_pan,start,count,trades_order_list);
    if (trades_order_list.size()<=0) {
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
                                      star_logic::TradesPosition& trades_position) {
    bool r = false;
    star_logic::TradesPosition tmp_trades_position;
    TRADES_POSITION_LIST symbol_position_list;
    TRADES_POSITION_LIST user_position_list;
    //检查是否存在
    r = base::MapGet<TRADES_POSITION_MAP,TRADES_POSITION_MAP::iterator,int64,star_logic::TradesPosition>
        (record_cache_->trades_positions_,trades_position.position_id(),tmp_trades_position);
    if(r) { //存在，修改状态
        tmp_trades_position = trades_position;
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
        tmp_trades_order = trades_order;
        return;
    }

    r = base::MapGet<SYMBOL_TRADES_ORDER_MAP,SYMBOL_TRADES_ORDER_MAP::iterator,std::string,TRADES_ORDER_LIST>
        (record_cache_->symbol_trades_order_,trades_order.symbol(),symbol_order_list);
    symbol_order_list.push_back(trades_order);
    record_cache_->symbol_trades_order_[trades_order.symbol()] = symbol_order_list;



    r = base::MapGet<USER_TRADES_ORDER_MAP,USER_TRADES_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (record_cache_->user_trades_order_,trades_order.buy_uid(),buy_trades_order_list);
    buy_trades_order_list.push_back(trades_order);
    record_cache_->user_trades_order_[trades_order.buy_uid()] = buy_trades_order_list;



    r = base::MapGet<USER_TRADES_ORDER_MAP,USER_TRADES_ORDER_MAP::iterator,int64,TRADES_ORDER_LIST>
        (record_cache_->user_trades_order_,trades_order.sell_uid(),sell_trades_order_list);
    sell_trades_order_list.push_back(trades_order);
    record_cache_->user_trades_order_[trades_order.sell_uid()] = sell_trades_order_list;

    record_cache_->trades_order_[trades_order.order_id()] = trades_order;
}

void RecordManager::SetTradesPosition(star_logic::TradesPosition& trades_position) {
    base_logic::WLockGd lk(lock_);
    if(trades_position.buy_sell() == BUY_TYPE) {
        SetTradesPosition(record_cache_->symbol_buy_trades_position_,record_cache_->user_buy_trades_position_,
                          trades_position);
    } else if(trades_position.buy_sell() == SELL_TYPE) {
        SetTradesPosition(record_cache_->symbol_sell_trades_position_,record_cache_->user_sell_trades_position_,
                          trades_position);
    }
}


void RecordManager::GetUserOrder(const int64 uid, const int64 start_pan,
                                const int32 start, const int32 count,
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
            if(order.open_position_time() > start_pan)
                trades_order_list.push_back(order);
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
