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
namespace trades_logic {

class TradesCache {
 public:
    TRADES_STAR_MAP  trades_star_map_;
    TIME_TASK_LIST   trades_task_list_;
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
 private:
  void Init();
  //创建时间任务
  void CreateTimeTask();
  void ProcessTimeTask(const time_t current_time, trades_logic::TimeTask& task);
  int64 ChangeTodayUnixTime(const base::Time::Exploded& exploded, const std::string& fromat_time);
  void AlterTradesStarState(const std::string& symbol,const bool state);
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
