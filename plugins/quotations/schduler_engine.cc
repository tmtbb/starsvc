//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "quotations/schduler_engine.h"
#include "quotations/quotations_proto_buf.h"
#include "quotations/operator_code.h"
#include "quotations/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

namespace quotations_logic {

QuotationsManager* QuotationsEngine::schduler_mgr_ = NULL;
QuotationsEngine* QuotationsEngine::schduler_engine_ = NULL;

QuotationsManager::QuotationsManager() {
  quotations_cache_ = new QuotationsCache();
  Init();
}

QuotationsManager::~QuotationsManager() {
  DeinitThreadrw(lock_);
  quotations_cache_->quotations_map_.clear();
  if (quotations_cache_) {
    delete quotations_cache_;
    quotations_cache_ = NULL;
  }

}

void QuotationsManager::Init() {
  InitThreadrw(&lock_);
}

void QuotationsManager::InitFoxreData() {
  InitRedisData("SINA:SINA:fx_seurcnh", FORXE_TYPE);
  InitRedisData("SINA:SINA:fx_sjpycnh", FORXE_TYPE);
  InitRedisData("SINA:SINA:fx_susdcnh", FORXE_TYPE);
}

void QuotationsManager::InitGoodsData() {
  InitRedisData("JH:DEFAULT:AG", GOODS_TYPE);
}

void QuotationsManager::InitRedisData(const std::string& hash_name,
                                      int32 atype) {
  std::list<swp_logic::Quotations> list;
  quotations_redis_->ReadHisTimeGoodsData(hash_name, list);
  list.sort(swp_logic::Quotations::after);
  int32 all = 1440;
  while (list.size() > 0 && all > 0) {
    swp_logic::Quotations quotations = list.front();
    list.pop_front();
    quotations.set_type(atype);
    SetQuotationsUnit(quotations);
    SetKQuotations(quotations);
    all--;
  }
}

void QuotationsManager::InitRedis(
    quotations_logic::QuotationsRedis* quotations_redis) {
  quotations_redis_ = quotations_redis;
}

void QuotationsManager::SetKQuotations(swp_logic::Quotations& quotation) {
  SetKQuotationsUnit(FIVE_MINUTE, quotation);
  SetKQuotationsUnit(QUARTER_MINUTE, quotation);
  SetKQuotationsUnit(HALF_HOUR, quotation);
  SetKQuotationsUnit(ONE_HOUR, quotation);
}

void QuotationsManager::SetKQuotationsUnit(const int32 time_symbol,
                                           swp_logic::Quotations& quotation) {
  int64 current_time = (quotation.current_unix_time() / time_symbol)
      * time_symbol;
  std::string key = quotation.platform() + ":" + quotation.exchange_name() + ":"
      + quotation.symbol();
  K_QUOTATIONS_MAP k_quotations_map;
  swp_logic::Quotations tquotation;

  bool r = base::MapGet<K_ALL_QUOTATIONS_MAP, K_ALL_QUOTATIONS_MAP::iterator,
      int32, K_QUOTATIONS_MAP>(quotations_cache_->current_k_all_map_,
                               time_symbol, k_quotations_map);
  r = base::MapGet<K_QUOTATIONS_MAP, K_QUOTATIONS_MAP::iterator, std::string,
      swp_logic::Quotations>(k_quotations_map, key, tquotation);

  //判断时间是否相等，如果相当调整价格，如果不相等将一下个放入历史列表中，放入新的
  tquotation.set_current_price(quotation.current_price());
  if (!r) {  //不存在 说明该行情数据为开始存当前K线
    k_quotations_map[key] = quotation;
  } else {  //存在
    if ((quotation.current_unix_time() / time_symbol)
        == (tquotation.current_unix_time() / time_symbol)) {
      if (quotation.current_price() < tquotation.low_price())
        tquotation.set_low_price(quotation.current_price());
      else if (quotation.current_price() > tquotation.high_price())
        tquotation.set_high_price(quotation.current_price());
      k_quotations_map[key] = tquotation;
    } else {
      swp_logic::Quotations new_quotation;
      tquotation.set_closed_yesterday_price(tquotation.current_price());
      new_quotation.set_opening_today_price(quotation.current_price());
      new_quotation.set_high_price(quotation.current_price());
      new_quotation.set_low_price(quotation.current_price());
      new_quotation.set_current_unix_time(current_time);
      new_quotation.set_exchange_name(quotation.exchange_name());
      new_quotation.set_symbol(quotation.symbol());
      new_quotation.set_platform_name(quotation.platform());
      k_quotations_map[key] = new_quotation;
      //放入历史列表
      SetKQuotations(key, time_symbol, tquotation);
    }
  }
  quotations_cache_->current_k_all_map_[time_symbol] = k_quotations_map;
}

void QuotationsManager::SetKQuotations(const std::string& key,
                                       const int32 time_symbol,
                                       swp_logic::Quotations& quotation) {

  K_HIS_QUOTATIONS_MAP his_quotations_map;
  QUOTATIONS_LIST his_quotations_list;
  bool r = base::MapGet<K_ALL_HIS_QUOTATIONS_MAP,
      K_ALL_HIS_QUOTATIONS_MAP::iterator, const int32, K_HIS_QUOTATIONS_MAP>(
      quotations_cache_->k_history_all_map_, time_symbol, his_quotations_map);

  r = base::MapGet<K_HIS_QUOTATIONS_MAP, K_HIS_QUOTATIONS_MAP::iterator,
      std::string, QUOTATIONS_LIST>(his_quotations_map, key,
                                    his_quotations_list);
  his_quotations_list.push_back(quotation);
  his_quotations_map[key] = his_quotations_list;
  quotations_cache_->k_history_all_map_[time_symbol] = his_quotations_map;
}

void QuotationsManager::SetQuotationsUnit(swp_logic::Quotations& quotation) {
  bool r = false;
  QUOTATIONS_MAP exchange_quotations;
  QUOTATIONS_LIST quotations_list;

  LAST_QUOTATIONS_MAP last_exchange_quotations;
  swp_logic::Quotations temp_quotations;

  std::string key;
  key = quotation.platform() + ":" + quotation.exchange_name() + ":"
      + quotation.symbol();
  //读取上一分钟报价
  r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
      int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                  quotation.type(), last_exchange_quotations);
  r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
      std::string, swp_logic::Quotations>(last_exchange_quotations, key,
                                          temp_quotations);
  //存储最近一次报价，存储实时报价数据，但list中存储分时数据
  int64 last_unix_time = temp_quotations.current_unix_time() / 60 * 60;
  int64 current_unix_time = quotation.current_unix_time() / 60 * 60;
  last_exchange_quotations[key] = quotation;
  quotations_cache_->last_quotations_map_[quotation.type()] =
      last_exchange_quotations;
  if (last_unix_time == current_unix_time&&r) {
    //LOG_DEBUG2("last time %lld",temp_quotations.current_unix_time()/60*60);
    return;
  }

  r = base::MapGet<QUOTATIONS_ALL_MAP, QUOTATIONS_ALL_MAP::iterator, int32,
      QUOTATIONS_MAP>(quotations_cache_->quotations_map_, quotation.type(),
                      exchange_quotations);
  r = base::MapGet<QUOTATIONS_MAP, QUOTATIONS_MAP::iterator, std::string,
      QUOTATIONS_LIST>(exchange_quotations, key, quotations_list);

  /*LOG_MSG2("exchange_name:{%s} platform:{%s} symbol:{%s} change:{%f}  pchg:{%f} high_price:{%f} low_price:{%f} opening_today_price:{%f} closed_yesterday_price:{%f} current_price:{%f} current_unix_time:{%lld}",
   quotation.exchange_name().c_str(), quotation.platform().c_str(),quotation.symbol().c_str(),
   quotation.change(),quotation.pchg(),quotation.high_price(),quotation.low_price(),
   quotation.opening_today_price(),quotation.closed_yesterday_price(),quotation.current_price(),
   quotation.current_unix_time());*/

  quotations_list.push_back(quotation);
  quotations_list.sort(swp_logic::Quotations::cmp);
  exchange_quotations[key] = quotations_list;
  quotations_cache_->quotations_map_[quotation.type()] = exchange_quotations;

}

void QuotationsManager::SetQuotations(swp_logic::Quotations& quotation) {
  base_logic::WLockGd lk(lock_);
  SetQuotationsUnit(quotation);
  SetKQuotations(quotation);

}

void QuotationsManager::TimeEvent(int opcode, int time) {
  switch (opcode) {
    default:
      break;
  }
}

void QuotationsManager::SendKChartLine(const int socket, const int64 session,
                                       const int32 reversed,
                                       const int32 kchar_type,
                                       const std::string& exchange_name,
                                       const std::string& platform_name,
                                       const std::string& symbol,
                                       const int64 start_time,
                                       const int64 end_time,
                                       const int32 count) {
  net_reply::KChartLine kchart_line;
  std::list<swp_logic::Quotations> list;
  int32 base_num = 50;
  if (reversed /1000 == HTTP)
    base_num = count;
  else
    base_num = base_num < count ? base_num : count;
  std::string key = platform_name + ":" + exchange_name + ":" + symbol;
  GetKChartLine(kchar_type, key, list, end_time);
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_KCHART_DATA,session);
    return;
  }
  list.sort(swp_logic::Quotations::after);
  int32 t_count = 0;
  while (list.size() > 0 && t_count < count) {
    swp_logic::Quotations quotations = list.front();
    list.pop_front();
    if (quotations.current_unix_time() > start_time)
      continue;
    net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
    //r_real_time_unit->set_change(quotations.change());
    //r_real_time_unit->set_pchg(quotations.pchg());
    r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
    r_real_time_unit->set_closed_yesterday_price(
        quotations.closed_yesterday_price());
    //r_real_time_unit->set_current_price(quotations.current_price());
    r_real_time_unit->set_high_price(quotations.high_price());
    r_real_time_unit->set_low_price(quotations.low_price());
    r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
    r_real_time_unit->set_exchange_name(quotations.exchange_name());
    r_real_time_unit->set_platform_name(quotations.platform());
    r_real_time_unit->set_symbol(quotations.symbol());
    kchart_line.set_unit(r_real_time_unit->get());
    t_count++;
    if (kchart_line.Size() % base_num == 0 && kchart_line.Size() != 0) {
      kchart_line.set_stype(kchar_type);
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_KCHART_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                session, 0);
      packet_control.body_ = kchart_line.get();
      send_message(socket, &packet_control);
      kchart_line.Reset();
    }
  }

  if (kchart_line.Size() > 0) {
    kchart_line.set_stype(kchar_type);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_KCHART_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
              session, 0);
    packet_control.body_ = kchart_line.get();
    send_message(socket, &packet_control);
  }

}

void QuotationsManager::SendTimeLine(const int socket, const int64 session,
                                     const int32 reversed, const int32 atype,
                                     const std::string& exchange_name,
                                     const std::string& platform_name,
                                     const std::string& symbol,
                                     const int64 start_time,
                                     const int64 end_time,
                                     const int32 count) {
  net_reply::TimeLine time_line;
  std::list<swp_logic::Quotations> list;
  int32 base_num = 50;
  if (reversed /1000 == HTTP)
    base_num = count;
  else
    base_num = base_num < count ? base_num : count;
  std::string key = platform_name + ":" + exchange_name + ":" + symbol;
  GetTimeLine(atype, key, list, end_time);
  if (list.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_TIME_LINE_DATA,session);
    return;
  }
  list.sort(swp_logic::Quotations::cmp);
  int32 t_count = 0;
  while (list.size() > 0 && t_count < count) {
    swp_logic::Quotations quotations = list.front();
    list.pop_front();
    if (quotations.current_unix_time() > start_time)
      continue;
    net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
    r_real_time_unit->set_change(quotations.change());
    r_real_time_unit->set_pchg(quotations.pchg());
    r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
    r_real_time_unit->set_closed_yesterday_price(
        quotations.closed_yesterday_price());
    r_real_time_unit->set_current_price(quotations.current_price());
    //r_real_time_unit->set_high_price(quotations.high_price());
    //r_real_time_unit->set_low_price(quotations.low_price());
    r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
    r_real_time_unit->set_exchange_name(quotations.exchange_name());
    r_real_time_unit->set_platform_name(quotations.platform());
    r_real_time_unit->set_symbol(quotations.symbol());
    time_line.set_unit(r_real_time_unit->get());
    t_count++;
    if (time_line.Size() % base_num == 0 && time_line.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_QUOTATIONS_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                session, 0);
      packet_control.body_ = time_line.get();
      send_message(socket, &packet_control);
      time_line.Reset();
    }
  }

  if (time_line.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_QUOTATIONS_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
              session, 0);
    packet_control.body_ = time_line.get();
    send_message(socket, &packet_control);
  }

}

void QuotationsManager::SendRealTime(const int socket, const int64 session,
                                     const int32 reversed,
                                     base_logic::ListValue* value) {
  net_reply::RealTime net_reply_real_time;
  for (base_logic::ListValue::iterator it = value->begin(); it != value->end();
      it++) {
    swp_logic::Quotations quotations;
    base_logic::Value* value = (*it);

    net_request::RealTimeUnit real_time_unit;
    real_time_unit.set_htt_packet((base_logic::DictionaryValue*) (value));

    //查找对应的行情数据
    std::string key = real_time_unit.platform_name() + ":"
        + real_time_unit.exchange_name() + ":" + real_time_unit.symbol();
    GetRealTime(real_time_unit.atype(), key, &quotations);
    if (quotations.current_unix_time() <= 0)
      continue;
    net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
    r_real_time_unit->set_change(quotations.change());
    r_real_time_unit->set_pchg(quotations.pchg());
    r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
    r_real_time_unit->set_closed_yesterday_price(
        quotations.closed_yesterday_price());
    r_real_time_unit->set_current_price(quotations.current_price());
    r_real_time_unit->set_high_price(quotations.high_price());
    r_real_time_unit->set_low_price(quotations.low_price());
    r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
    r_real_time_unit->set_exchange_name(quotations.exchange_name());
    r_real_time_unit->set_platform_name(quotations.platform());
    r_real_time_unit->set_symbol(quotations.symbol());
    r_real_time_unit->set_system_unix_time(time(NULL));

    net_reply_real_time.set_unit(r_real_time_unit->get());
  }

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, R_QUOTATIONS_REAL_TIME_DATA, QUOTATIONS_TYPE, 0,
            session, 0);
  packet_control.body_ = net_reply_real_time.get();
  send_message(socket, &packet_control);
}

void QuotationsManager::GetKChartLine(const int32 chart_type,
                                      const std::string& key,
                                      std::list<swp_logic::Quotations>& list,
                                      const int64 end_time) {
  bool r = false;
  base_logic::RLockGd lk(lock_);
  K_HIS_QUOTATIONS_MAP his_quotations_map;
  QUOTATIONS_LIST his_quotations_list;
  r = base::MapGet<K_ALL_HIS_QUOTATIONS_MAP, K_ALL_HIS_QUOTATIONS_MAP::iterator,
      const int32, K_HIS_QUOTATIONS_MAP>(quotations_cache_->k_history_all_map_,
                                         chart_type, his_quotations_map);

  r = base::MapGet<K_HIS_QUOTATIONS_MAP, K_HIS_QUOTATIONS_MAP::iterator,
      std::string, QUOTATIONS_LIST>(his_quotations_map, key,
                                    his_quotations_list);
  int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60)
      - (8 * 60 * 60);  //当前为东八区

  if (!(end_time == 0))
    start_pan = end_time;

  his_quotations_list.sort(swp_logic::Quotations::after);
  QUOTATIONS_LIST::iterator it = his_quotations_list.begin();
  for (; it != his_quotations_list.end(); it++) {
    swp_logic::Quotations quotations = (*it);
    //LOG_MSG2("count:{%d} index:{%d} unix_time:{%lld}", quotations_list.size(),
    //  i,quotations.current_unix_time());
    if (quotations.current_unix_time() >= start_pan)
      list.push_back(quotations);
    else
      break;
  }
}

void QuotationsManager::GetTimeLine(const int32 atype,
                                    const std::string& symbol,
                                    std::list<swp_logic::Quotations>& list,
                                    const int64 end_time) {
  bool r = false;
  base_logic::RLockGd lk(lock_);
  QUOTATIONS_MAP exchange_quotations;
  QUOTATIONS_LIST quotations_list;
  r = base::MapGet<QUOTATIONS_ALL_MAP, QUOTATIONS_ALL_MAP::iterator, int32,
      QUOTATIONS_MAP>(quotations_cache_->quotations_map_, atype,
                      exchange_quotations);
  if (!r)
    return;

  r = base::MapGet<QUOTATIONS_MAP, QUOTATIONS_MAP::iterator, std::string,
      QUOTATIONS_LIST>(exchange_quotations, symbol, quotations_list);

  quotations_list.sort(swp_logic::Quotations::cmp);
//遍历
  int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60)
      - (8 * 60 * 60);  //当前为东八区
  if (!(end_time == 0))
    start_pan  = end_time;
  int i = 0;
  QUOTATIONS_LIST::iterator it = quotations_list.begin();
  for (; it != quotations_list.end(); it++) {
    i++;
    swp_logic::Quotations quotations = (*it);
    //LOG_MSG2("count:{%d} index:{%d} unix_time:{%lld} start_pan:{%lld}", quotations_list.size(),
      //i,quotations.current_unix_time(),start_pan);
    if (quotations.current_unix_time() >= start_pan)
      list.push_back(quotations);
    /*else
      break;*/
  }
}

void QuotationsManager::GetRealTime(const int32 atype,
                                    const std::string& symbol,
                                    swp_logic::Quotations* quotations) {
  bool r = false;
  base_logic::WLockGd lk(lock_);
  LAST_QUOTATIONS_MAP exchange_quotations;
  swp_logic::Quotations qs;
  r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
      int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                  atype, exchange_quotations);
  if (!r)
    return;

  r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
      std::string, swp_logic::Quotations>(exchange_quotations, symbol, qs);
  if (!r)
    return;
  (*quotations) = qs;
}

void QuotationsManager::LoginQuotationsCenter(const int socket) {
  quotations_logic::net_other::Login login;
  login.set_aid(time(NULL));
  login.set_atype(4);
  login.set_passowrd("1234567x");
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, LOGIN_QUOTATIONS, 1, 0, 0, 0);
  packet_control.body_ = login.get();
  send_message(socket, &packet_control);
}

}
