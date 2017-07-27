//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef STAR_NET_FLASH_OPERATOR_CODE_H_
#define STAR_NET_FLASH_OPERATOR_CODE_H_


#include "net/comm_head.h"


namespace netflash_logic {


#define R_GET_PUB_STAR_STATUS NETFLASH_TYPE * 1000 + 1
#define S_GET_PUB_STAR_STATUS NETFLASH_TYPE * 1000 + 2

#define R_GET_STAR_PUB_INFO NETFLASH_TYPE * 1000 + 3
#define S_GET_STAR_PUB_INFO NETFLASH_TYPE * 1000 + 4

#define R_CONFIRM_FLASH_ORDER NETFLASH_TYPE * 1000 + 5
#define S_CONFIRM_FLASH_ORDER NETFLASH_TYPE * 1000 + 6

#define R_GET_PUB_STAR_LIST NETFLASH_TYPE * 1000 + 7
#define S_GET_PUB_STAR_LIST NETFLASH_TYPE * 1000 + 8

#define R_GET_STAR_EXPR_INFO NETFLASH_TYPE * 1000 + 9
#define S_GET_STAR_EXPR_INFO NETFLASH_TYPE * 1000 + 10

}
#endif 

