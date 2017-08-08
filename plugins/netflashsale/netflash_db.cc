//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry
#include <mysql.h>
#include "netflash_db.h"
#include "logic/logic_unit.h"
#include "basic/basic_util.h"

namespace netflash_logic {

NetflashDB::NetflashDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

NetflashDB::~NetflashDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}

void NetflashDB::CallgetStarExperience(void* param, base_logic::Value* value){
  base_storage::DBStorageEngine* engine = (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  base_logic::ListValue *list = new base_logic::ListValue();
  base_logic::DictionaryValue* dict = reinterpret_cast<base_logic::DictionaryValue*>(value);
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

bool NetflashDB::OnGetStarExperience(const std::string& code, base_logic::DictionaryValue &ret){
  bool r = false;
  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  
  std::string sql = "call proc_getstarexperience('"
      + code
      + "')";
  dic->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),CallgetStarExperience);
  if (!r) {
    if (dic) delete dic;
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


void NetflashDB::CallCreateNetflashOrder(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        dict->SetBigInteger(L"resultvalue", atoi(rows[0]));
    }
  }
  else{
    LOG_ERROR ("proc_CreateflashOrder count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool NetflashDB::OnCreateNetflashOrder(star_logic::TradesOrder& netflash_trades_order, int64& result) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_CreateflashOrder(" + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.order_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.buy_position_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.sell_position_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.buy_uid())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.sell_uid())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.amount())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.open_position_time())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.close_position_time())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.handle_type())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.sell_handle_type())
      + "," + base::BasicUtil::StringUtil::Int64ToString(netflash_trades_order.buy_handle_type())
      + ",0," + base::BasicUtil::StringUtil::DoubleToString(netflash_trades_order.open_price())
      + ",0.0,'" + netflash_trades_order.symbol()+"');";
  dict->SetString(L"sql", sql);
  LOG_MSG2("sql [%s]", sql.c_str());
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallCreateNetflashOrder);
  if (!r)
  {
      if (dict) delete dict;
      return false;
  }

  dict->GetBigInteger(L"resultvalue", &result);
  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

bool NetflashDB::OnUpdateNetflashsaleResult(const int64& orderid, const int32 status) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_UpdateFlashsaleResult('" 
      + base::BasicUtil::StringUtil::Int64ToString(orderid) + "','"
      + base::BasicUtil::StringUtil::Int64ToString(status) 
      + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return r;
}


bool NetflashDB::OnUpdatePublishStarInfo(const std::string& symbol,const int64 publishtype,
                              const int64 publasttime,const int64 pubbegintime) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_UpdatePublishStarInfo('" 
      + symbol 
      + "','" + base::BasicUtil::StringUtil::Int64ToString(publishtype) 
      + "','" + base::BasicUtil::StringUtil::DoubleToString(publasttime)
      + "','" + base::BasicUtil::StringUtil::DoubleToString(pubbegintime)
      + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
  {
      if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

}  // namespace netflash_logic

