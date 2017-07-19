//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_HISTORY_ERRNO_H_
#define SWP_HISTORY_ERRNO_H_

enum USERERRNO {
  NO_DATABASE_ERR = -300,
  NO_USER_EXIST = -301,
  NO_PASSWORD_ERRNOR = -302,
  NO_CHECK_TOKEN_ERRNO = -303,
  NO_USER_EXIST_REGISTER = -304,
  NO_VERSION_INFO = -305,
  NO_SAVE_DEVICE_ERR = -306,
  NO_USER_NOT_EXIST = -307,
  NO_AGENTID = -308, //
  NO_SUB_AGENTID = -309, //
  NO_MEMBERID = -310, //
  NO_CHANNEL = -311 //
};

#endif
