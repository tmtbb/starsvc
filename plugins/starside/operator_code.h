//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_STARSIDE_OPERATOR_CODE_H_
#define SWP_STARSIDE_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace starside_logic {
#define R_STARSIDE_TRANSSTATIS STARSIDE_TYPE * 1000 + 1
#define S_STARSIDE_TRANSSTATIS STARSIDE_TYPE * 1000 + 2

#define R_STARSIDE_DETAIL STARSIDE_TYPE * 1000 + 3
#define S_STARSIDE_DETAIL STARSIDE_TYPE * 1000 + 4

#define R_STARSIDE_GETSTARSERVICE STARSIDE_TYPE * 1000 + 5  //获取明星活动类型
#define S_STARSIDE_GETSTARSERVICE STARSIDE_TYPE * 1000 + 6

#define R_STARSIDE_GETALLSERVICE STARSIDE_TYPE * 1000 + 7 //获取所有活动类型
#define S_STARSIDE_GETALLSERVICE STARSIDE_TYPE * 1000 + 8

#define R_STARSIDE_GETSTARMEETREL STARSIDE_TYPE * 1000 + 9 //获取预约明星列表
#define S_STARSIDE_GETSTARMEETREL STARSIDE_TYPE * 1000 + 10
//--------------------modify
#define R_STARSIDE_UPDSTARSERVICE STARSIDE_TYPE * 1000 + 11 //修改明星活动类型
#define S_STARSIDE_UPDSTARSERVICE STARSIDE_TYPE * 1000 + 12

#define R_STARSIDE_UPDSTARMEETREL STARSIDE_TYPE * 1000 + 13 //修改明星预约状态(明星同意预约)
#define S_STARSIDE_UPDSTARMEETREL STARSIDE_TYPE * 1000 + 14

#define R_STARSIDE_GETOWNSTARUSER STARSIDE_TYPE * 1000 + 19 //获取拥有该明星时间的用户
#define S_STARSIDE_GETOWNSTARUSER STARSIDE_TYPE * 1000 + 20 
///

#define R_STARSIDE_RECHARGE  STARSIDE_TYPE * 1000 + 15
#define S_STARSIDE_RECHARGE  STARSIDE_TYPE * 1000 + 16

#define R_STARSIDE_OWNSTAR STARSIDE_TYPE * 1000 + 17
#define S_STARSIDE_OWNSTAR STARSIDE_TYPE * 1000 + 18
}
#endif
