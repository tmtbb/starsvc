#ifndef SWP_IMCLOUD_OPERATOR_CODE_H_
#define SWP_IMCLOUD_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace market_logic {
#define R_MARKETTYPES_GET    MARKET_TYPE * 1000 + 1
#define S_MARKETTYPES_GET    MARKET_TYPE * 1000 + 2

#define R_MARKSTARLIST_GET    MARKET_TYPE * 1000 + 3
#define S_MARKSTRRLIST_GET    MARKET_TYPE * 1000 + 4

#define R_MARKSTARBRIEF_GET    MARKET_TYPE * 1000 + 5
#define S_MARKSTARBRIEF_GET    MARKET_TYPE * 1000 + 6

}
#endif