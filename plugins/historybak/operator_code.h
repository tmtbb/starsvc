//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_HISTORY_OPERATOR_CODE_H_
#define SWP_HISTORY_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace history_logic {
#define R_HISTORY_TRADES    HISTORY_TYPE * 1000 + 1
#define S_HISTORY_TRADES    HISTORY_TYPE * 1000 + 2

#define R_HISTORY_RECHARGE  HISTORY_TYPE * 1000 + 3
#define S_HISTORY_RECHARGE  HISTORY_TYPE * 1000 + 4

#define R_HISOTRY_WITHDRAWALS HISTORY_TYPE * 1000 + 5
#define S_HISOTRY_WITHDRAWALS HISTORY_TYPE * 1000 + 6

#define R_HISTORY_TRADES_HANDLE HISTORY_TYPE * 1000 + 7
#define S_HISTORY_TRADES_HANDLE HISTORY_TYPE * 1000 + 8

}
#endif
