//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef STAR_SCHDULER_ENGINE_H__
#define STAR_SCHDULER_ENGINE_H__

#include "logic/time.h"
#include "logic/star_infos.h"
#include "trades/trades_db.h"
#include "trades/trades_info.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"

typedef std::map<std::string, trades_logic::TradesStar> TRADES_STAR_MAP;
typedef std::list<trades_logic::TimeTask> TIME_TASK_LIST;
typedef std::map<std::string, trades_logic::TimeTask> TIME_TASK_MAP;

typedef std::list<star_logic::TradesPosition> TRADES_POSITION_LIST; //交易列表

typedef std::map<std::string, TRADES_POSITION_LIST> TRADES_POSITION_MAP;

typedef std::map<double, TRADES_POSITION_LIST> PRICE_POSITION_MAP; //价格表对应的持仓

typedef std::map<std::string, PRICE_POSITION_MAP> TRADEING_POSITION_MAP; //明星对应的价格


enum STAR_TRADES_STATUS {
    NO_STAR_TRADES = -100,
    NO_TIME = -101
};

enum MATCH_STATUS {
    MATCHING = 0,
    NO_SYMBOL = -100,
    NO_MATCH_PRICE = -101
};

namespace trades_logic {

class TradesCache {
public:
    TRADES_STAR_MAP        trades_star_map_;
    TIME_TASK_LIST         trades_task_list_;
    TIME_TASK_MAP          trades_task_map_;
    TRADES_POSITION_MAP    buy_trades_position_; //求购表 //用于排序
    TRADES_POSITION_MAP    sell_trades_position_; //转让表 //用于排序
    TRADEING_POSITION_MAP  buy_trading_position_; //用于价格匹配
    TRADEING_POSITION_MAP  sell_trading_position_; //用于价格匹配
};



class TradesManager {
public:
    TradesManager();
    virtual ~TradesManager();

public:
    void TimeEvent(int opcode, int time);
    void InitDB(trades_logic::TradesDB* trades_db);
    void InitData();
    void TimeStarEvent();

    void CreateTradesPosition(const int socket, const int64 session, const int32 reserved,
                            const int64 uid, const std::string& symbol, const std::string& wid,
                            const int64 buy_sell, const int64 amount, const double price);

    void GetStarTradingInfo(const int socket, const int64 session, const int32 reserved,
                            const std::string& symbol,const int32 atype,
                            const int32 start, const int32 count);
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

    void MatchNotice(const std::string& symbol, const int socket, const int64 session, const int64 uid, 
                const int64 u_position_id, const int64 t_position_id,const int64 tid);

    void SendMatch(const int socket, const int64 session, const std::string& symbol,
                    const int64 uid, const int64 u_position_id,const int64 tid,
                    const int64 t_position_id);
private:
    TradesCache *trades_cache_;
    trades_logic::TradesDB* trades_db_;
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
