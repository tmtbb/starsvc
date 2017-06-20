//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
//

#ifndef STAR_TRADES_ERRNO_H_
#define STAR_TRADES_ERRNO_H_

enum TRADESERRNO {
    FORMAT_ERRNO = -100,
    NO_LOCK_USER_ORDER_VALID = -101,
    NO_HAVE_TRADES_TIME = -501,
    NO_HAVE_TRADES_STAR = -502,
    NO_HAVE_ORDER = -503,
    NO_CANCEL_ERROR = -504,
    NO_UPDATE_DB_ERROR = -505,
    NO_EXCEPTION_DATA = -506,
    NO_CANNCEL_TIME_OUT = -507
};

#endif
