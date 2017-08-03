//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry
#include <mysql.h>
#include "flashsale/flash_db.h"
#include "logic/logic_unit.h"
#include "basic/basic_util.h"

namespace flash_logic {

FlashDB::FlashDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

FlashDB::~FlashDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}

void FlashDB::CallGetPublishRule(void* param, base_logic::Value* value) {
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
          info_value->SetString(L"star_code",rows[0]);

      if (rows[1] != NULL)
          info_value->SetString(L"star_name", rows[1]);

      if (rows[2] != NULL)
          info_value->SetString(L"star_pic", rows[2]);

      if (rows[3] != NULL)
          info_value->SetInteger(L"star_type", atoi(rows[3]));

      if (rows[4] != NULL)
          info_value->SetInteger(L"publish_type", atoi(rows[4]));

      if (rows[5] != NULL)
          info_value->SetBigInteger(L"publish_time", atoi(rows[5]));

      if (rows[6] != NULL)
          info_value->SetBigInteger(L"publish_last_time", atoi(rows[6]));

      if (rows[7] != NULL)
          info_value->SetBigInteger(L"publish_begin_time", atoi(rows[7]));

      if (rows[8] != NULL)
          info_value->SetBigInteger(L"publish_end_time", atoi(rows[8]));

      if (rows[9] != NULL)
          info_value->SetString(L"back_pic", rows[9]);

      if (rows[10] != NULL)
          info_value->SetReal(L"publish_price", atof(rows[10]));

      if (rows[11] != NULL)
          info_value->SetString(L"work", rows[11]);

      list->Append((base_logic::Value *) (info_value));
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
  }
  else{
    LOG_ERROR ("proc_GetAllStarPublishInfo count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool FlashDB::OnFetchPublishStar(std::map<std::string, flash_logic::PulishStar>& map) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetAllStarPublishInfo();";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetPublishRule);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
  //    return false;
  
  r = dict->GetList(L"resultvalue", &listvalue);
  while (r && listvalue->GetSize()) {
      flash_logic::PulishStar pubstar;
      base_logic::Value *result_value;
      listvalue->Remove(0, &result_value);
      base_logic::DictionaryValue *dict_result_value =
          (base_logic::DictionaryValue *) (result_value);
      pubstar.ValueSerialization(dict_result_value);
      map[pubstar.symbol()] = pubstar;
      delete dict_result_value;
      dict_result_value = NULL;
  }

  if (dict) {
      delete dict;
      dict = NULL;
  }

  return r;
}

void FlashDB::CallCreateFlashOrder(void* param, base_logic::Value* value) {
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
    LOG_ERROR ("proc_CreateFlashOrder count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool FlashDB::OnCreateFlashOrder(star_logic::TradesOrder& flash_trades_order, int64& result) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_CreateFlashOrder(" + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.order_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.buy_position_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.sell_position_id())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.buy_uid())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.sell_uid())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.amount())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.open_position_time())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.close_position_time())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.handle_type())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.sell_handle_type())
      + "," + base::BasicUtil::StringUtil::Int64ToString(flash_trades_order.buy_handle_type())
      + ",0," + base::BasicUtil::StringUtil::DoubleToString(flash_trades_order.open_price())
      + ",0.0,'" + flash_trades_order.symbol()+"');";
  dict->SetString(L"sql", sql);
  LOG_MSG2("sql [%s]", sql.c_str());
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallCreateFlashOrder);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;

  dict->GetBigInteger(L"resultvalue", &result);
  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

bool FlashDB::OnUpdateFlashsaleResult(const int64 uid,const std::string& symbol,const int64 amount,const double totlePrice) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_UpdateFlashsaleResult('" 
      + base::BasicUtil::StringUtil::Int64ToString(uid) 
      + "','" + symbol 
      + "','" + base::BasicUtil::StringUtil::Int64ToString(amount) 
      + "','" + base::BasicUtil::StringUtil::DoubleToString(totlePrice)
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

bool FlashDB::OnUpdatePublishStarInfo(const std::string& symbol,const int64 publishtype,
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

}  // namespace flash_logic

