//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_TRADES_OPERATOR_CODE_H_
#define SWP_TRADES_OPERATOR_CODE_H_


#include "net/comm_head.h"


namespace trades_logic {

#define R_TRADES_GOODS_DATA    TRADES_TYPE * 1000 + 1
#define S_TRADES_GOODS_DATA    TRADES_TYPE * 1000 + 2

#define R_TRADES_OPEN_POSITION TRADES_TYPE * 1000 + 3
#define S_TRADES_OPEN_POSITION TRADES_TYPE * 1000 + 4

#define R_TRADES_CURRENT_POSITION TRADES_TYPE * 1000 + 5
#define S_TRADES_CURRENT_POSITION TRADES_TYPE * 1000 + 6

#define R_FLIGHT_INFO  TRADES_TYPE * 1000 + 7
#define S_FLIGHT_INFO  TRADES_TYPE * 1000 + 8



}
#endif
