//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STARSIDE_SCHDULER_ENGINE_H__
#define STARSIDE_SCHDULER_ENGINE_H__

#include "logic/star_infos.h"
#include "starside/starside_info.h"
#include "starside/starside_db.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "manager/data_share_mgr.h"

namespace starside_logic {



typedef std::map<int64, star_logic::Withdrawals> WITHDRAWALS_MAP;/*提现ID*/
typedef std::map<int64, WITHDRAWALS_MAP> ALLWITHDRAWALS_MAP;/*用户ID 为KEY*/

typedef std::map<int64, star_logic::TOwnStar> OWNSTAR_MAP;
typedef std::map<int64, OWNSTAR_MAP> ALL_OWNSTAR_MAP;/*uid 为KEY*/
//-----------------------------------------------
typedef std::list<starside_logic::TranscationStatistics> TRANSSTATIS_MAP;
typedef std::map<std::string, TRANSSTATIS_MAP> ALL_TRANSSTATIS_MAP;/*starcode 为key*/

typedef std::list<starside_logic::StarMeetRelForFan> STARMEETRELFORFAN;
typedef std::map<std::string, STARMEETRELFORFAN> ALL_STARMEETRELFORFAN_MAP;/*starcode 为key*/

typedef std::list<starside_logic::ServiceItem> ALL_SERVICEITEM;
//____________________________________________________________
typedef std::list<starside_logic::StarOwnService> STAROWNSERVICE;
typedef std::map<std::string, STAROWNSERVICE> ALL_STAROWNSERVICE;
//拥有明星时间的用户列表
typedef std::list<starside_logic::TOwnStarUser> OWNSTARUSERLIST;
typedef std::map<std::string, OWNSTARUSERLIST> ALL_OWNSTARUSERLIST; //明星code
//-----------------------------------------------

class StarSideCache {
 public:
  ALLWITHDRAWALS_MAP all_withdrawals_map_;
  ALL_OWNSTAR_MAP all_ownstar_map_;

  ALL_TRANSSTATIS_MAP all_transstatis_map_; //交易统计
  ALL_STARMEETRELFORFAN_MAP all_starmeetrel_map_; //明星粉丝预约列表
  ALL_SERVICEITEM all_serviceitem;
  ALL_STAROWNSERVICE all_starownservice_; //所有明星活动类型
  ALL_OWNSTARUSERLIST all_ownstaruser_; //
};

class StarSideManager {
 public:
  StarSideManager();
  virtual ~StarSideManager();
  void InitDB(starside_logic::StarSideDB* starside_db);
  void InitStarSideRechargeData();
  void InitOwnStarData();
  void InitStarSideWithDrawals();
  void InitStarSideTransStatis();
  void InitStarMeetRelList();
  void InitServiceItem(); 
  void InitStarOwnService(); 
  void InitOwnStarUser(); 

  void InitSchdulerEngine(manager_schduler::SchdulerEngine* schduler_engine);

/*
  void SendStarSideTrades(const int socket, const int64 session,
                         const int32 reversed,
                         const int64 uid, const std::string& symbol,
                         const int64 pos,
                         const int64 count = 10);
*/

  void SendStarSideOwnStar(const int socket, const int64 session,
                           const int32 revered, const int64 uid,
                           const int32 status, const int64 pos,
                           const int64 count = 10);

  void SendStarSideTransStatis(const int socket, const int64 session,
                           const int32 revered, const std::string &starcode,
                           const int32 stardate, const int32 enddate);

  void SendStarSideRecharge(const int socket, const int64 session,
                           const int32 revered, const int64 uid,
                           const int32 status, const int64 pos,
                           const std::string queryTime, const int64 count = 10);

  void SendStarOwnService(const int socket, const int64 session,
                                const std::string &starcode); 
  void SendStarMeetRel(const int socket, const int64 session,
                                const std::string &starcode,
                                const int64 startpos, const int64 count); 
  void ServiceDefInfo(const int socket, const int64 session); 

  void ProfitDetail(const int socket, const int64 session, 
                    const std::string &starcode, const int64 orderdate); 
  void UpdStarMeetRel(const int socket, const int64 session, 
                    const int64 meet_id, const int64 meet_type); 

  void UpdStarService(const int socket, const int64 session, 
                    const std::string &starcode, const int64 mid, 
                    const int64 type); 
  //modify
  void GetOwnStarUser(const int socket, const int64 session, 
                   const std::string &starcode,int64 start, int64 count); 
  
  void ModifyStarMeetRelLock(const std::string &starcode, const int64 id,
                const int64 type);
 private:
  void Init();

  void SetStarSideRechargeNoLock(star_logic::Recharge& recharge);
  void GetStarSideRechargeNoLock(const int64 uid, const int32 status,
                                const std::string queryTime,
                                std::list<star_logic::Recharge>& list,
                                const int64 pos = 0, const int64 count = 10);
//________--

  void SetOwnStarNoLock(star_logic::TOwnStar& ownstar);

  void GetStarSideOwnStarNoLock(const int64 uid, const int32 status,
                                std::list<star_logic::TOwnStar>& list,
                                const int64 pos = 0, const int64 count = 10);
  void GetStarSideTransStatisNoLock(const std::string &starcode, 
                    std::list<starside_logic::TranscationStatistics>& list,
                    const int64 pos = 0, const int64 count = 10);
  void GetStarOwnServiceNoLock(
    const std::string &starcode, 
    std::list<starside_logic::StarOwnService>& list); 

  void GetStarMeetRelNoLock(
    const std::string &starcode, 
    std::list<starside_logic::StarMeetRelForFan>& list); 
  void GetOwnStarUserNoLock(
    const std::string &starcode, 
    std::list<starside_logic::TOwnStarUser>& list); 
 private:
  starside_logic::StarSideDB* starside_db_;
  StarSideCache *starside_cache_;
  struct threadrw_t *lock_;
  manager_schduler::SchdulerEngine* schduler_engine_;
};

class StarSideEngine {
 private:
  static StarSideEngine* schduler_engine_;
  static StarSideManager* schduler_mgr_;

  StarSideEngine() {
  }
  virtual ~StarSideEngine() {
  }
 public:
  static StarSideManager* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new StarSideManager();
    return schduler_mgr_;
  }

  static StarSideEngine* GetQuotationsEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new StarSideEngine();
    return schduler_engine_;
  }

  static void FreeSchdulerManager() {
    if (schduler_mgr_) {
      delete schduler_mgr_;
      schduler_mgr_ = NULL;
    }
  }

  static void FreeStarSideEngine() {
    if (schduler_engine_) {
      delete schduler_engine_;
      schduler_engine_ = NULL;
    }
  }
};
}

#endif
