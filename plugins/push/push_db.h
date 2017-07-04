//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef PUSH_DB_H__
#define PUSH_DB_H__

#include <string>
#include <list>
#include "storage/data_engine.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace push_logic {

class CPushDB {
 public:
  CPushDB(config::FileConfig* config);
  ~CPushDB();

 public:
  bool onGetUserDeviceId(const int64 uid, std::string &deviceId, int32& deviceType);

 public:
  static void CallGetUserDeviceId(void* param, base_logic::Value* value);

 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace push_logic

#endif /* PUSH_DB_H__ */
