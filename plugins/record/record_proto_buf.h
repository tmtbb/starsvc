//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef STAR_RECORD_PROTO_BUF_H_
#define STAR_RECORD_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace record_logic {

namespace net_request {


class PositionCount{
public:
    PositionCount()
            : id_(NULL)
            , token_(NULL)
            , symbol_(NULL){}

    ~PositionCount(){
        if (id_) {delete id_; id_ = NULL;}
        if (token_) {delete token_; token_ = NULL;}
        if (symbol_) {delete symbol_; symbol_ = NULL;}
    }


    bool set_http_packet(base_logic::DictionaryValue* value);
    
    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    int64 id() const {
        int64 id = 0;
        id_->GetAsBigInteger(&id);
        return id;
    }

    std::string token() const {
        std::string token;
        token_->GetAsString(&token);
        return token;
    }

    std::string symbol() const {
        std::string symbol;
        symbol_->GetAsString(&symbol);
        return symbol;
    }

public:
    base_logic::FundamentalValue*   id_;
    base_logic::StringValue*        token_;
    base_logic::StringValue*        symbol_;
};

class TodayPosition {
public:
    TodayPosition()
        : id_(NULL)
        , token_(NULL)
        , status_(NULL)
        , start_(NULL)
        , count_(NULL) {}

    ~TodayPosition() {
        if (id_) {
            delete id_;
            id_ = NULL;
        }
        if (token_) {
            delete token_;
            token_ = NULL;
        }
        if (status_) {
            delete status_;
            status_ = NULL;
        }
        if (start_) {
            delete start_;
            start_ = NULL;
        }
        if (count_) {
            delete count_;
            count_ = NULL;
        }
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }
    
    void set_status(const int32 status) {
        status_ = new base_logic::FundamentalValue(status);
    }

    void set_start(const int32 start) {
        start_ = new base_logic::FundamentalValue(start);
    }

    void set_count(const int32 count) {
        count_ = new base_logic::FundamentalValue(count);
    }

    const int64 id() const {
        int64 id = 0;
        id_->GetAsBigInteger(&id);
        return id;
    }

    const std::string token() const {
        std::string token;
        token_->GetAsString(&token);
        return token;
    }

    const int32 status() const {
        int32 status;
        status_->GetAsInteger(&status);
        return status;
    }

    const int32 start() const {
        int32  start;
        start_->GetAsInteger(&start);
        return start;
    }


    const int32 count() const {
        int32 count;
        count_->GetAsInteger(&count);
        return count;
    }
public:
    base_logic::FundamentalValue*   id_;
    base_logic::StringValue*        token_;
    base_logic::FundamentalValue*   status_;
    base_logic::FundamentalValue*   start_;
    base_logic::FundamentalValue*   count_;
};

typedef TodayPosition  TodayOrder;
typedef TodayPosition  HisPosition;
typedef TodayPosition  HisOrder;


class FansPosition {
public:
    FansPosition()
        : id_(NULL)
        , token_(NULL)
        , buy_sell_(NULL)
        , symbol_(NULL)
        , start_(NULL)
        , count_(NULL) {}

    ~FansPosition() {
        if (id_) {
            delete id_;
            id_ = NULL;
        }
        if (token_) {
            delete token_;
            token_ = NULL;
        }
        if (symbol_) {
            delete symbol_;
            symbol_ = NULL;
        }
        if (buy_sell_) {
            delete buy_sell_;
            buy_sell_ = NULL;
        }
        if (start_) {
            delete start_;
            start_ = NULL;
        }
        if (count_) {
            delete count_;
            count_ = NULL;
        }
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }
    
    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_buy_sell(const int32 buy_sell) {
        buy_sell_ = new base_logic::FundamentalValue(buy_sell);
    }

    void set_start(const int32 start) {
        start_ = new base_logic::FundamentalValue(start);
    }

    void set_count(const int32 count) {
        count_ = new base_logic::FundamentalValue(count);
    }

    const int64 id() const {
        int64 id = 0;
        id_->GetAsBigInteger(&id);
        return id;
    }

    const std::string token() const {
        std::string token;
        token_->GetAsString(&token);
        return token;
    }

    const std::string symbol() const {
        std::string symbol;
        symbol_->GetAsString(&symbol);
        return symbol;
    }

    const int32 buy_sell() const {
        int32 buy_sell;
        buy_sell_->GetAsInteger(&buy_sell);
        return buy_sell;
    }

    const int32 start() const {
        int32  start;
        start_->GetAsInteger(&start);
        return start;
    }


    const int32 count() const {
        int32 count;
        count_->GetAsInteger(&count);
        return count;
    }
public:
    base_logic::FundamentalValue*   id_;
    base_logic::StringValue*        token_;
    base_logic::StringValue*        symbol_;
    base_logic::FundamentalValue*   buy_sell_;
    base_logic::FundamentalValue*   start_;
    base_logic::FundamentalValue*   count_;
};

typedef FansPosition  FansOrder;
}

namespace net_reply {


class PositionCount {
 public:
    PositionCount()
        : buy_count_(NULL)
        , sell_count_(NULL)
        , symbol_(NULL)
        , value_(NULL){}

    ~PositionCount(){
        if (value_) {delete value_; value_ = NULL;}
    }


    void set_buy_count(const int32 buy_count){
        buy_count_ = new base_logic::FundamentalValue(buy_count);
    }

    void set_sell_count(const int32 sell_count) {
        sell_count_ = new base_logic::FundamentalValue(sell_count);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (buy_count_!= NULL)
            value_->Set(L"buyCount", buy_count_);
        if (sell_count_ != NULL)
            value_->Set(L"sellCount", sell_count_);
        if (symbol_ != NULL)
            value_->Set(L"symbol", symbol_);
        return value_;
    }

 public:
    base_logic::FundamentalValue* buy_count_;
    base_logic::FundamentalValue* sell_count_;
    base_logic::StringValue*      symbol_;
    base_logic::DictionaryValue*  value_;
};



class TradesUnit {
public:
    TradesUnit()
        : order_id_(NULL),
          position_id_(NULL),
          buy_uid_(NULL),
          sell_uid_(NULL),
          symbol_(NULL),
          amount_(NULL),
          open_price_(NULL),
          open_time_(NULL),
          open_cost_(NULL),
          open_charge_(NULL),
          close_time_(NULL),
          close_price_(NULL),
          gross_profit_(NULL),
          handle_(NULL),
          buy_handle_type_(NULL),
          sell_handle_type_(NULL),
          value_(NULL) {
    }

    ~TradesUnit() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_position_id(const int64 position_id) {
        position_id_ = new base_logic::FundamentalValue(position_id);
    }

    void set_order_id(const int64 order_id) {
        order_id_ = new base_logic::FundamentalValue(order_id);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_amount(const int64 amount) {
        amount_ = new base_logic::FundamentalValue(amount);
    }

    void set_open_price(const double open_price) {
        open_price_ = new base_logic::FundamentalValue(open_price);
    }

    void set_open_time(const int64 open_time) {
        open_time_ = new base_logic::FundamentalValue(open_time);
    }

    void set_open_cost(const double open_cost) {
        open_cost_ = new base_logic::FundamentalValue(open_cost);
    }

    void set_open_charge(const double open_charge) {
        open_charge_ = new base_logic::FundamentalValue(open_charge);
    }

    void set_handle(const int32 handle) {
        handle_ = new base_logic::FundamentalValue(handle);
    }

    void set_close_time(const int64 close_time) {
        close_time_ = new base_logic::FundamentalValue(close_time);
    }

    void set_close_price(const double close_price) {
        close_price_ = new base_logic::FundamentalValue(close_price);
    }

    void set_gross_profit(const double gross_profit) {
        gross_profit_ = new base_logic::FundamentalValue(gross_profit);
    }

    void set_buy_uid(const int64 buy_uid) {
        buy_uid_ = new base_logic::FundamentalValue(buy_uid);
    }

    void set_sell_uid(const int64 sell_uid) {
        sell_uid_ = new base_logic::FundamentalValue(sell_uid);
    }

    void set_buy_handle(const int32 handle_type){
        buy_handle_type_ = new base_logic::FundamentalValue(handle_type);
    }

    void set_sell_handle(const int32 handle_type) {
        sell_handle_type_ = new base_logic::FundamentalValue(handle_type);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (position_id_ != NULL)
            value_->Set(L"positionId", position_id_);
        if (order_id_ != NULL)
            value_->Set(L"orderId", order_id_);
        if (buy_uid_ != NULL)
            value_->Set(L"buyUid", buy_uid_);
        if (sell_uid_ != NULL)
            value_->Set(L"sellUid", sell_uid_);
        if (symbol_ != NULL)
            value_->Set(L"symbol", symbol_);
        if (amount_ != NULL)
            value_->Set(L"amount", amount_);
        if (open_price_ != NULL)
            value_->Set(L"openPrice", open_price_);
        if (open_time_ != NULL)
            value_->Set(L"openTime", open_time_);
        if (open_cost_ != NULL)
            value_->Set(L"openCost", open_cost_);
        if (open_charge_ != NULL)
            value_->Set(L"openCharge", open_charge_);
        if (close_time_ != NULL)
            value_->Set(L"closeTime", close_time_);
        if (close_price_ != NULL)
            value_->Set(L"closePrice", close_price_);
        if (gross_profit_ != NULL)
            value_->Set(L"grossProfit", gross_profit_);
        if (buy_handle_type_ != NULL)
            value_->Set(L"buyHandle", buy_handle_type_);
        if (sell_handle_type_ != NULL)
            value_->Set(L"sellHandler", sell_handle_type_);
        if (handle_ != NULL)
            value_->Set(L"handle", handle_);
        return value_;
    }

private:
    base_logic::FundamentalValue* order_id_;
    base_logic::FundamentalValue* position_id_;
    base_logic::FundamentalValue* buy_uid_;
    base_logic::FundamentalValue* sell_uid_;
    base_logic::StringValue* symbol_;
    base_logic::FundamentalValue* amount_;
    base_logic::FundamentalValue* open_price_;
    base_logic::FundamentalValue* open_time_;
    base_logic::FundamentalValue* open_cost_;
    base_logic::FundamentalValue* open_charge_;
    base_logic::FundamentalValue* close_time_;
    base_logic::FundamentalValue* close_price_;
    base_logic::FundamentalValue* gross_profit_;
    base_logic::FundamentalValue* handle_;
    base_logic::FundamentalValue* buy_handle_type_;
    base_logic::FundamentalValue* sell_handle_type_;
    base_logic::DictionaryValue* value_;
};

class PositionUnit {
public:
    PositionUnit()
        : position_id_(NULL),
          id_(NULL),
          code_id_(NULL),
          symbol_(NULL),
          name_(NULL),
          buy_sell_(NULL),
          amount_(NULL),
          r_amount_(NULL),
          open_price_(NULL),
          position_time_(NULL),
          open_cost_(NULL),
          open_charge_(NULL),
          close_time_(NULL),
          close_price_(NULL),
          gross_profit_(NULL),
          limit_(NULL),
          stop_(NULL),
          close_type_(NULL),
          is_deferred_(NULL),
          deferred_(NULL),
          interval_(NULL),
          result_(NULL),
          handle_(NULL),
          value_(NULL) {
    }

    ~PositionUnit() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_position_id(const int64 position_id) {
        position_id_ = new base_logic::FundamentalValue(position_id);
    }

    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_code_id(const int64 code_id) {
        code_id_ = new base_logic::FundamentalValue(code_id);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_name(const std::string& name) {
        name_ = new base_logic::StringValue(name);
    }

    void set_buy_sell(const int32 buy_sell) {
        buy_sell_ = new base_logic::FundamentalValue(buy_sell);
    }

    void set_amount(const int64 amount) {
        amount_ = new base_logic::FundamentalValue(amount);
    }

    void set_r_amount(const int64 r_amount){
        r_amount_ = new base_logic::FundamentalValue(r_amount);
    }

    void set_open_price(const double open_price) {
        open_price_ = new base_logic::FundamentalValue(open_price);
    }

    void set_position_time(const int64 position_time) {
        position_time_ = new base_logic::FundamentalValue(position_time);
    }

    void set_open_cost(const double open_cost) {
        open_cost_ = new base_logic::FundamentalValue(open_cost);
    }

    void set_open_charge(const double open_charge) {
        open_charge_ = new base_logic::FundamentalValue(open_charge);
    }

    void set_result(const bool result) {
        result_ = new base_logic::FundamentalValue(result);
    }

    void set_close_time(const int64 close_time) {
        close_time_ = new base_logic::FundamentalValue(close_time);
    }

    void set_close_price(const double close_price) {
        close_price_ = new base_logic::FundamentalValue(close_price);
    }

    void set_gross_profit(const double gross_profit) {
        gross_profit_ = new base_logic::FundamentalValue(gross_profit);
    }

    void set_limit(const double limit) {
        limit_ = new base_logic::FundamentalValue(limit);
    }

    void set_stop(const double stop) {
        stop_ = new base_logic::FundamentalValue(stop);
    }

    void set_close_type(const int32 close_type) {
        close_type_ = new base_logic::FundamentalValue(close_type);
    }

    void set_is_deferred(const bool is_deferred) {
        is_deferred_ = new base_logic::FundamentalValue(is_deferred);
    }

    void set_deferred(const double deferred) {
        deferred_ = new base_logic::FundamentalValue(deferred);
    }

    void set_interval(const double interval) {
        interval_ = new base_logic::FundamentalValue(interval);
    }

    void set_handle(const int32 handle) {
        handle_ = new base_logic::FundamentalValue(handle);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (position_id_ != NULL)
            value_->Set(L"positionId", position_id_);
        if (id_ != NULL)
            value_->Set(L"id", id_);
        if (code_id_ != NULL)
            value_->Set(L"codeId", code_id_);
        if (symbol_ != NULL)
            value_->Set(L"symbol", symbol_);
        if (name_ != NULL)
            value_->Set(L"name", name_);
        if (buy_sell_ != NULL)
            value_->Set(L"buySell", buy_sell_);
        if (amount_ != NULL)
            value_->Set(L"amount", amount_);
        if (r_amount_ != NULL)
            value_->Set(L"rtAmount",r_amount_);
        if (open_price_ != NULL)
            value_->Set(L"openPrice", open_price_);
        if (position_time_ != NULL)
            value_->Set(L"positionTime", position_time_);
        if (open_cost_ != NULL)
            value_->Set(L"openCost", open_cost_);
        if (open_charge_ != NULL)
            value_->Set(L"openCharge", open_charge_);
        if (close_time_ != NULL)
            value_->Set(L"closeTime", close_time_);
        if (close_price_ != NULL)
            value_->Set(L"closePrice", close_price_);
        if (gross_profit_ != NULL)
            value_->Set(L"grossProfit", gross_profit_);
        if (limit_ != NULL)
            value_->Set(L"limit", limit_);
        if (stop_ != NULL)
            value_->Set(L"stop", stop_);
        if (close_type_ != NULL)
            value_->Set(L"closeType", close_type_);
        if (is_deferred_ != NULL)
            value_->Set(L"isDeferred", is_deferred_);
        if (deferred_ != NULL)
            value_->Set(L"deferred", deferred_);
        if (result_ != NULL)
            value_->Set(L"result", result_);
        if (interval_ != NULL)
            value_->Set(L"interval", interval_);
        if (handle_ != NULL)
            value_->Set(L"handle", handle_);
        return value_;
    }

private:
    base_logic::FundamentalValue* position_id_;
    base_logic::FundamentalValue* id_;
    base_logic::FundamentalValue* code_id_;
    base_logic::StringValue* symbol_;
    base_logic::StringValue* name_;
    base_logic::FundamentalValue* buy_sell_;
    base_logic::FundamentalValue* amount_;
    base_logic::FundamentalValue* r_amount_;
    base_logic::FundamentalValue* open_price_;
    base_logic::FundamentalValue* position_time_;
    base_logic::FundamentalValue* open_cost_;
    base_logic::FundamentalValue* open_charge_;
    base_logic::FundamentalValue* close_time_;
    base_logic::FundamentalValue* close_price_;
    base_logic::FundamentalValue* gross_profit_;
    base_logic::FundamentalValue* limit_;
    base_logic::FundamentalValue* stop_;
    base_logic::FundamentalValue* close_type_;
    base_logic::FundamentalValue* is_deferred_;
    base_logic::FundamentalValue* deferred_;
    base_logic::FundamentalValue* interval_;
    base_logic::FundamentalValue* result_;
    base_logic::FundamentalValue* handle_;
    base_logic::DictionaryValue* value_;
};

class UserInfoUnit{
 public:
    UserInfoUnit()
        : uid_(NULL)
        , gender_(NULL)
        , nickname_(NULL)
        , head_url_(NULL)
        , value_(NULL){}

    ~UserInfoUnit(){
        if (value_) {delete value_; value_ = NULL;}
    }

    void set_uid(const int64 uid) {
        uid_ = new base_logic::FundamentalValue(uid);
    }

    void set_gender(const int32 gender) {
        gender_ = new base_logic::FundamentalValue(gender);
    }

    void set_nickname(const std::string& nickname) {
        nickname_ = new base_logic::StringValue(nickname);   
    }

    void set_head_url(const std::string& head_url) {
        head_url_ = new base_logic::StringValue(head_url);
    }


    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (uid_ != NULL)
            value_->Set(L"uid", uid_);
        if (gender_ != NULL)
            value_->Set(L"gender", gender_);
        if (nickname_ != NULL)
            value_->Set(L"nickname", nickname_);
        if (head_url_ != NULL)
            value_->Set(L"headUrl", head_url_);
        return value_;
    }


 private:
    base_logic::FundamentalValue* uid_;
    base_logic::FundamentalValue* gender_;
    base_logic::StringValue* nickname_;
    base_logic::StringValue* head_url_;
    base_logic::DictionaryValue* value_;
};



class UserTrades {
public:
    UserTrades()
        :  value_(NULL) {
        value_ = new base_logic::DictionaryValue;
    }

    ~UserTrades() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_user(base_logic::DictionaryValue* value) {
        //user_info_ = base_logic::DictionaryValue(value);
        value_->Set(L"user",value);
    }

    void set_trades(base_logic::DictionaryValue* value) {
        value_->Set(L"trades",value);
    }

    base_logic::DictionaryValue* get() {
        return value_;
    }

private:
    base_logic::DictionaryValue* user_info_;
    base_logic::DictionaryValue* trades_info_;
    base_logic::DictionaryValue* value_;
};


class UserOrder {
public:
    UserOrder()
        :  value_(NULL) {
        value_ = new base_logic::DictionaryValue;
    }

    ~UserOrder() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_buy_user(base_logic::DictionaryValue* value) {
        //user_info_ = base_logic::DictionaryValue(value);
        value_->Set(L"buy_user",value);
    }

    void set_sell_user(base_logic::DictionaryValue* value) {
        value_->Set(L"sell_user", value);
    }

    void set_trades(base_logic::DictionaryValue* value) {
        value_->Set(L"trades",value);
    }

    base_logic::DictionaryValue* get() {
        return value_;
    }

private:
    base_logic::DictionaryValue* user_info_;
    base_logic::DictionaryValue* trades_info_;
    base_logic::DictionaryValue* value_;
};

class AllPosition {
public:
    AllPosition()
        : positions_info_(NULL),
          value_(NULL) {
        positions_info_ = new base_logic::ListValue;
    }

    ~AllPosition() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_unit(base_logic::DictionaryValue* value) {
        positions_info_->Append((base_logic::Value*) (value));
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (!positions_info_->empty()) {
            value_->Set(L"positionsList", positions_info_);
        } else {
            delete positions_info_;
            positions_info_ = NULL;
        }
        return value_;
    }

    const int32 Size() {
        return positions_info_->GetSize();
    }

    void Reset() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
        positions_info_ = new base_logic::ListValue;
    }
private:
    base_logic::ListValue* positions_info_;
    base_logic::DictionaryValue* value_;
};


class AllOrder {
public:
    AllOrder()
        : order_info_(NULL),
          value_(NULL) {
        order_info_ = new base_logic::ListValue;
    }

    ~AllOrder() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_unit(base_logic::DictionaryValue* value) {
        order_info_->Append((base_logic::Value*) (value));
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (!order_info_->empty()) {
            value_->Set(L"ordersList", order_info_);
        } else {
            delete order_info_;
            order_info_ = NULL;
        }
        return value_;
    }

    const int32 Size() {
        return order_info_->GetSize();
    }

    void Reset() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
        order_info_ = new base_logic::ListValue;
    }
private:
    base_logic::ListValue* order_info_;
    base_logic::DictionaryValue* value_;
};


}

}

#endif
