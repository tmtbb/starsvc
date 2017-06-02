//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_USERS_DB_H_
#define SWP_USERS_DB_H_

#include <string>
#include <list>
#include "storage/data_engine.h"
#include "logic/star_infos.h"
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
                    const std::string& ip, star_logic::UserInfo& user);

  bool CheckAccountExist(const std::string& phone);

  bool AccountBalance(const int64 uid, double & balance, std::string &pwd);

  bool AccountRealNameInfo(const int64 uid, std::string &realname,std::string &id_card); 
  bool GetUserInfo(const int64 uid, const std::string& ip,
                   star_logic::UserInfo& userinfo);

 public:
  static void CallRegisterAccount(void* param, base_logic::Value* value);

  static void CallLoginAccount(void* param, base_logic::Value* value);

  static void CallLoginwxAccount(void* param, base_logic::Value* value);
  
  static void CallAccountBalance(void* param, base_logic::Value* value);

  static void CallCheckAccountExist(void* param, base_logic::Value* value);

  static void CallChangePasswd(void* param, base_logic::Value* value);
  static void CallAccountRealName(void* param, base_logic::Value* value); 
  
  bool WXBindAccount(const std::string& phone_num, const std::string& passwd,
                       const int32 type,int64& uid, int32& result, const std::string &openid, const std::string &nick_name, 
		       const std::string &head_url, const std::string &agent_id, const std::string &recommend,
		       const std::string &device_id, const int64 member_id);
  bool LoginWiXin(const std::string& open_id, const std::string& device_id,
                    const std::string& ip, base_logic::DictionaryValue &ret);

  bool UserChangePasswd(const std::string& phone_num,const std::string& oldpasswd,
  								const std::string& newpasswd);

  bool ResetAccount(const std::string& phone_num,const std::string& passwd);
  bool ModifyPwd(const int64 &uid, const std::string &newpwd);
  bool Certification(const int64 &uid, const std::string &idcard, const std::string &realname);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace history_logic

#endif /* SWP_HISTORY_DB_H_ */
