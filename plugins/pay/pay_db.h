//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_PAY_DB_H_
#define SWP_PAY_DB_H_

#include <string>
#include <list>
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace pay_logic {

class PayDB {
 public:
  PayDB(config::FileConfig* config);
  ~PayDB();

 public:
  bool OnCreateRechargeOrder(const int64 uid, const int64 rid, const double price,
                             const int32 rtype);

  bool OnUpdateRechargeOrder(const int64 uid, const int64 rid, const int32 status,
                             const int32 rtype);

  bool OnUpdateCallBackRechargeOrder(const int64 rid,const double price,
                                     const std::string& transaction_id,
                                     const int32 astatus,int64& uid,
                                     double& balance);
 
  bool OnCreateUnionWithDraw(const int64 uid, const int64 rid, const double price);

  bool OnCheckPayPwd(const int64 uid, std::string& pwd, int32& flag); 
 
 public:
  static void CallCreateRechargeOrder(void* param, base_logic::Value* value);
  static void CallUpdateCallBackRechargeOrder(void* param, base_logic::Value* value);
  static void CallCheckPayPwd(void* param, base_logic::Value* value);
  static void CallUnionWithdrow(void* param, base_logic::Value* value); 
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace history_logic

#endif /* SWP_HISTORY_DB_H_ */
