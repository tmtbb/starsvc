//  Copyright (c) 2015-2018 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#ifndef RECORD_KAFKA_H_
#define RECORD_KAFKA_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "queue/kafka_consumer.h"
#include "queue/kafka_producer.h"
#include <list>
#include <map>
#include <string>

namespace record_logic {

class RecordKafka {
public:
    RecordKafka(config::FileConfig *config);
    RecordKafka(const int32 svc_id, base::ConnAddr& addr);
    virtual ~RecordKafka();
private:
    void InitConsumer(base::ConnAddr& addr);
    void InitProducer(base::ConnAddr& addr);
public:
    bool FectchBatchTempTask(std::list<base_logic::DictionaryValue*> *list);
private:
    kafka_consumer kafka_consumer_;
    kafka_producer kafka_producer_;
};
}  // namespace record_logic

#endif /* TASK_DB_H_ */
