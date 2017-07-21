//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry

#ifndef STAR_TRADES_DB_H_
#define STAR_TRADES_DB_H_

#include <string>
#include <list>
#include "config/config.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include "logic/star_infos.h"
#include "storage/data_engine.h"
#include "flashsale/flash_info.h"

namespace flash_logic {

class FlashDB {
public:
    FlashDB(config::FileConfig* config);
    ~FlashDB();

public:
    bool OnFetchPublishStar(std::map<std::string, flash_logic::PulishStar>& map);
    bool OnCreateFlashOrder(star_logic::TradesOrder& flash_order, int64& result);
    bool OnUpdateFlashsaleResult(const int64 uid,const std::string& symbol,
                const int64 amount,const double totlePrice);
    bool OnUpdatePublishStarInfo(const std::string& symbol,const int64 publishtype,
                              const int64 publasttime,const int64 pubbegintime);

private:
    static void CallGetPublishRule(void* param, base_logic::Value* value);
    static void CallCreateFlashOrder(void* param, base_logic::Value* value);
private:
    base_logic::DataEngine* mysql_engine_;
};

}  // namespace flash_logic

#endif /* STAR_TRADES_DB_H_ */
