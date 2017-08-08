//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry

#ifndef SATR_NET_FLASH_DB_H_
#define SATR_NET_FLASH_DB_H_

#include <string>
#include <list>
#include "config/config.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include "logic/star_infos.h"
#include "storage/data_engine.h"
//#include "netflashsale/netflash_info.h"

namespace netflash_logic {

class NetflashDB {
public:
  NetflashDB(config::FileConfig* config);
  ~NetflashDB();

public:
  bool OnGetStarExperience(const std::string& code, base_logic::DictionaryValue &ret);
  bool OnCreateNetflashOrder(star_logic::TradesOrder& netflash_order, int64& result);
  bool OnUpdatePublishStarInfo(const std::string& symbol,const int64 publishtype,
                            const int64 publasttime,const int64 pubbegintime);
  bool OnUpdateNetflashsaleResult(const int64& orderid, const int32 status);

private:
  static void CallgetStarExperience(void* param, base_logic::Value* value);
  static void CallCreateNetflashOrder(void* param, base_logic::Value* value);
private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace netflash_logic

#endif /* SATR_NET_FLASH_DB_H_ */
