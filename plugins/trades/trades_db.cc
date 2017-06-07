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

bool TradesDB::OnFetchPlatformStar(std::map<std::string, trades_logic::TradesStar>& map) {

    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetTradesRule();";
    base_logic::ListValue *listvalue;
    dict->SetString(L"sql", sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetTradsRule);
    if (!r)
        return false;
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

