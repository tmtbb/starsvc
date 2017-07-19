//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_STARSIDE_DB_H_
#define SWP_STARSIDE_DB_H_

#include <string>
#include <list>
#include "starside/starside_info.h"
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace starside_logic {

class StarSideDB {
 public:
  StarSideDB(config::FileConfig* config);
  ~StarSideDB();

 public:
  bool OnHistroyTradesRecord(std::list<star_logic::TradesPosition>* list);

  bool OnHistroyRechargeRecord(std::list<star_logic::Recharge>* list);
  bool OnOwnStarRecord(std::list<star_logic::TOwnStar>* list);

  bool OnHistroyTransStatis(std::list<starside_logic::TranscationStatistics>* list);
  bool OnHistroyStarMeetRel(std::list<starside_logic::StarMeetRelForFan>* list); 
  bool OnHistroyStarOwnService(std::list<starside_logic::StarOwnService>* list); 

  bool OnUpdStarMeetRel(const int64 meet_id, const int64 meet_type);
  bool OnUpdStarService(const std::string &starcode, const int64 mid, const int64 type);

  bool OnProfitDetail(const int socket, const int64 session,
           const std::string &starcode, const int64 orderdate,
           double &lastprice, double &dayprice, double &profit); 
  bool OnHistroyServiceItem(std::list<starside_logic::ServiceItem>* list); 
 public:

  static void CallHistroyRechargeRecord(void* param, base_logic::Value* value);

  static void CallOwnStarRecord(void* param, base_logic::Value* value);

  static void CallHistorTransStatisRecord(void* param, base_logic::Value* value);
  static void CallHistorStarMeetRelRecord(void* param, base_logic::Value* value); 
  static void CallHistorStarOwnService(void* param, base_logic::Value* value); 


  static void CallProfitDetail(void* param, base_logic::Value* value);

  static void CallServiceDef(void* param, base_logic::Value* value); 
 private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace starside_logic

#endif /* SWP_STARSIDE_DB_H_ */
