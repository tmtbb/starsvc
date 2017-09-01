//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#include "circle/kafka.h"
#include "basic/basic_util.h"
#include "basic/radom_in.h"
#include "basic/template.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "logic/time.h"
#include <mysql.h>
#include <set>
#include <sstream>

namespace circle_logic {

Kafka::Kafka(config::FileConfig *config) {

/*
   if (CONSUMER_INIT_SUCCESS
   != kafka_consumer_.Init(
   0,
   "tw",
   "139.224.34.22:9092",
   //"61.147.114.85:9092,61.147.114.80:9092,61.147.114.81:9092",
   NULL))
*/ 
  for(std::list<base::ConnAddr>::iterator it = config->kafka_list_.begin();
            it != config->kafka_list_.end(); it++) {
        base::ConnAddr addr = (*it);
        if (addr.additional() == "consumer")
        {
            if (CONSUMER_INIT_SUCCESS != kafka_consumer_.Init(addr)) 
                LOG_ERROR2("consumer init failed: host:%s source %s additional %s",
                   addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
            else
                LOG_MSG2("consumer init success: host:%s source %s additional %s",
                   addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
        }
        else if(addr.additional() == "producer")
        {
            if (CONSUMER_INIT_SUCCESS != kafka_producer_.Init(addr))
                LOG_ERROR2("producer init failed: host:%s source %s additional %s",
                           addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
            else
                LOG_MSG2("producer init success: host:%s source %s additional %s",
                         addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());

        }
  }

}

Kafka::Kafka(const int32 svc_id, base::ConnAddr& addr) {
  std::string source = addr.source() + "_" + base::BasicUtil::StringUtil::Int64ToString(svc_id);
  base::ConnAddr new_addr(svc_id, addr.host(), addr.port(),
                          addr.usr(), addr.pwd(),source);

  if (CONSUMER_INIT_SUCCESS != kafka_consumer_.Init(new_addr))
    LOG_ERROR("kafka consumer kafka_task_algo init failed");
  else
    LOG_MSG("kafka consumer kafka_task_algo init success");
}

Kafka::~Kafka() {
  kafka_consumer_.Close();
  kafka_producer_.Close();
  LOG_MSG("~Kafka()");
}

bool Kafka::AddStorageInfo(base_logic::DictionaryValue* task_info) {
  //scoped_ptr<base_logic::DictionaryValue> dict(
   //   new base_logic::DictionaryValue());
  
  int re = PUSH_DATA_SUCCESS;
	
  re = kafka_producer_.PushData(task_info);
    //LOG_DEBUG2("key_name=%s", hbase->key_name); LOG_DEBUG2("pos_name=%s", hbase->pos_name);
  //PrintTaskInfo(task_info);
  
  if (PUSH_DATA_SUCCESS == re)
    return true;
  else {
    LOG_ERROR("kafka producer send data failed");
    return false;
  }
}

bool Kafka::AddStorageInfo(const std::string& ip, const int64 port, const int32 size) {
  //scoped_ptr<base_logic::DictionaryValue> dict(
   //   new base_logic::DictionaryValue());
  
  int re = PUSH_DATA_SUCCESS;
  base_logic::DictionaryValue* task_info = new base_logic::DictionaryValue();
	  
    //LOG_DEBUG("push data to newsparser_task");
  task_info->Set(L"ip", base_logic::Value::CreateStringValue(ip));
  task_info->Set(L"port", base_logic::Value::CreateIntegerValue(port));
  task_info->Set(L"size", base_logic::Value::CreateIntegerValue(size));
	
  re = kafka_producer_.PushData(task_info);
    //LOG_DEBUG2("key_name=%s", hbase->key_name); LOG_DEBUG2("pos_name=%s", hbase->pos_name);
  //PrintTaskInfo(task_info);
  delete task_info;
  
  if (PUSH_DATA_SUCCESS == re)
    return true;
  else {
    LOG_ERROR("kafka producer send data failed");
    return false;
  }
}



bool Kafka::FectchBatchQuestionTask(std::vector<base_logic::DictionaryValue*> &vec) {
  std::set < std::string > data_list;
  std::string data;
  // todo 换成性能更高的回调函数模式，需爬虫流量控制功能配合
  for (int i = 0; i < 1000; i++) {
    int pull_re = kafka_consumer_.PullData(data);
    if (CONSUMER_CONFIG_ERROR == pull_re) {
      LOG_MSG2("CONSUMER_CONFIG_ERROR,pull_re=%d", pull_re);
    }
    if (PULL_DATA_DATA_NULL == pull_re) {
      //LOG_MSG("data =[]____________________________________________________" );
      break;
    }
    if (PULL_DATA_TIMEOUT == pull_re) {
      LOG_MSG2("consumer get url timeout,pull_re=%d", pull_re);
      break;
    }
    //LOG_MSG2("data =[%s]____________________________________________________", data.c_str());
    data_list.insert(data);
  }
  std::string t_ip;
  int64 t_port, t_size;

  for (std::set<std::string>::iterator it = data_list.begin(); it != data_list.end(); it++) {
    std::string data = *it;
    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(0, &data);
    int error_code = 0;
    std::string error_str;
    base_logic::Value *value = engine->Deserialize(&error_code, &error_str);
    if (0 != error_code || NULL == value)
      continue;
    base_logic::DictionaryValue *task_info_dic = (base_logic::DictionaryValue *) value;

    //task_info_dic->GetString(L"ip", &t_ip);
    //task_info_dic->GetBigInteger(L"port", &t_port);
    //task_info_dic->GetBigInteger(L"size", &t_size);

    //LOG_MSG2("__________find _= ip[%s] port [%d] size[%d] _____________", iter->second.ip().c_str(), iter->second.port(), iter->second.size());
    vec.push_back(task_info_dic);
    //delete task_info_dic;
    base_logic::ValueSerializer::DeleteSerializer(0, engine);
  }  
  // LOG_DEBUG2("update task info, total task num:%d", list->size());

  return true;
}
/*
bool Kafka::FectchBatchTempTask(std::map<std::string, router_logic::TServerItem> &s_map) {
  std::set < std::string > data_list;
  std::string data;
  // todo 换成性能更高的回调函数模式，需爬虫流量控制功能配合
  for (int i = 0; i < 1000; i++) {
    int pull_re = kafka_consumer_.PullData(data);
    if (CONSUMER_CONFIG_ERROR == pull_re) {
      LOG_MSG2("CONSUMER_CONFIG_ERROR,pull_re=%d", pull_re);
    }
    if (PULL_DATA_DATA_NULL == pull_re) {
      //LOG_MSG("data =[]____________________________________________________" );
      break;
    }
    if (PULL_DATA_TIMEOUT == pull_re) {
      LOG_MSG2("consumer get url timeout,pull_re=%d", pull_re);
      break;
    }
    //LOG_MSG2("data =[%s]____________________________________________________", data.c_str());
    data_list.insert(data);
  }
  std::string t_ip;
  int64 t_port, t_size;
  router_logic::TServerItem item; 
  std::string key;
  for (std::set<std::string>::iterator it = data_list.begin(); it != data_list.end(); it++) {
    std::string data = *it;
    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(0, &data);
    int error_code = 0;
    std::string error_str;
    base_logic::Value *value = engine->Deserialize(&error_code, &error_str);
    if (0 != error_code || NULL == value)
      continue;
    base_logic::DictionaryValue *task_info_dic = (base_logic::DictionaryValue *) value;

    std::map<std::string, router_logic::TServerItem>::iterator iter;
    task_info_dic->GetString(L"ip", &t_ip);
    task_info_dic->GetBigInteger(L"port", &t_port);
    task_info_dic->GetBigInteger(L"size", &t_size);

    key = t_ip + base::BasicUtil::StringUtil::Int64ToString(t_port);
    iter = s_map.find(key);
    if (iter == s_map.end()) //未找到
    {
      //item.set_port((int8) t_port);
      item.set_ip(t_ip);
      item.set_port((int32)t_port);
      item.set_size((int32)t_size);
      item.set_is_effective(true);
      item.set_upd_last_time(time_t(NULL));
      s_map.insert(std::pair<std::string, router_logic::TServerItem>(key, item)); 

      LOG_MSG2("nofind _= ip[%s] port [%d] ____________________________________________________", item.ip().c_str(), item.port());
    }
    else
    {
      iter->second.set_is_effective(true);
      iter->second.set_size((int32)t_size);
      iter->second.set_upd_last_time(time_t(NULL));
      LOG_MSG2("__________find _= ip[%s] port [%d] size[%d] _____________", iter->second.ip().c_str(), iter->second.port(), iter->second.size());
    }

    delete task_info_dic;
    base_logic::ValueSerializer::DeleteSerializer(0, engine);
  }  // LOG_DEBUG2("update task info, total task num:%d", list->size());

  return true;
}
*/


/*
*/
}  // namespace crawler_task_logic
