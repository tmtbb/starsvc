#ifndef SWP_IMCLOUD_OPERATOR_CODE_H_
#define SWP_IMCLOUD_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace users_logic {
#define R_IMCLOUD_REGISTER    IM_TYPE * 1000 + 1
#define S_IMCLOUD_REGISTER    IM_TYPE * 1000 + 2

#define R_IMCLOUD_LOGIN  IM_TYPE * 1000 + 3
#define S_IMCLOUD_LOGIN  IM_TYPE * 1000 + 4

#define R_IMCLOUD_GETTOKEN IM_TYPE * 1000 + 5
#define S_IMCLOUD_GETTOKEN IM_TYPE * 1000 + 6

#define R_IMCLOUD_REFRESHTOKEN IM_TYPE * 1000 + 7
#define S_IMCLOUD_REFRESHTOKEN IM_TYPE * 1000 + 8
}
#endif