//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_PAY_OPERATOR_CODE_H_
#define SWP_PAY_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace pay_logic {

#define R_WEIXIN_PAY    PAY_TYPE * 1000 + 33
#define S_WEIXIN_PAY    PAY_TYPE * 1000 + 34

#define R_WEIXIN_CLT    PAY_TYPE * 1000 + 35
#define S_WEIXIN_CLT    PAY_TYPE * 1000 + 36

#define R_WEIXIN_SVC    PAY_TYPE * 1000 + 37
#define S_WEIXIN_SVC    PAY_TYPE * 1000 + 38

#define S_NOTICE_SVC    PAY_TYPE * 1000 + 2

#define R_THIRD_PAY    PAY_TYPE * 1000 + 39
#define S_THIRD_PAY    PAY_TYPE * 1000 + 40

#define R_THIRD_CLT    PAY_TYPE * 1000 + 41
#define S_THIRD_CLT    PAY_TYPE * 1000 + 42

#define R_THIRD_SVC    PAY_TYPE * 1000 + 43
#define S_THIRD_SVC    PAY_TYPE * 1000 + 44

#define R_THIRD_CASH    PAY_TYPE * 1000 + 45
#define S_THIRD_CASH    PAY_TYPE * 1000 + 46

#define R_THIRD_CASH_CLT    PAY_TYPE * 1000 + 47
#define S_THIRD_CASH_CLT    PAY_TYPE * 1000 + 48

#define R_THIRD_CASH_SVC    PAY_TYPE * 1000 + 49
#define S_THIRD_CASH_SVC    PAY_TYPE * 1000 + 50

}
#endif
