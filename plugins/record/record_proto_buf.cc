//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年1月12日 Author: kerry

#include "record_proto_buf.h"
#include "basic/basic_util.h"

namespace record_logic {

namespace net_request {

bool TodayPosition::set_http_packet(base_logic::DictionaryValue* value) {
    bool r = false;
    int64 id = 0;
    std::string token;
    int64 big_start = 0;
    int32 start = 0;
    int64 big_count = 10;
    int32 count = 0;
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
    start = big_start;
    set_start(start);


    r = value->GetBigInteger(L"count", &big_count);
    count = big_count;
    set_count(count);
    return true;
}

}

namespace net_reply {

}

}
