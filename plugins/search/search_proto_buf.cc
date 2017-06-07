//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include "search/search_proto_buf.h"
#include "basic/basic_util.h"

namespace search_logic {

namespace net_request {

bool SearchStarInfo::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  std::string message;
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

   r = value->GetString(L"message", &message);
   if (r)
       set_message(message);
    else
        return false;
  return true;
}

}

}
