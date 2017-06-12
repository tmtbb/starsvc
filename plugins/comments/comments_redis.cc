//  Copyright (c) 2016-2017 The comments Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "comments/comments_redis.h"
#include "storage/data_engine.h"
#include "basic/basic_util.h"
#include "logic/logic_comm.h"
#define COMMENTS_HEAD "CMS:" //comments
namespace comments_logic {

CommentsRedis::CommentsRedis(config::FileConfig* config) {
  redis_engine_ = base_logic::DataEngine::Create(REIDS_TYPE);
  redis_engine_->InitParam(config->redis_list_);
}

CommentsRedis::~CommentsRedis() {
  if (redis_engine_) {
    delete redis_engine_;
    redis_engine_ = NULL;
  }
}

bool CommentsRedis::WriteData(comments_logic::net_request::FansComments &fanscms)
{
  bool r = false;
  std::string str_name = COMMENTS_HEAD + fanscms.star_code();//starcode
  std::string str_key =base::BasicUtil::StringUtil::Int64ToString(time(NULL));//
  std::string str_value = "";
                              
  base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
  if (engine == NULL) {
    LOG_ERROR("engine create null");
    return false;
  }   
  r = engine->Serialize((*(fanscms.get())), &str_value);
  if (engine) { delete engine; engine = NULL; }
//__________
  base_logic::DictionaryValue *dict = new base_logic::DictionaryValue();

  base_logic::StringValue* name = new base_logic::StringValue(str_name);
  base_logic::StringValue* key = new base_logic::StringValue(str_key);
  base_logic::StringValue* value_str = new base_logic::StringValue(str_value);

  dict->Set(L"name", name); 
  dict->Set(L"key", key); 
  dict->Set(L"value", value_str); 
//____
/*
  std::string hash_name;
  std::string key_name;
  std::string value_str2;
  base_logic::Value* value_name;
  r = dict->GetString(L"name", &hash_name);
  if (!r) {
    LOG_ERROR("hash name error");
    return false;
  }

  r = dict->GetString(L"key", &key_name);
  if (!r) {
    LOG_ERROR("key_name name error");
    return false;
  }

  r = dict->Get(L"value", &value_name);
  if (!r) {
    LOG_ERROR("value_name name error");
    return false;
  }

  r = value_name->GetAsString(&value_str2);
  if (!r) {
    LOG_ERROR("value name error");
    return false;
  }

  LOG_ERROR2("hash_name[%s], key_name[%s], value_str2[%s]", hash_name.c_str(), key_name.c_str(), value_str2.c_str());
*/
  r = redis_engine_->WriteData(HASH_VALUE, dict); //
  delete dict;
  return r;
}
/*
  */

bool CommentsRedis::ReadCommentsData(const std::string& name, std::list<star_logic::Comments>& comments_list){

  //[hash name] rule : platform:exchangename:type: FX:PMEC:AG
  // [key name] rule : unix - minute
  base_logic::DictionaryValue value;
  value.SetString(L"name", name);

  bool r = redis_engine_->ReadData(LIST_VALUE, &value,NULL);
  if (!r)
    return false;
  base_logic::ListValue* list = NULL;

  r = value.GetList(L"value",&list);
  if (!r)
    return false;
  for (base_logic::ListValue::iterator it = list->begin(); it != list->end(); ++it) {
    star_logic::Comments comments;
    base_logic::Value* string_value = (*it);
    std::string str;
    r = string_value->GetAsString(&str);
    if (!r)
      continue;
    comments.ValueDeserialize(str);
    comments_list.push_back(comments);
    LOG_ERROR2("str[%s]_____________________________", str.c_str());
  }
  return r;

  return true;
}


bool CommentsRedis::ReadData( const std::string& name, std::list<std::string>& lst) {

  //[hash name] rule : platform:exchangename:type: FX:PMEC:AG
  // [key name] rule : unix - minute
  base_logic::DictionaryValue value;
  value.SetString(L"name", name);

  bool r = redis_engine_->ReadData(LIST_VALUE, &value,
                                   NULL);
  if (!r)
    return false;
  base_logic::ListValue* list = NULL;

  r = value.GetList(L"value",&list);
  if (!r)
    return false;
  for (base_logic::ListValue::iterator it = list->begin(); it != list->end(); ++it) {
    //star_logic::Comments comments;
    base_logic::Value* string_value = (*it);
    std::string str;
    r = string_value->GetAsString(&str);
    if (!r)
      continue;
    //comments.ValueDeserialize(str);
    //comments_list.push_back(comments);
    LOG_ERROR2("str[%s]_____________________________", str.c_str());
  }
  return r;

  return true;
}
}
