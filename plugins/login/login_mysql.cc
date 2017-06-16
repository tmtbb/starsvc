// Copyright (c) 2016 The tourism Authors. All rights reserved.
// login_mysql.cc

#include "login/login_mysql.h"

#include <mysql/mysql.h>
#include <sstream>

#include "storage/data_engine.h"
#include "comm/comm_head.h"

#include "login/errno.h"
#include "logic/logic_comm.h"

namespace login {

LoginMysql::LoginMysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

LoginMysql::~LoginMysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}

int32 LoginMysql::RegisterInsertAndSelect(std::string phone_num,
                                          std::string passwd, DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_RegisterInsertAndSelect('" << phone_num << "','" << passwd
        << "')";
    dic->SetString(L"sql", ss.str());
    LOG_DEBUG2("%s", ss.str().c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),
                                CallRegisterInsertAndSelect);
    //注册一定有结果返回
    if (!r || dic->empty()) {
      err = SQL_EXEC_ERR;
      break;
    }
    //    dic->GetBigInteger(L"result")
  } while (0);
  return err;
}

int32 LoginMysql::UserLoginSelect(std::string phone, std::string& client_ip,
                                  DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserLoginSelect('" << phone << "','" << client_ip << "')";
    dic->SetString(L"sql", ss.str());
    LOG_DEBUG2("%s", ss.str().c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),
                                CallUserLoginSelect);
    if (!r) {
      err = SQL_EXEC_ERR;
      break;
    }
    if (dic->empty()) {
      err = PHONE_OR_PASSWD_ERR;
      break;
    }
  } while (0);
  return err;
}

int32 LoginMysql::ChangePasswdUpdate(std::string phone, std::string passwd) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_ChangePasswdUpdate('" << phone << "','" << passwd << "');";
    DicValue dic;
    dic.SetString(L"sql", ss.str());
    LOG_DEBUG2("%s", ss.str().c_str()); LOG_DEBUG2("%s", ss.str().c_str());
    r = mysql_engine_->WriteData(0, (base_logic::Value*) (&dic));
    if (!r)
      err = SQL_EXEC_ERR;
  } while (0);
  return err;
}

int32 LoginMysql::CheckPasswdSelect(int64 uid, std::string pass, int64 type,
                                    DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_CheckPasswdSelect(" << uid << ",'" << pass << "'," << type
        << ")";
    LOG_DEBUG2("%s", ss.str().c_str());
    /*    r = mysql_engine_->ReadData(ss.str(), dic, CallCheckPasswdSelect);
     if (!r) {
     err = SQL_EXEC_ERROR;
     break;
     }*/
  } while (0);
  return err;
}

/*int32 LoginMysql::ChangePasswdSelect(int64 uid, std::string oldpass,
 std::string newpass, int64 ctype,
 int64 ptype, DicValue* dic) {
 int32 err = 0;
 bool r = false;
 do {
 std::stringstream ss;
 ss << "call proc_ChangePasswdSelect(" << uid << ",'" << oldpass << "','"
 << newpass << "'," << ctype << "," << ptype << ")";
 LOG(INFO)<< "sql:" << ss.str();
 r = mysql_engine_->ReadData(ss.str(), dic, CallChangePasswdSelect);
 if (!r) {
 err = SQL_EXEC_ERROR;
 break;
 }
 } while (0);
 return err;
 }*/

void LoginMysql::CallRegisterInsertAndSelect(void* param,
                                             base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result", atoll(rows[0]));
        //用户已注册过
        if (atoi(rows[0]) == 0)
          break;
      }
      if (rows[1] != NULL)
        dict->SetBigInteger(L"id", atoll(rows[1]));
    }
  } else {
    LOG_ERROR ("CallRegisterInsertAndSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

void LoginMysql::CallUserLoginSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* userinfo = new DicValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetBigInteger(L"id", atoll(rows[0]));
      if (rows[1] != NULL)
        userinfo->SetString(L"screenName", rows[1]);
      if (rows[2] != NULL)
        userinfo->SetCharInteger(L"gender", atoi(rows[2]));
      dict->Set(L"userinfo", userinfo);
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

void LoginMysql::CallCheckPasswdSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result_", atoll(rows[0]));
      }
    }
  } else {
    LOG_ERROR ("CallCheckPasswdSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

void LoginMysql::CallChangePasswdSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL) {
        dict->SetBigInteger(L"result_", atoll(rows[0]));
      }
    }
  } else {
    LOG_ERROR ("CallChangePasswdSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

}  // namespace login

