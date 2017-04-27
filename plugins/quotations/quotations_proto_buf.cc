//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include "quotations_proto_buf.h"
#include "basic/basic_util.h"

namespace quotations_logic {

namespace net_other {

bool RealTime::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  double change = 0.0;
  double pchg = 0.0;
  double opening_today_price = 0.0;
  double closed_yesterday_price = 0.0;
  double current_price = 0.0;
  int64 current_unix_time = 0.0;
  double high_price = 0.0;
  double low_price = 0.0;
  int64 type = 0.0;
  std::string exchange_name;
  std::string platform_name;
  std::string symbol;

  if (value == NULL)
    return false;

  r = value->GetReal(L"change", &change);
  if (r)
    set_change(change);

  r = value->GetReal(L"pchg", &pchg);
  if (r)
    set_pchg(pchg);

  r = value->GetReal(L"openingTodayPrice", &opening_today_price);
  if (r)
    set_opening_today_price(opening_today_price);

  r = value->GetReal(L"closedYesterdayPrice", &closed_yesterday_price);
  if (r)
    set_closed_yesterday_price(closed_yesterday_price);

  r = value->GetReal(L"currentPrice", &current_price);
  if (r)
    set_current_price(current_price);

  r = value->GetBigInteger(L"priceTime", &current_unix_time);
  if (r)
    set_current_unix_time(current_unix_time);

  r = value->GetReal(L"lowPrice", &low_price);
  if (r)
    set_low_price(low_price);

  r = value->GetReal(L"highPrice", &high_price);
  if (r)
    set_high_price(high_price);

  r = value->GetBigInteger(L"type", &type);
  if (r)
    set_type(type);

  r = value->GetString(L"exchangeName", &exchange_name);
  if (r)
    set_exchange_name(exchange_name);

  r = value->GetString(L"platformName", &platform_name);
  if (r)
    set_platform_name(platform_name);

  r = value->GetString(L"symbol", &symbol);
  if (r)
    set_symbol(symbol);

  return true;
}

}

namespace net_request {

bool TimeLine::set_htt_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  std::string exchange_name;
  std::string platform_name;
  std::string symbol;
  int64 temp_atype;
  int32 atype;
  int64 big_count;
  int32 count;
  int64 start_time;
  int64 end_time;

  if (value == NULL)
    return false;

  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetString(L"exchangeName", &exchange_name);
  if (r)
    set_exchange_name(exchange_name);
  else
    return false;

  r = value->GetString(L"platformName", &platform_name);
  if (r)
    set_platform_name(platform_name);
  else
    return false;

  r = value->GetString(L"symbol", &symbol);
  if (r)
    set_symbol(symbol);
  else
    return false;

  r = value->GetBigInteger(L"aType", &temp_atype);
  if (r) {
    atype = temp_atype;
    set_atype(atype);
  } else
    return false;

  r = value->GetBigInteger(L"startTime", &start_time);
  if (!r)
    start_time = time(NULL);
  set_start_time(start_time);

  r = value->GetBigInteger(L"endTime", &end_time);
  if (r){
    set_end_time(end_time);
  }else{
    set_end_time(0);
  }

  r = value->GetBigInteger(L"count", &big_count);
  if (r)
    count = big_count;
  else
    count = 50;
  set_count(count);

  return true;
}

bool RealTimeUnit::set_htt_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  std::string exchange_name;
  std::string platform_name;
  std::string symbol;
  int64 temp_atype;
  int32 atype;

  if (value == NULL)
    return false;

  r = value->GetString(L"exchangeName", &exchange_name);
  if (r)
    set_exchange_name(exchange_name);
  else
    return false;

  r = value->GetString(L"platformName", &platform_name);
  if (r)
    set_platform_name(platform_name);
  else
    return false;

  r = value->GetString(L"symbol", &symbol);
  if (r)
    set_symbol(symbol);
  else
    return false;

  r = value->GetBigInteger(L"aType", &temp_atype);
  if (r) {
    atype = temp_atype;
    set_atype(atype);
  } else
    return false;

  return true;
}

bool KChartTimeLine::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  std::string exchange_name;
  std::string platform_name;
  std::string symbol;
  int64 temp_atype;
  int32 atype;
  int64 big_count;
  int32 count;
  int64 start_time;
  int64 end_time;

  if (value == NULL)
    return false;

  r = value->GetString(L"exchangeName", &exchange_name);
  if (r)
    set_exchange_name(exchange_name);
  else
    return false;

  r = value->GetString(L"platformName", &platform_name);
  if (r)
    set_platform_name(platform_name);
  else
    return false;
  /*  r = value->GetReal(L"opening_today_price", &opening_today_price);
  if (r)
    set_opening_today_price(opening_today_price);

  r = value->GetReal(L"closed_yesterday_price", &closed_yesterday_price);
  if (r)
    set_closed_yesterday_price(closed_yesterday_price);

  r = value->GetReal(L"current_price", &current_price);
  if (r)
    set_current_price(current_price);

  r = value->GetReal(L"current_unix_time", &current_unix_time);
  if (r)
    set_current_unix_time(current_unix_time);

  r = value->GetReal(L"low_price", &low_price);
  if (r)
    set_low_price(low_price);

  r = value->GetReal(L"high_price", &high_price);
  if (r)
    set_high_price(high_price);

  r = value->GetReal(L"type", &type);
  if (r)
    set_type(type);

  r = value->GetString(L"exchange_name", &exchange_name);
  if (r)
    set_exchange_name(exchange_name);

  r = value->GetString(L"platform_name", &platform_name);
  if (r)
  set_platform_name(platform_name);*/

  r = value->GetString(L"symbol", &symbol);
  if (r)
    set_symbol(symbol);
  else
    return false;

  r = value->GetBigInteger(L"chartType", &temp_atype);
  if (r) {
    atype = temp_atype;
    set_chart_type(atype);
  } else
    return false;

  r = value->GetBigInteger(L"startTime", &start_time);
  if (!r)
    start_time = time(NULL);
  set_start_time(start_time);

  r = value->GetBigInteger(L"endTime", &end_time);
  if (r){
    set_end_time(end_time);
  }else{
    set_end_time(0);
  }

  r = value->GetBigInteger(L"count", &big_count);
  if (r)
    count = big_count;
  else
    count = 50;
  set_count(count);

  return true;
}

bool RealTime::set_htt_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;

  if (value == NULL)
    return false;

  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetList(L"symbolInfos", &symbol_infos_);
  if (!r)
    return false;

  return true;
}

}

namespace net_reply {

}

}
