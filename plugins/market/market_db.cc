
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
void Market_Mysql::callgetmarketstartransfer(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int count_ = 1;
	int64 startnum,endnum;
	dict->GetBigInteger(L"startnum",&startnum);
	dict->GetBigInteger(L"endnum",&endnum);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"name", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"head", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetReal(L"price", atof(rows[2]));
		}
	  if (rows[3] != NULL){
			ret->SetString(L"time", rows[3]);
		}
	  if(count_ >= startnum && endnum >= count_){
	  		list->Append((base_logic::Value *) (ret));
	  	}
	  count_++;
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));

	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getmarketstartransfer(const std::string& code,int64 startnum,
										int64 endnum,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getstartransferlist('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	dic->SetBigInteger(L"startnum",startnum);
	dic->SetBigInteger(L"endnum",endnum);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetmarketstartransfer);
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

void Market_Mysql::callgetmarketstarseek(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int count_ = 1;
	int64 startnum,endnum;
	dict->GetBigInteger(L"startnum",&startnum);
	dict->GetBigInteger(L"endnum",&endnum);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"name", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"head", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetReal(L"price", atof(rows[2]));
		}
	  if (rows[3] != NULL){
			ret->SetString(L"time", rows[3]);
		}
	  if(count_ >= startnum && endnum >= count_){
	  		list->Append((base_logic::Value *) (ret));
	  	}
	  count_++;
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));

	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getmarketstarseek(const std::string& code,int64 startnum,
										int64 endnum,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getstarseeklist('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	dic->SetBigInteger(L"startnum",startnum);
	dic->SetBigInteger(L"endnum",endnum);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetmarketstarseek);
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
bool Market_Mysql::addoptionstar(const std::string& phone,const std::string& starcode){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_addoptionstar('"
		  + phone + "','" + starcode
		  + "')";
	dic->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),calladdoptionstar);
	if (!r) {
	  return false;
	}
	DicValue* resultvalue = new DicValue();
	if(!(dic->GetDictionary(L"resultvalue",&resultvalue))){
		return false;
	}
	int64 result;
	r = resultvalue->GetBigInteger(L"result",&result);
	if(r && result > 0){
		return true;
	}
	return false;
}
void Market_Mysql::calladdoptionstar(void* param, base_logic::Value* value)	{
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	DicValue* dict = reinterpret_cast<DicValue*>(value);

	base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  if (rows[0] != NULL){
			ret->SetBigInteger(L"result", atoi(rows[0]));
		}
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (ret));
	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

bool Market_Mysql::getoptionstarlist(const std::string& code,int64 startnum,
									int64 endnum,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getoptionstarlist('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	dic->SetBigInteger(L"startnum",startnum);
	dic->SetBigInteger(L"endnum",endnum);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetoptionstarlist);
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
  
void Market_Mysql::callgetoptionstarlist(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int count_ = 1;
	int64 startnum,endnum;
	dict->GetBigInteger(L"startnum",&startnum);
	dict->GetBigInteger(L"endnum",&endnum);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"starcode", rows[0]);
			ret->SetReal(L"price",100.5);		//实时价格暂时写死
			ret->SetReal(L"updown",-0.34);		//价格波动暂时写死
		}
	  if (rows[1] != NULL){
			ret->SetString(L"name", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetString(L"head", rows[2]);
		}
	  if(count_ >= startnum && endnum >= count_){
	  		list->Append((base_logic::Value *) (ret));
	  	}
	  count_++;
	}
	dict->Set(L"resultvalue", (base_logic::Value *) (list));

	}
	else {
		LOG_ERROR ("CallUserLoginSelect count < 0");
	}
	dict->Remove(L"sql", &value);
}

void Market_Mysql::callgetstarachive(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"achive", rows[0]);
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
bool Market_Mysql::getstarachive(const std::string& code,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getstarachive('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetstarachive);
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

void Market_Mysql::callgetstarexperience(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"experience", rows[0]);
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
bool Market_Mysql::getstarexperience(const std::string& code,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	sql = "call proc_getstarexperience('"
		  + code
		  + "')";
	dic->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callgetstarexperience);
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

bool Market_Mysql::searchstarlist(const std::string& code,DicValue &ret){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	
	sql = "select star_name,star_code from star_starinfolist where star_name like '%"
		  + code + "%' or star_code like '%"
		  + code + "%'" + " limit 10";
	LOG_MSG2("=============sql==========%s",sql.c_str());
	dic->SetString(L"sql", sql);
	r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),callsearchstarlist);
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
void Market_Mysql::callsearchstarlist(void* param, base_logic::Value* value){
	base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
	MYSQL_ROW rows;
	int32 num = engine->RecordCount();
	base_logic::ListValue *list = new base_logic::ListValue();
	DicValue* dict = reinterpret_cast<DicValue*>(value);
	int64 startnum,endnum;

	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"name", rows[0]);
		}
	  if (rows[1] != NULL){
			ret->SetString(L"code", rows[1]);
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
	if(result == NULL)
		return false;
	std::string introduction;
	std::string expeience;
	std::string achievement;
	std::string colleage;
	std::string work;
	std::string birth;
	std::string constellation;//星座
	std::string nation;
	std::string nationality;
	std::string name;
	std::string pic1;
	std::string pic2;
	std::string pic3;
	std::string pic4;
	std::string pic5;
	if(result->GetString(L"introduction",&introduction)){
		ret.SetString(L"introduction",introduction);
	}
	if(result->GetString(L"colleage",&colleage)){
		ret.SetString(L"colleage",colleage);
	}
	if(result->GetString(L"work",&work)){
		ret.SetString(L"work",work);
	}	
	if(result->GetString(L"birth",&birth)){
		ret.SetString(L"birth",birth);
	}
	if(result->GetString(L"constellation",&constellation)){
		ret.SetString(L"constellation",constellation);
	}
	if(result->GetString(L"nation",&nation)){
		ret.SetString(L"nation",nation);
	}
	if(result->GetString(L"nationality",&nationality)){
		ret.SetString(L"nationality",nationality);
	}			
	if(result->GetString(L"name",&name)){
		ret.SetString(L"name",name);
	}
	if(result->GetString(L"pic1",&pic1)){
		ret.SetString(L"pic1",pic1);
	}
	if(result->GetString(L"pic2",&pic2)){
		ret.SetString(L"pic2",pic2);
	}
	if(result->GetString(L"pic3",&pic3)){
		ret.SetString(L"pic3",pic3);
	}
	if(result->GetString(L"pic4",&pic4)){
		ret.SetString(L"pic4",pic4);
	}
	if(result->GetString(L"pic5",&pic5)){
		ret.SetString(L"pic5",pic5);
	}
	int64 m_seconds;
	if(result->GetBigInteger(L"seconds",&m_seconds)){
		ret.SetBigInteger(L"seconds",m_seconds);
	}
	return true;
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
			ret->SetString(L"colleage", rows[1]);
		}
	  if (rows[2] != NULL){
			ret->SetString(L"work", rows[2]);
		}
	  if (rows[3] != NULL){
			ret->SetString(L"birth", rows[3]);
		}
	  if (rows[4] != NULL){
			ret->SetString(L"nation", rows[4]);
		}
	  if (rows[5] != NULL){
			ret->SetString(L"nationality", rows[5]);
		}
	  if (rows[6] != NULL){
			ret->SetString(L"name", rows[6]);
		}
	  if (rows[7] != NULL){
			ret->SetString(L"pic1", rows[7]);
		}
	  if (rows[8] != NULL){
			ret->SetString(L"pic2", rows[8]);
		}
	  if (rows[9] != NULL){
			ret->SetString(L"pic3", rows[9]);
		}
	  if (rows[10] != NULL){
			ret->SetString(L"pic4", rows[10]);
		}
	  if (rows[11] != NULL){
			ret->SetString(L"pic5", rows[11]);
		}
	  if (rows[12] != NULL){
			ret->SetBigInteger(L"seconds", atoi(rows[12]));
		}
	  if (rows[13] != NULL){
			ret->SetString(L"constellation", rows[4]);
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

bool Market_Mysql::getmarketstarlist(int64& type,DicValue &ret,int64& startnum,int64& endnum,int64 sorttype){
	bool r = false;
    DicValue* dic = new DicValue();
	
	std::string sql;
	//sql = "call proc_getmarkstarlist("
	//	  + base::BasicUtil::StringUtil::Int64ToString(type)
	//	  + ")";
	if(sorttype == 0)
		sql = "SELECT star_code,star_name,star_accid,star_gender,star_type,star_pic,updown FROM star_starinfolist WHERE star_type="
			  + base::BasicUtil::StringUtil::Int64ToString(type) + " ORDER BY updown";
	else
		sql = "SELECT star_code,star_name,star_accid,star_gender,star_type,star_pic,updown FROM star_starinfolist WHERE star_type="
			  + base::BasicUtil::StringUtil::Int64ToString(type) + " ORDER BY updown desc";
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
	double updown = rand()%20;
	double price = rand()%10;
	if (num > 0) {
	while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
	  base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
	  if (rows[0] != NULL){
			ret->SetString(L"code", rows[0]);
			ret->SetReal(L"price",price);	//暂时写死价格
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
	  if (rows[5] != NULL){
			ret->SetString(L"head", rows[5]);
		}
	  if (rows[6] != NULL){
			ret->SetReal(L"updown", atof(rows[6]));
		}
	  if(m_count <= endnum && startnum <= m_count){
	  	/*
	  	size_t index = 0;
	  	for(base_logic::ListValue::iterator it = list->begin();it != list->end();it++){
			double pri;
			base_logic::DictionaryValue* _it = (base_logic::DictionaryValue*)(*it);
			_it->GetReal(L"updown",&pri);
			//list->Append((base_logic::Value *) (ret));
			if(updown > pri)
				list->Insert(index,(base_logic::Value *) (ret));
			index++;
		}
		*/
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
	  if(startnum > 0 && endnum > 0){
		dic->SetBigInteger(L"startnum", startnum);
	  	dic->SetBigInteger(L"endnum", endnum);
	  }else
	  {
	  	return false;
	  }
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

