//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STAR_HISTORY_OPERATOR_CODE_H_
#define STAR_HISTORY_OPERATOR_CODE_H_


#include "net/comm_head.h"
#include "comm/comm_head.h"


namespace record_logic {
//用户当日委托纪录
#define R_TODAY_POSITION_RECORD  HISTORY_TYPE * 1000 + 1
#define S_TODAY_POSITION_RECORD  HISTORY_TYPE * 1000 + 2
//用户历史委托纪录
#define R_ALL_POSITION_RECORD    HISTORY_TYPE * 1000 + 5
#define S_ALL_POSITION_RECORD    HISTORY_TYPE * 1000 + 6

//用户当日交易记录
#define R_TODAY_ORDER_RECORD  HISTORY_TYPE * 1000 + 7
#define S_TODAY_ORDER_RECORD  HISTORY_TYPE * 1000 + 8
//用户历史交易记录
#define R_ALL_ORDER_RECORD    HISTORY_TYPE * 1000 + 9
#define S_ALL_ORDER_RECORD    HISTORY_TYPE * 1000 + 10
//粉丝委托
#define R_FANS_POSITION      HISTORY_TYPE * 1000 + 11
#define S_FANS_POSITION      HISTORY_TYPE * 1000 + 12
//粉丝排行榜
#define R_FANS_ORDER         HISTORY_TYPE * 1000 + 15
#define S_FANS_ORDER         HISTORY_TYPE * 1000 + 16
//明星交易记录数
#define R_POSITION_COUNT     HISTORY_TYPE * 1000 + 17
#define S_POSITION_COUNT     HISTORY_TYPE * 1000 + 18
//获取明星委托
#define R_STAR_POSITION     HISTORY_TYPE * 1000 + 21
#define S_STAR_POSITION     HISTORY_TYPE * 1000 + 22
//获取所有明星委托，用于弹幕
#define R_ALL_POSITION     HISTORY_TYPE * 1000 + 22
#define S_ALL_POSITION     HISTORY_TYPE * 1000 + 23
}
#endif
