//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STAR_RECORD_DB_H_
#define STAR_RECORD_DB_H_

#include <string>
#include <list>
#include "record/record_info.h"
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace record_logic {

class RecordDB {
 public:
    RecordDB(config::FileConfig* config);
    ~RecordDB();

 public:
    bool OnGetAllUserInfo(std::map<int64, star_logic::UserInfo>& user_map);
    bool OnGetAllOrderInfo(std::map<int64, star_logic::TradesOrder>& order_map);
    bool OnGetAllPositionInfo(std::map<int64, star_logic::TradesPosition>& position_map);
    bool OnCreateTradesPosition(star_logic::TradesPosition& trades_position);
    bool OnUpdateTradesPosition(star_logic::TradesPosition& trades_position);
 private:
    static void CallGetAllUserInfo(void* param, base_logic::Value* value);
    static void CallGetAllOrderInfo(void* param, base_logic::Value* value);
    static void CallGetAllPositionInfo(void* param, base_logic::Value* value);
 private:
    base_logic::DataEngine* mysql_engine_;
};

}  // namespace record_logic

#endif /* STAR_RECORD_DB_H_ */
