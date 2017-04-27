// Copyright (c) 2016 The tourism Authors. All rights reserved.
// login_mysql.h

#ifndef PLUGINS_LOGIN_LOGIN_MYSQL_H_
#define PLUGINS_LOGIN_LOGIN_MYSQL_H_

#include "config/config.h"
#include "basic/basictypes.h"

#include "pub/storage/data_engine.h"

#include "pub/net/typedef.h"

namespace login {

class LoginMysql {
 public:
  LoginMysql(config::FileConfig* config);
  ~LoginMysql();

 public:
  int32 RegisterInsertAndSelect(std::string phone_num, std::string passwd,
                                DicValue* dic);
  int32 UserLoginSelect(std::string phone_num, std::string& client_ip,
                        DicValue* dic);

  int32 UserDetailSelect(std::string uids, DicValue* dic);
  int32 ChangePasswdUpdate(std::string phone, std::string passwd);
  int32 CheckPasswdSelect(int64 uid, std::string pass, int64 type,
                          DicValue* dic);

  static void CallUserLoginSelect(void* param, base_logic::Value* value);
  static void CallUserDetailSelect(void* param, base_logic::Value* value);
  static void CallRegisterInsertAndSelect(void* param,
                                          base_logic::Value* value);
  static void CallCheckPasswdSelect(void* param, base_logic::Value* value);
  static void CallChangePasswdSelect(void* param, base_logic::Value* value);
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace login

#endif  // PLUGINS_LOGIN_LOGIN_MYSQL_H_
