//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "search/search_db.h"
#include "basic/basic_util.h"

namespace search_logic {

SearchDB::SearchDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

SearchDB::~SearchDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}

bool SearchDB::OnGetStarSearchDetail(std::list<star_logic::StarInfo>& list) {
    bool r = false;
    base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

    std::string sql;
    sql = "call proc_GetStarSearchDetail()";
    base_logic::ListValue* listvalue;
    dict->SetString(L"sql",sql);
    r = mysql_engine_->ReadData(0, (base_logic::Value*)(dict),
                                CallGetSearchDetail);
    if (!r)
    {   
        if (dict) delete dict;
        return false;
    }
    dict->GetList(L"resultvalue", &listvalue);
    while (listvalue->GetSize()) {
        star_logic::StarInfo star;
        base_logic::Value* result_value;
        listvalue->Remove(0, &result_value);
        base_logic::DictionaryValue* dict_result_value =
            (base_logic::DictionaryValue*)(result_value);
        star.ValueSerialization(dict_result_value);
        list.push_back(star);
        delete dict_result_value;
        dict_result_value = NULL;
    }

    if (dict) {
        delete dict;
        dict = NULL;
    }
    return true;
}

void SearchDB::CallGetSearchDetail(void* param, base_logic::Value* value) {
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
                info_value->SetBigInteger(L"gender", atoll(rows[2]));

            if (rows[3] != NULL)
                info_value->SetString(L"pic", rows[3]);

            if (rows[4] != NULL)
                info_value->SetString(L"weibo_index_id", rows[4]);

            if (rows[5] != NULL)
                info_value->SetString(L"nation", rows[5]);

            if (rows[6] != NULL)
                info_value->SetString(L"nationality", rows[6]);

            if (rows[7] != NULL)
                info_value->SetString(L"introduction", rows[7]);

            if (rows[8] != NULL)
                info_value->SetString(L"jianpin", rows[8]);

            if (rows[9] != NULL)
                info_value->SetString(L"quanpin", rows[9]);

            list->Append((base_logic::Value *) (info_value));
        }
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

}  // namespace search_logic

