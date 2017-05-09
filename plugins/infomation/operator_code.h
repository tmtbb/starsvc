#ifndef SWP_IMCLOUD_OPERATOR_CODE_H_
#define SWP_IMCLOUD_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace infomation_logic {
#define R_STARINFOLIST_GET    INFO_TYPE * 1000 + 1
#define S_STARINFOLIST_GET    INFO_TYPE * 1000 + 2

#define R_STARINFOLIST_ADD    INFO_TYPE * 1000 + 3
#define S_STARINFOLIST_ADD    INFO_TYPE * 1000 + 4

#define R_STARINFOLIST_DEL    INFO_TYPE * 1000 + 5
#define S_STARINFOLIST_DEL    INFO_TYPE * 1000 + 6

#define R_STARINFOLIST_EDIT    INFO_TYPE * 1000 + 7
#define S_STARINFOLIST_EDIT    INFO_TYPE * 1000 + 8


}
#endif