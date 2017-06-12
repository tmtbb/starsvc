//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_BANK_OPERATOR_CODE_H_
#define SWP_BANK_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace bank_logic {

#define R_WITH_DRAW_CASH    BANK_TYPE * 1000 + 1
#define S_WITH_DRAW_CASH    BANK_TYPE * 1000 + 2

#define R_GET_BIND_BANK    BANK_TYPE * 1000 + 3
#define S_GET_BIND_BANK    BANK_TYPE * 1000 + 4

#define R_BIND_BANK_CARD   BANK_TYPE * 1000 + 5
#define S_BIND_BANK_CARD   BANK_TYPE * 1000 + 6

#define R_UNBIND_BANK_CARD  BANK_TYPE * 1000 + 7
#define S_UNBIND_BANK_CARD  BANK_TYPE * 1000 + 8

#define R_GET_BANK_CARD_INFO BANK_TYPE * 1000 + 9
#define S_GET_BANK_CARD_INFO BANK_TYPE * 1000 + 10
}
#endif
