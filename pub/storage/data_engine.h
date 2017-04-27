//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry


#ifndef SWP_STORAGE_STORAGE_ENGINE_H_
#define SWP_STORAGE_STORAGE_ENGINE_H_

#include <list>
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "config/config.h"
#include "storage/storage.h"


enum STORAGETYPE{
    REIDS_TYPE = 0,
    MYSQL_TYPE = 1,
    MEM_TYPE = 2
};

enum MEMCACHE_TYPE{
    MEM_KEY_VALUE = 0,
    BATCH_KEY_VALUE = 1,
};

enum REDIS_TYPE{
    HASH_VALUE = 0,
    LIST_VALUE = 1,
    READIS_KEY_VALUE = 1

};


namespace base_logic {

class DataEngine {
 public:
    DataEngine() {}
    virtual ~DataEngine() {}
    static DataEngine* Create(int32 type);
    void Init(config::FileConfig* config);
    void Dest();
 public:
    virtual void Release() = 0;
    virtual void InitParam(std::list<base::ConnAddr>& addrlist) = 0;
 public:
    virtual bool ReadData(const int32 type, base_logic::Value* value,
    void (*storage_get)(void*, base_logic::Value*)) = 0;

    virtual bool WriteData(const int32 type, base_logic::Value* value) = 0;

 private:
    config::FileConfig* config_;
};
}  // namespace base_logic


#endif /* PLUGINS_CRAWLERSVC_STORAGE_STORAGE_BASE_ENGINE_H_ */
