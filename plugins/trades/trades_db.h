//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry

#ifndef STAR_TRADES_DB_H_
#define STAR_TRADES_DB_H_

#include <string>
#include <list>
#include "trades/trades_info.h"
#include "logic/star_infos.h"
#include "storage/data_engine.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace trades_logic {

class TradesDB {
public:
    TradesDB(config::FileConfig* config);
    ~TradesDB();

public:
    bool OnFetchPlatformStar(std::map<std::string, trades_logic::TradesStar>& map);
    bool OnCreateTradesOrder(star_logic::TradesOrder& trades_order);
    bool OnUpdateOrderState(const int64 order_id,const int64 uid, const int32 state,
                            const int32 uid_type);

    int32 OnConfirmOrder(const int64 order_id,const int64 buy_uid, const int64 sell_uid);
private:
    static void CallGetTradsRule(void* param, base_logic::Value* value);
    static void CallCreateOrder(void* param, base_logic::Value* value);
    static void CallUpdateOrderState(void* param, base_logic::Value* value);
    static void CallConfirmOrder(void* param, base_logic::Value* value);
private:
    base_logic::DataEngine* mysql_engine_;
};

}  // namespace trades_logic

#endif /* STAR_TRADES_DB_H_ */
