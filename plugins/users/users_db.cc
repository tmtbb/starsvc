//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "users/users_db.h"
#include "basic/basic_util.h"

namespace users_logic {

UsersDB::UsersDB(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

UsersDB::~UsersDB() {
  if (mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
}

bool UsersDB::CheckAccountExist(const std::string& phone) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  //call actuals.proc_CheckUserIsExist('18668163052')
  sql = "call proc_CheckUserIsExist('" + phone + "');";

  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallCheckAccountExist);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);
  int32 result = 0;
  r = info_value->GetInteger(L"result", &result);
  r = (r && result == 0) ? true : false; /*0表示不存在未注册*/
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool UsersDB::RegisterAccount(const std::string& phone_num,
                              const std::string& passwd, const int32 type,
                              int64& uid, int32& result, const std::string &agentid, 
				const std::string &recommend, const int64 memberid) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  int64 big_type = type;
  //call actuals.proc_RegisterAccount('18668169052','1234124123')
  sql = "call proc_RegisterAccount('" + phone_num + "','" + passwd + "',"
      + base::BasicUtil::StringUtil::Int64ToString(big_type)  + ","
      + base::BasicUtil::StringUtil::Int64ToString(memberid) 
      + ",'" + agentid + "','" + recommend
      + "');";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallRegisterAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  r = info_value->GetBigInteger(L"uid", &uid);
  r = info_value->GetInteger(L"result", &result);
  r = (r && uid > 0) ? true : false;
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool UsersDB::GetUserInfo(const int64 uid, const std::string& ip,
                          swp_logic::UserInfo& userinfo) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

  ////call actuals.proc_GetUserinfo(68,'127.0.0.1')
  sql = "call proc_GetUserinfo("
      + base::BasicUtil::StringUtil::Int64ToString(uid) + ",'" + ip + "')";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallLoginAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  userinfo.ValueSerialization(info_value);
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool UsersDB::LoginAccount(const std::string& phone_num,
                           const std::string& passwd, const std::string& ip,
                           swp_logic::UserInfo& user) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

  //call actuals.proc_LoginAccount('18668169052','4bcf73028a526f5ae6899759ab332c3d3b173855bef3b22b19224cd5233d39c0','127.0,0.1')
  sql = "call proc_LoginAccount('" + phone_num + "','" + passwd + "','" + ip
      + "')";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallLoginAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  user.ValueSerialization(info_value);
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool UsersDB::AccountBalance(const int64 uid, double & balance) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

//call actuals.proc_AccountBalance(68)
  sql = "call proc_AccountBalance("
      + base::BasicUtil::StringUtil::Int64ToString(uid) + ");";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallAccountBalance);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  r = info_value->GetReal(L"balance", &balance);
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

void UsersDB::CallLoginAccount(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[0]));
      if (rows[1] != NULL)
        info_value->SetString(L"member_id", rows[1]);
      if (rows[2] != NULL)
        info_value->SetReal(L"balance", atof(rows[2]));
      if (rows[3] != NULL)
        info_value->SetString(L"phone", rows[3]);
      if (rows[4] != NULL)
        info_value->SetInteger(L"type", atoi(rows[4]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void UsersDB::CallRegisterAccount(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetInteger(L"result", atoi(rows[0]));
      if (rows[1] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[1]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void UsersDB::CallAccountBalance(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetReal(L"balance", atof(rows[0]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void UsersDB::CallCheckAccountExist(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetInteger(L"result", atoi(rows[0]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}


}  // namespace history_logic

