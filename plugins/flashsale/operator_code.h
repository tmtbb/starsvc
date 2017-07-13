//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef STAR_FLASH_OPERATOR_CODE_H_
#define STAR_FLASH_OPERATOR_CODE_H_


#include "net/comm_head.h"


namespace flash_logic {


#define R_GET_PUB_STAR_STATUS FLASH_TYPE * 1000 + 1
#define S_GET_PUB_STAR_STATUS FLASH_TYPE * 1000 + 2

#define R_GET_PUB_STAR_INFO FLASH_TYPE * 1000 + 3
#define S_GET_PUB_STAR_INFO FLASH_TYPE * 1000 + 4

#define R_CONFIRM_FLASH_ORDER FLASH_TYPE * 1000 + 5
#define S_CONFIRM_FLASH_ORDER FLASH_TYPE * 1000 + 6


#define S_ORDER_RESULT TRADES_TYPE * 1000 + 102

}
#endif 

