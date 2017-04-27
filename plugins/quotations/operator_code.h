//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_QUOTATIONS_OPERATOR_CODE_H_
#define SWP_QUOTATIONS_OPERATOR_CODE_H_


#include "net/comm_head.h"


namespace quatations_logic {

#define LOGIN_QUOTATIONS               2001
#define R_QUOTATIONS_REAL_TIME_DATA    QUOTATIONS_TYPE * 1000 + 1
#define S_QUOTATIONS_REAL_TIME_DATA    QUOTATIONS_TYPE * 1000 + 2
#define R_QUOTATIONS_TIME_LINE_DATA    QUOTATIONS_TYPE * 1000 + 3
#define S_QUOTATIONS_TIME_LINE_DATA    QUOTATIONS_TYPE * 1000 + 4
#define R_KCHART_TIME_LINE_DATA        QUOTATIONS_TYPE * 1000 + 5
#define S_KCHART_TIME_LINE_DATA        QUOTATIONS_TYPE * 1000 + 6

}
#endif
