//  Copyright (c) 2015-2018 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#ifndef PUSH_KAFKA_H_
#define PUSH_KAFKA_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "queue/kafka_consumer.h"
#include "queue/kafka_producer.h"
#include <list>
#include <map>
#include <string>

namespace push_logic {

class CPushKafka {
public:
    CPushKafka(config::FileConfig *config);
    CPushKafka(const int32 svc_id, base::ConnAddr& addr);
    virtual ~CPushKafka();
public:
    bool FectchBatchTask(std::list<base_logic::DictionaryValue*> *list);
private:
    kafka_consumer kafka_consumer_;
};
}  // namespace push_logic

#endif /* PUSH_KAFKA_H_ */
