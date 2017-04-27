//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef HISTORY_SCHDULER_ENGINE_H__
#define HISTORY_SCHDULER_ENGINE_H__

#include "logic/swp_infos.h"
#include "history/history_info.h"
#include "history/history_db.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "manager/data_share_mgr.h"

namespace history_logic {

typedef std::map<int64, swp_logic::TradesPosition> TRADES_MAP;/*交易ID为key*/
typedef std::map<int64, TRADES_MAP> ALL_TRADES_MAP;/*用户ID 为key*/

typedef std::map<int64, swp_logic::Recharge> RECHARGE_MAP;/*充值ID为key*/
typedef std::map<int64, RECHARGE_MAP> ALL_RECHAGE_MAP;/*用户ID 为KEY*/

typedef std::map<int64, swp_logic::Withdrawals> WITHDRAWALS_MAP;/*提现ID*/
typedef std::map<int64, WITHDRAWALS_MAP> ALLWITHDRAWALS_MAP;/*用户ID 为KEY*/

class HistoryCache {
 public:
  ALL_TRADES_MAP all_trades_map_;
  ALL_RECHAGE_MAP all_rechage_map_;
  ALLWITHDRAWALS_MAP all_withdrawals_map_;
};

class HistoryManager {
 public:
  HistoryManager();
  virtual ~HistoryManager();
  void InitDB(history_logic::HistoryDB* history_db);
  void InitHistoryTradesData();
  void InitHistoryRechargeData();
  void InitHistoryWithDrawals();

  void InitSchdulerEngine(manager_schduler::SchdulerEngine* schduler_engine);

  void SendHistoryTrades(const int socket, const int64 session,
                         const int32 reversed, const int64 uid, const std::string& symbol,
                         const int64 pos,
                         const int64 count = 10);

  void SendHistoryRecharge(const int socket, const int64 session,
                           const int32 revered, const int64 uid,
                           const int32 status, const int64 pos,
                           const int64 count = 10);

  void SendHistoryWithDrawls(const int socket, const int64 session,
                             const int32 revered, const int64 uid,
                             const int32 status, const int64 pos,
                             const int64 count = 10);

  void HandleHistoryTrade(const int socket, const int64 session,
                          const int32 revered, const int64 uid,
                          const int64 tid,const int32 handle);

 private:
  void Init();
  void SetHistoryTradesNoLock(swp_logic::TradesPosition& trades);
  void GetHistoryTradesNoLock(const int64 uid,
                              std::list<swp_logic::TradesPosition>& list,
                              const int64 pos = 0, const int64 count = 10);

  void ModifyHistoryTradesNoLock(const int64 uid, const int64 tid,
                           const int32 handle);

  void SetHistoryRechargeNoLock(swp_logic::Recharge& recharge);
  void GetHistoryRechargeNoLock(const int64 uid, const int32 status,
                                std::list<swp_logic::Recharge>& list,
                                const int64 pos = 0, const int64 count = 10);

  void SetHistoryWithDrawlsNoLock(swp_logic::Withdrawals& withdrawls);
  void GetHistoryDrawlNoLock(const int64 uid, const int32 status,
                             std::list<swp_logic::Withdrawals>& list,
                             const int64 pos = 0, const int64 count = 10);
 private:
  history_logic::HistoryDB* history_db_;
  HistoryCache *history_cache_;
  struct threadrw_t *lock_;
  manager_schduler::SchdulerEngine* schduler_engine_;
};

class HistoryEngine {
 private:
  static HistoryEngine* schduler_engine_;
  static HistoryManager* schduler_mgr_;

  HistoryEngine() {
  }
  virtual ~HistoryEngine() {
  }
 public:
  static HistoryManager* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new HistoryManager();
    return schduler_mgr_;
  }

  static HistoryEngine* GetQuotationsEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new HistoryEngine();
    return schduler_engine_;
  }

  static void FreeSchdulerManager() {
    if (schduler_mgr_) {
      delete schduler_mgr_;
      schduler_mgr_ = NULL;
    }
  }

  static void FreeHistoryEngine() {
    if (schduler_engine_) {
      delete schduler_engine_;
      schduler_engine_ = NULL;
    }
  }
};
}

#endif
