//  Copyright (c) 2017-2018 The Star Authors. All rights reserved.
//  Created on: 2017年5月30日 Author: kerry

#ifndef STAR_QUOTATIONS_DB_H_
#define STAR_QUOTATIONS_DB_H_

#include <string>
#include <map>
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace quotations_logic {

class QuotationsDB {
 public:
  QuotationsDB(config::FileConfig* config);
  ~QuotationsDB();
   
  bool OnGetStarInfo(std::map<std::string,star_logic::StarInfo>& map);
  bool OngetSysParamValue(std::map<std::string,std::string>& parammap);

private: 
  static void CallGetStarInfo(void* param, base_logic::Value* value);

  static void CallgetSysParamValue(void* param, base_logic::Value* value);
private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace quotations_logic

#endif /* STAR_QUOTATIONS_DB_H_ */
