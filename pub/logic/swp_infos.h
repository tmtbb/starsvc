//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_PUB_LOGIC_SWP_INFOS_H_
#define SWP_PUB_LOGIC_SWP_INFOS_H_

#include <string>
#include <map>
#include <list>

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"

enum PAYTYPE {
  WX_APP = 1,
  UNIPNPAY = 2
};

enum {
  STOCK_TYPE = 1,
  GOODS_TYPE = 2,
  FUTURES_TYPE = 3,
  FORXE_TYPE = 4,
};

enum BSTYPE {
  BUY_TYPE = 1,
  SELL_TYPE = -1
};

enum TIMETYPE {
  ONE_MINUTE = 60,
  FIVE_MINUTE = 300,
  HALF_HOUR = 1800,
  ONE_HOUR = 3600
};

enum HANDLETYPE {
  NO_HANDLE = -1,
  WAIT_HANDLE = 0,
  DOUBLE_PROFIT = 1,
  FACT_HANDLE = 2,
  REFUND_HANDLE = 3
};

namespace swp_logic {

class Withdrawals {
 public:
  Withdrawals();
  Withdrawals(const Withdrawals& withdrawals);
  Withdrawals& operator =(const Withdrawals& withdrawals);

  ~Withdrawals() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool close_after(const Withdrawals& t_withdrawals,
                          const Withdrawals& r_withdrawals) {
    return Data::close_after(t_withdrawals.data_, r_withdrawals.data_);
  }

  void set_uid(const int64 uid) {
    data_->uid_ = uid;
  }
  void set_wid(const int64 wid) {
    data_->wid_ = wid;
  }
  void set_status(const int32 status) {
    data_->status_ = status;
  }
  void set_amount(const double amount) {
    data_->amount_ = amount;
  }
  void set_charge(const double charge) {
    data_->charge_ = charge;
  }
  void set_withdraw_unix_time(const int64 withdraw_unix_time) {
    data_->withdraw_unix_time_ = withdraw_unix_time;
  }
  void set_withdraw_time(const std::string& withdraw_time) {
    data_->withdraw_time_ = withdraw_time;
  }
  void set_handle_time(const std::string& handle_time) {
    data_->handle_time_ = handle_time;
  }
  void set_bank(const std::string& bank) {
    data_->bank_ = bank;
  }

  void set_branch_bank(const std::string& branch_bank) {
    data_->branch_bank_ = branch_bank;
  }

  void set_card_no(const std::string& card_no) {
    data_->card_no_ = card_no;
  }

  void set_name(const std::string& name) {
    data_->name_ = name;
  }

  void set_comment(const std::string& comment) {
    data_->comment_ = comment;
  }

  const int64 uid() const {
    return data_->uid_;
  }

  const int64 wid() const {
    return data_->wid_;
  }

  const int32 status() const {
    return data_->status_;
  }

  const double amount() const {
    return data_->amount_;
  }

  const double charge() const {
    return data_->charge_;
  }

  const int64 withdraw_unix_time() const {
    return data_->withdraw_unix_time_;
  }

  const std::string& withdraw_time() const {
    return data_->withdraw_time_;
  }

  const std::string& handle_time() const {
    return data_->handle_time_;
  }

  const std::string& bank() const {
    return data_->bank_;
  }

  const std::string& branch_bank() const {
    return data_->branch_bank_;
  }

  const std::string& card_no() const {
    return data_->card_no_;
  }

  const std::string& name() const {
    return data_->name_;
  }

  const std::string& comment() const {
    return data_->comment_;
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);

  class Data {
   public:
    Data()
        : refcount_(1),
          uid_(0),
          wid_(0),
          status_(0),
          amount_(0.0),
          charge_(0.0),
          withdraw_unix_time_(0) {
    }

    static bool close_after(const Data* t_data, const Data* r_data) {
      return t_data->withdraw_unix_time_ > r_data->withdraw_unix_time_;
    }

   public:
    int64 uid_;
    int64 wid_;
    int32 status_;
    double amount_;
    double charge_;
    int64 withdraw_unix_time_;
    std::string withdraw_time_;
    std::string handle_time_;
    std::string bank_;
    std::string branch_bank_;
    std::string card_no_;
    std::string name_;
    std::string comment_;
    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
   private:
    int refcount_;
  };
  Data* data_;
};

class Recharge {
 public:
  Recharge();
  Recharge(const Recharge& recharge);
  Recharge& operator =(const Recharge& recharge);

  ~Recharge() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool close_after(const Recharge& t_recharge,
                          const Recharge& r_recharge) {
    return Data::close_after(t_recharge.data_, r_recharge.data_);
  }

  void set_rid(const int64 rid) {
    data_->rid_ = rid;
  }

  void set_uid(const int64 uid) {
    data_->uid_ = uid;
  }

  void set_status(const int32 status) {
    data_->status_ = status;
  }

  void set_amount(const double amount) {
    data_->amount_ = amount;
  }

  void set_deposit_type(const int32 deoposit_type) {
    data_->deposit_type_ = deoposit_type;
  }

  void set_deposit_time(const std::string& deposit_time) {
    data_->deposit_time_ = deposit_time;
  }

  void set_deposit_name(const std::string& deposit_name) {
    data_->deposit_name_ = deposit_name;
  }

  void set_deposit_unix_time(const int64 unix_time) {
    data_->deposit_unix_time_ = unix_time;
  }

  const int64 rid() const {
    return data_->rid_;
  }

  const int64 uid() const {
    return data_->uid_;
  }

  const int32 deposit_type() const {
    return data_->deposit_type_;
  }

  const double amount() const {
    return data_->amount_;
  }

  const int32 status() const {
    return data_->status_;
  }

  const int64 deposit_unix_time() const {
    return data_->deposit_unix_time_;
  }

  const std::string& deposit_time() const {
    return data_->deposit_time_;
  }

  const std::string& deposit_name() const {
    return data_->deposit_name_;
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);

  class Data {
   public:
    Data()
        : refcount_(1),
          rid_(0),
          status_(0),
          amount_(0.0),
          deposit_type_(0),
          deposit_unix_time_(0),
          uid_(0) {
    }

    static bool close_after(const Data* t_data, const Data* r_data) {
      return t_data->deposit_unix_time_ > r_data->deposit_unix_time_;
    }

   public:
    int64 rid_;
    int32 status_;
    int64 uid_;
    double amount_;
    int32 deposit_type_;
    int64 deposit_unix_time_;
    std::string deposit_time_;
    std::string deposit_name_;
    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
   private:
    int refcount_;
  };
  Data* data_;
};

class UserInfo {
 public:
  UserInfo();
  UserInfo(const UserInfo& userinfo);
  UserInfo& operator =(const UserInfo& userinfo);

  ~UserInfo() {
    if (data_ != NULL) {
      data_->Release();
    }
  }
  const int64 uid() const {
    return data_->uid_;
  }
  const double balance() const {
    return data_->balance_;
  }
  const int socket_fd() const {
    return data_->socket_fd_;
  }
  const int32 gender() const {
    return data_->gender_;
  }
  const int32 type() const {
    return data_->type_;
  }
  const std::string& phone_num() const {
    return data_->phone_num_;
  }
  const std::string nickname() const {
    return data_->nickname_;
  }
  const std::string& token() const {
    return data_->token_;
  }
  const std::string& head_url() const {
    return data_->head_url_;
  }

  const int32 send_error_count() const {
    return data_->send_error_count_;
  }

  const int32 recv_error_count() const {
    return data_->recv_error_count_;
  }

  const bool is_effective() const {
    return data_->is_effective_;
  }

  const time_t send_last_time() const {
    return data_->send_last_time_;
  }

  const time_t recv_last_time() const {
    return data_->recv_last_time_;
  }

  void set_type(const int32 type) {
    data_->type_ = type;
  }
  void set_uid(const int64 uid) {
    data_->uid_ = uid;
  }
  void set_socket_fd(const int socket_fd) {
    data_->socket_fd_ = socket_fd;
  }
  void set_gender(const int32 gender) {
    data_->gender_ = gender;
  }
  void set_phone_num(const std::string& phone_num) {
    data_->phone_num_ = phone_num;
  }
  void set_nickname(const std::string& nickname) {
    data_->nickname_ = nickname;
  }
  void set_token(const std::string& token) {
    data_->token_ = token;
  }
  void set_head_url(const std::string& head_url) {
    data_->head_url_ = head_url;
  }

  void set_balance(const double balance) {
    data_->balance_ = balance;
  }

  void set_is_effective(const bool is_effective) {
    data_->is_effective_ = is_effective;
  }

  void set_send_last_time(const time_t send_last_time) {
    data_->send_last_time_ = send_last_time;
  }

  void set_recv_last_time(const time_t recv_last_time) {
    data_->recv_last_time_ = recv_last_time;
  }

  void add_send_error_count() {
    __sync_fetch_and_add(&data_->send_error_count_, 1);
  }

  void add_recv_error_count() {
    __sync_fetch_and_add(&data_->recv_error_count_, 1);
  }


  void ValueSerialization(base_logic::DictionaryValue* dict);

  class Data {
   public:
    Data()
        : refcount_(1),
          uid_(0),
          socket_fd_(0),
          gender_(0),
          type_(0),
          balance_(0.0),
          is_effective_(false),
          recv_last_time_(0),
          send_last_time_(0),
          recv_error_count_(0),
          send_error_count_(0){
    }

   public:
    int64 uid_;
    int socket_fd_;
    int32 gender_;
    int32 type_;
    int32 send_error_count_;
    int32 recv_error_count_;
    double balance_;
    bool is_effective_;
    time_t send_last_time_;
    time_t recv_last_time_;
    std::string phone_num_;
    std::string nickname_;
    std::string token_;
    std::string head_url_;
    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
   private:
    int refcount_;
  };
  Data* data_;
};

class TradesPosition {
 public:
  TradesPosition();
  TradesPosition(const TradesPosition& trades_positions);

  TradesPosition& operator =(const TradesPosition& trades_positions);

  ~TradesPosition() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool close_after(const TradesPosition& t_trades_position,
                          const TradesPosition& r_trades_position) {
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);

  bool check_buy_sell(double close_price) {
    int32 buy_sell = 0;
    data_->close_price_ = close_price;
    double difference = close_price - data_->open_price_;
    if(difference > 0 and data_->buy_sell_ == BUY_TYPE) /*平仓价大于建仓价，只有买入才算正确*/
      data_->result_ = true;
    else if (difference < 0 and data_->buy_sell_ == SELL_TYPE) /*平仓小于于建仓价，只有卖出才算正确*/
      data_->result_ = true;
    else
      data_->result_ = false;
    /*if (difference > 0)
      buy_sell = BUY_TYPE;
    else if (difference < 0)
      buy_sell = SELL_TYPE;
    if (difference == 0)
      data_->result_ = false;
    else
      data_->result_ = (buy_sell == data_->buy_sell_) ? true : false;*/
    return data_->result_;
  }

  void c_gross_profit() {
    if (data_->close_type_)
      data_->gross_profit_ = data_->open_cost_;
  }

  void create_position_id() {
    data_->position_id_ = base::SysRadom::GetInstance()->GetRandomID();
  }

  void set_uid(const int64 uid) {
    data_->uid_ = uid;
  }

  void set_position_id(const int64 position_id) {
    data_->position_id_ = position_id;
  }

  void set_code_id(const int32 code_id) {
    data_->code_id_ = code_id;
  }

  void set_buy_sell(const int32 buy_sell) {
    data_->buy_sell_ = buy_sell;
  }

  void set_close_type(const int32 close_type) {
    data_->close_type_ = close_type;
  }

  void set_is_deferred(const bool is_deferred) {
    data_->deferred_ = is_deferred;
  }

  void set_amount(const int64 amount) {
    data_->amount_ = amount;
  }

  void set_open_position_time(const int64 open_position_time) {
    data_->open_position_time_ = open_position_time;
  }

  void set_close_position_time(const int64 close_position_time) {
    data_->close_position_time_ = close_position_time;
  }

  void set_open_price(const double open_price) {
    data_->open_price_ = open_price;
  }

  void set_open_cost(const double open_cost) {
    data_->open_cost_ = open_cost;
  }

  void set_open_all_cost(const double open_all_cost) {
    data_->open_all_cost_ = open_all_cost;
  }

  void set_open_charge(const double open_charge) {
    data_->open_charge_ = open_charge;
  }

  void set_close_price(const double close_price) {
    data_->close_price_ = close_price;
  }

  void set_limit(const double limit) {
    data_->limit_ = limit;
  }

  void set_stop(const double stop) {
    data_->stop_ = stop;
  }

  void set_deferred(const double deferred) {
    data_->deferred_ = deferred;
  }

  void set_code(const std::string& code) {
    data_->code_ = code;
  }

  void set_symbol(const std::string& symbol) {
    data_->symbol_ = symbol;
  }

  void set_name(const std::string& name) {
    data_->name_ = name;
  }

  void set_result(const bool result) {
    data_->result_ = result;
  }

  void set_handle(const int32 handle) {
    data_->handle_ = handle;
  }

  void set_gross_profit(const double gross_profit) {
    data_->gross_profit_ = gross_profit;
  }

  void set_goods_key(const std::string& goods_key) {
    data_->goods_key_ = goods_key;
  }
  const int64 uid() const {
    return data_->uid_;
  }

  const int64 position_id() const {
    return data_->position_id_;
  }

  const int32 code_id() const {
    return data_->code_id_;
  }

  const int32 buy_sell() const {
    return data_->buy_sell_;
  }

  const int32 close_type() const {
    return data_->close_type_;
  }

  const bool is_deferred() const {
    return data_->is_deferred_;
  }

  const int64 amount() const {
    return data_->amount_;
  }

  const int32 handle() const {
    return data_->handle_;
  }
  const int64 open_position_time() const {
    return data_->open_position_time_;
  }

  const int64 close_position_time() const {
    return data_->close_position_time_;
  }

  const double open_price() const {
    return data_->open_price_;
  }

  const double open_cost() const {
    return data_->open_cost_;
  }

  const double open_all_cost() const {
    return data_->open_all_cost_;
  }

  const double open_charge() const {
    return data_->open_charge_;
  }

  const double close_price() const {
    return data_->close_price_;
  }

  const double limit() const {
    return data_->limit_;
  }

  const double stop() const {
    return data_->stop_;
  }

  const double gross_profit() const {
    return data_->gross_profit_;
  }

  const double deferred() const {
    return data_->deferred_;
  }

  const bool result() const {
    return data_->result_;
  }

  const std::string& code() const {
    return data_->code_;
  }

  const std::string& symbol() const {
    return data_->symbol_;
  }

  const std::string& name() const {
    return data_->name_;
  }

  const std::string& goods_key() const {
    return data_->goods_key_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          uid_(0),
          position_id_(0),
          code_id_(0),
          buy_sell_(0),
          close_type_(0),
          is_deferred_(false),
          result_(false),
          amount_(0),
          handle_(WAIT_HANDLE),
          open_position_time_(0),
          close_position_time_(0),
          gross_profit_(0.0),
          open_price_(0.0),
          open_cost_(0.0),
          open_all_cost_(0.0),
          open_charge_(0.0),
          close_price_(0.0),
          limit_(0.0),
          stop_(0.0),
          deferred_(0.0) {
    }

    static bool close_after(const Data* t_data, const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }
   public:
    int64 uid_;
    int64 position_id_;
    int32 code_id_;  //属于哪个商品
    int32 buy_sell_;  // 1,买 2,卖
    int32 close_type_;
    int32 handle_;
    bool is_deferred_;
    bool result_;
    int64 amount_;
    int64 open_position_time_;
    int64 close_position_time_;
    double gross_profit_;
    double open_price_;
    double open_cost_;
    double open_all_cost_;
    double open_charge_;
    double close_price_;
    double limit_;
    double stop_;
    double deferred_;
    std::string code_;
    std::string symbol_;
    std::string name_;
    std::string goods_key_;

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
   private:
    int refcount_;
  };
  Data* data_;
};

class Quotations {
 public:
  Quotations();

  Quotations(const Quotations& quotations);

  Quotations& operator =(const Quotations& quotations);

  ~Quotations() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool before(const Quotations& t_info, const Quotations& r_info) {
    return Data::before(t_info.data_, r_info.data_);
  }

  static bool after(const Quotations& t_info, const Quotations& r_info) {
    return Data::after(t_info.data_, r_info.data_);
  }

  static bool cmp(const Quotations& t_info, const Quotations& r_info) {
    return Data::after(t_info.data_, r_info.data_);
  }

  void set_platform_name(const std::string& platform) {
    if (data_)
      data_->platform_name_ = platform;
  }

  void set_symbol(const std::string& symbol) {
    if (data_)
      data_->symbol_ = symbol;
  }

  void set_exchange_name(const std::string& exchange_name) {
    if (data_)
      data_->exchange_name_ = exchange_name;
  }

  void set_current_price(const double current_price) {
    if (data_)
      data_->current_price_ = current_price;
  }

  void set_high_price(const double high_price) {
    if (data_)
      data_->high_price_ = high_price;
  }

  void set_low_price(const double low_price) {
    if (data_)
      data_->low_price_ = low_price;
  }

  void set_opening_today_price(const double opening_today_price) {
    if (data_)
      data_->opening_today_price_ = opening_today_price;
  }

  void set_closed_yesterday_price(const double closed_yesterday_price) {
    if (data_)
      data_->closed_yesterday_price_ = closed_yesterday_price;
  }

  void set_change(const double change) {
    if (data_)
      data_->change_ = change;
  }

  void set_pchg(const double pchg) {
    if (data_)
      data_->pchg_ = pchg;
  }

  void set_type(const int32 type) {
    if (data_)
      data_->type_ = type;
  }

  void set_current_unix_time(const int64 current_unix_time) {
    if (data_)
      data_->current_unix_time_ = current_unix_time;
  }

  const std::string& platform() const {
    if (data_)
      return data_->platform_name_;
  }

  const std::string& symbol() const {
    if (data_)
      return data_->symbol_;
  }

  const std::string& exchange_name() const {
    if (data_)
      return data_->exchange_name_;
  }
  const double current_price() const {
    if (data_)
      return data_->current_price_;
  }

  const double high_price() const {
    if (data_)
      return data_->high_price_;
  }

  const double low_price() const {
    if (data_)
      return data_->low_price_;
  }

  const double opening_today_price() const {
    if (data_)
      return data_->opening_today_price_;
  }

  const double closed_yesterday_price() const {
    if (data_)
      return data_->closed_yesterday_price_;
  }

  const double change() const {
    if (data_)
      return data_->change_;
  }

  const double pchg() const {
    if (data_)
      return data_->pchg_;
  }

  const int64 current_unix_time() const {
    if (data_)
      return data_->current_unix_time_;
  }

  const int32 type() const {
    if (data_)
      return data_->type_;
  }

  std::string ValueSerialize();
  void ValueDeserialize(std::string& str);

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          type_(0),
          current_price_(0.0),
          high_price_(0.0),
          low_price_(0.0),
          opening_today_price_(0.0),
          closed_yesterday_price_(0.0),
          change_(0.0),
          pchg_(0.0),
          current_unix_time_(0) {
    }

    ~Data() {
    }

   public:
    std::string platform_name_;
    std::string symbol_;
    std::string exchange_name_;
    int32 type_;  //1,股票 2，现货 3，期货
    double current_price_;
    double high_price_;
    double low_price_;
    double opening_today_price_;
    double closed_yesterday_price_;
    double change_;
    double pchg_;
    int64 current_unix_time_;

    static bool before(const Data* t_info, const Data* r_info) {
      if (t_info == NULL || r_info == NULL)
        return false;
      return t_info->current_unix_time_ <= r_info->current_unix_time_;
    }

    static bool after(const Data* t_info, const Data* r_info) {
      if (t_info == NULL || r_info == NULL)
        return false;
      return t_info->current_unix_time_ > r_info->current_unix_time_;
    }

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }
    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }

   private:
    int refcount_;
  };
  Data* data_;
};

}  // namespace quotations_logic

#endif /* QUOTATIONS_PUB_LOGIC_QUOTATIONS_INFOS_H_ */
