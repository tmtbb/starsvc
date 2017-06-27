//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#ifndef PAY_STORAGER_KAFKA_H_
#define PAY_STORAGER_KAFKA_H_

#include <string>
#include <list>
#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"
#include "queue/kafka_producer.h"

namespace pay_logic {

class StroagerKafka {
 public:
    StroagerKafka(config::FileConfig* config);
    virtual ~StroagerKafka();


 public:
    bool AddStorageInfo(const std::string& ip, const int64 port, const int32 size); 

    void Test();
 private:
    kafka_producer kafka_producer_;
};

}  // namespace storager_logic

#endif /* STORAGE_DB_H_ */
