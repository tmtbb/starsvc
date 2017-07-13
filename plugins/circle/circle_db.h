//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2015年9月17日 Author: kerry

#ifndef STAR_CIRCLE_DB_H_
#define STAR_CIRCLE_DB_H_

#include <string>
#include <list>
#include "config/config.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include "logic/star_infos.h"
#include "storage/data_engine.h"
#include "circle_info.h"

namespace circle_logic {

class CircleDB {
public:
  CircleDB(config::FileConfig* config);
  ~CircleDB();

public:
  bool OnFetchAllCircleInfo(base_logic::ListValue*& ret_list);

  bool OnCreateCircleOrder(circle_logic::Circle& circle_info);

  bool OnDeleteCircle(const int64 circleid);

  bool OnUpdateCircle(circle_logic::Circle& circle_info);

private:
	static void CallGetAllCircleInfo(void* param, base_logic::Value* value);
private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace circle_logic

#endif /* STAR_CIRCLE_DB_H_ */
