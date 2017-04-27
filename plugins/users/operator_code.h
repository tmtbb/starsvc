//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_USERS_OPERATOR_CODE_H_
#define SWP_USERS_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace users_logic {
#define R_ACCOUNT_REGISTER    USERS_TYPE * 1000 + 1
#define S_ACCOUNT_REGISTER    USERS_TYPE * 1000 + 2

#define R_ACCOUNT_LOGIN  USERS_TYPE * 1000 + 3
#define S_ACCOUNT_LOGIN  USERS_TYPE * 1000 + 4

#define R_ACCOUNT_ASSET   USERS_TYPE * 1000 + 7
#define S_ACCOUNT_ASSET   USERS_TYPE * 1000 + 8

#define R_ACCOUNT_CHECK   USERS_TYPE * 1000 + 9
#define S_ACCOUNT_CHECK   USERS_TYPE * 1000 + 10

#define R_REGISTER_VERFIY_CODE USER_TYPE * 1000 + 11
#define S_REGISTER_VERFIY_CODE USER_TYPE * 1000 + 12
}
#endif
