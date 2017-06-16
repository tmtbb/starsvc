//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年1月12日 Author: kerry

#include "record_proto_buf.h"
#include "basic/basic_util.h"

namespace record_logic {

namespace net_request {




bool PositionCount::set_http_packet(base_logic::DictionaryValue* value) {
    bool r = false;
    int64 id = 0;
    std::string token;
    std::string symbol;
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
        return false;
    return true;
}

bool TodayPosition::set_http_packet(base_logic::DictionaryValue* value) {
    bool r = false;
    int64 id = 0;
    std::string token;
    int64 big_status = 1;
    int32 status = 0;
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

    r = value->GetBigInteger(L"status", &big_status);
    status = big_status;
    set_status(status);
    r = value->GetBigInteger(L"start", &big_start);
    start = big_start;
    set_start(start);


    r = value->GetBigInteger(L"count", &big_count);
    count = big_count;
    set_count(count);
    return true;
}



bool FansPosition::set_http_packet(base_logic::DictionaryValue* value) {
    bool r = false;
    int64 id = 0;
    std::string token;
    std::string symbol;
    int64 big_buy_sell = 1;
    int32 buy_sell = 0;
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


    r = value->GetString(L"symbol", &symbol);
    if (r)
        set_symbol(symbol);
    else
        return false;


    r = value->GetBigInteger(L"buySell", &big_buy_sell);
    if (r){
        buy_sell = big_buy_sell;
        set_buy_sell(buy_sell);
    }else
        return false;


    r = value->GetBigInteger(L"start", &big_start);
    start = big_start;
    set_start(start);


    r = value->GetBigInteger(L"count", &big_count);
    count = big_count;
    r = value->GetBigInteger(L"start", &big_start);
    start = big_start;
    set_start(start);


    r = value->GetBigInteger(L"count", &big_count);
    count = big_count;
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
