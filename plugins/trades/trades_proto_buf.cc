//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include "trades/trades_proto_buf.h"
#include "basic/basic_util.h"

namespace trades_logic {
namespace net_other {

void RealTime::set_http_packet(base_logic::DictionaryValue* value) {
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
}

}

namespace net_request {


bool FlightInfo::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  int64 gid = 0;
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

  r = value->GetBigInteger(L"gid", &gid);
  if (r)
    set_gid(gid);
  else
    return false;

  return true;
}
bool CurrentPosition::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  int64 big_start = 0;
  int32 start = 0;
  int64 big_count = 0;
  int32 count = 0;
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

  r = value->GetBigInteger(L"start", &big_start);
  if (r)
    start = big_start;
  else
    start = 0;
  set_start(start);

  r = value->GetBigInteger(L"count", &big_count);
  if (r)
    count = big_count;
  else
    count = 10000;
  set_count(count);

  return true;
}

bool OpenPosition::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 big_buy_sell = 0;
  int32 buy_sell = 0;
  int64 amount = 0;
  double price = 0.0;
  double limit = 0.0;
  double stop = 0.0;
  int64 big_deferred = 0;
  int32 deferred = 0;
  std::string token;
  int32 code_id = 0;
  int64 big_code_id = 0;

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

  r = value->GetBigInteger(L"codeId", &big_code_id);
  if (r) {
    code_id = big_code_id;
    set_code_id(code_id);
  } else
    return false;

  r = value->GetBigInteger(L"buySell", &big_buy_sell);
  if (r) {
    buy_sell = big_buy_sell;
    set_buy_sell(buy_sell);
  } else
    return false;

  r = value->GetBigInteger(L"amount", &amount);
  if (r)
    set_amount(amount);
  else
    return false;

  r = value->GetReal(L"price", &price);
  if (r)
    set_price(price);

  r = value->GetReal(L"limit", &limit);
  if (r)
    set_limit(limit);

  r = value->GetReal(L"stop", &stop);
  if (r)
    set_stop(stop);

  r = value->GetBigInteger(L"deferred", &big_deferred);
  if (r) {
    deferred = big_deferred;
    set_deferred(deferred);
  }

  return true;
}

bool Goods::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 big_start = 0;
  int32 start = 0;
  int64 big_count = 0;
  int32 count = 0;
  std::string token;
  int64 pid = 0;

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

  r = value->GetBigInteger(L"pid", &pid);
  if (r) {
    int32 temp = 0;
    temp = pid;
    set_pid(temp);
  } else
    return false;

  r = value->GetBigInteger(L"start", &big_start);
  if (r) {
    start = big_start;
  } else {
    start = 0;
  }
  set_start(start);

  r = value->GetBigInteger(L"count", &big_count);
  if (r) {
    count = big_count;
  } else {
    count = 100000;
  }
  set_count(count);

  return true;
}

}

}
