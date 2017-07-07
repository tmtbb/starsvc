//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef DATA_MYSQL_POOL_H__
#define DATA_MYSQL_POOL_H__

#include "storage/storage.h"
#include "basic/basictypes.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "logic/logic_comm.h"

namespace base_logic {

class MYSQL_Pool {
 public:
	MYSQL_Pool();
	virtual ~MYSQL_Pool(){};
 public:
	void Init(std::list<base::ConnAddr>& addrlist, const int32 db_conn_num = 1);
	void Dest();
 public:
	base_storage::DBStorageEngine* DBConnectionPop(void);
	void DBConnectionPush(base_storage::DBStorageEngine* engine);
 public:
	std::list<base_storage::DBStorageEngine*>     db_conn_pool_;
	threadrw_t*                                   db_pool_lock_;
	std::list<base::ConnAddr>                     addrlist_;
};

}

#endif
