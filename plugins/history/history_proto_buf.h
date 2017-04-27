//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_HISTORY_PROTO_BUF_H_
#define SWP_HISTORY_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace history_logic {

namespace net_request {

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

class HistoryHandle {
 public:
  HistoryHandle()
      : id_(NULL),
        token_(NULL),
        tid_(NULL),
        handle_(NULL) {
  }

  ~HistoryHandle() {
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

class HistoryPosition {
 public:
  HistoryPosition()
      : id_(NULL),
        token_(NULL),
        start_(NULL),
        count_(NULL),
        symbol_(NULL),
        htype_(NULL) {
  }

  ~HistoryPosition() {
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

class HistoryRecharge {
 public:
  HistoryRecharge()
      : id_(NULL),
        token_(NULL),
        status_(NULL),
        start_(NULL),
        count_(NULL) {
  }

  ~HistoryRecharge() {
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

 public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* status_;
  base_logic::FundamentalValue* start_;
  base_logic::FundamentalValue* count_;
};

typedef HistoryRecharge HistoryWithDrawals;
typedef HistoryPosition HandlePosition;

}

namespace net_reply {

class HistoryHandle {
 public:
  HistoryHandle()
      : id_(NULL),
        handle_(NULL),
        value_(NULL) {
  }

  ~HistoryHandle() {
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
        value_(NULL) {
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
    return value_;
  }

 private:
  base_logic::FundamentalValue* rid_;
  base_logic::FundamentalValue* amount_;
  base_logic::StringValue* deposit_time_;
  base_logic::FundamentalValue* deposit_type_;
  base_logic::StringValue* deposit_name_;
  base_logic::FundamentalValue* status_;
  base_logic::DictionaryValue* value_;
};

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

}

}

#endif
