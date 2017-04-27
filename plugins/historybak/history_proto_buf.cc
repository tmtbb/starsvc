//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年1月12日 Author: kerry

#include "history_proto_buf.h"
#include "basic/basic_util.h"

namespace history_logic {

namespace net_request {


bool HistoryHandle::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 tid = 0;
  int64 bigt_handle = 0;
  int32 handle = 0;
  std::string token;
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


  r = value->GetBigInteger(L"tid",&tid);
  if (r)
    set_tid(tid);
  else
    return false;

  r = value->GetBigInteger(L"handle",&bigt_handle);
  if (r){
    handle = bigt_handle;
    set_handle(handle);
  }else
    return false;

  return true;
}


bool HistoryPosition::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 big_start = 0;
  int32 start = 0;
  int64 big_count = 0;
  int32 count = 0;
  std::string token;
  std::string symbol;
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



  r = value->GetString(L"symbol", &symbol);
  if (r)
    set_symbol(symbol);
  else
    set_symbol("all");

  r = value->GetBigInteger(L"start",&big_start);
  if (r)
    start = big_start;
  else
    start = 0;
  set_start(start);


  r = value->GetBigInteger(L"count",&big_count);
  if (r)
    count = big_count;
  else
    count = 10;
  set_count(count);

  return true;
}


bool HistoryRecharge::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 big_start = 0;
  int32 start = 0;
  int64 big_count = 0;
  int32 count = 0;
  int64 big_status = 0;
  int32 status = 0;
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

  r = value->GetBigInteger(L"status",&big_status);
  if (r)
    status = big_status;
  else
    status = 0;
  set_status(status);

  r = value->GetBigInteger(L"startPos",&big_start);
  if (r)
    start = big_start;
  else
    start = 0;
  set_start(start);


  r = value->GetBigInteger(L"count",&big_count);
  if (r)
    count = big_count;
  else
    count = 10;
  set_count(count);

  return true;
}

}

namespace net_reply {

}

}
