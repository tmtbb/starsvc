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

#define R_RESET_PAY_PASS USERS_TYPE * 1000 + 5
#define S_RESET_PAY_PASS USERS_TYPE * 1000 + 6

#define R_ACCOUNT_ASSET   USERS_TYPE * 1000 + 7
#define S_ACCOUNT_ASSET   USERS_TYPE * 1000 + 8

#define R_ACCOUNT_CHECK   USERS_TYPE * 1000 + 9
#define S_ACCOUNT_CHECK   USERS_TYPE * 1000 + 10

#define R_REGISTER_VERFIY_CODE USERS_TYPE * 1000 + 11
#define S_REGISTER_VERFIY_CODE USERS_TYPE * 1000 + 12

//add
#define R_WX_LOGIN USERS_TYPE * 1000 + 13
#define S_WX_LOGIN USERS_TYPE * 1000 + 14
#define R_WX_BIND_ACCOUNT USERS_TYPE * 1000 + 15
#define S_WX_BIND_ACCOUNT USERS_TYPE * 1000 + 16

#define R_ACCOUNT_CHANGEPASSWD USERS_TYPE * 1000 + 17
#define S_ACCOUNT_CHANGEPASSWD USERS_TYPE * 1000 + 18

#define R_USRES_RESET_PASSWD USERS_TYPE * 1000 + 19
#define S_USRES_RESET_PASSWD USERS_TYPE * 1000 + 20


#define R_CERTIFICATION USERS_TYPE * 1000 + 21
#define S_CERTIFICATION USERS_TYPE * 1000 + 22
 
#define R_ACCOUNT_REALINFO   USERS_TYPE * 1000 + 27
#define S_ACCOUNT_REALINFO   USERS_TYPE * 1000 + 28

#define R_CHECK_ACCOUNT_EXIST   USERS_TYPE * 1000 + 29
#define S_CHECK_ACCOUNT_EXIST   USERS_TYPE * 1000 + 30
//add 修改nickname
#define R_USRES_RESET_NICK_NAME USERS_TYPE * 1000 + 31
#define S_USRES_RESET_NICK_NAME USERS_TYPE * 1000 + 32

#define R_GET_VERSION USERS_TYPE * 1000 + 33
#define S_GET_VERSION USERS_TYPE * 1000 + 34

#define R_SET_DEVICE_INFO USERS_TYPE * 1000 + 35
#define S_SET_DEVICE_INFO USERS_TYPE * 1000 + 36
//获取佣金信息
#define R_SET_COMMISSION_INFO USERS_TYPE * 1000 + 37
#define S_SET_COMMISSION_INFO USERS_TYPE * 1000 + 38

#define S_LOGIN_EXISTS USERS_TYPE * 1000 + 40
}
#endif
