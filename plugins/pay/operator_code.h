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

#define R_UNION_PAY    PAY_TYPE * 1000 + 39
#define S_UNION_PAY    PAY_TYPE * 1000 + 40

#define R_UNION_CLT    PAY_TYPE * 1000 + 41
#define S_UNION_CLT    PAY_TYPE * 1000 + 42

#define R_UNION_SVC    PAY_TYPE * 1000 + 44

#define R_CERTIFICATION PAY_TYPE * 1000 + 45
#define S_CERTIFICATION PAY_TYPE * 1000 + 46

#define R_CHECK_PAY_PWD PAY_TYPE * 1000 + 11
#define S_CHECK_PAY_PWD PAY_TYPE * 1000 + 12

//
#define R_ALIPAY_PAY PAY_TYPE * 1000 + 49
#define S_ALIPAY_PAY PAY_TYPE * 1000 + 50

#define R_ALIPAY_CLT PAY_TYPE * 1000 + 51 
#define S_ALIPAY_CLT PAY_TYPE * 1000 + 52

#define R_ALIPAY_SVC PAY_TYPE * 1000 + 53
#define S_ALIPAY_SVC PAY_TYPE * 1000 + 54

#define R_CANCLE_PAY PAY_TYPE * 1000 + 55
#define S_CANCLE_PAY PAY_TYPE * 1000 + 56

#define R_UNION_WITHDRAW PAY_TYPE * 1000 + 57
#define S_UNION_WITHDRAW PAY_TYPE * 1000 + 58
}
#endif
