//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "starside/schduler_engine.h"
#include "starside/starside_proto_buf.h"
#include "starside/operator_code.h"
#include "starside/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"
#include <string>
#include <stdlib.h>

namespace starside_logic {

StarSideManager* StarSideEngine::schduler_mgr_ = NULL;
StarSideEngine* StarSideEngine::schduler_engine_ = NULL;

StarSideManager::StarSideManager() {
  starside_cache_ = new StarSideCache();
  Init();
}

StarSideManager::~StarSideManager() {
  DeinitThreadrw(lock_);

}

void StarSideManager::Init() {
  InitThreadrw(&lock_);
}

void StarSideManager::InitDB(starside_logic::StarSideDB* starside_db) {
  starside_db_ = starside_db;
}

void StarSideManager::InitSchdulerEngine(
    manager_schduler::SchdulerEngine* schduler_engine) {
  schduler_engine_ = schduler_engine;
}
void StarSideManager::InitOwnStarUser()
{

  base_logic::WLockGd lk(lock_);
  std::list<starside_logic::TOwnStarUser> list;
  starside_db_->OnGetOwnStarUser(&list);

  std::list<starside_logic::TOwnStarUser> temp;
  std::string str;
  while (list.size() > 0)
  {
    starside_logic::TOwnStarUser item = list.front();
    list.pop_front();
    if (str.length() > 0) //
    {
        if (!strcmp(str.c_str(), item.starcode().c_str())) // code相等
            temp.push_back(item);
        else //不等
        {
            starside_cache_->all_ownstaruser_[str] = temp;
            str = item.starcode();
            temp.clear();
            temp.push_back(item);
        }
    }
    else
    {
        str = item.starcode();
        temp.push_back(item);
    }
  }
  if (str.length() > 0 && temp.size() > 0)
    starside_cache_->all_ownstaruser_[str] = temp;
  LOG_DEBUG2("all_ownstaruser_ list.size[%d]____________________________________________",list.size() );
}

void StarSideManager::InitStarOwnService() {
  base_logic::WLockGd lk(lock_);
  std::list<starside_logic::StarOwnService> list;
  starside_db_->OnHistroyStarOwnService(&list); //从数据库加载数据

  std::list<starside_logic::StarOwnService> temp;
  std::string str;
  while (list.size() > 0)
  {
    starside_logic::StarOwnService item = list.front();
    list.pop_front();
    if (str.length() > 0) //
    {
        if (!strcmp(str.c_str(), item.starcode().c_str())) // code相等
            temp.push_back(item);
        else //不等
        {
            starside_cache_->all_starownservice_[str] = temp;
            str = item.starcode();
            temp.clear();
            temp.push_back(item);
        }
    }
    else
    {
        str = item.starcode();
        temp.push_back(item);
    }
  }
  if (str.length() > 0 && temp.size() > 0)
    starside_cache_->all_starownservice_[str] = temp;
  LOG_DEBUG2("StarOwnService list.size[%d]____________________________________________",list.size() );
}

void StarSideManager::InitServiceItem() {
  base_logic::WLockGd lk(lock_);
  starside_cache_->all_serviceitem.clear();

  starside_db_->OnHistroyServiceItem(&(starside_cache_->all_serviceitem)); //从数据库加载数据
}
void StarSideManager::InitStarMeetRelList() {
  base_logic::WLockGd lk(lock_);
  std::list<starside_logic::StarMeetRelForFan> list;
  starside_db_->OnHistroyStarMeetRel(&list); //从数据库加载数据

  LOG_DEBUG2("InitStarMeetRelListlist.size[%d]____________________________________________",list.size() );

  std::list<starside_logic::StarMeetRelForFan> temp;
  std::string str;
  while (list.size() > 0)
  {
    starside_logic::StarMeetRelForFan item = list.front();
    list.pop_front();
    if (str.length() > 0) //
    {
        if (!strcmp(str.c_str(), item.code().c_str())) // code相等
            temp.push_back(item);
        else //不等
        {
            starside_cache_->all_starmeetrel_map_[str] = temp;
            str = item.code();
            temp.clear();
            temp.push_back(item);
        }
    }
    else
    {
        str = item.code();
        temp.push_back(item);
    }
  }
  if (str.length() > 0 && temp.size() > 0)
    starside_cache_->all_starmeetrel_map_[str] = temp;
  LOG_DEBUG2("InitStarSideTransStatis   list.size[%d]____________________________________________",list.size() );
}
void StarSideManager::InitStarSideTransStatis() {
  base_logic::WLockGd lk(lock_);
  std::list<starside_logic::TranscationStatistics> list;
  LOG_DEBUG2("InitStarSideTransStatis   list.size[%d]____________________________________________",list.size() );
  starside_db_->OnHistroyTransStatis(&list); //从数据库加载数据

  std::list<starside_logic::TranscationStatistics> temp;
  std::string str;
  while (list.size() > 0)
  {
    starside_logic::TranscationStatistics item = list.front();
    list.pop_front();
    if (str.length() > 0) //
    {
        if (!strcmp(str.c_str(), item.code().c_str())) // code相等
            temp.push_back(item);
        else //不等
        {
            starside_cache_->all_transstatis_map_[str] = temp;
            str = item.code();
            temp.clear();
            temp.push_back(item);
        }
    }
    else
    {
        str = item.code();
        temp.push_back(item);
    }
  }
  if (str.length() > 0 && temp.size() > 0)
    starside_cache_->all_transstatis_map_[str] = temp;
  LOG_DEBUG2("InitStarSideTransStatis   list.size[%d]____________________________________________",list.size() );
}

void StarSideManager::InitStarSideRechargeData() {
  base_logic::WLockGd lk(lock_);
  std::list<star_logic::Recharge> list;
  starside_db_->OnHistroyRechargeRecord(&list);
  while (list.size() > 0) {
    star_logic::Recharge recharge = list.front();
    list.pop_front();
    SetStarSideRechargeNoLock(recharge);

  }
}

void StarSideManager::InitOwnStarData() {
  base_logic::WLockGd lk(lock_);
  std::list<star_logic::TOwnStar> list;
  starside_db_->OnOwnStarRecord(&list);
  while (list.size() > 0) {
  //LOG_DEBUG2("list.size[%d]____________________________________________",list.size() );
    star_logic::TOwnStar star = list.front();
    list.pop_front();
    SetOwnStarNoLock(star);

  }
}

void StarSideManager::ProfitDetail(const int socket, 
                    const int64 session, const std::string &starcode, 
                    const int64 orderdate) {
  starside_logic::net_reply::ProfitDetail rep_profitdetail;
  double lastprice = 0.0;
  double day_price = 0.0;
  double profit = 0.0;
  bool r = starside_db_->OnProfitDetail(socket, session, 
    starcode, orderdate, lastprice, day_price, profit);
    
rep_profitdetail.set_max_price(lastprice);
rep_profitdetail.set_min_price(day_price);
rep_profitdetail.set_price(profit);

struct PacketControl packet_control;
MAKE_HEAD(packet_control, S_STARSIDE_DETAIL, 1, 0, session, 0);
packet_control.body_ = rep_profitdetail.get();
send_message(socket, &packet_control);

}
void StarSideManager::GetOwnStarUser(const int socket, const int64 session, 
                   const std::string &starcode, int64 startpos, int64 count)
{

  std::list<starside_logic::TOwnStarUser> list;
  {
    base_logic::RLockGd lk(lock_);  //
    GetOwnStarUserNoLock(starcode, list);
  }

  //没有对应的历史记录
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }
/*
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
*/

  int32 base_num = 10;
//  int32 count = 10;
  base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllTransStatis all_net_ownstaruser;
  //list.sort(star_logic::Recharge::close_after);
  while (list.size() > 0 && t_count < count) {
      
      
    starside_logic::TOwnStarUser item = list.front();
    list.pop_front();
    t_start++;
    if (t_start < startpos)
        continue;

    net_reply::TOwnStarUser* net_item = new net_reply::TOwnStarUser;

    net_item->set_uid(item.uid());
    net_item->set_ownseconds(item.ownseconds());
    net_item->set_appoint(item.appoint());
    net_item->set_starcode(item.starcode());
    net_item->set_starname(item.user_nickname());
    net_item->set_faccid(item.faccid());
    net_item->set_headurl(item.headurl());


    all_net_ownstaruser.set_unit(net_item->get());
    t_count++;
    if (all_net_ownstaruser.Size() % base_num == 0
        && all_net_ownstaruser.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_GETOWNSTARUSER, 1, 0, session, 0);
      packet_control.body_ = all_net_ownstaruser.get();
      send_message(socket, &packet_control);
      all_net_ownstaruser.Reset();
    }
    
  }

  if (all_net_ownstaruser.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_GETOWNSTARUSER, 1, 0, session, 0);
    packet_control.body_ = all_net_ownstaruser.get();
    send_message(socket, &packet_control);
  }
  
}

void StarSideManager::UpdStarMeetRel(const int socket, 
                    const int64 session, const int64 meet_id, 
                    const int64 meet_type) {
   
  bool r = starside_db_->OnUpdStarMeetRel(meet_id, meet_type);

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, R_STARSIDE_UPDSTARMEETREL, 1, 0, session, 0);
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result",result);
  packet_control.body_ = ret;

  send_message(socket, &packet_control);
  

}

void StarSideManager::UpdStarService(const int socket, 
                    const int64 session, const std::string &starcode, 
                    const int64 mid, const int64 type) {
    
   
  bool r = starside_db_->OnUpdStarService(starcode, mid, type);

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_STARSIDE_UPDSTARSERVICE , 1, 0, session, 0);
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result",result);
  packet_control.body_ = ret;

  send_message(socket, &packet_control);
  

}

void StarSideManager::UpdStarMeetDate(const int socket, 
                    const int64 session, const std::string &starcode, 
                    const std::string &meet_city, const std::string &stardate,
                    const std::string &enddate) {
    
   
  bool r = starside_db_->OnUpdStarMeetDate(starcode, meet_city, stardate, enddate);

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_STARSIDE_UPDSTARMEETDATE , 1, 0, session, 0);
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result",result);
  packet_control.body_ = ret;

  send_message(socket, &packet_control);
  

}

void StarSideManager::SendStarSideRecharge(const int socket, const int64 session,
                                         const int32 revered, const int64 uid,
                                         const int32 status, const int64 pos,
                                         const std::string queryTime, const int64 count) {
  std::list<star_logic::Recharge> recharge_list;
  {
    base_logic::RLockGd lk(lock_);  //1:处理中,2:成功,3:失败
    GetStarSideRechargeNoLock(uid, status, queryTime, recharge_list, 0, 0);
  }

  //没有对应的历史记录
  if (recharge_list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_RECHARGE, session);
    return;
  }
  int32 base_num = 10;
  if (revered / 1000 == HTTP)
    base_num = count;
  else
    base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllRecharge all_net_rechagre;
  recharge_list.sort(star_logic::Recharge::close_after);
  while (recharge_list.size() > 0 && t_count < count) {
    star_logic::Recharge recharge = recharge_list.front();
    recharge_list.pop_front();
    t_start++;
    if (t_start < pos)
      continue;
    net_reply::Recharge* net_recharge = new net_reply::Recharge;
    net_recharge->set_rid(recharge.rid());
    net_recharge->set_amount(recharge.amount());
    net_recharge->set_deposit_name(recharge.deposit_name());
    net_recharge->set_deposit_time(recharge.deposit_time());
    net_recharge->set_deposit_type(recharge.deposit_type());
    net_recharge->set_status(recharge.status());
    net_recharge->set_recharge_type(recharge.recharge_type());
    net_recharge->set_transaction_id(recharge.transaction_id());
    all_net_rechagre.set_unit(net_recharge->get());
    t_count++;
    if (all_net_rechagre.Size() % base_num == 0
        && all_net_rechagre.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_RECHARGE, 1, 0, session, 0);
      packet_control.body_ = all_net_rechagre.get();
      send_message(socket, &packet_control);
      all_net_rechagre.Reset();
    }
  }

  if (all_net_rechagre.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_RECHARGE, 1, 0, session, 0);
    packet_control.body_ = all_net_rechagre.get();
    send_message(socket, &packet_control);
  }
}

void StarSideManager::SendStarSideOwnStar(const int socket, const int64 session,
                                         const int32 revered, const int64 uid,
                                         const int32 status, const int64 pos,
                                         const int64 count) {
  std::list<star_logic::TOwnStar> ownstar_list;
  //LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
  {
    base_logic::RLockGd lk(lock_);  //
    GetStarSideOwnStarNoLock(uid, status, ownstar_list, 0, 0);
  }

  //LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
  //没有对应的历史记录
  if (ownstar_list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }
/*
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
*/
  int32 base_num = 10;
  if (revered / 1000 == HTTP)
    base_num = count;
  else
    base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllOwnStar all_net_ownstar;
  //ownstar_list.sort(star_logic::Recharge::close_after);
  while (ownstar_list.size() > 0 && t_count < count) {
    star_logic::TOwnStar ownstar = ownstar_list.front();
    ownstar_list.pop_front();
    t_start++;
    if (t_start < pos)
      continue;

    net_reply::OwnStar* net_ownstar = new net_reply::OwnStar;

    net_ownstar->set_uid(ownstar.uid());
    net_ownstar->set_ownseconds(ownstar.ownseconds());
    net_ownstar->set_appoint(ownstar.appoint());
    net_ownstar->set_starcode(ownstar.starcode());
    net_ownstar->set_starname(ownstar.starname());
    net_ownstar->set_faccid(ownstar.faccid());
    net_ownstar->set_headurl(ownstar.headurl());

    all_net_ownstar.set_unit(net_ownstar->get());
    t_count++;
    if (all_net_ownstar.Size() % base_num == 0
        && all_net_ownstar.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_RECHARGE, 1, 0, session, 0);
      packet_control.body_ = all_net_ownstar.get();
      send_message(socket, &packet_control);
      all_net_ownstar.Reset();
    }
  }

  if (all_net_ownstar.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_OWNSTAR, 1, 0, session, 0);
    packet_control.body_ = all_net_ownstar.get();
    send_message(socket, &packet_control);
  }
}

void StarSideManager::SendStarSideTransStatis(const int socket, 
                                        const int64 session,
                                        const int32 revered, 
                                        const std::string &starcode,
                                        const int32 stardate,
                                        const int32 enddate) {
  std::list<starside_logic::TranscationStatistics> transstatis_list;
  {
    base_logic::RLockGd lk(lock_);  //
    GetStarSideTransStatisNoLock(starcode, transstatis_list, 0, 0);
  }

  //没有对应的历史记录
  if (transstatis_list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }
/*
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
*/

  int32 base_num = 10;
  int32 count = 10;
  if (revered / 1000 == HTTP)
    base_num = count;
  else
    base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllTransStatis all_net_transstatis;
  //transstatis_list.sort(star_logic::Recharge::close_after);
  while (transstatis_list.size() > 0 && t_count < count) {
      
    starside_logic::TranscationStatistics transstatis = transstatis_list.front();
    transstatis_list.pop_front();
    t_start++;

    if (transstatis.open_position_time() < stardate)
      continue;
    if (transstatis.open_position_time() > enddate)
      break;

    net_reply::TransStatis* net_transstatis = new net_reply::TransStatis;


    net_transstatis->set_starcode(transstatis.code());
    net_transstatis->set_order_count(transstatis.order_count());
    net_transstatis->set_order_num(transstatis.order_num());
    //net_transstatis->set_price(transstatis.price());
    net_transstatis->set_price(transstatis.open_price());
    net_transstatis->set_profit(transstatis.price());

    net_transstatis->set_max_price(transstatis.max_price());
    net_transstatis->set_min_price(transstatis.min_price());
    net_transstatis->set_orderdate(transstatis.open_position_time());

    all_net_transstatis.set_unit(net_transstatis->get());
    t_count++;
    if (all_net_transstatis.Size() % base_num == 0
        && all_net_transstatis.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_TRANSSTATIS, 1, 0, session, 0);
      packet_control.body_ = all_net_transstatis.get();
      send_message(socket, &packet_control);
      all_net_transstatis.Reset();
    }
    
  }

  if (all_net_transstatis.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_TRANSSTATIS, 1, 0, session, 0);
    packet_control.body_ = all_net_transstatis.get();
    send_message(socket, &packet_control);
  }
  else
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);

  
}

void StarSideManager::SendStarMeetRel(const int socket, 
                                    const int64 session,
                                    const std::string &starcode,
                                    const int64 startpos, const int64 count) {
  std::list<starside_logic::StarMeetRelForFan> list;
  {
    base_logic::RLockGd lk(lock_);  //
    GetStarMeetRelNoLock(starcode, list);
  }

  //没有对应的历史记录
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }
/*
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
*/

  int32 base_num = 10;
  //int32 count = 10;
  base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllTransStatis all_net_meetrel;
  //list.sort(star_logic::Recharge::close_after);
  while (list.size() > 0 && t_count < count) {
      
      
    starside_logic::StarMeetRelForFan item = list.front();
    list.pop_front();
    t_start++;
    if (t_start < startpos)
        continue;

    net_reply::StarMeetRel* net_item = new net_reply::StarMeetRel;



    net_item->set_starcode(item.code());
    net_item->set_id(item.id());
    net_item->set_uid(item.uid());
    net_item->set_meet_type(item.meet_type());
    net_item->set_mid(item.mid());
    net_item->set_nickname(item.nickname());
    net_item->set_headurl(item.headurl());
    net_item->set_name(item.name());
    net_item->set_meet_city(item.meet_city());
    net_item->set_appoint_time(item.appoint_time());
    net_item->set_order_time(item.order_time());
    net_item->set_comment(item.comment());

    all_net_meetrel.set_unit(net_item->get());
    t_count++;
    if (all_net_meetrel.Size() % base_num == 0
        && all_net_meetrel.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_GETSTARMEETREL, 1, 0, session, 0);
      packet_control.body_ = all_net_meetrel.get();
      send_message(socket, &packet_control);
      all_net_meetrel.Reset();
    }
    
  }

  if (all_net_meetrel.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_GETSTARMEETREL, 1, 0, session, 0);
    packet_control.body_ = all_net_meetrel.get();
    send_message(socket, &packet_control);
  }
  
}


void StarSideManager::GetStarSideOwnStarNoLock(
    const int64 uid, const int32 status, std::list<star_logic::TOwnStar>& list,
    const int64 pos, const int64 count) {
  //LOG_DEBUG2("GetStarSideOwnStarNoLock________________ [%d] _ [%d]",uid,  starside_cache_->all_ownstar_map_.size() );
  OWNSTAR_MAP ownstar_map;
  base::MapGet<ALL_OWNSTAR_MAP, ALL_OWNSTAR_MAP::iterator, int64, OWNSTAR_MAP>(
      starside_cache_->all_ownstar_map_, uid, ownstar_map);

  for (OWNSTAR_MAP::iterator it = ownstar_map.begin();
      it != ownstar_map.end(); it++) {
    star_logic::TOwnStar ownstar = it->second;
    list.push_back(ownstar);
  }
}

void StarSideManager::GetStarSideTransStatisNoLock(
    const std::string &starcode, std::list<starside_logic::TranscationStatistics>& list,
    const int64 pos, const int64 count) {
  TRANSSTATIS_MAP transstatis_map;
  base::MapGet<ALL_TRANSSTATIS_MAP, ALL_TRANSSTATIS_MAP::iterator, std::string, TRANSSTATIS_MAP>(
      starside_cache_->all_transstatis_map_, starcode, list);

/*
  for (OWNSTAR_MAP::iterator it = ownstar_map.begin();
      it != ownstar_map.end(); it++) {
    star_logic::TOwnStar ownstar = it->second;
    list.push_back(ownstar);
  }
  */
}

void StarSideManager::ModifyStarMeetRelLock(const std::string &starcode, 
                    const int64 id, const int64 type)
{
    base_logic::RLockGd lk(lock_);  //
    std::list<starside_logic::StarMeetRelForFan>& list = 
        starside_cache_->all_starmeetrel_map_[starcode];
    std::list<starside_logic::StarMeetRelForFan>::iterator iter = list.begin();
    for (; iter != list.end(); iter++)
    {
        if (iter->id() == id)
        {
            iter->set_meet_type(type);
            break;
        }
    }
}

void StarSideManager::GetStarMeetRelNoLock(
    const std::string &starcode, 
    std::list<starside_logic::StarMeetRelForFan>& list) {

  base::MapGet<ALL_STARMEETRELFORFAN_MAP, ALL_STARMEETRELFORFAN_MAP::iterator, std::string, STARMEETRELFORFAN>(
      starside_cache_->all_starmeetrel_map_, starcode, list);

}

void StarSideManager::GetOwnStarUserNoLock(
    const std::string &starcode, 
    std::list<starside_logic::TOwnStarUser>& list) {

  base::MapGet<ALL_OWNSTARUSERLIST, ALL_OWNSTARUSERLIST::iterator, std::string, OWNSTARUSERLIST>(
      starside_cache_->all_ownstaruser_, starcode, list);

}

void StarSideManager::GetStarOwnServiceNoLock(
    const std::string &starcode, 
    std::list<starside_logic::StarOwnService>& list) {

  base::MapGet<ALL_STAROWNSERVICE, ALL_STAROWNSERVICE::iterator, std::string, STAROWNSERVICE>(
      starside_cache_->all_starownservice_, starcode, list);

}

void StarSideManager::SetOwnStarNoLock(star_logic::TOwnStar& ownstar) {

  //LOG_DEBUG("test ___ownstar.size[%]____________________________________________" );
  OWNSTAR_MAP ownstar_map;

  //LOG_DEBUG("test222 ___ownstar.size[%]____________________________________________" );
  base::MapGet<ALL_OWNSTAR_MAP, ALL_OWNSTAR_MAP::iterator, int64, OWNSTAR_MAP>(
      starside_cache_->all_ownstar_map_, ownstar.uid(), ownstar_map);

  //LOG_DEBUG("test333 ___ownstar.size[%]____________________________________________" );
  //starside_cache_->all_ownstar_map_.find(ownstar.uid());
  ownstar_map[atoll(ownstar.starcode().c_str())] = ownstar;
  //LOG_DEBUG2("ownstar.size[%d]__________ starcode[%s] __________________________________",ownstar_map.size() , ownstar.starcode().c_str());
  starside_cache_->all_ownstar_map_[ownstar.uid()] = ownstar_map;
  //LOG_DEBUG2("allownstar.size[%d]ownstar.uid[%d]____________________________________________",starside_cache_->all_ownstar_map_.size(), ownstar.uid() );


}

void StarSideManager::ServiceDefInfo(const int socket, 
                                    const int64 session) {
 
 std::list<starside_logic::ServiceItem> list ;
 {
  base_logic::RLockGd lk(lock_);  //
  list = starside_cache_->all_serviceitem;
 }

  //没有对应的历史记录
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }
/*
    LOG_DEBUG2("packet_length %d____________________________________________",ownstar_list.size() );
*/

  int32 base_num = 20;
  int32 count = 20;
  base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllTransStatis all_net_meetrel;
  //list.sort(star_logic::Recharge::close_after);
  while (list.size() > 0 && t_count < count) {
      
    starside_logic::ServiceItem item = list.front();
    list.pop_front();
    t_start++;

    net_reply::ServiceItem* net_item = new net_reply::ServiceItem;

    net_item->set_mid(item.mid());
    net_item->set_price(item.price());
    net_item->set_name(item.name());
    net_item->set_showpic_url(item.showpic_url());

    all_net_meetrel.set_unit(net_item->get());
    t_count++;
    if (all_net_meetrel.Size() % base_num == 0
        && all_net_meetrel.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_GETALLSERVICE , 1, 0, session, 0);
      packet_control.body_ = all_net_meetrel.get();
      send_message(socket, &packet_control);
      all_net_meetrel.Reset();
    }
    
  }

  if (all_net_meetrel.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_GETALLSERVICE , 1, 0, session, 0);
    packet_control.body_ = all_net_meetrel.get();
    send_message(socket, &packet_control);
  }
  
}
//________________________________________________________________

void StarSideManager::SendStarOwnService(const int socket, 
                                    const int64 session,
                                    const std::string &starcode) {
  std::list<starside_logic::StarOwnService> list;
  {
    base_logic::RLockGd lk(lock_);  //
    GetStarOwnServiceNoLock(starcode, list);
  }

  //没有对应的历史记录
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_HISTROY_DATA_OWNSTAR, session);
    return;
  }

  int32 base_num = 15;
  int32 count = 15;
  base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  starside_logic::net_reply::AllTransStatis all_net_starownservice; //数组
  //list.sort(star_logic::Recharge::close_after);
  while (list.size() > 0 && t_count < count) {
      
      
    starside_logic::StarOwnService item = list.front();
    list.pop_front();
    t_start++;

    net_reply::StarOwnService* net_item = new net_reply::StarOwnService;



    net_item->set_starcode(item.starcode());
    net_item->set_mid(item.mid());
    net_item->set_name(item.name());
    net_item->set_meet_city(item.meet_city());
    net_item->set_stardate(item.stardate());
    net_item->set_enddate(item.enddate());
    

    all_net_starownservice.set_unit(net_item->get());
    t_count++;
    if (all_net_starownservice.Size() % base_num == 0
        && all_net_starownservice.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_STARSIDE_GETSTARSERVICE, 1, 0, session, 0);
      packet_control.body_ = all_net_starownservice.get();
      send_message(socket, &packet_control);
      all_net_starownservice.Reset();
    }
    
  }

  if (all_net_starownservice.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_STARSIDE_GETSTARSERVICE, 1, 0, session, 0);
    packet_control.body_ = all_net_starownservice.get();
    send_message(socket, &packet_control);
  }
  
}
}
