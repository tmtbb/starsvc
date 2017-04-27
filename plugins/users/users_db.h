//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_USERS_DB_H_
#define SWP_USERS_DB_H_

#include <string>
#include <list>
#include "storage/data_engine.h"
#include "logic/swp_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace users_logic {

class UsersDB {
 public:
  UsersDB(config::FileConfig* config);
  ~UsersDB();

 public:
  bool RegisterAccount(const std::string& phone_num, const std::string& passwd,
                       const int32 type,int64& uid, int32& result, const std::string &agentid, const std::string &recommend, const int64 memberid);

  bool LoginAccount(const std::string& phone_num, const std::string& passwd,
                    const std::string& ip, swp_logic::UserInfo& user);

  bool CheckAccountExist(const std::string& phone);

  bool AccountBalance(const int64 uid, double & balance);

  bool GetUserInfo(const int64 uid, const std::string& ip,
                   swp_logic::UserInfo& userinfo);

 public:
  static void CallRegisterAccount(void* param, base_logic::Value* value);

  static void CallLoginAccount(void* param, base_logic::Value* value);

  static void CallAccountBalance(void* param, base_logic::Value* value);

  static void CallCheckAccountExist(void* param, base_logic::Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace history_logic

#endif /* SWP_HISTORY_DB_H_ */
