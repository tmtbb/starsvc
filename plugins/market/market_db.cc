
#include <mysql/mysql.h>
#include <sstream>

#include "storage/data_engine.h"
#include "comm/comm_head.h"

#include "logic/logic_comm.h"
#include "basic/basic_util.h"
#include "market_db.h"

namespace market_mysql {

Market_Mysql::Market_Mysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

Market_Mysql::~Market_Mysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}

bool Market_Mysql::getstarbrief(const std::string& code,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getstarbrief('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	LOG_MSG2("====sql=====================%s",sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetstarbrief);
	if (!r) {
	  return false;
	}
	base_logic::DictionaryValue* result = NULL;
	r = dic->GetDictionary(L"resultvalue",&result);
	std::string introduction;
	std::string expeience;
	std::string achievement;
	r = result->GetString(L"introduction",&introduction);
	r = result->GetString(L"expeience",&expeience);
	r = result->GetString(L"achievement",&achievement);
	if(r){
	    ret.SetString(L"introduction",introduction);
		ret.SetString(L"expeience",expeience);
		ret.SetString(L"achievement",achievement);
		return true;
	}
	return false;
}
void Market_Mysql::callgetstarbrief(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	//base_logic::ListValue *list = new base_logic::ListValue();
	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  if (rows[0] != NULL){
			ret->SetString(L"introduction", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"expeience", rows[1]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"achievement", rows[2]);
		}
	  //list->Append((base_logic::Value *) (ret));
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (ret));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getmarketstarlist(int64& type,DicValue &ret,int64& startnum,int64& endnum){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getmarkstarlist("
		  + base::BasicUtil::StringUtil::Int64ToString(type)
		  + ")";
	LOG_MSG2("=============sql==========%s,startnum:%d,endnum:%d",sql.c_str(),startnum,endnum);
	dic->SetString(L"sql", sql);
	dic->SetBigInteger(L"startnum",startnum);
	dic->SetBigInteger(L"endnum",endnum);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetmarketstarlist);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
	    ret.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}
void Market_Mysql::callgetmarketstarlist(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int64 startnum,endnum;
	int64 m_count = 1;
	dict->GetBigInteger(L"startnum",&startnum);
	dict->GetBigInteger(L"endnum",&endnum);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"code", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"name", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetString(L"accid", rows[2]);
		}
	  if (rows[3] != NULL){
			ret->SetBigInteger(L"gender", atoi(rows[3]));
		}
	  if (rows[4] != NULL){
			ret->SetBigInteger(L"type", atoi(rows[4]));
		}
	  if(m_count <= endnum && startnum <= m_count){
		list->Append((base_logic::Value *) (ret));
	  }
	  m_count++;
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getmarkettypes(DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getmarkettypes()";
	dic->SetString(L"sql", sql);

	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetmarkettypes);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
	    ret.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}
void Market_Mysql::callgetmarkettypes(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetBigInteger(L"type", atoi(rows[0]));
		}
	  if (rows[1] != NULL){
			ret->SetString(L"name", rows[1]);
		}
	  list->Append((base_logic::Value *) (ret));
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getstarnews(const std::string& code,const std::string& name,DicValue &ret_result,
								   int64& startnum,int64& endnum,int64& all){
	bool r = false;
    DicValue* dic = new DicValue();
	std::string sql;
	if(all == 0){
	  sql = "call proc_getstarnewsinfo('"
	  + code + "','"
	  + name +  "');";
	}else{
	  sql = "call proc_getallstarnewsinfo()";
	  dic->SetBigInteger(L"startnum", startnum);
	  dic->SetBigInteger(L"endnum", endnum);
	}
	
	dic->SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),Callgetstarnewsinfo);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
		
	    ret_result.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}
bool Market_Mysql::getorderstarinfo(const std::string& code,const std::string& phone,DicValue &ret_result){
	bool r = false;
    DicValue* dic = new DicValue();
	std::string sql;
	sql = "call proc_getorderstarinfo('"
	  + code + "','"
	  + phone +  "');";

	dic->SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),Callgetorderstarinfo);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
		
	    ret_result.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}

void Market_Mysql::Callgetstarnewsinfo(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
	  (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int64 startnum;
	dict->GetBigInteger(L"startnum",&startnum);
	int64 endnum;
	dict->GetBigInteger(L"endnum",&endnum);
	LOG_MSG2("starnum = %d,endnum = %d",startnum,endnum);
	if (num > 0) {

	int count = 1;
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetBigInteger(L"id", atoi(rows[0]));
		}
	  if (rows[1] != NULL){
			ret->SetString(L"starname", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetString(L"starcode", rows[2]);
		}
	  if (rows[3] != NULL){
			ret->SetString(L"showpic_url", rows[3]);
		}
	  if (rows[4] != NULL){
			ret->SetString(L"subject_name", rows[4]);
		}
	  if (rows[5] != NULL){
			ret->SetString(L"link_url", rows[5]);
		}
	  if (rows[6] != NULL){
			ret->SetString(L"remarks", rows[6]);
		}
	  if (rows[7] != NULL){
			ret->SetString(L"times", rows[7]);
		}
	  if(count >= startnum && count <= endnum){
			list->Append((base_logic::Value *) (ret));
	  }
	  count++;
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

void Market_Mysql::Callgetorderstarinfo(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
	  (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"starname", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"faccid", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetString(L"starcode", rows[2]);
		}
	  if (rows[3] != NULL){
			ret->SetBigInteger(L"ownseconds", atoi(rows[3]));
		}
	  list->Append((base_logic::Value *) (ret));
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}
bool Market_Mysql::getbannerinfo(const std::string& code,DicValue &ret_result,int64 all){
	bool r = false;
    DicValue* dic = new DicValue();
	std::string sql;
    if(all==0){
		sql = "call proc_getbannerinfo('"
	  + code +  "');";
	}else
		sql = "call proc_getbannerinfoall()";
	
    
	dic->SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),Callgetbannerinfo);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
	    ret_result.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}
void Market_Mysql::Callgetbannerinfo(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
	  (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"code", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"pic_url", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetBigInteger(L"type", atoi(rows[2]));
		}
	  if (rows[3] != NULL){
			ret->SetString(L"name", rows[3]);
		}
	  list->Append((base_logic::Value *) (ret));
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getstarinfo(const std::string& code,const std::string& phone,DicValue &ret_result,int64 all){
	bool r = false;
    DicValue* dic = new DicValue();
	std::string sql;
    if(all==0){
		sql = "call proc_getstarinfo('"
	  + code + "','"
	  + phone +  "');";
	}else
		sql = "call proc_getstarinfoall()";
	
    
	dic->SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),Callgetinfo);
	if (!r) {
	  return false;
	}
	int64 result;
	base_logic::ListValue *listvalue;
	r = dic->GetList(L"resultvalue",&listvalue);
	if(r && listvalue->GetSize()>0){
	    ret_result.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
}
void Market_Mysql::Callgetinfo(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine =
	  (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
    base_logic::ListValue *list = new base_logic::ListValue();
	
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
	  		ret->SetString(L"code", rows[0]);
	  	}
	  if (rows[1] != NULL){
	  		ret->SetString(L"phone", rows[1]);
	  	}
	  if (rows[2] != NULL){
	  		ret->SetString(L"name", rows[2]);
	  	}
	  if (rows[3] != NULL){
	  		ret->SetBigInteger(L"gender", atoi(rows[3]));
	  	}
	  if (rows[4] != NULL){
	  		ret->SetString(L"brief", rows[4]);
	  	}
	  if (rows[5] != NULL){
	  		ret->SetBigInteger(L"price", atoi(rows[5]));
	  	}
	  if (rows[6] != NULL){
	  		ret->SetString(L"accid", rows[6]);
	  	}
	  if (rows[7] != NULL){
	  		ret->SetString(L"pic_url", rows[7]);
	  	}
	  //dict->Set(L"resultvalue", (base_logic::Value *) (ret));
	  list->Append((base_logic::Value *) (ret));
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::addstarinfo(const std::string& code,
  						const std::string& phone,
  						const std::string& name,
  						const int64 gender,
  						const std::string& brief_url,
  						const double price,
  						const std::string& accid,
  						const std::string& picurl){
	bool r = false;
    DicValue* dic = new DicValue();
	std::string sql;

	sql = "call proc_addstarinfo('"
	  + code + "','"
	  + phone + "','"
	  + name + "','"
	  + base::BasicUtil::StringUtil::Int64ToString(gender) + "','"
	  + brief_url + "','"
	  + base::BasicUtil::StringUtil::DoubleToString(price) + "','"
	  + accid + "','" + picurl + "');";

	dic->SetString(L"sql", sql);
	LOG_DEBUG2("%s", sql.c_str());
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),Callpublicback);
	if (!r) {
	  return false;
	}
	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	int64 result;
	dic->GetDictionary(L"resultvalue",&ret);
	r = ret->GetBigInteger(L"result", &result);
	if(r && result == 1){
			return true;
	}
	return false;
}
void Market_Mysql::Callpublicback(void* param, base_logic::Value* value){
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
	  		dict->Set(L"resultvalue", (base_logic::Value *) (ret));
	  	}
	}
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}


}

