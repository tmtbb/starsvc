//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry
#include <mysql.h>
#include "circle_db.h"
#include "logic/logic_unit.h"
#include "basic/basic_util.h"

namespace circle_logic {

CircleDB::CircleDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

CircleDB::~CircleDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}

void CircleDB::CallGetAllCircleInfo(void* param, base_logic::Value* value) {
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
          info_value->SetBigInteger(L"circle_id", atoi(rows[0]));

      if (rows[1] != NULL)
          info_value->SetString(L"star_code", rows[1]);

      if (rows[2] != NULL)
          info_value->SetBigInteger(L"create_time", atoi(rows[2]));

      if (rows[3] != NULL)
          info_value->SetString(L"content", rows[3]);

      if (rows[4] != NULL)
          info_value->SetString(L"pic_url", rows[4]);

      if (rows[5] != NULL)
          info_value->SetString(L"approval_list", rows[5]);

      if (rows[6] != NULL)
          info_value->SetString(L"comment_list", rows[6]);

      list->Append((base_logic::Value *) (info_value));
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
  }
  else{
    LOG_ERROR ("proc_GetAllCircleInfo count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool CircleDB::OnFetchAllCircleInfo(base_logic::ListValue*& ret_list) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql = "call proc_GetAllCircleInfo();";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllCircleInfo);
  if (!r)
      return false;
  
  dict->GetList(L"resultvalue", &ret_list);

  return true;
}

bool CircleDB::OnCreateCircleOrder(circle_logic::Circle& circle_info) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  std::string t_sapprove, t_scomments;
  {
    base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      0, &t_sapprove);
    engine->Serialize(*((base_logic::Value*) circle_info.GetApproveList()));
  }
  {
    base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      0, &t_scomments);
    engine->Serialize(*((base_logic::Value*) circle_info.GetCommentList()));
  }

  std::string sql;
  sql = "call proc_CreateCircleOrder('" 
      + circle_info.GetSymbol()
      + "','" + base::BasicUtil::StringUtil::Int64ToString(circle_info.GetCircleId())
      + "','" + base::BasicUtil::StringUtil::Int64ToString(circle_info.GetCreateTime())
      + "','" + circle_info.GetContent()
      + "','" + circle_info.GetPicUrl()
      + "','" + t_sapprove
      + "','" + t_scomments
      +"');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
      return false;

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

bool CircleDB::OnDeleteCircle(const int64 circleid) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_DeleteCircle('" 
      + base::BasicUtil::StringUtil::Int64ToString(circleid)
      +"');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
      return false;

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

bool CircleDB::OnUpdateCircle(circle_logic::Circle& circle_info) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  std::string t_sapprove, t_scomments;
  {
    base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      0, &t_sapprove);
    engine->Serialize(*(circle_info.GetApproveList()));
  }
  {
    base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      0, &t_scomments);
    engine->Serialize(*(circle_info.GetCommentList()));
  }

  std::string sql;
  sql = "call proc_UpdateCircleInfo('" 
      + circle_info.GetSymbol()
      + "','" + base::BasicUtil::StringUtil::Int64ToString(circle_info.GetCircleId())
      + "','" + base::BasicUtil::StringUtil::Int64ToString(circle_info.GetStatus())
      + "','" + t_sapprove
      + "','" + t_scomments
      +"');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), NULL);
  if (!r)
      return false;

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}


}  // namespace circle_logic

