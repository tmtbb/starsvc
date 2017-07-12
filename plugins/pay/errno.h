//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_HISTORY_ERRNO_H_
#define SWP_HISTORY_ERRNO_H_

enum PAYNO {
  NO_DATABASE_ERR = -700,
  WX_ORDER_ERROR = -701,
  STOAGE_ORDER_ERROR = -702,
  UNIPNPAY_ORDER_ERROR = -704,
  UNIWITHDRAW_ERROR = -705, //提现失败
  NO_CHECK_TOKEN_ERRNO = -303
};

#endif
