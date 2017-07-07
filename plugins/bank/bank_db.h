//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_BANK_DB_H_
#define SWP_BANK_DB_H_

#include <string>
#include <list>
#include "bank/bank_info.h"
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace bank_logic {

class BankDB {
 public:
  BankDB(config::FileConfig* config);
  ~BankDB();

 public:
  bool OnGetBankCard(const int64 uid,
                         std::map<int32, bank_logic::BankCard>& bank_map);

  bool OnGetBankCard(const int64 uid, std::list<bank_logic::BankCard>& bank_list);

  bool OnBindBankCard(const int64 uid, const std::string& card_no, 
                      const std::string& name,
                      bank_logic::BankCard& card); 
 
  bool OnUnbindBankCard(const int64 uid, const int32 bankcard_id);

  
  bool OnGetBankCardInfo(const std::string& card_no, int32& bank_id, std::string& card_name, std::string& bank_name);

  bool OnWithDrawCash(const int64 uid, const int64 wid, const int64 card_id, 
                      const double money, const double charge, int32& result, double& balance);
 
public:
  static void CallGetBankCard(void* param, base_logic::Value* value);

  static void CallBindBankCard(void* param, base_logic::Value* value);

  static void CallUnbindBankCard(void* param, base_logic::Value* value);

  static void CallBankCardInfo(void* param, base_logic::Value* value);  
 
  static void CallWithDrawCash(void* param, base_logic::Value* value); 
private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace bank_logic

#endif /* SWP_HISTORY_DB_H_ */
