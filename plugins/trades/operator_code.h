//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef STAR_TRADES_OPERATOR_CODE_H_
#define STAR_TRADES_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace trades_logic {


#define R_TRADES_OPEN_POSITION TRADES_TYPE * 1000 + 1
#define S_TRADES_OPEN_POSITION TRADES_TYPE * 1000 + 2

#define R_TRADES_CURRENT_POSITION TRADES_TYPE * 1000 + 3
#define S_TRADES_CURRENT_POSITION TRADES_TYPE * 1000 + 4

#define R_TRADES_SYMBOL_STATUS TRADES_TYPE * 1000 + 5
#define S_TRADES_SYMBOL_STATUS TRADES_TYPE * 1000 + 6

#define R_CONFIRM_ORDER TRADES_TYPE * 1000 + 7
#define S_CONFIRM_ORDER TRADES_TYPE * 1000 + 8

#define R_CANCEL_ORDER TRADES_TYPE * 1000 + 9

#define S_MATCH_NOTICE TRADES_TYPE * 1000 + 101
#define S_ORDER_RESULT TRADES_TYPE * 1000 + 102

}
#endif
