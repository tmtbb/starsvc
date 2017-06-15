//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef RECORD_SCHDULER_ENGINE_H__
#define RECORD_SCHDULER_ENGINE_H__

#include "logic/star_infos.h"
#include "record/record_info.h"
#include "record/record_db.h"
#include "record/record_kafka.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "logic/base_values.h"
#include <map>
#include <list>

namespace record_logic {
;
typedef std::map<int64, star_logic::TradesPosition> TRADES_POSITION_MAP;
typedef std::list<star_logic::TradesPosition> TRADES_POSITION_LIST;
typedef std::map<std::string, TRADES_POSITION_LIST> SYMBOL_TRADES_POSITION_MAP;
typedef std::map<int64,TRADES_POSITION_LIST> USER_TRADES_POSITION_MAP;

typedef std::map<int64, star_logic::TradesOrder> TRADES_ORDER_MAP;
typedef std::list<star_logic::TradesOrder> TRADES_ORDER_LIST;
typedef std::map<std::string,TRADES_ORDER_LIST> SYMBOL_TRADES_ORDER_MAP;
typedef std::map<int64,TRADES_ORDER_LIST> USER_TRADES_ORDER_MAP;

class RecordCache {
public:
    SYMBOL_TRADES_POSITION_MAP  symbol_buy_trades_position_;
    USER_TRADES_POSITION_MAP    user_buy_trades_position_;

    SYMBOL_TRADES_POSITION_MAP  symbol_sell_trades_position_;
    USER_TRADES_POSITION_MAP    user_sell_trades_position_;

    TRADES_POSITION_MAP         trades_positions_;


    SYMBOL_TRADES_ORDER_MAP     symbol_trades_order_;
    USER_TRADES_ORDER_MAP       user_trades_order_;

    TRADES_ORDER_MAP            trades_order_;
};

class RecordManager {
public:
    RecordManager();
    virtual ~RecordManager();
    void InitDB(record_logic::RecordDB* record_db);
    void InitKafka(record_logic::RecordKafka* record_kafka);
    void DistributionRecord();
    void TodayPosition(const int socket, const int64 session, const int32 reserved,
                       const int64 uid,const int32 start, const int32 count);

    void TodayOrder(const int socket, const int64 session, const int32 reserved,
                    const int64 uid,const int32 start, const int32 count);

    void HisPosition(const int socket, const int64 session, const int32 reserved,
                     const int64 uid,const int32 start, const int32 count);

    void HisOrder(const int socket, const int64 session, const int32 reserved,
                  const int64 uid,const int32 start, const int32 count);
private:
    void SetTradesPosition(base_logic::DictionaryValue* dict);
    void SetTradesOrder(base_logic::DictionaryValue* dict);
    void SetTradesPosition(star_logic::TradesPosition& trades_position);
    void SetTradesOrder(star_logic::TradesOrder& trades_order);
    void SetTradesPosition(SYMBOL_TRADES_POSITION_MAP& symbol_trades_position,
                           USER_TRADES_POSITION_MAP& user_trades_position,
                           star_logic::TradesPosition& trades_position);


    void GetUserOrder(const int64 uid, const int64 start_pan, const int32 start, const int32 count,
            std::list<star_logic::TradesOrder>& trades_order_list);

    void SendOrder(const int socket,const int64 session, const int32 reserved,
                    const int operator_code, const int32 start, const int32 count, const int64 uid,
                    std::list<star_logic::TradesOrder>& trades_order_list);

    void GetUserPosition(const int64 uid, const int64 start_pan, const int32 start, const int32 count,
            std::list<star_logic::TradesPosition>& trades_positions_list);

    void SendPosition(const int socket,const int64 session, const int32 reserved,
                    const int operator_code, const int32 start, const int32 count,
                    std::list<star_logic::TradesPosition>& trades_positions_list);
private:
    void Init();
private:
    record_logic::RecordDB* record_db_;
    record_logic::RecordKafka* record_kafka_;
    RecordCache *record_cache_;
    struct threadrw_t *lock_;
};

class RecordEngine {
private:
    static RecordEngine* schduler_engine_;
    static RecordManager* schduler_mgr_;

    RecordEngine() {
    }
    virtual ~RecordEngine() {
    }
public:
    static RecordManager* GetSchdulerManager() {
        if (schduler_mgr_ == NULL)
            schduler_mgr_ = new RecordManager();
        return schduler_mgr_;
    }

    static RecordEngine* GetQuotationsEngine() {
        if (schduler_engine_ == NULL)
            schduler_engine_ = new RecordEngine();
        return schduler_engine_;
    }

    static void FreeSchdulerManager() {
        if (schduler_mgr_) {
            delete schduler_mgr_;
            schduler_mgr_ = NULL;
        }
    }

    static void FreeRecordEngine() {
        if (schduler_engine_) {
            delete schduler_engine_;
            schduler_engine_ = NULL;
        }
    }
};

}

#endif
