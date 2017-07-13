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

//??ȡ?ѹ?????
#define R_GET_ORDERSTAR    INFO_TYPE * 1000 + 11
#define S_GET_ORDERSTAR    INFO_TYPE * 1000 + 12

//??ȡ??????ѯ
#define R_GET_STARNEWS    INFO_TYPE * 1000 + 13
#define S_GET_STARNEWS    INFO_TYPE * 1000 + 14

//??ȡbanner??Ϣ
#define R_GET_BANNER    INFO_TYPE * 1000 + 15
#define S_GET_BANNER    INFO_TYPE * 1000 + 16

//??ȡ??˿????
#define R_GET_FANSCOMMENT    INFO_TYPE * 1000 + 17
#define S_GET_FANSCOMMENT    INFO_TYPE * 1000 + 18

//??ȡ???Ƿ????б?
#define R_GET_STAR_SERVICE    INFO_TYPE * 1000 + 19
#define S_GET_STAR_SERVICE    INFO_TYPE * 1000 + 20
//?û????????Ƿ???
#define R_ORDER_STAR_SERVICE    INFO_TYPE * 1000 + 21
#define S_ORDER_STAR_SERVICE    INFO_TYPE * 1000 + 22

//??ȡ?û???????????
#define R_ORDER_STAR_NUM    INFO_TYPE * 1000 + 23
#define S_ORDER_STAR_NUM    INFO_TYPE * 1000 + 24

//??ȡ?û?????ʱ??
#define R_ORDER_STAR_TIME    INFO_TYPE * 1000 + 25
#define S_ORDER_STAR_TIME    INFO_TYPE * 1000 + 26
//??ȡ????ʱ??
#define R_STAR_TIME    INFO_TYPE * 1000 + 27
#define S_STAR_TIME    INFO_TYPE * 1000 + 28
//获取用户约见明星详情
#define R_USER_STAR_MEETINFO    INFO_TYPE * 1000 + 29
#define S_USER_STAR_MEETINFO    INFO_TYPE * 1000 + 30
//获取某个明星信息，与获取明星经验配合使用
#define R_STAR_INFO    INFO_TYPE * 1000 + 31
#define S_STAR_INFO    INFO_TYPE * 1000 + 32
}
#endif
