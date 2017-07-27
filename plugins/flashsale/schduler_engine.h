//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef STAR_FLASH_SCHDULER_ENGINE_H__
#define STAR_FLASH_SCHDULER_ENGINE_H__

#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "logic/time.h"
#include "logic/star_infos.h"
#include "manager/data_share_mgr.h"
#include "flashsale/flash_db.h"
#include "flashsale/flash_info.h"
#include "flashsale/flash_kafka.h"
#include "flashsale/share_memory.h"

namespace flash_logic {

typedef std::list<flash_logic::TimeTask> TIME_TASK_LIST;//for sort
typedef std::map<std::string, flash_logic::TimeTask> TIME_TASK_MAP;//KEY:symbol
typedef std::map<std::string, flash_logic::PulishStar> PUBLISH_STAR_MAP;//KEY:symbol
typedef std::map<int64, star_logic::TradesOrder> USER_ORDER_MAP;

class FlashManager {
public:
    FlashManager();
    virtual ~FlashManager();

public:
    void TimeEvent(int opcode, int time);
    void InitDB(flash_logic::FlashDB* flash_db);
    void InitKafka(flash_logic::FlashKafka* flash_kafka);
    void InitData();
    void InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine);
    void TimeStarEvent();

    void ConfirmOrder(const int socket, const int64 session, const int32 reserved,
            const int64 uid,const std::string& symbol,const int64 amount, const double price);
    
    void FlashSymbolStatus(const int socket, const int64 session, const int32 reserved,
                            const std::string& symbol);

    void SymbolInfo(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol);
private:
    void Init();
    //创建时间任务
    void CreateTimeTask();
    void ProcessTimeTask(const time_t current_time, flash_logic::TimeTask& task);

    void SetFlashOrder(const int64 uid,const std::string& symbol,const int64 amount,const double price,
                              star_logic::TradesOrder& flash_trades_order);

    void SendNoiceMessage(const int64 uid, const int64 order_id,const int32 result, const int64 session);

private:
    manager_schduler::SchdulerEngine* manager_schduler_engine_;
    flash_logic::FlashDB* flash_db_;
    flash_logic::FlashKafka*  flash_kafka_;
    struct threadrw_t *lock_;
    CShareMemory* share_memory_;

    PUBLISH_STAR_MAP    publish_star_map_;
    TIME_TASK_LIST  flash_task_list_;
    TIME_TASK_MAP   flash_task_map_;
    USER_ORDER_MAP  user_order_map_;
};

class FlashEngine {
private:
    static FlashEngine* schduler_engine_;
    static FlashManager* schduler_mgr_;

    FlashEngine() {
    }
    virtual ~FlashEngine() {
    }
public:
    static FlashManager* GetSchdulerManager() {
        if (schduler_mgr_ == NULL)
            schduler_mgr_ = new FlashManager();
        return schduler_mgr_;
    }

    static FlashEngine* GetFlashEngine() {
        if (schduler_engine_ == NULL)
            schduler_engine_ = new FlashEngine();
        return schduler_engine_;
    }

    static void FreeSchdulerManager() {
        if (schduler_mgr_) {
            delete schduler_mgr_;
            schduler_mgr_ = NULL;
        }
    }

    static void FreeFlashEngine() {
        if (schduler_engine_) {
            delete schduler_engine_;
            schduler_engine_ = NULL;
        }
    }
};
}

#endif
