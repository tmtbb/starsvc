//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef DATA_REDIS_ENGINE_H__
#define DATA_REDIS_ENGINE_H__

#include <string>
#include <list>
#include "redis_pool.h"
#include "storage/data_engine.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace base_logic {

class DataRedisEngine : public DataEngine {
 public:
  DataRedisEngine() {
  }
  virtual ~DataRedisEngine() {
  }

 public:
  void InitParam(std::list<base::ConnAddr> &addrlist);
  void Release();

 public:
  bool ReadData(const int32 type, base_logic::Value *value,
                void (*storage_get)(void *, base_logic::Value *));

  bool WriteData(const int32 type, base_logic::Value *value);
 private:
  bool WriteHashValue(base_logic::Value* value);
 private:
  bool ReadListValue(base_logic::Value* value);

 private:
  std::list<base::ConnAddr> addrlist_;
  base_logic::RedisPool redis_pool_;

};

} /*namespace base_logic */

#endif
