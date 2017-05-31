//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_HISTORY_DB_H_
#define SWP_HISTORY_DB_H_

#include <string>
#include <list>
#include "history/history_info.h"
#include "storage/data_engine.h"
#include "logic/swp_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace history_logic {

class HistoryDB {
 public:
  HistoryDB(config::FileConfig* config);
  ~HistoryDB();

 public:
  bool OnHistroyTradesRecord(std::list<swp_logic::TradesPosition>* list);

  bool OnHistroyRechargeRecord(std::list<swp_logic::Recharge>* list);
  bool OnOwnStarRecord(std::list<swp_logic::TOwnStar>* list);

  bool OnHistroyWithDraw(std::list<swp_logic::Withdrawals>* list);

  bool OnHandleHistroyTrades(const int64 tid, const int64 uid, const int32 type,
                             const int32 handle, int32& r_handle);

  bool OnHandleWithdraw(const int64 uid, const int64 wid, const int64 bank_id,
                        const double price, const double charge,
                        const std::string& comment, int32& result,
                        swp_logic::Withdrawals& withdrawals);

 public:
  static void CallHistroyTradesRecord(void* param, base_logic::Value* value);

  static void CallHistroyRechargeRecord(void* param, base_logic::Value* value);

  static void CallOwnStarRecord(void* param, base_logic::Value* value);

  static void CallHistorWithDrawRecord(void* param, base_logic::Value* value);

  static void CallHandleHistroyTrades(void* param, base_logic::Value* value);

  static void CallHandleWithdraw(void* param, base_logic::Value* value);

 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace history_logic

#endif /* SWP_HISTORY_DB_H_ */
