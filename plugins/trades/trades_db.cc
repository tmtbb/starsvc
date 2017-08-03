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

bool TradesDB::OnUpdateOrderState(const int64 order_id, const int64 uid, 
                            const int32 state, const int32 uid_type) {
    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
    std::string sql;
    sql = "call proc_UpdateTradesOrderState(" 
        + base::BasicUtil::StringUtil::Int64ToString(order_id) + ","
        + base::BasicUtil::StringUtil::Int64ToString(uid) + ","
        + base::BasicUtil::StringUtil::Int64ToString(state) + ","
        + base::BasicUtil::StringUtil::Int64ToString(uid_type)+");";

    dict->SetString(L"sql", sql);
    r = mysql_engine_->WriteData(0, (base_logic::Value *) (dict));
    if (!r)
        return false;

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}



//0.交易成功，1.转让方持有的时间不足 2.求购余额不足 .3.数据库异常
int32 TradesDB::OnConfirmOrder(const int64 order_id, const int64 buy_uid,const int64 sell_uid) {
    
    bool r = false;
    int32 result = 0;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
    std::string sql;
    base_logic::DictionaryValue* info_value = NULL;
    sql = "call proc_ConfirmOrder("
        + base::BasicUtil::StringUtil::Int64ToString(order_id) + ","
        + base::BasicUtil::StringUtil::Int64ToString(buy_uid) + ","
        + base::BasicUtil::StringUtil::Int64ToString(sell_uid) + ");";

    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallConfirmOrder);
    
    if (!r && dict->empty())
    {
        if (dict) delete dict;
        return 3;
    }
    
    dict->GetDictionary(L"resultvalue", &info_value);
    
    r = info_value->GetInteger(L"result", &result);
    if (!r) 
    {
        if (dict) delete dict;
        return 3;
    }

    
    if (dict) {
        delete dict;
        dict = NULL;
    }
    return result;
}


bool TradesDB::OnCreateTradesOrder(star_logic::TradesOrder& trades_order) {
    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_CreateTradesOrder(" + base::BasicUtil::StringUtil::Int64ToString(trades_order.order_id())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.buy_position_id())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.sell_position_id())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.buy_uid())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.sell_uid())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.amount())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.open_position_time())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.close_position_time())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.handle_type())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.sell_handle_type())
        + "," + base::BasicUtil::StringUtil::Int64ToString(trades_order.buy_handle_type())
        + ",0," + base::BasicUtil::StringUtil::DoubleToString(trades_order.open_price())
        + ",0.0,'" + trades_order.symbol()+"');";
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

bool TradesDB::OnFetchPlatformStar(std::map<std::string, trades_logic::TradesStar>& map) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetTradesRule();";
    base_logic::ListValue *listvalue;
    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetTradsRule);
    if (!r)
    {
        if (dict) delete dict;
        return false;
    }
    dict->GetList(L"resultvalue", &listvalue);
    while (listvalue->GetSize()) {
        trades_logic::TradesStar star;
        base_logic::Value *result_value;
        listvalue->Remove(0, &result_value);
        base_logic::DictionaryValue *dict_result_value =
            (base_logic::DictionaryValue *) (result_value);
        star.ValueSerialization(dict_result_value);
        map[star.symbol()] = star;
        delete dict_result_value;
        dict_result_value = NULL;
    }

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}


void TradesDB::CallConfirmOrder(void* param, base_logic::Value* value) {
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

void TradesDB::CallGetTradsRule(void* param, base_logic::Value* value) {
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
                info_value->SetString(L"symbol",rows[0]);

            if (rows[1] != NULL)
                info_value->SetString(L"name", rows[1]);

            if (rows[2] != NULL)
                info_value->SetString(L"wid", rows[2]);

            if (rows[3] != NULL)
                info_value->SetInteger(L"trades_time", atoi(rows[3]));

            if (rows[4] != NULL)
                info_value->SetInteger(L"interval_time", atoi(rows[4]));

            if (rows[5] != NULL)
                info_value->SetString(L"start_time", rows[5]);

            if (rows[6] != NULL)
                info_value->SetString(L"end_time", rows[6]);

            list->Append((base_logic::Value *) (info_value));
        }
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

}  // namespace trades_logic

