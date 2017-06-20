//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef STAR_SCHDULER_ENGINE_H__
#define STAR_SCHDULER_ENGINE_H__

#include "logic/time.h"
#include "logic/star_infos.h"
#include "trades/trades_db.h"
#include "trades/trades_info.h"
#include "trades/trades_kafka.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "manager/data_share_mgr.h"

namespace trades_logic {
typedef std::map<std::string, trades_logic::TradesStar> TRADES_STAR_MAP;

typedef std::list<trades_logic::TimeTask> TIME_TASK_LIST;
typedef std::map<std::string, trades_logic::TimeTask> TIME_TASK_MAP;

typedef std::list<star_logic::TradesPosition> TRADES_POSITION_LIST; //交易列表

typedef std::map<std::string, TRADES_POSITION_LIST> TRADES_POSITION_MAP;

typedef std::map<double, TRADES_POSITION_LIST> PRICE_POSITION_MAP; //价格表对应的持仓

typedef std::map<std::string, PRICE_POSITION_MAP> TRADEING_POSITION_MAP; //明星对应的价格


typedef std::map<int64, star_logic::TradesOrder>  TRADES_ORDER_MAP;//订单号-订单记录

typedef std::list<star_logic::TradesOrder> TRADES_ORDER_LIST;

typedef std::map<std::string,TRADES_ORDER_LIST>  KEY_ORDER_MAP;

typedef std::map<int64, TRADES_ORDER_LIST> USER_ORDER_MAP;

enum STAR_TRADES_STATUS {
    NO_STAR_TRADES = -100,
    NO_TIME = -101
};

enum MATCH_STATUS {
    MATCHING = 0,
    NO_SYMBOL = -100,
    NO_MATCH_PRICE = -101
};


class TradesCache {
public:
    TRADES_STAR_MAP        trades_star_map_;
    TIME_TASK_LIST         trades_task_list_;
    TIME_TASK_MAP          trades_task_map_;

    //挂单
    TRADES_POSITION_MAP    buy_trades_position_; //求购表 //用于排序
    TRADES_POSITION_MAP    sell_trades_position_; //转让表 //用于排序
    TRADEING_POSITION_MAP  buy_trading_position_; //用于价格匹配
    TRADEING_POSITION_MAP  sell_trading_position_; //用于价格匹配


    //撮合订单
    TRADES_ORDER_MAP      all_trades_order_;
    KEY_ORDER_MAP         symbol_trades_order_;
    USER_ORDER_MAP        user_trades_order_;
};



class TradesManager {
public:
    TradesManager();
    virtual ~TradesManager();

public:
    void TimeEvent(int opcode, int time);
    void InitDB(trades_logic::TradesDB* trades_db);
    void InitKafka(trades_logic::TradesKafka* trades_kafka);
    void InitData();
    void InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine);
    void TimeStarEvent();

    void CreateTradesPosition(const int socket, const int64 session, const int32 reserved,
                              const int64 uid, const std::string& symbol, const std::string& wid,
                              const int64 buy_sell, const int64 amount, const double price);

    void ConfirmOrder(const int socket, const int64 session, const int32 reserved,
                      const int64 uid, const int64 order_id, const int64 position_id);
    
    void TradesSymbolInfo(const int socket, const int64 session, const int32 reserved,
                        const std::string& symbol);

    void CancelOrder(const int socket, const int64 session, const int32 reserved,
                        const int64 uid, const int64 order_id);
private:
    void Init();
    //创建时间任务
    void CreateTimeTask();
    void ProcessTimeTask(const time_t current_time, trades_logic::TimeTask& task);
    int64 ChangeTodayUnixTime(const base::Time::Exploded& exploded, const std::string& fromat_time);
    void AlterTradesStarState(const std::string& symbol,const bool state);
    int64 GetTradesStarStatus(const std::string& symbol); //若不能交易，返回交易的事件，以秒为单位，日可以交易返回0

    int32 MatchTrades(const int socket, const int64 session, const int32 reserved,
                      TRADEING_POSITION_MAP& tradeing_postion, star_logic::TradesPosition& trades);

    void SetTradesPosition(TRADES_POSITION_MAP& trades_position,TRADEING_POSITION_MAP& trading_position,
                           star_logic::TradesPosition& trades);

    void MatchNotice(const int socket, const int64 session, const int32 reserved,
                     star_logic::TradesOrder& trades_order);


    void SetTradesOrder(star_logic::TradesPosition& buy_position,star_logic::TradesPosition& sell_position,
                        star_logic::TradesOrder& order);

    void SendOrderResult(const int socket,const int64 session, const int32 reserved,
                        const int64 buy_uid,const int64 sell_uid,const int32 result,
                        const int64 order_id);

    void SendConfirmOrder(const int socket, const int64 session, const int32 reserved,
                          const int64 uid, const int64 order_id, const int32 status);

    void ClearSymbolTrades(const std::string& symbol);

    void ClearTradesPosition(TRADES_POSITION_MAP& trades_position_map,const std::string& symbol);

    void ClearTradesOrder(KEY_ORDER_MAP& symbol_trades_order, const std::string& symbol);

    void SendNoiceMessage(const int64 uid, const int32 operator_code, const int64 session,
                            base_logic::DictionaryValue* message);
private:
    manager_schduler::SchdulerEngine* schduler_engine_;
    TradesCache *trades_cache_;
    trades_logic::TradesDB* trades_db_;
    trades_logic::TradesKafka*  trades_kafka_;
    struct threadrw_t *lock_;
};

class TradesEngine {
private:
    static TradesEngine* schduler_engine_;
    static TradesManager* schduler_mgr_;

    TradesEngine() {
    }
    virtual ~TradesEngine() {
    }
public:
    static TradesManager* GetSchdulerManager() {
        if (schduler_mgr_ == NULL)
            schduler_mgr_ = new TradesManager();
        return schduler_mgr_;
    }

    static TradesEngine* GetTradesEngine() {
        if (schduler_engine_ == NULL)
            schduler_engine_ = new TradesEngine();
        return schduler_engine_;
    }

    static void FreeSchdulerManager() {
        if (schduler_mgr_) {
            delete schduler_mgr_;
            schduler_mgr_ = NULL;
        }
    }

    static void FreeTradesEngine() {
        if (schduler_engine_) {
            delete schduler_engine_;
            schduler_engine_ = NULL;
        }
    }
};
}

#endif
