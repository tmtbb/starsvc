//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef DATA_REDIS_POOL_H__
#define DATA_REDIS_POOL_H__

#include "storage/storage.h"
#include "basic/basictypes.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "logic/logic_comm.h"

namespace base_logic {

class RedisPool {
 public:
  RedisPool();
  virtual ~RedisPool(){};
 public:
  void Init(std::list<base::ConnAddr>& addrlist, const int32 conn_num = 5);
  void Dest();

  base_storage::DictionaryStorageEngine* RedisConnectionPop(void);
  void RedisConnectionPush(base_storage::DictionaryStorageEngine* engine);
 public:
  std::list<base::ConnAddr> addrlist_;
  std::list<base_storage::DictionaryStorageEngine*> dic_conn_pool_;
  threadrw_t* dic_pool_lock_;

};

}

#endif /* BASE_DIC_REDIS_AUTO_H_ */
