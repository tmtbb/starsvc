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
  r = (r && result == 1) ? true : false; /*0表示不存在未注册*/
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}
bool UsersDB::WXBindAccount(const std::string& phone_num,
                              const std::string& passwd, const int32 type,
                              int64& uid, int32& result, const std::string &openid, 
			      const std::string &nick_name, const std::string &head_url,
			      const std::string &agent_id, const std::string &recommend,
			      const std::string &device_id, const int64 member_id) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  int64 big_type = type;
  //call actuals.proc_RegisterAccount('18668169052','1234124123')
  sql = "call proc_WXBindAccount('" + phone_num + "','" + passwd + "',"
      + base::BasicUtil::StringUtil::Int64ToString(big_type)  + ","
      + base::BasicUtil::StringUtil::Int64ToString(member_id)  + ",'"
      + head_url + "','" + nick_name + "','" + openid 
      + "','" + agent_id + "','" + recommend
      + "','" + device_id + "');";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallRegisterAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  r = info_value->GetBigInteger(L"uid", &uid);
  r = info_value->GetInteger(L"result", &result);
  LOG_ERROR2("uid %d,result %d",uid,result);
  r = (r && result > 0) ? true : false;
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}
bool UsersDB::LoginWiXin(const std::string& open_id,
                           const std::string &device_id,
			   const std::string& ip,base_logic::DictionaryValue &ret) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

  //call actuals.proc_LoginAccount('18668169052','4bcf73028a526f5ae6899759ab332c3d3b173855bef3b22b19224cd5233d39c0','127.0,0.1')
  sql = "call proc_LoginWiXin('" + open_id + "','"+ device_id + "','" + ip
      + "')";
  LOG_MSG2("================sql==================%s",sql.c_str());
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallLoginwxAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  std::string phone;
  int64 type,uid;
  r = info_value->GetString(L"phone", &phone);
  if(!info_value->GetBigInteger(L"type",&type))
  		return false;
  if(!info_value->GetBigInteger(L"id",&uid))
  		return false;
  r = (r && phone.length() > 1) ? true : false;
  if (!r)
    return false;

  base_logic::DictionaryValue *tmp = new base_logic::DictionaryValue();
  tmp->SetString(L"phone",phone);
  tmp->SetBigInteger(L"type",type);
  tmp->SetBigInteger(L"id",uid);

  ret.Set(L"userinfo",(base_logic::Value*)tmp);
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}
void UsersDB::CallLoginwxAccount(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetString(L"phone", rows[0]);
	  if (rows[1] != NULL)
        info_value->SetBigInteger(L"type", atoi(rows[1]));
	  if (rows[2] != NULL)
        info_value->SetBigInteger(L"id", atoi(rows[2]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

bool UsersDB::UserChangePasswd(const std::string& phone_num,const std::string& oldpasswd,
  								const std::string& newpasswd){
	bool r = false;
	std::string sql;
	base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
	
	sql = "call proc_changepasswd('" + phone_num+ "','"+ oldpasswd + "','" + newpasswd
      + "')";
	LOG_ERROR2("sql = %s",sql.c_str());
	dict->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallChangePasswd);

	base_logic::DictionaryValue *info_value = NULL;
	dict->GetDictionary(L"resultvalue", &info_value);
	int32 result;
  	r = info_value->GetInteger(L"result", &result);
	if(!r || (result<1))
		return false;
	return true;
}
void UsersDB::CallChangePasswd(void* param, base_logic::Value* value) {
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
      + ",'" + agentid + "','" + recommend + "'," + "''"
      + ");";
  LOG_ERROR2("sqlcommand = %s",sql.c_str());
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallRegisterAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  //r = info_value->GetBigInteger(L"uid", &uid);
  bool r2 = info_value->GetInteger(L"result", &result);
  LOG_ERROR2("result : %d, r2 : %d",result,r2);
  r = (r2 && result > 0) ? true : false;

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
  LOG_ERROR2("sql==================%s",sql.c_str());
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallLoginAccount);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);

  int64 uid;
  int32 type;
  std::string phone;
  if(info_value->GetBigInteger(L"uid", &uid)){
    if(uid <= 0)
      return false;
    user.set_uid(uid);
  }
  else
  {
    return false;
  }
  if(info_value->GetInteger(L"type", &type))
  		user.set_type(type);
  if(info_value->GetString(L"phone",&phone))
  		user.set_phone_num(phone);
  
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool UsersDB::AccountBalance(const int64 uid, double & balance, std::string &pwd) {
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
  r = info_value->GetString(L"passwd", &pwd);
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool UsersDB::AccountRealNameInfo(const int64 uid, std::string &realname,std::string &id_card) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

//call actuals.proc_AccountBalance(68)
  sql = "call proc_AccountRealInfo("
      + base::BasicUtil::StringUtil::Int64ToString(uid) + ");";

  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallAccountRealName);
  if (!r)
    return false;

  dict->GetDictionary(L"resultvalue", &info_value);
  double balance = 0.0;
  //r = info_value->GetReal(L"balance", &balance);
  r = info_value->GetString(L"realname", &realname);
  r = info_value->GetString(L"id_card", &id_card);
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
    if (rows[0] != NULL){
        info_value->SetBigInteger(L"uid", atoll(rows[0]));
        //info_value->SetInteger(L"uid", atoi(rows[0]));
    }
	if (rows[1] != NULL){
        info_value->SetString(L"phone", rows[1]);
    }
  	if (rows[2] != NULL){
        info_value->SetInteger(L"type", atoi(rows[2]));
    }
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
      if (rows[1] != NULL)
        info_value->SetString(L"passwd", (rows[1]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void UsersDB::CallAccountRealName(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetString(L"realname", (rows[0]));
      if (rows[1] != NULL)
        info_value->SetString(L"id_card", (rows[1]));
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
bool UsersDB::ResetAccount(const std::string& phone_num,const std::string& passwd){
	bool r = false;
	std::string sql;
	base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
	
	sql = "call proc_resetpasswd('" + phone_num+ "','" + passwd + "')";
	LOG_ERROR2("sql = %s",sql.c_str());
	dict->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallChangePasswd);

	base_logic::DictionaryValue *info_value = NULL;
	dict->GetDictionary(L"resultvalue", &info_value);
	int32 result;
  	r = info_value->GetInteger(L"result", &result);
	if(!r || (result<1))
		return false;
	return true;
}	

bool UsersDB::ModifyPwd(const int64 &uid, const std::string &newpwd)
{ 
  
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
try
{ 
  //sql = "call proc_ModifyPwd('" + phone + "','" + newpwd + "');";
  sql = "call proc_ModifyPayPwd('" + base::BasicUtil::StringUtil::Int64ToString(uid) + "','" + newpwd + "');";
  
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallRegisterAccount);
  if (!r)
    return false;
  
  dict->GetDictionary(L"resultvalue", &info_value);
  int64 ruid = 0;
  int result = 0;
  r = info_value->GetBigInteger(L"uid", &ruid);
  r = info_value->GetInteger(L"result", &result);
  r = (r && ruid > 0) ? true : false;
  
}   
catch (...)
{
    LOG_ERROR("UsersDB::ModifyPwd Error!!!" );
    r = false;
}   
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}


bool UsersDB::Certification(const int64 &uid, const std::string &idcard, const std::string &realname)
{ 
  
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
try
{ 
  //sql = "call proc_ModifyPwd('" + phone + "','" + newpwd + "');";
  sql = "call proc_Certification('" + base::BasicUtil::StringUtil::Int64ToString(uid) 
  + "','" + idcard + "','" + realname + "');";
  
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallRegisterAccount);
  if (!r)
    return false;
  
  dict->GetDictionary(L"resultvalue", &info_value);
  int64 ruid = 0;
  int result = 0;
  r = info_value->GetBigInteger(L"uid", &ruid);
  r = info_value->GetInteger(L"result", &result);
  r = (r && ruid > 0) ? true : false;
  
}   
catch (...)
{
    LOG_ERROR("UsersDB::ModifyPwd Error!!!" );
    r = false;
}   
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}
}  // namespace history_logic

