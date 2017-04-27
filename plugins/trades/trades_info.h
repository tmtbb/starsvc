//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_TRADES_INFOS_H_
#define SWP_TRADES_INFOS_H_

#include <string>
#include <map>
#include <list>

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"

namespace trades_logic {

enum BSTYPE {
  BUY_TYPE = 1,
  SELL_TYPE = 2
};

enum TIMETYPE {
  ONE_MINUTE = 60,
  FIVE_MINUTE = 300,
  HALF_HOUR = 1800,
  ONE_HOUR = 3600
};

enum CLOSETYPE {
  ORDINARY_TYPE = 1,
  AUTO_TYPE = 2,
  LOSE_TYPE = 3,
  CLOSED_TYPE = 4,
  FORCED_TYPE = 5,
  TIMER_TYPE = 6,
};

class TimeTask {
 public:
  TimeTask();
  TimeTask(const TimeTask& time_task);

  TimeTask& operator =(const TimeTask& time_task);

  ~TimeTask() {
    if (data_ != NULL)
      data_->Release();
  }

  static bool cmp(const trades_logic::TimeTask& t_time_task,
                  const trades_logic::TimeTask& r_time_task) {
    //return t_time_task.end_time() <= t_time_task.end_time();
    return Data::cmp(t_time_task.data_, r_time_task.data_);
  }

  void set_id(const int64 id) {
    data_->id_ = id;
  }

  void set_start_time(const int64 start_time) {
    data_->start_time_ = start_time;
  }

  void set_end_time(const int64 end_time) {
    data_->end_time_ = end_time;
  }

  const int64 id() const {
    return data_->id_;
  }

  const int64 start_time() const {
    return data_->start_time_;
  }

  const int64 end_time() const {
    return data_->end_time_;
  }

  void set_time(const int64 start_time, const int64 timing_time) {
    data_->start_time_ = start_time;
    data_->end_time_ = data_->start_time_ + timing_time;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          id_(0),
          start_time_(0),
          end_time_(0) {
    }

   public:
    int64 id_;
    int64 start_time_;
    int64 end_time_;

    static bool cmp(const Data* t_data,
                      const Data* r_data) {
      return t_data->end_time_ < r_data->end_time_;
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

class FlightInfo {
 public:
 public:
  FlightInfo();
  FlightInfo(const FlightInfo& flight_info);

  FlightInfo& operator =(const FlightInfo& flight_info);

  ~FlightInfo() {
    if (data_ != NULL)
      data_->Release();
  }


  void ValueSerialization(base_logic::DictionaryValue* dict);

  void set_id(const int64 id){
    data_->id_ = id;
  }

  void set_gid(const int64 gid) {
    data_->gid_ = gid;
  }

  void set_max(const int32 max){
    data_->max_ = max;
  }

  void set_min(const int32 min) {
    data_->min_ = min;
  }

  const int64 id() const {
    return data_->id_;
  }

  const int64 gid() const {
    return data_->gid_;
  }

  const int32 max() const {
    return data_->max_;
  }

  const int32 min() const {
    return data_->min_;
  }

  const std::string& name() const {
    return data_->name_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          id_(0),
          gid_(0),
          max_(0),
          min_(0){
    }

   public:
    int64 id_;
    int64 gid_;
    int32 max_;
    int32 min_;
    std::string name_;


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


/*
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

  void ValueSerialization(base_logic::DictionaryValue* dict);

  bool check_buy_sell(double close_price) {
    int32 buy_sell = 0;
    data_->close_price_ = close_price;
    double difference = close_price - data_->open_price_;
    if (difference > 0)
      buy_sell = BUY_TYPE;
    else if (difference < 0)
      buy_sell = SELL_TYPE;
    if (difference == 0)
      data_->result_ = false;
    else
      data_->result_ = (buy_sell == data_->buy_sell_) ? true : false;
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

   public:
    int64 uid_;
    int64 position_id_;
    int32 code_id_;  //属于哪个商品
    int32 buy_sell_;  // 1,买 2,卖
    int32 close_type_;
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
*/

class GoodsInfo {
 public:
  GoodsInfo();

  GoodsInfo(const GoodsInfo& goods_info);

  GoodsInfo& operator =(const GoodsInfo& goods_info);

  ~GoodsInfo() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool before(const GoodsInfo& t_goodsinfo, const GoodsInfo& r_goodsinfo){
    return Data::before(t_goodsinfo.data_, r_goodsinfo.data_);
  }

  const int32 id() const {
    return data_->id_;
  }

  const int32 platform_id() const {
    return data_->plaform_id_;
  }

  const std::string& name() const {
    return data_->name_;
  }

  const std::string& code() const {
    return data_->code_;
  }

  const std::string& symbol() const {
    return data_->symbol_;
  }

  const std::string& unit() const {
    return data_->unit_;
  }

  const double amount() const {
    return data_->amount_;
  }

  const double profit() const {
    return data_->profit_;
  }

  const double deposit() const {
    return data_->deposit_;
  }

  const double close() const {
    return data_->close_;
  }

  const double open() const {
    return data_->open_;
  }

  const double deferred() const {
    return data_->deferred_;
  }

  const int64 max() const {
    return data_->max_;
  }

  const int64 min() const {
    return data_->min_;
  }

  const std::string& exchange_name() const {
    return data_->exchange_name_;
  }

  const std::string& platform_name() const {
    return data_->platform_name_;
  }

  const std::string& show_name() const {
    return data_->show_name_;
  }

  const std::string show_symbol() const {
    return data_->show_symbol_;
  }

  const int8 status() const {
    return data_->status_;
  }

  const int8 sort() const {
    return data_->sort_;
  }

  const int64 interval() const {
    return data_->interval_;
  }

  void set_id(const int32 id) {
    data_->id_ = id;
  }

  void set_platform_id(const int32 platform_id) {
    data_->plaform_id_ = platform_id;
  }

  void set_name(const std::string& name) {
    data_->name_ = name;
  }

  void set_code(const std::string& code) {
    data_->code_ = code;
  }

  void set_symbol(const std::string& symbol) {
    data_->symbol_ = symbol;
  }

  void set_unit(const std::string& unit) {
    data_->unit_ = unit;
  }

  void set_amount(const double amount) {
    data_->amount_ = amount;
  }

  void set_profit(const double profit) {
    data_->profit_ = profit;
  }

  void set_deposit(const double deposit) {
    data_->deposit_ = deposit;
  }

  void set_close(const double close) {
    data_->close_ = close;
  }

  void set_open(const double open) {
    data_->open_ = open;
  }

  void set_deferred(const double deferred) {
    data_->deferred_ = deferred;
  }

  void set_max(const int64 max) {
    data_->max_ = max;
  }

  void set_min(const int64 min) {
    data_->min_ = min;
  }

  void set_exchange_name(const std::string& exchange_name) {
    data_->exchange_name_ = exchange_name;
  }

  void set_platform_name(const std::string& platform_name) {
    data_->platform_name_ = platform_name;
  }

  void set_show_name(const std::string& show_name) {
    data_->show_name_ = show_name;
  }

  void set_show_symbol(const std::string& show_symbol) {
    data_->show_symbol_ = show_symbol;
  }

  void set_sort(const int8 sort) {
    data_->sort_ = sort;
  }

  void set_status(const int8 status) {
    data_->status_ = status;
  }

  void set_interval(const int64 interval) {
    data_->interval_ = interval;
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);
 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          id_(0),
          plaform_id_(0),
          interval_(0),
          amount_(0.0),
          profit_(0.0),
          deposit_(0.0),
          close_(0.0),
          open_(0.0),
          min_(0),
          max_(0),
          deferred_(0.0),
          sort_(0),
          status_(0) {
    }

    ~Data() {
    }

   public:
    int32 id_;
    int32 plaform_id_;
    int8 sort_;
    int8 status_;
    std::string name_;
    std::string code_;
    std::string symbol_;
    std::string unit_;
    double amount_;
    double profit_;
    double deposit_;
    double close_;
    double open_;
    double deferred_;
    int64 max_;
    int64 min_;
    int64 interval_;
    std::string exchange_name_;
    std::string platform_name_;
    std::string show_symbol_;
    std::string show_name_;

    static bool before(const Data* t_data,
                      const Data* r_data) {
      return t_data->id_ < r_data->id_;
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
