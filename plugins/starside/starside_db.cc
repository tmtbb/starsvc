//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "starside/starside_db.h"
#include "basic/basic_util.h"

namespace starside_logic {

StarSideDB::StarSideDB(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

StarSideDB::~StarSideDB() {
  if (mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
}

bool StarSideDB::OnProfitDetail(const int socket, const int64 session,
               const std::string &starcode, const int64 orderdate,
               double &lastprice, double &dayprice, double &profit) {
    
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  sql = "call proc_ProfitDetail("
      + base::BasicUtil::StringUtil::Int64ToString(orderdate) + ",'"
      +  starcode + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallProfitDetail);
  if (!r)
    return false;
  //int32 result;
  //dict->GetInteger(L"result", &result);
  //if (result == 1) 
  {
    dict->GetDictionary(L"resultvalue", &info_value);
    info_value->GetReal(L"yeslast_price", &lastprice);
    info_value->GetReal(L"price", &dayprice);
    info_value->GetReal(L"profit", &profit);

  }
  if (dict) {
    delete dict;
    dict = NULL;
  }
  
  return true;
}


bool StarSideDB::OnUpdStarService(const std::string &starcode,
        const int64 mid, const int64 type)
{ 
    
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  //base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  sql = "call proc_UptStarService('"
      + starcode + "',"
      + base::BasicUtil::StringUtil::Int64ToString(mid) 
      + "," + base::BasicUtil::StringUtil::Int64ToString(type) 
      + ");";
  dict->SetString(L"sql", sql);
  //r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
   //                           CallProfitDetail);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
    return false;

  {
      /*
    dict->GetDictionary(L"resultvalue", &info_value);
    info_value->GetReal(L"yeslast_price", &lastprice);
    info_value->GetReal(L"price", &dayprice);
    info_value->GetReal(L"profit", &profit);
    */

  }
  if (dict) {
    delete dict;
    dict = NULL;
  }
  
  return true;
}

bool StarSideDB::OnUpdStarMeetDate(const std::string &starcode,
        const std::string &meet_city, const std::string &stardate,
        const std::string &enddate)
{ 
    
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  //base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  sql = "call proc_UpdStarServiceMeetDate('"
      + starcode + "','" + meet_city
      + "','" + stardate + "','" + enddate 
      + "');";
  dict->SetString(L"sql", sql);
  //r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
   //                           CallProfitDetail);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
    return false;

  {
      /*
    dict->GetDictionary(L"resultvalue", &info_value);
    info_value->GetReal(L"yeslast_price", &lastprice);
    info_value->GetReal(L"price", &dayprice);
    info_value->GetReal(L"profit", &profit);
    */

  }
  if (dict) {
    delete dict;
    dict = NULL;
  }
  
  return true;
}
bool StarSideDB::OnUpdStarMeetRel(const int64 meet_id, const int64 meet_type)
{ 
    
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  //base_logic::DictionaryValue *info_value = NULL;
  std::string sql;
  sql = "call proc_UptStarMeetRel("
      + base::BasicUtil::StringUtil::Int64ToString(meet_id) + ","
      + base::BasicUtil::StringUtil::Int64ToString(meet_type) + ");";
  dict->SetString(L"sql", sql);
  //r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
   //                           CallProfitDetail);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
    return false;

  {
      /*
    dict->GetDictionary(L"resultvalue", &info_value);
    info_value->GetReal(L"yeslast_price", &lastprice);
    info_value->GetReal(L"price", &dayprice);
    info_value->GetReal(L"profit", &profit);
    */

  }
  if (dict) {
    delete dict;
    dict = NULL;
  }
  
  return true;
}

bool StarSideDB::OnHistroyServiceItem(std::list<starside_logic::ServiceItem>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetAllServiceDef()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallServiceDef);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    starside_logic::ServiceItem item;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    item.ValueSerialization(dict_result_value);
    list->push_back(item);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool StarSideDB::OnHistroyStarOwnService(std::list<starside_logic::StarOwnService>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetStarOwnService()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallHistorStarOwnService);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    starside_logic::StarOwnService item;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    item.ValueSerialization(dict_result_value);
    list->push_back(item);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool StarSideDB::OnHistroyStarMeetRel(std::list<starside_logic::StarMeetRelForFan>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetMeetRelForFans()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallHistorStarMeetRelRecord);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    starside_logic::StarMeetRelForFan item;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    item.ValueSerialization(dict_result_value);
    list->push_back(item);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool StarSideDB::OnGetOwnStarUser(std::list<starside_logic::TOwnStarUser>* list) {
    
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_OwnStarForUser()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallOwnStarUser);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    starside_logic::TOwnStarUser item;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    item.ValueSerialization(dict_result_value);
    list->push_back(item);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  
  return true;
}
bool StarSideDB::OnHistroyTransStatis(std::list<starside_logic::TranscationStatistics>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetStarSideTransStatis()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallHistorTransStatisRecord);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    starside_logic::TranscationStatistics item;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    item.ValueSerialization(dict_result_value);
    list->push_back(item);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool StarSideDB::OnHistroyRechargeRecord(std::list<star_logic::Recharge>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetStarSideRecharge()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallHistroyRechargeRecord);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    star_logic::Recharge recharge;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    recharge.ValueSerialization(dict_result_value);
    list->push_back(recharge);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool StarSideDB::OnOwnStarRecord(std::list<star_logic::TOwnStar>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetOwnStar()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallOwnStarRecord);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    star_logic::TOwnStar ownstar;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    ownstar.ValueSerialization(dict_result_value);
    list->push_back(ownstar);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}


void StarSideDB::CallHistroyRechargeRecord(void* param,
                                          base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"rid", atoll(rows[0]));

      if (rows[1] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[1]));

      if (rows[2] != NULL)
        info_value->SetReal(L"amount", atof(rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"deposit_time", rows[3]);

      if (rows[4] != NULL)
        info_value->SetInteger(L"deposit_type", atol(rows[4]));

      if (rows[5] != NULL)
        info_value->SetBigInteger(L"deposit_unit_time", atoll(rows[5]));

      if (rows[6] != NULL)
        info_value->SetInteger(L"status", atoi(rows[6]));
        
      if (rows[7] != NULL)
        info_value->SetInteger(L"recharge_type", atoi(rows[7]));

      if (rows[8] != NULL)
        info_value->SetString(L"transaction_id", (rows[8]));
      else
      {
        std::string tmp = "";
        info_value->SetString(L"transaction_id", tmp.c_str());
      }


      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}


void StarSideDB::CallOwnStarRecord(void* param,
                                          base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[0]));

      if (rows[1] != NULL)
        info_value->SetString(L"starname", rows[1]);

      if (rows[2] != NULL)
        info_value->SetString(L"faccid", (rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"starcode", rows[3]);

      if (rows[4] != NULL)
        info_value->SetBigInteger(L"ownseconds", atoll(rows[4]));

      if (rows[5] != NULL)
        info_value->SetInteger(L"appoint", atol(rows[5]));
      
      if (rows[6] != NULL)
        info_value->SetString(L"head_url", rows[6]);

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}



void StarSideDB::CallHistorTransStatisRecord(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"order_number", atoll(rows[0]));

      if (rows[1] != NULL)
        info_value->SetReal(L"order_price", atof(rows[1]));

      if (rows[2] != NULL)
        info_value->SetInteger(L"order_count", atoi(rows[2]));

      if (rows[3] != NULL)
        info_value->SetReal(L"price", atof(rows[3]));


      if (rows[4] != NULL)
      {
        try{
            info_value->SetBigInteger(L"starcode", atoll(rows[4]));
        }
        catch (...) { 
            info_value->SetBigInteger(L"starcode", 0);
        }
        info_value->SetString(L"starcode2", (rows[4]));
      }

      if (rows[5] != NULL)
        info_value->SetReal(L"max_price", atof(rows[5]));

      if (rows[6] != NULL)
        info_value->SetReal(L"min_price", atof(rows[6]));

      if (rows[7] != NULL)
        info_value->SetBigInteger(L"order_date", atoll(rows[7]));

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}



void StarSideDB::CallProfitDetail(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 result = 0;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

//      if (rows[1] != NULL)
 //       info_value->SetBigInteger(L"uid", atoll(rows[1]));

      if (rows[0] != NULL)
        info_value->SetReal(L"yeslast_price", atof(rows[0]));

      if (rows[1] != NULL)
        info_value->SetReal(L"price", atof(rows[1]));

      if (rows[2] != NULL)
        info_value->SetReal(L"profit", atof(rows[2]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
  dict->SetInteger(L"result", result);
}

void StarSideDB::CallHistorStarMeetRelRecord(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"id", atoll(rows[0]));

      if (rows[1] != NULL)
        info_value->SetString(L"starcode", (rows[1]));

      if (rows[2] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"nickname", (rows[3]));

      if (rows[4] != NULL)
        info_value->SetString(L"headurl", (rows[4]));

      if (rows[5] != NULL)
        info_value->SetBigInteger(L"mid", atoll(rows[5]));

      if (rows[6] != NULL)
        info_value->SetString(L"name", (rows[6]));
      if (rows[7] != NULL)
        info_value->SetString(L"meet_city", (rows[7]));

      if (rows[8] != NULL)
        info_value->SetString(L"appoint_time", (rows[8]));

      if (rows[9] != NULL)
        info_value->SetString(L"order_time", (rows[9]));

      if (rows[10] != NULL)
        info_value->SetString(L"comment", (rows[10]));

      if (rows[11] != NULL)
        info_value->SetBigInteger(L"meet_type", atoll(rows[11]));

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

void StarSideDB::CallOwnStarUser(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetString(L"starcode", (rows[0]));

      if (rows[1] != NULL)
        info_value->SetBigInteger(L"uid", atoll(rows[1]));


      if (rows[2] != NULL)
        info_value->SetBigInteger(L"ownseconds", atoll(rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"faccid", (rows[3]));

      if (rows[4] != NULL)
        info_value->SetInteger(L"appoint", atoll(rows[4]));

      if (rows[5] != NULL)
        info_value->SetString(L"user_nickname", (rows[5]));

      if (rows[6] != NULL)
        info_value->SetString(L"head_url", (rows[6]));


      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

void StarSideDB::CallHistorStarOwnService(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetString(L"starcode", (rows[0]));

      if (rows[1] != NULL)
        info_value->SetBigInteger(L"mid", atoll(rows[1]));

      if (rows[2] != NULL)
        info_value->SetString(L"name", (rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"meet_city", (rows[3]));

      if (rows[4] != NULL)
        info_value->SetString(L"stardate", (rows[4]));

      if (rows[5] != NULL)
        info_value->SetString(L"enddate", (rows[5]));

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}
void StarSideDB::CallServiceDef(void* param, base_logic::Value* value) {

  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 result = 0;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();
//      if (rows[1] != NULL)
 //       info_value->SetBigInteger(L"uid", atoll(rows[1]));

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"mid", atoll(rows[0]));

      if (rows[1] != NULL)
        info_value->SetReal(L"price", atof(rows[1]));

      if (rows[2] != NULL)
        info_value->SetString(L"name", (rows[2]));

      if (rows[3] != NULL)
        info_value->SetString(L"showpic_url", (rows[3]));
      else
      {
        std::string tmp = "";
        info_value->SetString(L"showpic_url",tmp.c_str());
      }
      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}
}  // namespace starside_logic

