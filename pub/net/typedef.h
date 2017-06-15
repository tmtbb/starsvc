// Copyright (c) 2016 The tourism Authors. All rights reserved.
// typedef.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_NET_TYPEDEF_H_
#define PUB_NET_TYPEDEF_H_

#include <map>

#include "comm/user_info.h"
//#include "pub/comm/share_guide.h"
#include "logic/base_values.h"

typedef std::map<int64, UserInfo*> UserMap;
typedef std::map<int64, std::string> DeviceTokenMap;
typedef std::map<int64, int32> UnReadMap;
typedef std::map<int64, std::string> NickMap;
typedef std::map<std::string, std::string> SMSTokenMap;
typedef std::map<int64, int64> ShareIdMap;
typedef std::map<int64, ShareIdMap> ShareTypeMap;
typedef std::map<int32, int64> SaltMap;


#endif  // PUB_NET_TYPEDEF_H_
