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

}
#endif
