//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "data_redis_engine.h"

namespace base_logic {

void DataRedisEngine::InitParam(std::list<base::ConnAddr>& addrlist) {
  redis_pool_.Init(addrlist);
  addrlist_ = addrlist;
}

void DataRedisEngine::Release() {
  redis_pool_.Dest();
}

bool DataRedisEngine::WriteData(const int32 type, base_logic::Value* value) {
  bool r = false;
  switch (type) {
    case HASH_VALUE: {
      WriteHashValue(value);
      break;
    }
  }
  return r;
}

bool DataRedisEngine::ReadData(const int32 type, base_logic::Value* value,
                               void (*storage_get)(void*, base_logic::Value*)) {
  bool r = false;
  switch (type) {
    case LIST_VALUE: {
      r = ReadListValue(value);
      break;
    }
  }
  return r;
}

bool DataRedisEngine::ReadListValue(base_logic::Value* value) {
  std::string hash_name;
  std::list<std::string> list;
  base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*) (value);
  bool r = dict->GetString(L"name", &hash_name);
  if (!r) {
    LOG_ERROR("hash name error");
    return false;
  }
  base_storage::DictionaryStorageEngine* engine =
      redis_pool_.RedisConnectionPop();

  r = engine->GetHashValues(hash_name.c_str(), hash_name.length(), list);
  if (!r)
    return r;

  base_logic::ListValue* list_value = new base_logic::ListValue;
  while(list.size() > 0) {
    std::string str = list.front();
    list.pop_front();
    base_logic::StringValue* string_value = new base_logic::StringValue(str);
    list_value->Append(string_value);
  }

  //base_logic::Value* temp_list = (base_logic::Value*)(list_value);
  dict->Set(L"value", (base_logic::Value*)list_value);
  redis_pool_.RedisConnectionPush(engine);
  return true;
}

bool DataRedisEngine::WriteHashValue(base_logic::Value* value) {
  //hash_name->key_name->value

  bool r = false;
  std::string hash_name;
  std::string key_name;
  std::string value_str;
  base_logic::Value* value_name;
  base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*) (value);
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

  base_storage::DictionaryStorageEngine* engine =
      redis_pool_.RedisConnectionPop();
  if (engine == NULL) {
    LOG_ERROR("GetConnection Error");
    return false;
  }

  r = dict->Get(L"value", &value_name);
  if (!r) {
    LOG_ERROR("value_name name error");
    return false;
  }

  r = value_name->GetAsString(&value_str);
  if (!r) {
    LOG_ERROR("value name error");
    return false;
  }

  engine->AddHashElement(hash_name.c_str(), key_name.c_str(), key_name.length(),
                         value_str.c_str(), value_str.length());

  redis_pool_.RedisConnectionPush(engine);
  return true;
}

}
