//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "storage/data_engine.h"
#include "storage/data_mysql_engine.h"
#include "storage/data_redis_engine.h"

namespace base_logic {

DataEngine* DataEngine::Create(int32 type) {
  DataEngine* engine = NULL;
  switch (type) {
    case MYSQL_TYPE: {
      engine = new DataMYSQLEngine();
      break;
    }
    case REIDS_TYPE: {
      engine = new DataRedisEngine();
      break;
    }
    default:
      break;
  }
  return engine;
}

void DataEngine::Init(config::FileConfig* config) {
  config_ = config;
}

void DataEngine::Dest() {
}

}  // namespace base_logic

