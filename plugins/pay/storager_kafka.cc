//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015.9.28 Author: kerry

#include <sstream>
#include "pay/storager_kafka.h"
#include "storage/storage.h"
#include "config/config.h"
#include "logic/logic_unit.h"
#include "basic/basic_util.h"
#include "basic/radom_in.h"

namespace pay_logic {

StroagerKafka::StroagerKafka(config::FileConfig* config) {

/*
  if (PRODUCER_INIT_SUCCESS
      != kafka_producer_.Init(
          0,
          "tw",
          "139.224.34.22:9092",
          NULL))
*/
////
  if (PRODUCER_INIT_SUCCESS
      != kafka_producer_.Init(config->kafka_list_.front()))
    LOG_ERROR(" ________________________ producer newsparser_task_test init failed");
  else
    LOG_ERROR(" ________________________ producer newsparser_task_test init success");
}

StroagerKafka::~StroagerKafka() {
  kafka_producer_.Close();
}

void PrintTaskInfo(const base_logic::DictionaryValue* task_info) {
  if (NULL == task_info)
    return;
  int port, size;
  std::string ip;
  task_info->GetInteger(L"size", &size);
  task_info->GetInteger(L"port", &port);
  task_info->GetString(L"ip", &ip);
  //LOG_DEBUG2("analyze_id = %d pos_name=%s", id, value.c_str());
}

void StroagerKafka::Test() {
  const std::string test = "TTTTTTT___";
  if (PUSH_DATA_SUCCESS != kafka_producer_.PushData(test.c_str(), test.length()))
  {
    LOG_ERROR("test __________ kafka producer send data failed");
    LOG_ERROR("test __________ kafka producer send data failed");
    LOG_ERROR("test __________ kafka producer send data failed");
    LOG_ERROR("test __________ kafka producer send data failed");
  }
}

bool StroagerKafka::AddStorageInfo(const std::string& ip, const int64 port, const int32 size) {
  scoped_ptr<base_logic::DictionaryValue> dict(
      new base_logic::DictionaryValue());
  
  int re = PUSH_DATA_SUCCESS;
  base_logic::DictionaryValue* task_info = new base_logic::DictionaryValue();
	  
    //LOG_DEBUG("push data to newsparser_task");
  task_info->Set(L"ip", base_logic::Value::CreateStringValue(ip));
  task_info->Set(L"port", base_logic::Value::CreateIntegerValue(port));
  task_info->Set(L"size", base_logic::Value::CreateIntegerValue(size));
	
  re = kafka_producer_.PushData(task_info);
    //LOG_DEBUG2("key_name=%s", hbase->key_name); LOG_DEBUG2("pos_name=%s", hbase->pos_name);
  PrintTaskInfo(task_info);
  delete task_info;
  
  if (PUSH_DATA_SUCCESS == re)
    return true;
  else {
    LOG_ERROR("kafka producer send data failed");
    return false;
  }
}

}  // namespace storager_logic
