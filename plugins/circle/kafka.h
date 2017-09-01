//  Copyright (c) 2015-2018 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#ifndef CIRCLE_CRAWLER_TASK_KAFKA_H_
#define CIRCLE_CRAWLER_TASK_KAFKA_H_

#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include "queue/kafka_consumer.h"
#include "queue/kafka_producer.h"
///#include "router/router_proto_buf.h"
#include <list>
#include <map>
#include <string>

namespace circle_logic {
    
typedef enum{
    UserAsk = 0,
    StarAnswer = 1,
    UserSeeQuestion
}MessType;

class Kafka {
 public:
  Kafka(config::FileConfig *config);
  Kafka(const int32 svc_id, base::ConnAddr& addr);
  virtual ~Kafka();

 public:
  bool FectchBatchQuestionTask(std::vector<base_logic::DictionaryValue *> &v);
//  bool FectchBatchTempTask(std::map<std::string, router_logic::TServerItem> &s_map);

  bool AddStorageInfo(const std::string& ip, const int64 port, const int32 size); 
  bool AddStorageInfo(base_logic::DictionaryValue* task_info); 
 private:
  kafka_consumer kafka_consumer_;
  kafka_producer kafka_producer_;
};
}  // namespace crawler_task_logic

#endif /* TASK_DB_H_ */
