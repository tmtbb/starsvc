//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_TRADES_PROTO_BUF_H_
#define SWP_TRADES_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace trades_logic {

namespace net_request {

class TradesSymbol {
public:
    TradesSymbol()
        : id_(NULL)
        , token_(NULL)
        , symbol_(NULL){}

    ~TradesSymbol() {
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

public:
    base_logic::FundamentalValue* id_;
    base_logic::StringValue*      token_;
    base_logic::StringValue*      symbol_;
};

class CancelOrder {
public:
    CancelOrder()
        : id_(NULL)
        , token_(token_)
        , order_id_(NULL) {}
    ~CancelOrder() {
        if (id_) {delete id_; id_ = NULL;}
        if (token_) {delete token_; token_ = NULL;}
        if (order_id_) {delete order_id_; order_id_ = NULL;}
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void set_id(const int64 id){
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token){
        token_ = new base_logic::StringValue(token);
    }

    void set_order_id(const int64 order_id){
        order_id_ = new base_logic::FundamentalValue(order_id);    
    }

    const int64 id() const {
        int64 id = 0;
        id_->GetAsBigInteger(&id);
    }

    const std::string token() const {
        std::string token;
        token_->GetAsString(&token);
        return token;
    }

    const int64 order_id() const {
        int64 order_id = 0;
        order_id_->GetAsBigInteger(&order_id);
        return order_id;
    }

public:
    base_logic::FundamentalValue* id_;
    base_logic::StringValue* token_;
    base_logic::FundamentalValue* order_id_;
};
class CurrentPosition {
public:
    CurrentPosition()
        : id_(NULL),
          token_(NULL),
          atype_(NULL),
          start_(NULL),
          count_(NULL) {
    }

    ~CurrentPosition() {
        if (id_) {
            delete id_;
            id_ = NULL;
        }
        if (token_) {
            delete token_;
            token_ = NULL;
        }
        if (start_) {
            delete start_;
            start_ = NULL;
        }

        if (count_) {
            delete count_;
            count_ = NULL;
        }

        if (atype_) {
            delete atype_;
            atype_ = NULL;
        }
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }

    void set_start(const int32 start) {
        start_ = new base_logic::FundamentalValue(start);
    }

    void set_count(const int32 count) {
        count_ = new base_logic::FundamentalValue(count);
    }

    void set_atype(const int32 atype) {
        atype_ = new base_logic::FundamentalValue(atype);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    const int64 id() const {
        int64 id = 0;
        if (id_ != NULL)
            id_->GetAsBigInteger(&id);
        return id;
    }

    const std::string token() const {
        std::string token;
        if (token_ != NULL)
            token_->GetAsString(&token);
        return token;
    }

    const std::string symbol() const {
        std::string symbol;
        if (symbol_ != NULL)
            symbol_->GetAsString(&symbol);
        return symbol;
    }
    const int32 atype() const {
        int32 atype = 0;
        if (atype_ != NULL)
            atype_->GetAsInteger(&atype);
        return atype;
    }

    const int32 start() const {
        int32 start = 0;
        start_->GetAsInteger(&start);
        return start;
    }

    const int32 count() const {
        int32 count = 0;
        count_->GetAsInteger(&count);
        return count;
    }

public:
    base_logic::FundamentalValue* id_;
    base_logic::StringValue* token_;
    base_logic::StringValue* symbol_;
    base_logic::FundamentalValue* atype_;
    base_logic::FundamentalValue* start_;
    base_logic::FundamentalValue* count_;
};

class ConfirmOrder {
public:
    ConfirmOrder()
        :id_(NULL)
        ,token_(NULL)
        ,order_id_(NULL)
        ,position_id_(NULL) {}

    ~ConfirmOrder() {
        if (id_) {
            delete id_;
            id_ = NULL;
        }
        if (token_) {
            delete token_;
            token_ = NULL;
        }
        if (order_id_) {
            delete order_id_;
            order_id_ = NULL;
        }
        if (position_id_) {
            delete position_id_;
            position_id_ = NULL;
        }
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void  set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }

    void set_order_id(const int64 order_id) {
        order_id_ = new base_logic::FundamentalValue(order_id);
    }

    void set_position_id(const int64 position_id) {
        position_id_ = new base_logic::FundamentalValue(position_id);
    }

    const int64 id() const {
        int64 id = 0;
        if (id_ != NULL)
            id_->GetAsBigInteger(&id);
        return id;
    }

    const std::string token() const {
        std::string token;
        if (token_ != NULL)
            token_->GetAsString(&token);
        return token;
    }

    const int64 order_id() const {
        int64 order_id = 0;
        if (order_id_ != NULL)
            order_id_->GetAsBigInteger(&order_id);
        return order_id;
    }

    const int64 position_id() const {
        int64 position_id = 0;
        if (position_id_ != NULL)
            position_id_->GetAsBigInteger(&position_id);
        return position_id;
    }

public:
    base_logic::FundamentalValue*  id_;
    base_logic::StringValue*       token_;
    base_logic::FundamentalValue*  order_id_;
    base_logic::FundamentalValue*  position_id_;
};

class OpenPosition {
public:
    OpenPosition()
        : id_(NULL),
          token_(NULL),
          buy_sell_(NULL),
          amount_(NULL),
          price_(NULL),
          symbol_(NULL),
          wid_(NULL) {
    }

    ~OpenPosition() {
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
        if (amount_) {
            delete amount_;
            amount_ = NULL;
        }
        if (price_) {
            delete price_;
            price_ = NULL;
        }
        if (wid_) {
            delete wid_;
            wid_ = NULL;
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

    void set_wid(const std::string& wid) {
        wid_ = new base_logic::StringValue(wid);
    }

    void set_buy_sell(const int32 buy_sell) {
        buy_sell_ = new base_logic::FundamentalValue(buy_sell);
    }

    void set_amount(const int64 amount) {
        amount_ = new base_logic::FundamentalValue(amount);
    }

    void set_price(const double price) {
        price_ = new base_logic::FundamentalValue(price);
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

    const std::string wid() const {
        std::string wid;
        //wid_->GetAsString(&wid);
        return wid;
    }

    const int32 buy_sell() const {
        int32 buy_sell;
        buy_sell_->GetAsInteger(&buy_sell);
        return buy_sell;
    }

    const int64 amount() const {
        int64 amount = 0;
        amount_->GetAsBigInteger(&amount);
        return amount;
    }

    const double price() const {
        double price = 0.0;
        price_->GetAsReal(&price);
        return price;
    }

public:
    base_logic::FundamentalValue* id_;
    base_logic::StringValue* token_;
    base_logic::StringValue* symbol_;
    base_logic::StringValue* wid_;
    base_logic::FundamentalValue* buy_sell_;
    base_logic::FundamentalValue* amount_;
    base_logic::FundamentalValue* price_;
};

}

namespace net_reply {

class TradesPosition {
public:
    TradesPosition()
        : position_id_(NULL),
          id_(NULL),
          code_id_(NULL),
          symbol_(NULL),
          name_(NULL),
          buy_sell_(NULL),
          amount_(NULL),
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
          value_(NULL) {
    }

    ~TradesPosition() {
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
        if (interval_ != NULL)
            value_->Set(L"interval", interval_);
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
    base_logic::DictionaryValue* value_;
};


class TradesSymbol {
 public:
    TradesSymbol()
        : symbol_(NULL)
        , remaining_time_(NULL)
        , status_(NULL)
        , value_(NULL){}

    ~TradesSymbol() {
        if (value_) {delete value_; value_ = NULL;}
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_remaining_time(const int64 remaining_time) {
        remaining_time_ = new base_logic::FundamentalValue(remaining_time);
    }

    void set_status(const bool status) {
        status_ = new base_logic::FundamentalValue(status);
    }


    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (symbol_ != NULL)
            value_->Set(L"symbol", symbol_);
        if (remaining_time_ != NULL)
            value_->Set(L"remainingTime", remaining_time_);
        if (status_ != NULL)
            value_->Set(L"status", status_);
        return value_;
    }

 private:
    base_logic::StringValue*        symbol_;
    base_logic::FundamentalValue*   remaining_time_;
    base_logic::FundamentalValue*   status_;
    base_logic::DictionaryValue*    value_;
};

class MatchingNotice {
public:
    MatchingNotice()
        :order_id_(NULL)
        ,buy_uid_(NULL)
        ,sell_uid_(NULL)
        ,open_position_time_(NULL)
        ,open_price_(NULL)
        ,symbol_(NULL)
        ,amount_(NULL)
        ,position_id_(NULL)
        ,value_(NULL) {}

    ~MatchingNotice() {
        if (value_) {delete value_; value_ = NULL;}
    }

    void set_order_id(const int64 order_id) {
        order_id_ = new base_logic::FundamentalValue(order_id);
    }

    void set_buy_uid(const int64 buy_uid) {
        buy_uid_ = new base_logic::FundamentalValue(buy_uid);
    }

    void set_sell_uid(const int64 sell_uid) {
        sell_uid_ = new base_logic::FundamentalValue(sell_uid);
    }

    void set_open_position_time(const int64 open_position_time) {
        open_position_time_ = new base_logic::FundamentalValue(open_position_time);
    }

    void set_open_price(const double open_price) {
        open_price_ = new base_logic::FundamentalValue(open_price);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_amount(const int64 amount) {
        amount_ = new base_logic::FundamentalValue(amount);
    }

    void set_position_id(const int64 position_id) {
        position_id_ = new base_logic::FundamentalValue(position_id);
    }


    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (order_id_ != NULL)
            value_->Set(L"orderId", order_id_);
        if (buy_uid_ != NULL)
            value_->Set(L"buyUid", buy_uid_);
        if (sell_uid_ != NULL)
            value_->Set(L"sellUid", sell_uid_);
        if (amount_ != NULL)
            value_->Set("amount", amount_);
        if (open_position_time_ != NULL)
            value_->Set(L"openPositionTime", open_position_time_);
        if (open_price_ != NULL)
            value_->Set(L"openPrice", open_price_);
        if (symbol_ != NULL)
            value_->Set(L"symbol", symbol_);
        //if (position_id_ != NULL)
        //   value_->Set(L"positionId", position_id_);
        return value_;
    }

private:
    base_logic::FundamentalValue* order_id_;
    base_logic::FundamentalValue* buy_uid_;
    base_logic::FundamentalValue* sell_uid_;
    base_logic::FundamentalValue* open_position_time_;
    base_logic::FundamentalValue* open_price_;
    base_logic::FundamentalValue* amount_;
    base_logic::FundamentalValue* position_id_;
    base_logic::StringValue* symbol_;
    base_logic::DictionaryValue* value_;
};
class TradingUnit {
public:
    TradingUnit()
        :position_id_(NULL)
        ,symbol_(NULL)
        ,buy_sell_(NULL)
        ,open_price_(NULL)
        ,amount_(NULL)
        ,uid_(NULL)
        ,name_(NULL)
        ,pic_(NULL) {}

    void set_position_id(const int64 position_id) {
        position_id_ = new base_logic::FundamentalValue(position_id);
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_buy_sell(const int32 buy_sell) {
        buy_sell_ = new base_logic::FundamentalValue(buy_sell);
    }

    void set_open_price(const double open_price) {
        open_price_ = new base_logic::FundamentalValue(open_price);
    }

    void set_amount(const int32 amount) {
        amount_ = new base_logic::FundamentalValue(amount);
    }

    void set_uid(const int64 uid) {
        uid_ = new base_logic::FundamentalValue(uid);
    }

    void set_name(const std::string& name) {
        name_ = new base_logic::StringValue(name);
    }

    void set_pic(const std::string& pic) {
        pic_ = new base_logic::StringValue(pic);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (position_id_ != NULL)
            value_->Set(L"positionId", position_id_);
        if (symbol_ != NULL)
            value_->Set("symbol", symbol_);
        if (buy_sell_ != NULL)
            value_->Set(L"buy_sell", buy_sell_);
        if (open_price_ != NULL)
            value_->Set(L"openPrice", open_price_);
        if (amount_ != NULL)
            value_->Set(L"amount", amount_);
        if (uid_ != NULL)
            value_->Set(L"uid",uid_);
        if (name_ != NULL)
            value_->Set(L"name", name_);
        if (pic_ != NULL)
            value_->Set(L"pic", pic_);
        return value_;
    }

private:
    base_logic::FundamentalValue*    position_id_;
    base_logic::StringValue*         symbol_;
    base_logic::FundamentalValue*    buy_sell_;
    base_logic::FundamentalValue*    open_price_;
    base_logic::FundamentalValue*    amount_;
    base_logic::FundamentalValue*    uid_;
    base_logic::StringValue*         name_;
    base_logic::StringValue*         pic_;
    base_logic::DictionaryValue*     value_;
};

class AllTradesPosition {
public:
    AllTradesPosition()
        : positions_info_(NULL),
          value_(NULL) {
        positions_info_ = new base_logic::ListValue;
    }

    ~AllTradesPosition() {
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
            value_->Set(L"positioninfo", positions_info_);
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

class OrderConfirm {
public:
    OrderConfirm()
        : order_id_(NULL)
        , uid_(NULL)
        , status_(NULL)
        , value_(NULL){}
    ~OrderConfirm() {
        if (value_) {delete value_; value_ = NULL;}
    }

    void set_order_id(const int64 order_id) {
        order_id_ = new base_logic::FundamentalValue(order_id);
    }

    void set_uid(const int64 uid){
        uid_ = new base_logic::FundamentalValue(uid);
    }

    void set_status(const int32 status) {
        status_ = new base_logic::FundamentalValue(status);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (order_id_ != NULL)
            value_->Set(L"orderId",order_id_);
        if (uid_ != NULL)
            value_->Set(L"uid", uid_);
        if (status_ != NULL)
            value_->Set(L"status", status_);
        return value_;
    }
private:
    base_logic::FundamentalValue* order_id_;
    base_logic::FundamentalValue* uid_;
    base_logic::FundamentalValue* status_;
    base_logic::DictionaryValue* value_;
};
class OrderResult {
public:
    OrderResult()
        : order_id_(NULL)
        , result_(NULL)
        , value_(NULL) {}

    ~OrderResult() {
        if (value_){delete value_; value_ = NULL;}
    }

    void set_order_id(const int64 order_id) {
        order_id_ = new base_logic::FundamentalValue(order_id);
    }

    void set_result(const int32 result) {
        result_ = new base_logic::FundamentalValue(result);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (order_id_ != NULL)
            value_->Set(L"orderId",order_id_);
        if (result_ != NULL)
            value_->Set(L"result", result_);
        return value_;
    }

private:
    base_logic::FundamentalValue*  order_id_;
    base_logic::FundamentalValue*  result_;
    base_logic::DictionaryValue* value_;
};

}

}
#endif
