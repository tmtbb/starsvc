//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "record/record_db.h"
#include "basic/basic_util.h"

namespace record_logic {

RecordDB::RecordDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

RecordDB::~RecordDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}



bool RecordDB::OnCreateTradesPosition(star_logic::TradesPosition& trades_position) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_CreateTradesPosition("
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.position_id()) +","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.buy_sell()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.amount()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.r_amount()) + ","
        + base::BasicUtil::StringUtil::DoubleToString(trades_position.open_price()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.open_position_time()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.uid()) + ",'"
        + trades_position.symbol() + "',"
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.handle()) + ");";
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


bool RecordDB::OnUpdateTradesPosition(star_logic::TradesPosition& trades_position) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_UpdateTradesPosition("
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.position_id()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.r_amount()) + ","
        + base::BasicUtil::StringUtil::Int64ToString(trades_position.handle()) + ");";
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


bool RecordDB::OnGetAllUserInfo(std::map<int64, star_logic::UserInfo>& user_map) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetAllUserInfo()";
    base_logic::ListValue *listvalue;
    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllUserInfo);
    if (!r)
        return false;
    dict->GetList(L"resultvalue", &listvalue);
    while (listvalue->GetSize()) {
        star_logic::UserInfo userinfo;
        base_logic::Value *result_value;
        listvalue->Remove(0, &result_value);
        base_logic::DictionaryValue *dict_result_value =
            (base_logic::DictionaryValue *) (result_value);
        userinfo.ValueSerialization(dict_result_value);
        user_map[userinfo.uid()] = userinfo;
        delete dict_result_value;
        dict_result_value = NULL;
    }

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}


bool RecordDB::OnGetAllOrderInfo(std::map<int64, star_logic::TradesOrder>& order_map) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetAllTraderOrder()";
    base_logic::ListValue *listvalue;
    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllOrderInfo);
    if (!r)
        return false;
    dict->GetList(L"resultvalue", &listvalue);
    while (listvalue->GetSize()) {
        star_logic::TradesOrder order;
        base_logic::Value *result_value;
        listvalue->Remove(0, &result_value);
        base_logic::DictionaryValue *dict_result_value =
            (base_logic::DictionaryValue *) (result_value);
        order.ValueSerialization(dict_result_value);
        order_map[order.order_id()] = order;
        delete dict_result_value;
        dict_result_value = NULL;
    }

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}


bool RecordDB::OnGetAllPositionInfo(std::map<int64, star_logic::TradesPosition>& position_map) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetAllTraderPosition()";
    base_logic::ListValue *listvalue;
    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllPositionInfo);
    if (!r)
        return false;
    dict->GetList(L"resultvalue", &listvalue);
    while (listvalue->GetSize()) {
        star_logic::TradesPosition position;
        base_logic::Value *result_value;
        listvalue->Remove(0, &result_value);
        base_logic::DictionaryValue *dict_result_value =
            (base_logic::DictionaryValue *) (result_value);
        position.ValueSerialization(dict_result_value);
        position_map[position.position_id()] = position;
        delete dict_result_value;
        dict_result_value = NULL;
    }

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}

void RecordDB::CallGetAllUserInfo(void* param, base_logic::Value* value){

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
                info_value->SetBigInteger(L"uid",atoll(rows[0]));

            if (rows[1] != NULL)
                info_value->SetString(L"phone", rows[1]);

            if (rows[2] != NULL)
                info_value->SetString(L"nickname", rows[2]);

            if (rows[3] != NULL)
                info_value->SetString(L"head_url", rows[3]);

            if (rows[4] != NULL)
                info_value->SetInteger(L"gender", atoi(rows[4]));


            list->Append((base_logic::Value *) (info_value));
        }
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
}


void RecordDB::CallGetAllOrderInfo(void* param, base_logic::Value* value){

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

            if (rows[1] != NULL)
                info_value->SetBigInteger(L"order_id",atoll(rows[1]));

            if (rows[2] != NULL)
                info_value->SetString(L"symbol", rows[2]);

            if (rows[3] != NULL)
                info_value->SetBigInteger(L"handle", atoll(rows[3]));

            if (rows[4] != NULL)
                info_value->SetBigInteger(L"amount", atoll(rows[4]));
            
            if (rows[5] != NULL)
                info_value->SetReal(L"open_price", atof(rows[5]));

            if (rows[6] != NULL)
                info_value->SetBigInteger(L"sell_uid", atoll(rows[6]));

            if (rows[7] != NULL)
                info_value->SetBigInteger(L"buy_uid", atoll(rows[7]));

            if (rows[8] != NULL)
                info_value->SetBigInteger(L"open_position_time", atoll(rows[8]));

            if (rows[9] != NULL)
                info_value->SetBigInteger(L"close_position_time", atoll(rows[9]));

            if (rows[10] != NULL)
                info_value->SetBigInteger(L"sell_position_id", atoll(rows[10]));

            if (rows[11] != NULL)
                info_value->SetBigInteger(L"buy_position_id", atoll(rows[11]));
            
            if (rows[12] != NULL)
                info_value->SetBigInteger(L"sell_handle_type", atoll(rows[12]));

            if (rows[13] != NULL)
                info_value->SetBigInteger(L"buy_handle_type", atoll(rows[13]));

            list->Append((base_logic::Value *) (info_value));
        }
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
}




void RecordDB::CallGetAllPositionInfo(void* param, base_logic::Value* value){

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

            if (rows[1] != NULL)
                info_value->SetBigInteger(L"tid",atoll(rows[1]));

            if (rows[2] != NULL)
                info_value->SetBigInteger(L"buy_sell", atoll(rows[2]));

            if (rows[3] != NULL)
                info_value->SetBigInteger(L"amount", atoll(rows[3]));

            if (rows[4] != NULL)
                info_value->SetBigInteger(L"r_amount", atoll(rows[4]));
            
            if (rows[5] != NULL)
                info_value->SetReal(L"open_price", atof(rows[5]));

            if (rows[6] != NULL)
                info_value->SetBigInteger(L"open_position_time", atoll(rows[6]));

            if (rows[7] != NULL)
                info_value->SetBigInteger(L"uid", atoll(rows[7]));

            if (rows[8] != NULL)
                info_value->SetString(L"symbol", rows[8]);

            if (rows[9] != NULL)
                info_value->SetBigInteger(L"handle", atoll(rows[9]));


            list->Append((base_logic::Value *) (info_value));
        }
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
}
}  // namespace record_logic

