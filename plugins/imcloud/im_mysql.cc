
#include "imcloud/im_mysql.h"

#include <mysql/mysql.h>
#include <sstream>

#include "storage/data_engine.h"
#include "comm/comm_head.h"

#include "logic/logic_comm.h"
#include "basic/basic_util.h"

namespace im_mysql {

Im_Mysql::Im_Mysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

Im_Mysql::~Im_Mysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}
void Im_Mysql::Callgetuserinfofromaccid(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();

  base_logic::DictionaryValue *userinfo = new base_logic::DictionaryValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetString(L"name", rows[0]);
	  if (rows[1] != NULL)
        userinfo->SetString(L"head", rows[1]);
	  if (rows[2] != NULL)
        userinfo->SetString(L"accid", rows[2]);
	  if (rows[3] != NULL)
        userinfo->SetString(L"type", rows[3]);
      dict->Set(L"resultvalue", (base_logic::Value *) (userinfo));
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}
bool Im_Mysql::getuserinfofromaccid(const std::string& accid,std::string& head,std::string& name,
									std::string& type){
	bool r = false;
	DicValue dic;
	std::string sql;
	sql = "call star_getcloudinfofromaccid('"
	  + accid 
	  +  "');";
	dic.SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (&dic),
	                            Callgetuserinfofromaccid);
	if (!r || dic.empty()) {
	  return false;
	}

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	bool r1 = dic.GetDictionary(L"resultvalue", &ret);
	bool r2 = ret->GetString(L"head", &head);
	bool r3 = ret->GetString(L"name", &name);
	bool r4 = ret->GetString(L"type",&type);
	if(r1 && r2 &&r3 && r4){
			return true;
	}
	return false;
}
//写用户信息
int32 Im_Mysql::SetUserInfo(const std::string& phonenum,std::string accid,std::string token,
                        DicValue* dic){
  int32 err = 0;
  bool r = false;
  do {
    std::string sql;

    sql = "call star_addcloudinfo('"
      + phonenum +  "','"
      + accid + "','"
      + token + "'" + ");";

    //std::string m_sql = "call star_addcloudinfo(123,1770640,'m_name','m_accid')";
    dic->SetString(L"sql", sql);
    LOG_DEBUG2("%s", sql.c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),NULL);
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
//读用户信息
int32 Im_Mysql::GetStaticInfo(int64 phonenum,  DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::string sql;
    sql = "call star_getcloudinfo("
      + base::BasicUtil::StringUtil::Int64ToString(phonenum)
      +  ");";
    dic->SetString(L"sql", sql);
    LOG_DEBUG2("%s", sql.c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),
                                CallStaticSelect);
    if (!r) {
      err = SQL_EXEC_ERR;
      break;
    }
    if (dic->empty()) {
      err = PHONE_OR_PASSWD_ERR;
      break;
    }
  } while (0);
  base_logic::DictionaryValue *userinfo = NULL;
  dic->GetDictionary(L"userinfo", &userinfo);
  std::string username;
  std::string accid;
  std::string tokenvalue;
  userinfo->GetString(L"username", &username);
  userinfo->GetString(L"accid", &accid);
  userinfo->GetString(L"tokenvalue", &tokenvalue);
  LOG_DEBUG2("li============%s,%s",username.c_str(),accid.c_str(),tokenvalue.c_str());
  return err;
}
void Im_Mysql::CallStaticSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();

  base_logic::DictionaryValue *userinfo = new base_logic::DictionaryValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetString(L"username", rows[0]);
      if (rows[1] != NULL)
        userinfo->SetString(L"accid", rows[1]);
      if (rows[2] != NULL)
        userinfo->SetString(L"tokenvalue", rows[2]);
      dict->Set(L"userinfo", (base_logic::Value *) (userinfo));
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool Im_Mysql::gettalkingtimes(std::string& phone,std::string& starcode,int64 &times,
									  std::string& accid,std::string& faccid){
	bool r = false;
	DicValue dic;
	std::string sql;
	sql = "call proc_gettalkingtimes('"
	  + phone 
	  + "','" + starcode 
	  +  "');";
	dic.SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (&dic),
	                            Callgettalkingtimes);
	if (!r || dic.empty()) {
	  return false;
	}

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	bool r1 = dic.GetDictionary(L"resultvalue", &ret);
	bool r2 = ret->GetBigInteger(L"times", &times);
	bool r3 = ret->GetString(L"accid", &accid);
	bool r4 = ret->GetString(L"faccid", &faccid);
	if(r1 && r2 &&r3 && r4&& times > 0){
			return true;
	}
	return false;
}
void Im_Mysql::Callgettalkingtimes(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();

  base_logic::DictionaryValue *userinfo = new base_logic::DictionaryValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetBigInteger(L"times", atoi(rows[0]));
	  if (rows[1] != NULL)
        userinfo->SetString(L"accid", rows[1]);
	  if (rows[2] != NULL)
        userinfo->SetString(L"faccid", rows[2]);
      dict->Set(L"resultvalue", (base_logic::Value *) (userinfo));
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool Im_Mysql::delorderrecord(std::string& phone,std::string& starcode){
	bool r = false;
	DicValue dic;
	std::string sql;
	sql = "call proc_delorderstar('"
	  + phone 
	  + "','" + starcode 
	  +  "');";
	dic.SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (&dic),
	                            Callreducetalkingtimes);
	if (!r || dic.empty()) {
	  return false;
	}

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	int64 result;
	bool r1 = dic.GetDictionary(L"resultvalue", &ret);
	bool r2 = ret->GetBigInteger(L"result", &result);
	if(r1 && r2){
		if(result == 1)
			return true;
	}
	return false;
}

bool Im_Mysql::ReduceTalkingtimes(std::string& phone,std::string& starcode,int64 amount, 
	                                int64& ownseconds,std::string& accid,
                                  std::string& faccid, int64& result){
	bool r = false;
	DicValue dic;
	std::string sql;
	sql = "call proc_reducetalkingtimes('"
	  + phone 
	  + "','" + starcode 
	  + "','" + base::BasicUtil::StringUtil::Int64ToString(amount) 
	  +  "');";
	dic.SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (&dic),
	                            Callreducetalkingtimes);
	if (!r || dic.empty()) {
	  return false;
	}

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	bool r1 = dic.GetDictionary(L"resultvalue", &ret);
	bool r2 = ret->GetBigInteger(L"result", &result);
	bool r3 = ret->GetBigInteger(L"ownseconds", &ownseconds);
	bool r4 = ret->GetString(L"accid", &accid);
	bool r5 = ret->GetString(L"faccid", &faccid);
	if(r1 && r2 && r3 && r4 && r5){
		if(result == 1)
			return true;
	}
	return false;
}
void Im_Mysql::Callreducetalkingtimes(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
	  (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  if (rows[0] != NULL){
	  		ret->SetBigInteger(L"result", atoi(rows[0]));
	  }
	  if (rows[1] != NULL){
	  		ret->SetBigInteger(L"ownseconds", atoi(rows[1]));
	  }
	  if (rows[2] != NULL){
        ret->SetString(L"accid", rows[2]);
    }
	  if (rows[3] != NULL){
        ret->SetString(L"faccid", rows[3]);
    }
	  dict->Set(L"resultvalue", (base_logic::Value *) (ret));
	}
	}
	else {
		LOG_ERROR ("Callreducetalkingtimes count < 0");
	}
	dict->Remove(L"sql", &value);
}


}

