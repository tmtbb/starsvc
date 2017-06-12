//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "trades/trades_db.h"
#include "basic/basic_util.h"

namespace trades_logic {

TradesDB::TradesDB(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

TradesDB::~TradesDB() {
  if (mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
}

bool TradesDB::OnOpenPosition(swp_logic::TradesPosition& trades) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  std::string sql;
  base_logic::DictionaryValue *info_value = NULL;
  int32 result = 0;
  /*call actuals.proc_OpenPosition(888,55721932,13,1,6,0.0,8,1486791795,1486792578,7.12,20,30,0.15,0,0.0,0.0,0.0,'SHTY30MIN','fx_sjpycnh','上海-东京30分钟')
   */
  sql = "call proc_OpenPosition("
      + base::BasicUtil::StringUtil::Int64ToString(trades.uid()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.position_id()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.code_id()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.buy_sell()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.close_type()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.amount()) + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.open_position_time())
      + ","
      + base::BasicUtil::StringUtil::Int64ToString(trades.close_position_time())
      + "," + base::BasicUtil::StringUtil::DoubleToString(trades.open_price())
      + "," + base::BasicUtil::StringUtil::DoubleToString(trades.open_cost())
      + ","
      + base::BasicUtil::StringUtil::DoubleToString(trades.open_all_cost())
      + "," + base::BasicUtil::StringUtil::DoubleToString(trades.open_charge())
      + "," + base::BasicUtil::StringUtil::DoubleToString(trades.close_price())
      + "," + base::BasicUtil::StringUtil::DoubleToString(trades.limit()) + ","
      + base::BasicUtil::StringUtil::DoubleToString(trades.stop()) + ","
      + base::BasicUtil::StringUtil::DoubleToString(trades.deferred()) + ",\'"
      + trades.symbol() + "\',\'" + trades.name() + "\');";

  LOG_DEBUG2("%s",sql.c_str());
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallOnOpenPosition);
  if (!r && dict->empty())
    return false;
  dict->GetDictionary(L"resultvalue", &info_value);

  r = info_value->GetInteger(L"result", &result);
  r = (r && result > 0) ? true : false;
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

bool TradesDB::OnClosePosition(std::list<swp_logic::TradesPosition>* list) {
  bool r = false;
  //call actuals.proc_ClosePosition(3773378865228031997,7.33,12.1,-1)
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  std::string sql;
  base_logic::DictionaryValue *info_value = NULL;
  int32 result = 0;
  while ((*list).size() > 0) {
    swp_logic::TradesPosition trades_position = (*list).front();
    (*list).pop_front();
    int32 int_result = 0;
    int_result = trades_position.result() ? 1 : -1;
    sql += "call proc_ClosePosition("
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.uid()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.position_id()) + ","
        + base::BasicUtil::StringUtil::DoubleToString(trades_position.open_cost())+","
        + base::BasicUtil::StringUtil::DoubleToString(trades_position.close_price())+","
        + base::BasicUtil::StringUtil::DoubleToString(trades_position.gross_profit())+","
        + base::BasicUtil::StringUtil::Int64ToString(int_result) + ");";
  }

  dict->SetString(L"sql", sql);
  r = mysql_engine_->WriteData(0, (base_logic::Value *) (dict));
  if (!r)
    return false;
  return r;
}

bool TradesDB::OnFetchPlatformGoods(std::list<trades_logic::GoodsInfo>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_FetchPlatformGoods()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallFecthPlatformGoods);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    trades_logic::GoodsInfo goods;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    goods.ValueSerialization(dict_result_value);
    list->push_back(goods);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}


bool TradesDB::OnFetchFlightInfo(std::list<trades_logic::FlightInfo>* list) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  //call actuals.proc_FetchFlightInfo()
  sql = "call proc_FetchFlightInfo()";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallFetchFlightInfo);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    trades_logic::FlightInfo flight_info;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    flight_info.ValueSerialization(dict_result_value);
    list->push_back(flight_info);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

void TradesDB::CallOnOpenPosition(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();
  info_value->SetInteger(L"result", 0);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetInteger(L"result", atoi(rows[0]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void TradesDB::CallFecthPlatformGoods(void* param, base_logic::Value* value) {
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
        info_value->SetInteger(L"id", atoi(rows[0]));

      if (rows[1] != NULL)
        info_value->SetInteger(L"platform_id", atoi(rows[1]));

      if (rows[2] != NULL)
        info_value->SetString(L"code", rows[2]);

      if (rows[3] != NULL)
        info_value->SetString(L"name", rows[3]);

      if (rows[4] != NULL)
        info_value->SetString(L"symbol", rows[4]);

      if (rows[5] != NULL)
        info_value->SetString(L"unit", rows[5]);

      if (rows[6] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[6]), &temp);
        info_value->SetReal(L"amount", temp);
      }

      if (rows[7] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[7]), &temp);
        info_value->SetReal(L"profit", temp);
      }

      if (rows[8] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[8]), &temp);
        info_value->SetReal(L"deposit", temp);
      }

      if (rows[9] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[9]), &temp);
        info_value->SetReal(L"open", temp);
      }

      if (rows[10] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[10]), &temp);
        info_value->SetReal(L"close", temp);
      }

      if (rows[11] != NULL) {
        double temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToDouble(
            std::string(rows[11]), &temp);
        info_value->SetReal(L"deferred", temp);
      }

      if (rows[12] != NULL) {
        int64 temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToInt64(
            std::string(rows[12]), &temp);
        info_value->SetBigInteger(L"max", temp);
      }

      if (rows[13] != NULL) {
        int64 temp = 0.0;
        bool r = base::BasicUtil::StringUtil::StringToInt64(
            std::string(rows[13]), &temp);
        info_value->SetBigInteger(L"min", temp);
      }

      if (rows[14] != NULL)
        info_value->SetString(L"exchange_name", rows[14]);

      if (rows[15] != NULL)
        info_value->SetString(L"platform_name", rows[15]);

      if (rows[16] != NULL)
        info_value->SetCharInteger(L"status",
                                   logic::SomeUtils::StringToIntChar(rows[16]));

      if (rows[17] != NULL)
        info_value->SetCharInteger(L"sort",
                                   logic::SomeUtils::StringToIntChar(rows[16]));

      if (rows[18] != NULL)
        info_value->SetString(L"show_symbol", rows[18]);

      if (rows[19] != NULL)
        info_value->SetString(L"show_name", rows[19]);

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

void TradesDB::CallFetchFlightInfo(void* param, base_logic::Value* value) {
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
        info_value->SetBigInteger(L"gid", atoll(rows[1]));

      if (rows[2] != NULL)
        info_value->SetString(L"name", rows[2]);

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}


}  // namespace trades_logic

