//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.8.18 Author: kerry

#include "flashsale/flash_kafka.h"
#include "basic/radom_in.h"
#include "logic/logic_comm.h"

namespace flash_logic {

FlashKafka::FlashKafka(config::FileConfig* config) {
  /*
   if (PRODUCER_INIT_SUCCESS
   != kafka_producer_.Init(
   0, "kafka_newsparser_algo",
   "192.168.1.85:9091,192.168.1.80:9091,192.168.1.81:9091", NULL))
   LOG_ERROR("producer kafka_newsparser_algo init failed");
   else
   LOG_ERROR("producer kafka_newsparser_algo init success");
   */
  base::ConnAddr addr = config->kafka_list_.front();
  if (PRODUCER_INIT_SUCCESS
      != kafka_producer_.Init(addr))
      LOG_ERROR2("producer init failed: host:%s source %s additional %s",
                addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
  else
      LOG_MSG2("producer init success: host:%s source %s additional %s",
        addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
}

FlashKafka::FlashKafka(base::ConnAddr& addr) {
  if (PRODUCER_INIT_SUCCESS != kafka_producer_.Init(addr))
    LOG_ERROR2("producer init failed: host:%s source:%s additional %s",
              addr.host().c_str(),addr.source().c_str(),addr.additional().c_str());
  else
    LOG_MSG2("producer init success: host:%s source:%s additional:%s",
              addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
}

FlashKafka::~FlashKafka() {
  kafka_producer_.Close();
}

bool FlashKafka::SetFlashOrder(star_logic::TradesOrder& order) {
    int re = PUSH_DATA_SUCCESS;   
    base_logic::DictionaryValue* task_info = order.GetValue();
    re = kafka_producer_.PushData(task_info);
    delete task_info;
    if (PUSH_DATA_SUCCESS == re)
        return true;
    else {
        LOG_ERROR("kafka producer send data failed");
        return false;
    }
}

}
