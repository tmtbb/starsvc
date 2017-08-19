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
  bool OnFetchAllUserAskAnswer(base_logic::ListValue*& ret_list); 
  bool OnFetchAllUserSeeAsk(base_logic::ListValue*& ret_list); 

  bool OnCreateCircleOrder(circle_logic::Circle& circle_info);

  bool OnDeleteCircle(const int64 circleid);

  bool OnUpdateCircle(int64 uid, circle_logic::Circle& circle_info, int32 dec_time, int64& result);

  bool OnGetUserName(const int64 uid, base_logic::DictionaryValue*& dict);

  int64 OnUserAsk(const int64 uid, const int32 a_type, 
            const int32 p_type, const int32 c_type, 
            const std::string &starcode, const std::string &uask,
            const std::string &video_url);
  int64 OnStarAnswer(const int64 id, const int32 p_type, 
            const std::string &sanswer);
private:
  static void CallGetAllCircleInfo(void* param, base_logic::Value* value);
  static void CallGetAllUserAskAnswer(void* param, base_logic::Value* value);
  static void CallUserAsk(void* param, base_logic::Value* value);

  static void CallGetUserName(void* param, base_logic::Value* value);

  static void CallOnUpdateCircle(void* param, base_logic::Value* value);
  static void CallGetAllUserSeeAsk(void* param, base_logic::Value* value);
private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace circle_logic

#endif /* STAR_CIRCLE_DB_H_ */
