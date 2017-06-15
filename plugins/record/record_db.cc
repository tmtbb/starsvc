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

}  // namespace record_logic

