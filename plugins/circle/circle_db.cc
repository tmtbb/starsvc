//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry
#include <mysql.h>
#include "circle_db.h"
#include "logic/logic_unit.h"
#include "basic/basic_util.h"
#include "errno.h"

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
          info_value->SetInteger(L"create_time", atoi(rows[2]));

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
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;
  
  r = dict->GetList(L"resultvalue", &ret_list);

  return r;
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
  //if (!r)
   //   return false;
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }

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
  //if (!r)
   //   return false;
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

void CircleDB::CallOnUpdateCircle(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

      if (rows[0] != NULL)
        dict->SetBigInteger(L"resultvalue", atoi(rows[0]));

    }
  }
  else{
    LOG_ERROR ("proc_UpdateCircleInfo count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool CircleDB::OnUpdateCircle(int64 uid, circle_logic::Circle& circle_info, int32 dec_time,
                      int64& result) {
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
      + "','" + base::BasicUtil::StringUtil::Int64ToString(uid)
      + "','" + base::BasicUtil::StringUtil::Int64ToString(dec_time)
      +"');";
  LOG_DEBUG2("sql:%s",sql.c_str()); 
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallOnUpdateCircle);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;

  dict->GetBigInteger(L"resultvalue", &result);
  switch (result)
  {
  case -1:
    result = NO_CIRCLE_NO_ERR;
    break;
  case -2:
    result = NO_USER_NO_TIME_ERR;
    break;
  case -3:
    result = NO_USER_NO_BUY_STAR_TIME;
    break;
  default:
    result = 1;
  }

  if (dict) {
      delete dict;
      dict = NULL;
  }
  return true;
}

void CircleDB::CallGetUserName(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

      if (rows[0] != NULL)
          dict->SetString(L"user_name", rows[0]);
    
    }
  }
  else{
    LOG_ERROR ("proc_GetUserName count < 0");
  }
  dict->Remove(L"sql", &value);
}

bool CircleDB::OnGetUserName(const int64 uid, base_logic::DictionaryValue*& dict) {

  bool r = false;
  std::string sql = "call proc_GetUserName('"
    + base::BasicUtil::StringUtil::Int64ToString(uid)
    + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetUserName);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;
  
  return true;
}


bool CircleDB::OnFetchAllUserAskAnswer(base_logic::ListValue*& ret_list) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql = "call proc_GetAllUserAskAnswer();";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllUserAskAnswer);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;
  
  r = dict->GetList(L"resultvalue", &ret_list);

  return r;
}

void CircleDB::CallGetAllUserAskAnswer(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  int r_i  = 0;
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();
      r_i = 0;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"id", atoll(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"uid", atoll(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetString(L"starcode", (rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"ask_t", atoll(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"answer_t", atoll(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetInteger(L"s_total", atoi(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetInteger(L"a_type", atoi(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetInteger(L"p_type", atoi(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetInteger(L"c_type", atoi(rows[r_i]));
      r_i++;

      if (rows[r_i] != NULL)
          info_value->SetString(L"uask", (rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetString(L"sanswer", (rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetString(L"video_url", (rows[r_i]));
      r_i++;

      list->Append((base_logic::Value *) (info_value));
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
  }
  else{
    LOG_ERROR ("proc_GetAllUserAskAnswer count < 0");
  }
  dict->Remove(L"sql", &value);
}
//用户提问

int64 CircleDB::OnUserAsk(const int64 uid, const int32 a_type, 
            const int32 p_type, const int32 c_type, 
            const std::string &starcode, const std::string &uask,
            const std::string &video_url)
{


  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue* info_value = NULL;

  bool r = false;
  std::string sql = "call proc_UserAsk("
    + base::BasicUtil::StringUtil::Int64ToString(uid) + ","
    + base::BasicUtil::StringUtil::Int64ToString(a_type) + ","
    + base::BasicUtil::StringUtil::Int64ToString(c_type) + ","
    + base::BasicUtil::StringUtil::Int64ToString(p_type) 
    + ",'" + uask 
    + "','" + starcode 
    + "','" + video_url
    + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallUserAsk);
  if (!r)
  {
    if (dict) delete dict;
      return -1;
  }

  int64 id = -1;
  if(dict->GetDictionary(L"resultvalue", &info_value)){
      if (info_value)
        info_value->GetBigInteger(L"id", &id);
  }

  //if (!r)
   //   return false;
  
  return id;
}

void CircleDB::CallUserAsk(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  int r_i  = 0;
  if (num > 0) {
    base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      r_i = 0;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"id", atoll(rows[r_i]));
      r_i++;

    }
    dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
  }
  else{
    LOG_ERROR ("proc_UserAsk count < 0");
  }
  dict->Remove(L"sql", &value);
}
//明星回答
int64 CircleDB::OnStarAnswer(const int64 id, const int32 p_type, 
            const std::string &sanswer)
{
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue* info_value = NULL;

  bool r = false;
  std::string sql = "call proc_StarAnswer("
    + base::BasicUtil::StringUtil::Int64ToString(id) + ","
    + base::BasicUtil::StringUtil::Int64ToString(p_type) 
    + ",'" + sanswer
    + "');";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallUserAsk);
  if (!r)
  {
    if (dict) delete dict;
      return -1;
  }

  int64 answer_t = -1; //回答时间
  if(dict->GetDictionary(L"resultvalue", &info_value)){
      if (info_value)
        info_value->GetBigInteger(L"id", &answer_t );
  }
  //if (!r)
   //   return false;
  
  return answer_t;
}


bool CircleDB::OnFetchAllUserSeeAsk(base_logic::ListValue*& ret_list) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql = "call proc_GetAllUserSeeAsk();";
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetAllUserSeeAsk);
  if (!r)
  {
    if (dict) delete dict;
      return false;
  }
  //if (!r)
   //   return false;
  
  r = dict->GetList(L"resultvalue", &ret_list);

  return r;
}


void CircleDB::CallGetAllUserSeeAsk(void* param, base_logic::Value* value) 
{
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  int r_i  = 0;
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();
      r_i = 0;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"uid", atoll(rows[r_i]));
      r_i++;
      if (rows[r_i] != NULL)
          info_value->SetBigInteger(L"qid", atoll(rows[r_i]));
      r_i++;

      list->Append((base_logic::Value *) (info_value));
    }
    dict->Set(L"resultvalue", (base_logic::Value *) (list));
  }
  else{
    LOG_ERROR ("proc_GetAllUserSeeAsk count < 0");
  }
  dict->Remove(L"sql", &value);
}
}  // namespace circle_logic

