//  Copyright (c) 2015-2015 The comments Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include "comments_proto_buf.h"
#include "basic/basic_util.h"

namespace comments_logic {

namespace net_other {
/*
*/
}

namespace net_request {

bool HisComments::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  //int64 id = 0;
  std::string id = "";
  int64 big_start = 0;
  int32 start = 0;
  int64 big_count = 0;
  int32 count = 0;

  std::string token;
  if (value == NULL)
    return false;

  //r = value->GetBigInteger(L"symbol", &id);
  r = value->GetString(L"symbol", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetBigInteger(L"startPos", &big_start);
  if (r)
    start = big_start;
  else
    start = 0;
  set_start(start);

  r = value->GetBigInteger(L"count", &big_count);
  if (r)
    count = big_count;
  else
    count = 10;
  set_count(count);

  return true;
}

bool FansComments::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;

  std::string tmp;

  if (value == NULL)
    return false;

  r = value->GetString(L"symbol", &tmp);
  if (r)
    set_star_code(tmp);
  else
    return false;

  r = value->GetString(L"fans_id", &tmp);
  if (r)
    set_fans_id(tmp);
  else
    return false;

  r = value->GetString(L"nick_name", &tmp);
  if (r)
    set_nick_name(tmp);
  else
    return false;
 
  r = value->GetString(L"comments", &tmp);
  if (r)
    set_comments(tmp);
  else
    return false;
 
  r = value->GetString(L"head_url", &tmp);
  if (r)
    set_head_url(tmp);
  else
    return false;
  int64 t_tme = time(NULL);
  set_cms_time(t_tme);
  return true;
}

/*
*/
}

namespace net_reply {

}

}
