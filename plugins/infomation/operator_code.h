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

#define R_ADD_ORDERSTAR    INFO_TYPE * 1000 + 9
#define S_ADD_ORDERSTAR    INFO_TYPE * 1000 + 10

//获取已购明星
#define R_GET_ORDERSTAR    INFO_TYPE * 1000 + 11
#define S_GET_ORDERSTAR    INFO_TYPE * 1000 + 12

//获取明星咨询
#define R_GET_STARNEWS    INFO_TYPE * 1000 + 13
#define S_GET_STARNEWS    INFO_TYPE * 1000 + 14

//获取banner信息
#define R_GET_BANNER    INFO_TYPE * 1000 + 15
#define S_GET_BANNER    INFO_TYPE * 1000 + 16

//获取粉丝评论
#define R_GET_FANSCOMMENT    INFO_TYPE * 1000 + 17
#define S_GET_FANSCOMMENT    INFO_TYPE * 1000 + 18

//获取明星服务列表
#define R_GET_STAR_SERVICE    INFO_TYPE * 1000 + 19
#define S_GET_STAR_SERVICE    INFO_TYPE * 1000 + 20
//用户订购明星服务
#define R_ORDER_STAR_SERVICE    INFO_TYPE * 1000 + 21
#define S_ORDER_STAR_SERVICE    INFO_TYPE * 1000 + 22
}
#endif
