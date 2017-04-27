//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "redis_pool.h"

namespace base_logic{


RedisPool::RedisPool()
:dic_pool_lock_(NULL){

}

void RedisPool::Init(std::list<base::ConnAddr>& addrlist,const int32 conn_num){
	bool r =false;
	addrlist_ = addrlist;
	InitThreadrw(&dic_pool_lock_);
	for (int i = 0; i<=conn_num;i++){
		base_storage::DictionaryStorageEngine* engine =
				base_storage::DictionaryStorageEngine::Create(base_storage::IMPL_RADIES);

		if (engine==NULL){
			assert(0);
			LOG_ERROR("create redis conntion error");
			continue;
		}

		LOG_MSG2("redis ip:%s port:%d",addrlist_.front().host().c_str(),
		         addrlist_.front().port());

		bool r =  engine->Connections(addrlist_);
		if (!r) {
		  LOG_ERROR("redis conntion error");
			continue;
		}

		dic_conn_pool_.push_back(engine);
	}
}


void RedisPool::Dest(){
	base_logic::WLockGd lk(dic_pool_lock_);
	while(dic_conn_pool_.size()>0){
		base_storage::DictionaryStorageEngine* engine = dic_conn_pool_.front();
		dic_conn_pool_.pop_front();
		if(engine){
			engine->Release();
			delete engine;
			engine =NULL;
		}
	}
	DeinitThreadrw(dic_pool_lock_);
}

void RedisPool::RedisConnectionPush(base_storage::DictionaryStorageEngine* engine){
	base_logic::WLockGd lk(dic_pool_lock_);
	dic_conn_pool_.push_back(engine);
}

base_storage::DictionaryStorageEngine* RedisPool::RedisConnectionPop(){
	base_logic::WLockGd lk(dic_pool_lock_);
	if(dic_conn_pool_.size()<=0)
		return NULL;
    base_storage::DictionaryStorageEngine* engine = dic_conn_pool_.front();
    dic_conn_pool_.pop_front();
    return engine;
}


}



