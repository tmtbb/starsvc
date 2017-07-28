//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_STARSIDE_PROTO_BUF_H_
#define SWP_STARSIDE_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace starside_logic {

namespace net_request {

class ProfitDetail {
 public:
  ProfitDetail()
      : id_(NULL),
        token_(NULL),
        orderdate_(NULL),
        starcode_(NULL),
        price_(NULL),
        comment_(NULL){
  }

  ~ProfitDetail() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }
    if (token_) {
      delete token_;
      token_ = NULL;
    }
    if (orderdate_) {
      delete orderdate_;
      orderdate_ = NULL;
    }
    if (price_) {
      delete price_;
      price_ = NULL;
    }
    if (starcode_) {
      delete starcode_;
      starcode_ = NULL;
    }
    if (comment_){
      delete comment_;
      comment_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_orderdate(const int64 orderdate) {
    orderdate_ = new base_logic::FundamentalValue(orderdate);
  }

  void set_price(const double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  void set_starcode(const std::string& starcode) {
    starcode_ = new base_logic::StringValue(starcode);
  }

  void set_comment(const std::string& comment) {
    comment_ = new base_logic::StringValue(comment);
  }

  const int64 id() const {
    int64 id = 0;
    if (id_)
      id_->GetAsBigInteger(&id);
    return id;
  }

  const std::string token() const {
    std::string token;
    if (token_)
      token_->GetAsString(&token);
    return token;
  }

  const int64 orderdate() const{
    int64 orderdate = 0;
    if (orderdate_)
      orderdate_->GetAsBigInteger(&orderdate);
    return orderdate;
  }

  const double price() const {
    double price = 0;
    if (price_)
      price_->GetAsReal(&price);
    return price;
  }

  const std::string starcode() const {
    std::string starcode;
    if (starcode_)
      starcode_->GetAsString(&starcode);
    return starcode;
  }

  const std::string comment() const {
    std::string comment;
    if (comment_)
      comment_->GetAsString(&comment);
    return comment;
  }
 private:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* orderdate_;
  base_logic::FundamentalValue* price_;
  base_logic::StringValue* starcode_;
  base_logic::StringValue* comment_;
};

class HandleWithdrawals {
 public:
  HandleWithdrawals()
      : id_(NULL),
        token_(NULL),
        bank_id_(NULL),
        price_(NULL),
        password_(NULL),
        comment_(NULL){
  }

  ~HandleWithdrawals() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }
    if (token_) {
      delete token_;
      token_ = NULL;
    }
    if (bank_id_) {
      delete bank_id_;
      bank_id_ = NULL;
    }
    if (price_) {
      delete price_;
      price_ = NULL;
    }
    if (password_) {
      delete password_;
      password_ = NULL;
    }
    if (comment_){
      delete comment_;
      comment_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_bank_id(const int64 bank_id) {
    bank_id_ = new base_logic::FundamentalValue(bank_id);
  }

  void set_price(const double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  void set_password(const std::string& password) {
    password_ = new base_logic::StringValue(password);
  }

  void set_comment(const std::string& comment) {
    comment_ = new base_logic::StringValue(comment);
  }

  const int64 id() const {
    int64 id = 0;
    if (id_)
      id_->GetAsBigInteger(&id);
    return id;
  }

  const std::string token() const {
    std::string token;
    if (token_)
      token_->GetAsString(&token);
    return token;
  }

  const int64 bank_id() const{
    int64 bank_id = 0;
    if (bank_id_)
      bank_id_->GetAsBigInteger(&bank_id);
    return bank_id;
  }

  const double price() const {
    double price = 0;
    if (price_)
      price_->GetAsReal(&price);
    return price;
  }

  const std::string password() const {
    std::string password;
    if (password_)
      password_->GetAsString(&password);
    return password;
  }

  const std::string comment() const {
    std::string comment;
    if (comment_)
      comment_->GetAsString(&comment);
    return comment;
  }
 private:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* bank_id_;
  base_logic::FundamentalValue* price_;
  base_logic::StringValue* password_;
  base_logic::StringValue* comment_;
};

class StarSideHandle {
 public:
  StarSideHandle()
      : id_(NULL),
        token_(NULL),
        tid_(NULL),
        handle_(NULL) {
  }

  ~StarSideHandle() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }

    if (token_) {
      delete token_;
      token_ = NULL;
    }

    if (tid_) {
      delete tid_;
      tid_ = NULL;
    }

    if (handle_) {
      delete handle_;
      handle_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_tid(const int64 tid) {
    tid_ = new base_logic::FundamentalValue(tid);
  }

  void set_handle(const int32 handle) {
    handle_ = new base_logic::FundamentalValue(handle);
  }

  const int64 id() const {
    int64 id = 0;
    id_->GetAsBigInteger(&id);
    return id;
  }

  const std::string& token() const {
    std::string token;
    token_->GetAsString(&token);
    return token;
  }

  const int64 tid() const {
    int64 tid = 0;
    tid_->GetAsBigInteger(&tid);
    return tid;
  }

  const int32 handle() const {
    int32 handle = 0;
    handle_->GetAsInteger(&handle);
    return handle;
  }

 public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* tid_;
  base_logic::FundamentalValue* handle_;
};

class StarSidePosition {
 public:
  StarSidePosition()
      : id_(NULL),
        token_(NULL),
        start_(NULL),
        count_(NULL),
        symbol_(NULL),
        htype_(NULL) {
  }

  ~StarSidePosition() {
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

    if (symbol_) {
      delete symbol_;
      symbol_ = NULL;
    }

    if (htype_) {
      delete htype_;
      htype_ = NULL;
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

  void set_start(const int32 start) {
    start_ = new base_logic::FundamentalValue(start);
  }

  void set_count(const int32 count) {
    count_ = new base_logic::FundamentalValue(count);
  }

  void set_htype(const int32 htype) {
    htype_ = new base_logic::FundamentalValue(htype);
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

  const int32 htype() const {
    int32 htype = 0;
    htype_->GetAsInteger(&htype);
    return htype;
  }

 public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* start_;
  base_logic::FundamentalValue* count_;
  base_logic::FundamentalValue* htype_;
  base_logic::StringValue* symbol_;
};

class StarSideRecharge {
 public:
  StarSideRecharge()
      : id_(NULL),
        token_(NULL),
        status_(NULL),
        start_(NULL),
        count_(NULL),
        query_time_(NULL) {
  }

  ~StarSideRecharge() {
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
    
    if(query_time_){
      delete query_time_;
      query_time_ = NULL;
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

  void set_status(const int32 status) {
    status_ = new base_logic::FundamentalValue(status);
  }
  
  void set_query_time(const std::string& queryTime) {
    query_time_ = new base_logic::StringValue(queryTime);
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

  const int32 status() const {
    int32 status = 0;
    status_->GetAsInteger(&status);
    return status;
  }
  
  const std::string query_time() const {
    std::string queryTime;
    query_time_->GetAsString(&queryTime);
    return queryTime;
  }

 public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* status_;
  base_logic::FundamentalValue* start_;
  base_logic::FundamentalValue* count_;
  base_logic::StringValue* query_time_;
};

typedef StarSideRecharge StarSideWithDrawals;
typedef StarSideRecharge StarSideOwnStar;
typedef StarSidePosition HandlePosition;


class StarSideTransStatis {
 public:
  StarSideTransStatis()
      : id_(NULL),
        token_(NULL),
        starcode_(NULL),
        status_(NULL),
        stardate_(NULL),
        enddate_(NULL),
        query_time_(NULL) {
  }

  ~StarSideTransStatis() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }
    if (token_) {
      delete token_;
      token_ = NULL;
    }

    if (stardate_) {
      delete stardate_;
      stardate_ = NULL;
    }

    if (enddate_) {
      delete enddate_;
      enddate_ = NULL;
    }
    if (starcode_) {
      delete starcode_;
      starcode_ = NULL;
    }
    
    if(query_time_){
      delete query_time_;
      query_time_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_stardate(const int32 stardate) {
    stardate_ = new base_logic::FundamentalValue(stardate);
  }

  void set_enddate(const int32 enddate) {
    enddate_ = new base_logic::FundamentalValue(enddate);
  }

  void set_status(const int32 status) {
    status_ = new base_logic::FundamentalValue(status);
  }
  
  void set_query_time(const std::string& queryTime) {
    query_time_ = new base_logic::StringValue(queryTime);
  }

  void set_starcode(const std::string& starcode) {
    starcode_ = new base_logic::StringValue(starcode);
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

  const int32 stardate() const {
    int32 stardate = 0;
    stardate_->GetAsInteger(&stardate);
    return stardate;
  }

  const int32 enddate() const {
    int32 enddate = 0;
    enddate_->GetAsInteger(&enddate);
    return enddate;
  }

  const int32 status() const {
    int32 status = 0;
    status_->GetAsInteger(&status);
    return status;
  }
  
  const std::string query_time() const {
    std::string queryTime;
    query_time_->GetAsString(&queryTime);
    return queryTime;
  }
  const std::string starcode() const {
    std::string tmp;
    starcode_->GetAsString(&tmp);
    return tmp;
  }

 public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::StringValue* starcode_;
  base_logic::FundamentalValue* stardate_;
  base_logic::FundamentalValue* enddate_;

  base_logic::FundamentalValue* status_;
  base_logic::StringValue* query_time_;
};
}

namespace net_reply {

class StarSideHandle {
 public:
  StarSideHandle()
      : id_(NULL),
        handle_(NULL),
        value_(NULL) {
  }

  ~StarSideHandle() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_handle(const int32 handle) {
    handle_ = new base_logic::FundamentalValue(handle);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (id_ != NULL)
      value_->Set(L"id", id_);
    if (handle_ != NULL)
      value_->Set(L"handle", handle_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* id_;
  base_logic::FundamentalValue* handle_;
  base_logic::DictionaryValue* value_;
};

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
        result_(NULL),
        handle_(NULL),
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

class Recharge {
 public:
  Recharge()
      : rid_(NULL),
        amount_(NULL),
        deposit_time_(NULL),
        deposit_type_(NULL),
        deposit_name_(NULL),
        status_(NULL),
        value_(NULL),
        transaction_id_(NULL),
        recharge_type_(NULL) {
  }

  ~Recharge() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_rid(const int64 rid) {
    rid_ = new base_logic::FundamentalValue(rid);
  }

  void set_amount(const double amount) {
    amount_ = new base_logic::FundamentalValue(amount);
  }

  void set_deposit_time(const std::string& deposit_time) {
    deposit_time_ = new base_logic::StringValue(deposit_time);
  }

  void set_deposit_type(const int32 deposit_type) {
    deposit_type_ = new base_logic::FundamentalValue(deposit_type);
  }

  void set_deposit_name(const std::string& deposit_name) {
    deposit_name_ = new base_logic::StringValue(deposit_name);
  }

  void set_status(const int32 status) {
    status_ = new base_logic::FundamentalValue(status);
  }
  
  void set_recharge_type(const int32 recharge_type) {
  	recharge_type_ = new base_logic::FundamentalValue(recharge_type);
  }
  void set_transaction_id(const std::string& transaction_id) {
    transaction_id_ = new base_logic::StringValue(transaction_id);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (rid_ != NULL)
      value_->Set(L"rid", rid_);
    if (amount_ != NULL)
      value_->Set(L"amount", amount_);
    if (deposit_time_ != NULL)
      value_->Set(L"depositTime", deposit_time_);
    if (deposit_type_ != NULL)
      value_->Set(L"depositType", deposit_type_);
    if (deposit_name_ != NULL)
      value_->Set(L"depositName", deposit_name_);
    if (status_ != NULL)
      value_->Set(L"status", status_);
    if (recharge_type_ != NULL)
      value_->Set(L"recharge_type", recharge_type_);
    if (transaction_id_ != NULL)
      value_->Set(L"transaction_id", transaction_id_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* rid_;
  base_logic::FundamentalValue* amount_;
  base_logic::StringValue* deposit_time_;
  base_logic::FundamentalValue* deposit_type_;
  base_logic::StringValue* deposit_name_;
  base_logic::FundamentalValue* status_;
  base_logic::FundamentalValue* recharge_type_;
  base_logic::StringValue* transaction_id_;
  base_logic::DictionaryValue* value_;
};


class OwnStar{
 public:
  OwnStar()
      : uid_(NULL),
        ownseconds_(NULL),
        appoint_(NULL),
        starcode_(NULL),
        starname_(NULL),
        faccid_(NULL),
        value_(NULL),
        headurl_(NULL) {
  }

  ~OwnStar() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_ownseconds(const int64 rid) {
    ownseconds_ = new base_logic::FundamentalValue(rid);
  }
  void set_appoint(const int32 deposit_type) {
    appoint_ = new base_logic::FundamentalValue(deposit_type);
  }

  void set_starcode(const std::string& deposit_name) {
    starcode_ = new base_logic::StringValue(deposit_name);
  }
  void set_starname(const std::string& deposit_name) {
    starname_ = new base_logic::StringValue(deposit_name);
  }
  void set_faccid(const std::string& deposit_name) {
    faccid_ = new base_logic::StringValue(deposit_name);
  }
  void set_headurl(const std::string& head_url) {
    headurl_ = new base_logic::StringValue(head_url);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (uid_ != NULL)
      value_->Set(L"uid", uid_);
    if (ownseconds_ != NULL)
      value_->Set(L"ownseconds", ownseconds_);
    if (appoint_ != NULL)
      value_->Set(L"appoint", appoint_);
    if (starcode_ != NULL)
      value_->Set(L"starcode", starcode_);
    if (starname_ != NULL)
      value_->Set(L"nickname", starname_);
    if (faccid_ != NULL)
      value_->Set(L"faccid", faccid_);
    if (headurl_ != NULL)
      value_->Set(L"head_url", headurl_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* ownseconds_;
  base_logic::FundamentalValue* appoint_;
  base_logic::StringValue* starcode_;
  base_logic::StringValue* starname_;
  base_logic::StringValue* faccid_;
  base_logic::StringValue* headurl_;

  base_logic::DictionaryValue* value_;
};
typedef OwnStar TOwnStarUser;
class WithDrawals {
 public:
  WithDrawals()
      : wid_(NULL),
        id_(NULL),
        amount_(NULL),
        charge_(NULL),
        status_(NULL),
        withdraw_time_(NULL),
        handle_time_(NULL),
        bank_(NULL),
        branch_bank_(NULL),
        card_no_(NULL),
        name_(NULL),
        commet_(NULL),
        value_(NULL) {
  }

  ~WithDrawals() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_wid(const int64 wid) {
    wid_ = new base_logic::FundamentalValue(wid);
  }

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_amount(const double amount) {
    amount_ = new base_logic::FundamentalValue(amount);
  }

  void set_charge(const double charge) {
    charge_ = new base_logic::FundamentalValue(charge);
  }

  void set_status(const int32 status) {
    status_ = new base_logic::FundamentalValue(status);
  }

  void set_withdraw_time(const std::string& withdraw_time) {
    withdraw_time_ = new base_logic::StringValue(withdraw_time);
  }

  void set_handle_time(const std::string& handle_time) {
    handle_time_ = new base_logic::StringValue(handle_time);
  }

  void set_bank(const std::string& bank) {
    bank_ = new base_logic::StringValue(bank);
  }

  void set_branch_bank(const std::string& branch_bank) {
    branch_bank_ = new base_logic::StringValue(branch_bank);
  }

  void set_card_no(const std::string& card_no) {
    card_no_ = new base_logic::StringValue(card_no);
  }

  void set_name(const std::string& name) {
    name_ = new base_logic::StringValue(name);
  }

  void set_commet(const std::string& commet) {
    commet_ = new base_logic::StringValue(commet);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (wid_ != NULL)
      value_->Set(L"wid", wid_);
    if (id_ != NULL)
      value_->Set(L"id", id_);
    if (amount_ != NULL)
      value_->Set(L"amount", amount_);
    if (charge_ != NULL)
      value_->Set(L"charge", charge_);
    if (status_ != NULL)
      value_->Set(L"status", status_);
    if (withdraw_time_ != NULL)
      value_->Set(L"withdrawTime", withdraw_time_);
    if (handle_time_ != NULL)
      value_->Set(L"handleTime", handle_time_);
    if (bank_ != NULL)
      value_->Set(L"bank", bank_);
    if (branch_bank_ != NULL)
      value_->Set(L"branchBank", branch_bank_);
    if (card_no_ != NULL)
      value_->Set(L"cardNo", card_no_);
    if (name_ != NULL)
      value_->Set("name", name_);
    if (commet_ != NULL)
      value_->Set(L"comment", commet_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* wid_;
  base_logic::FundamentalValue* id_;
  base_logic::FundamentalValue* amount_;
  base_logic::FundamentalValue* charge_;
  base_logic::FundamentalValue* status_;
  base_logic::StringValue* withdraw_time_;
  base_logic::StringValue* handle_time_;
  base_logic::StringValue* bank_;
  base_logic::StringValue* branch_bank_;
  base_logic::StringValue* card_no_;
  base_logic::StringValue* name_;
  base_logic::StringValue* commet_;
  base_logic::DictionaryValue* value_;
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

typedef AllTradesPosition AllWaitPosition;

class AllRecharge {
 public:
  AllRecharge()
      : deposits_info_(NULL),
        value_(NULL) {
    deposits_info_ = new base_logic::ListValue;
  }

  ~AllRecharge() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    deposits_info_->Append((base_logic::Value*) (value));
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!deposits_info_->empty()) {
      value_->Set(L"depositsinfo", deposits_info_);
    } else {
      delete deposits_info_;
      deposits_info_ = NULL;
    }
    return value_;
  }

  const int32 Size() {
    return deposits_info_->GetSize();
  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    deposits_info_ = new base_logic::ListValue;
  }
 private:
  base_logic::ListValue* deposits_info_;
  base_logic::DictionaryValue* value_;
};


class AllOwnStar {
 public:
  AllOwnStar ()
      : deposits_info_(NULL),
        value_(NULL) {
    deposits_info_ = new base_logic::ListValue;
  }

  ~AllOwnStar() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    deposits_info_->Append((base_logic::Value*) (value));
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!deposits_info_->empty()) {
      value_->Set(L"depositsinfo", deposits_info_);
    } else {
      delete deposits_info_;
      deposits_info_ = NULL;
    }
    return value_;
  }

  const int32 Size() {
    return deposits_info_->GetSize();
  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    deposits_info_ = new base_logic::ListValue;
  }
 private:
  base_logic::ListValue* deposits_info_;
  base_logic::DictionaryValue* value_;
};

class AllWithDraw {
 public:
  AllWithDraw()
      : withdraw_info_(NULL),
        value_(NULL) {
    withdraw_info_ = new base_logic::ListValue;
  }

  ~AllWithDraw() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    withdraw_info_->Append((base_logic::Value*) (value));
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!withdraw_info_->empty()) {
      value_->Set(L"withdrawList", withdraw_info_);
    } else {
      delete withdraw_info_;
      withdraw_info_ = NULL;
    }
    return value_;
  }

  const int32 Size() {
    return withdraw_info_->GetSize();
  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    withdraw_info_ = new base_logic::ListValue;
  }
 private:
  base_logic::ListValue* withdraw_info_;
  base_logic::DictionaryValue* value_;
};
//-------------------------------------------------------------------------
class TransStatis{
 public:
  TransStatis()
      : orderdate_(NULL),
        order_num_(NULL),
        order_count_(NULL),
        starcode_(NULL),
        max_price_(NULL),
        min_price_(NULL),
        value_(NULL),
        profit_(NULL),
        price_(NULL) {
  }

  ~TransStatis() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_orderdate(const int64 orderdate) {
    orderdate_ = new base_logic::FundamentalValue(orderdate);
  }

  void set_order_num(const int64 rid) {
    order_num_ = new base_logic::FundamentalValue(rid);
  }
  void set_order_count(const int32 deposit_type) {
    order_count_ = new base_logic::FundamentalValue(deposit_type);
  }

  void set_starcode(const std::string& deposit_name) {
    starcode_ = new base_logic::StringValue(deposit_name);
  }

  void set_max_price(double price) {
    max_price_ = new base_logic::FundamentalValue(price);
  }
  void set_min_price(double price) {
    min_price_ = new base_logic::FundamentalValue(price);
  }
  void set_price(double price) {
    price_ = new base_logic::FundamentalValue(price);
  }
  void set_profit(double price) {
    profit_ = new base_logic::FundamentalValue(price);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (orderdate_ != NULL)
      value_->Set(L"orderdate", orderdate_);
    if (order_num_ != NULL)
      value_->Set(L"order_num", order_num_);
    if (order_count_ != NULL)
      value_->Set(L"order_count", order_count_);
    if (starcode_ != NULL)
      value_->Set(L"starcode", starcode_);
    if (max_price_ != NULL)
      value_->Set(L"max_price", max_price_);
    if (min_price_ != NULL)
      value_->Set(L"min_price", min_price_);
    if (price_ != NULL)
      value_->Set(L"price", price_);
    if (profit_!= NULL)
      value_->Set(L"profit", profit_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* orderdate_;
  base_logic::FundamentalValue* order_num_;//时间总数
  base_logic::FundamentalValue* order_count_;//记录数
  base_logic::StringValue* starcode_;           //明星code

  base_logic::FundamentalValue* max_price_;
  base_logic::FundamentalValue* min_price_;
  base_logic::FundamentalValue* price_;
  base_logic::FundamentalValue* profit_;

  base_logic::DictionaryValue* value_;
};

class AllTransStatis {
 public:
  AllTransStatis ()
      : deposits_info_(NULL),
        value_(NULL) {
    deposits_info_ = new base_logic::ListValue;
  }

  ~AllTransStatis() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    deposits_info_->Append((base_logic::Value*) (value));
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!deposits_info_->empty()) {
      value_->Set(L"OrderList", deposits_info_);
    } else {
      delete deposits_info_;
      deposits_info_ = NULL;
    }
    return value_;
  }

  const int32 Size() {
    return deposits_info_->GetSize();
  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    deposits_info_ = new base_logic::ListValue;
  }
 private:
  base_logic::ListValue* deposits_info_;
  base_logic::DictionaryValue* value_;
};

class ProfitDetail{
 public:
  ProfitDetail()
      : orderdate_(NULL),
        order_num_(NULL),
        order_count_(NULL),
        starcode_(NULL),
        max_price_(NULL),
        min_price_(NULL),
        value_(NULL),
        price_(NULL) {
  }

  ~ProfitDetail() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_orderdate(const int64 orderdate) {
    orderdate_ = new base_logic::FundamentalValue(orderdate);
  }

  void set_order_num(const int64 rid) {
    order_num_ = new base_logic::FundamentalValue(rid);
  }
  void set_order_count(const int32 deposit_type) {
    order_count_ = new base_logic::FundamentalValue(deposit_type);
  }

  void set_starcode(const std::string& deposit_name) {
    starcode_ = new base_logic::StringValue(deposit_name);
  }

  void set_max_price(double price) {
    max_price_ = new base_logic::FundamentalValue(price);
  }
  void set_min_price(double price) {
    min_price_ = new base_logic::FundamentalValue(price);
  }
  void set_price(double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (orderdate_ != NULL)
      value_->Set(L"orderdate", orderdate_);
    if (order_num_ != NULL)
      value_->Set(L"order_num", order_num_);
    if (order_count_ != NULL)
      value_->Set(L"order_count", order_count_);
    if (starcode_ != NULL)
      value_->Set(L"starcode", starcode_);
    if (max_price_ != NULL)
      value_->Set(L"max_price", max_price_);
    if (min_price_ != NULL)
      value_->Set(L"min_price", min_price_);
    if (price_ != NULL)
      value_->Set(L"price", price_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* orderdate_;
  base_logic::StringValue* starcode_;           //明星code
  base_logic::FundamentalValue* price_;

  base_logic::FundamentalValue* max_price_;
  base_logic::FundamentalValue* min_price_;
  base_logic::FundamentalValue* order_num_;//时间总数
  base_logic::FundamentalValue* order_count_;//记录数

  base_logic::DictionaryValue* value_;
};

class StarMeetRel{
 public:
  StarMeetRel()
      : id_(NULL),
        uid_(NULL),
        mid_(NULL),
        meet_type_(NULL),
        starcode_(NULL),
        nickname_(NULL),
        headurl_(NULL),
        name_(NULL),
        meet_city_(NULL),
        order_time_(NULL),
        comment_(NULL),
        appoint_time_(NULL) {
  }

  ~StarMeetRel() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }


  void set_id(const int64 value) {
    id_ = new base_logic::FundamentalValue(value);
  }
  void set_uid(const int64 value) {
    uid_ = new base_logic::FundamentalValue(value);
  }
  void set_mid(const int64 value) {
    mid_ = new base_logic::FundamentalValue(value);
  }
  void set_meet_type(const int64 value) {
    meet_type_ = new base_logic::FundamentalValue(value);
  }

  void set_starcode(const std::string& value) {
    starcode_ = new base_logic::StringValue(value);
  }

  void set_nickname(const std::string& value) {
    nickname_ = new base_logic::StringValue(value);
  }

  void set_headurl(const std::string& value) {
    headurl_ = new base_logic::StringValue(value);
  }

  void set_name(const std::string& value) {
     name_ = new base_logic::StringValue(value);
  }
  void set_meet_city(const std::string& value) {
     meet_city_ = new base_logic::StringValue(value);
  }
  void set_appoint_time(const std::string& value) {
     appoint_time_ = new base_logic::StringValue(value);
  }
  void set_order_time(const std::string& value) {
     order_time_ = new base_logic::StringValue(value);
  }
  void set_comment(const std::string& value) {
     comment_ = new base_logic::StringValue(value);
  }
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (id_ != NULL)
      value_->Set(L"id", id_);
    if (uid_ != NULL)
      value_->Set(L"uid", uid_);
    if (mid_ != NULL)
      value_->Set(L"mid", mid_);

    if (meet_type_ != NULL)
      value_->Set(L"meet_type", meet_type_);
    if (starcode_ != NULL)
      value_->Set(L"starcode", starcode_);
    if (nickname_ != NULL)
      value_->Set(L"nickname", nickname_);
    if (headurl_ != NULL)
      value_->Set(L"headurl", headurl_);
    if (name_ != NULL)
      value_->Set(L"name", name_);
    if (meet_city_ != NULL)
      value_->Set(L"meet_city", meet_city_);
    if (appoint_time_ != NULL)
      value_->Set(L"appoint_time", appoint_time_);
    if (order_time_ != NULL)
      value_->Set(L"order_time", order_time_ );
    if (comment_ != NULL)
      value_->Set(L"comment", comment_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* id_;
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* mid_;//
  base_logic::FundamentalValue* meet_type_;//

  base_logic::StringValue* starcode_;           //明星code
  base_logic::StringValue* nickname_;           
  base_logic::StringValue* headurl_;           
  base_logic::StringValue* name_;           
  base_logic::StringValue* meet_city_;           
  base_logic::StringValue* appoint_time_;           
  base_logic::StringValue* order_time_;           
  base_logic::StringValue* comment_;           

  base_logic::DictionaryValue* value_;
};

class ServiceItem{
 public:
  ServiceItem()
      : mid_(NULL),
        name_(NULL),
        showpic_url_(NULL),
        value_(NULL),
        price_(NULL) {
  }

  ~ServiceItem() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_mid(const int64 mid) {
    mid_ = new base_logic::FundamentalValue(mid);
  }


  void set_name(const std::string& value) {
    name_ = new base_logic::StringValue(value);
  }
  void set_showpic_url(const std::string& value) {
    showpic_url_ = new base_logic::StringValue(value);
  }

  void set_price(double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (name_ != NULL)
      value_->Set(L"name", name_);
    if (mid_!= NULL)
      value_->Set(L"mid", mid_);
    if (price_ != NULL)
      value_->Set(L"price", price_);
    if (showpic_url_ != NULL)
      value_->Set(L"showpic_url", showpic_url_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* mid_;
  base_logic::FundamentalValue* price_;

  base_logic::StringValue* name_;           //
  base_logic::StringValue* showpic_url_;           //明星code


  base_logic::DictionaryValue* value_;
};


class StarOwnService{
 public:
  StarOwnService()
      : mid_(NULL),
        starcode_(NULL),
        name_(NULL),
        value_(NULL),
        price_(NULL) {
  }

  ~StarOwnService() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_mid(const int64 mid) {
    mid_ = new base_logic::FundamentalValue(mid);
  }


  void set_starcode(const std::string& value) {
    starcode_ = new base_logic::StringValue(value);
  }
  void set_name(const std::string& value) {
    name_ = new base_logic::StringValue(value);
  }

  void set_price(double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  void set_meet_city(const std::string& value) {
    meet_city_ = new base_logic::StringValue(value);
  }
  void set_stardate(const std::string& value) {
    stardate_ = new base_logic::StringValue(value);
  }
  void set_enddate(const std::string& value) {
    enddate_ = new base_logic::StringValue(value);
  }
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (starcode_ != NULL)
      value_->Set(L"starcode", starcode_);
    if (mid_!= NULL)
      value_->Set(L"mid", mid_);
    if (price_ != NULL)
      value_->Set(L"price", price_);
    if (name_ != NULL)
      value_->Set(L"name", name_);
    if (meet_city_ != NULL)
      value_->Set(L"meet_city", meet_city_);
    if (stardate_!= NULL)
      value_->Set(L"startdate", stardate_);
    if (enddate_ != NULL)
      value_->Set(L"enddate", enddate_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* mid_;
  base_logic::FundamentalValue* price_;

  base_logic::StringValue* starcode_;           //
  base_logic::StringValue* name_;           //
  base_logic::StringValue* meet_city_;           //
  base_logic::StringValue* stardate_;           //
  base_logic::StringValue* enddate_;           //


  base_logic::DictionaryValue* value_;
};
}

}

#endif
