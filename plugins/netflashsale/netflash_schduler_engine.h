//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef STAR_NET_FLASH_SCHDULER_ENGINE_H__
#define STAR_NET_FLASH_SCHDULER_ENGINE_H__

#include "logic/time.h"
#include "logic/star_infos.h"
#include "manager/data_share_mgr.h"
#include "netflash_db.h"
//#include "netflash_info.h"
#include "netflash_kafka.h"
#include "flashsale/share_memory.h"

namespace netflash_logic {

typedef std::list<flash_logic::TimeTask> TIME_TASK_LIST;//for sort
typedef std::map<std::string, flash_logic::TimeTask> TIME_TASK_MAP;//KEY:symbol
typedef std::map<std::string, flash_logic::PulishStar> PUBLISH_STAR_MAP;//key:symbol
typedef std::map<int64, star_logic::TradesOrder> USER_ORDER_MAP;
typedef std::map<std::string, base_logic::DictionaryValue> STAR_EXPER_MAP;//key:symbol


class NetflashManager {
public:
    NetflashManager();
    virtual ~NetflashManager();

public:
    void TimeEvent(int opcode, int time);
    void InitDB(netflash_logic::NetflashDB* netflash_db);
    void InitKafka(netflash_logic::NetflashKafka* netflash_kafka);
    void InitData();
    //void InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine);
    void TimeStarEvent();
    void SynchronizeStarInfo();

    bool NetSymbolList(const int socket, const int64 session, const int32 reserved);

    void ConfirmOrder(const int socket, const int64 session, const int32 reserved,
            const int64 uid,const std::string& symbol,const int64 amount, const double price);
    
    bool NetflashSymbolStatus(const int socket, const int64 session, const int32 reserved,
                            const std::string& symbol);

    bool SymbolInfo(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol);

    bool Getstarexperience(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol);
private:
    void Init();
    void ClearData();
    //创建时间任务
    void CreateTimeTask();
    void ProcessTimeTask(const time_t current_time, flash_logic::TimeTask& task);

    void SetNetflashOrder(const int64 uid,const std::string& symbol,const int64 amount,const double price,
                              star_logic::TradesOrder& netflash_trades_order);

    //void SendNoiceMessage(const int64 uid, const int64 order_id,const int32 result, const int64 session);

private:
    //manager_schduler::SchdulerEngine* manager_schduler_engine_;
    netflash_logic::NetflashDB* netflash_db_;
    netflash_logic::NetflashKafka*  netflash_kafka_;
    struct threadrw_t *lock_;
    flash_logic::CShareMemory* share_memory_;

    PUBLISH_STAR_MAP    publish_star_map_;
    TIME_TASK_LIST  netflash_task_list_;
    TIME_TASK_MAP   netflash_task_map_;
    //USER_ORDER_MAP  user_order_map_;
    STAR_EXPER_MAP  star_experience_map_;
};

class NetflashEngine {
private:
    static NetflashEngine* schduler_engine_;
    static NetflashManager* schduler_mgr_;

    NetflashEngine() {
    }
    virtual ~NetflashEngine() {
    }
public:
    static NetflashManager* GetSchdulerManager() {
        if (schduler_mgr_ == NULL)
            schduler_mgr_ = new NetflashManager();
        return schduler_mgr_;
    }

    static NetflashEngine* GetNetflashEngine() {
        if (schduler_engine_ == NULL)
            schduler_engine_ = new NetflashEngine();
        return schduler_engine_;
    }

    static void FreeSchdulerManager() {
        if (schduler_mgr_) {
            delete schduler_mgr_;
            schduler_mgr_ = NULL;
        }
    }

    static void FreeNetflashEngine() {
        if (schduler_engine_) {
            delete schduler_engine_;
            schduler_engine_ = NULL;
        }
    }
};
}

#endif
