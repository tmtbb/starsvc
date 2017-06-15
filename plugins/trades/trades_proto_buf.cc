//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include "trades/trades_proto_buf.h"
#include "basic/basic_util.h"

namespace trades_logic {

namespace net_request {

bool CancelOrder::set_http_packet(base_logic::DictionaryValue* value) {

    bool r = false;
    int64 id = 0;
    std::string token;
    int64 order_id = 0;
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

    r = value->GetBigInteger(L"orderId", &order_id);
    if (r)
        set_order_id(order_id);
    else
        return false;
    
    return true;
}

bool TradesSymbol::set_http_packet(base_logic::DictionaryValue* value) {
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

bool ConfirmOrder::set_http_packet(base_logic::DictionaryValue* value) {
    bool r = false;
    int64 id = 0;
    std::string token;
    int64 order_id = 0;
    int64 position_id = 0;
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

    r = value->GetBigInteger(L"orderId", &order_id);
    if (r)
        set_order_id(order_id);
    else
        return false;

    r = value->GetBigInteger(L"positionId", &position_id);
    if (r)
        set_position_id(position_id);
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
    int64 big_atype = 0;
    int32 atype = 0;
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

    r = value->GetBigInteger(L"atype", &big_atype);
    if (r)
        atype = big_atype;
    else
        return false;
    set_atype(atype);

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
        count = 10;
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
    std::string symbol;
    std::string wid;
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
    else
        return false;

    r = value->GetString(L"symbol", &symbol);
    if (r)
        set_symbol(symbol);
    else
        return false;
    r = value->GetString(L"wid", &wid);
    if (r)
        set_wid(wid);
    else
        set_wid("");
    return true;
}


}

}
