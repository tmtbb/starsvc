//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "push/push_db.h"
#include "basic/basic_util.h"

namespace push_logic {

CPushDB::CPushDB(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

CPushDB::~CPushDB() {
  if (mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
}

bool CPushDB::onGetUserDeviceId(const int64 uid, std::string &deviceId, int32& deviceType)
{
	bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue *info_value = NULL;
  std::string sql;

  sql = "call proc_GetUserDeviceId('" + base::BasicUtil::StringUtil::Int64ToString(uid) + "');";
  
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  LOG_ERROR2("sql = %s",sql.c_str());
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict),
                              CallGetUserDeviceId);
  if (!r)
  {
    if (dict) delete dict;
    return false;
  }
  //if (!r)
   // return false;
  
  dict->GetDictionary(L"resultvalue", &info_value);
  int32 result;
  r = info_value->GetInteger(L"result", &result);
  bool r2 = info_value->GetString(L"device_id", &deviceId);
  bool r3 = info_value->GetInteger(L"device_type", &deviceType);
  if(r && r2 && r3 && (result == 1)){
    r = true;
  }else{
    r = false;
  }
  
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return r;
}

void CPushDB::CallGetUserDeviceId(void* param, base_logic::Value* value) {  
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  base_logic::DictionaryValue *info_value = new base_logic::DictionaryValue();  
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      if (rows[0] != NULL)
        info_value->SetInteger(L"result", atoi(rows[0]));

      if (rows[1] != NULL)
        info_value->SetString(L"device_id", rows[1]);

      if (rows[2] != NULL)
        info_value->SetInteger(L"device_type", atoi(rows[2]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

}  // namespace push_logic

