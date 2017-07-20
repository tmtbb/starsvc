//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_STARSIDE_INFOS_H_
#define SWP_STARSIDE_INFOS_H_

#include <string>
#include <map>
#include <list>

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"

namespace starside_logic {


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

class TranscationStatistics {
 public:
  TranscationStatistics();
  TranscationStatistics(const TranscationStatistics& transcation_statistics);

  TranscationStatistics& operator =(const TranscationStatistics& transcation_statistics);

  ~TranscationStatistics() {
    if (data_ != NULL) {
      data_->Release();
    }
  }

  static bool close_after(const TranscationStatistics& t_trades_position, const TranscationStatistics& r_trades_position){
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);

  bool check_buy_sell(double close_price) {
    int32 buy_sell = 0;
    double difference = close_price - data_->open_price_;
    if (difference > 0)
      buy_sell = BUY_TYPE;
    else if (difference < 0)
      buy_sell = SELL_TYPE;
      /*
    if (difference == 0)
      data_->result_ = false;
    //else
      //data_->result_ = (buy_sell == data_->buy_sell_) ? true : false;
    //return data_->result_;
    */
    return true; 
  }


  void create_order_num() {
    data_->order_num_ = base::SysRadom::GetInstance()->GetRandomID();
  }

  void set_starcode(const int64 starcode) {
    data_->starcode_ = starcode;
  }

  void set_order_num(const int64 order_num) {
    data_->order_num_ = order_num;
  }

  void set_order_count(const int32 order_count) {
    data_->order_count_ = order_count;
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

  void set_price(const double price) {
    data_->price_ = price;
  }
  void set_max_price(const double price) {
    data_->max_price_ = price;
  }

  void set_min_price(const double price) {
    data_->min_price_ = price;
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
/*
  void set_result(const bool result) {
    data_->result_ = result;
  }
  */


  const int64 starcode() const {
    return data_->starcode_;
  }

  const int64 order_num() const {
    return data_->order_num_;
  }

  const int32 order_count() const {
    return data_->order_count_;
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





  const double price() const {
    return data_->price_;
  }
  const double max_price() const {
    return data_->max_price_;
  }
  const double min_price() const {
    return data_->min_price_;
  }



/*
  const bool result() const {
    return data_->result_;
  }
  */

  const std::string& code() const {
    return data_->code_;
  }

  const std::string& symbol() const {
    return data_->symbol_;
  }

  const std::string& name() const {
    return data_->name_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          starcode_(0),
          order_num_(0),
          order_count_(0),
          //result_(false),
          open_position_time_(0),
          close_position_time_(0),
          open_price_(0.0),
          max_price_(0.0),
          min_price_(0.0),
          price_(0.0) {
    }

    static bool close_after(const Data* t_data,
                      const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }

   public:
    int64 starcode_;            //明星code
    int64 order_num_;           //当天订单总时间数
    int32 order_count_;         //当天订单记录数

    //bool result_;
    int64 open_position_time_;
    double open_price_;
    double price_;
    double max_price_;
    double min_price_;
    std::string code_;

    std::string symbol_;
    std::string name_;
    int64 close_position_time_;

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

//明星粉丝预约对象
class StarMeetRelForFan {
 public:
  StarMeetRelForFan();
  StarMeetRelForFan(const StarMeetRelForFan& transcation_statistics);

  StarMeetRelForFan& operator =(const StarMeetRelForFan& transcation_statistics);

  ~StarMeetRelForFan() {
    if (data_ != NULL) {
      data_->Release();
    }
  }
/*
  static bool close_after(const StarMeetRelForFan& t_trades_position, const StarMeetRelForFan& r_trades_position){
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }
  */

  void ValueSerialization(base_logic::DictionaryValue* dict);

/*
  void create_order_num() {
    data_->meet_type_ = base::SysRadom::GetInstance()->GetRandomID();
  }
  */

  const int64 id() const {
    return data_->id_;
  }
  const int64 uid() const {
    return data_->uid_;
  }
  const int64 mid() const {
    return data_->mid_;
  }
  const int64 meet_type() const {
    return data_->meet_type_;
  }

  const std::string& code() const {
    return data_->code_;
  }
  const std::string& nickname() const {
    return data_->nickname_;
  }
  const std::string& headurl() const {
    return data_->headurl_;
  }
  const std::string& name() const {
    return data_->name_;
  }
  const std::string& meet_city() const {
    return data_->meet_city_; }
  const std::string& appoint_time() const {
    return data_->appoint_time_;
  }
  const std::string& order_time() const {
    return data_->order_time_;
  }
  const std::string& comment() const {
    return data_->comment_;
  }

  void set_meet_type(const int64 type) {
    data_->meet_type_ = type;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          uid_(0),
          id_(0),
          meet_type_(0),
          mid_(0)
          {
    }
/*
    static bool close_after(const Data* t_data,
                      const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }
    */

   public:
    int64 id_;            //数据库自增id
    std::string code_;

    int64 uid_;            //粉丝id
    std::string nickname_;
    std::string headurl_;

    int64 mid_; //
    std::string name_;

    int64 meet_type_;           //
    std::string meet_city_;

    std::string appoint_time_;
    std::string order_time_;
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
//_____ServiceItem
class ServiceItem {
 public:
  ServiceItem();
  ServiceItem(const ServiceItem& transcation_statistics);

  ServiceItem& operator =(const ServiceItem& transcation_statistics);

  ~ServiceItem() {
    if (data_ != NULL) {
      data_->Release();
    }
  }
/*
  static bool close_after(const ServiceItem& t_trades_position, const ServiceItem& r_trades_position){
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }
  */

  void ValueSerialization(base_logic::DictionaryValue* dict);


  const int64 mid() const {
    return data_->mid_;
  }

  const std::string& name() const {
    return data_->name_;
  }
  const double price() const {
    return data_->price_;
  }
  const std::string& showpic_url() const {
    return data_->showpic_url_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          price_(0.0),
          mid_(0)
          {
    }
/*
    static bool close_after(const Data* t_data,
                      const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }
    */

   public:

    int64 mid_; //
    std::string name_;
    double price_;
    std::string showpic_url_;

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


class StarOwnService {
 public:
  StarOwnService();
  StarOwnService(const StarOwnService& transcation_statistics);

  StarOwnService& operator =(const StarOwnService& transcation_statistics);

  ~StarOwnService() {
    if (data_ != NULL) {
      data_->Release();
    }
  }
/*
  static bool close_after(const StarOwnService& t_trades_position, const StarOwnService& r_trades_position){
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }
  */

  void ValueSerialization(base_logic::DictionaryValue* dict);


  const int64 mid() const {
    return data_->mid_;
  }

  const std::string& name() const {
    return data_->name_;
  }
  const double price() const {
    return data_->price_;
  }
  const std::string& starcode() const {
    return data_->starcode_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          price_(0.0),
          mid_(0)
          {
    }
/*
    static bool close_after(const Data* t_data,
                      const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }
    */

   public:

    int64 mid_; //
    std::string name_;
    double price_;
    std::string starcode_;

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


class TOwnStarUser {
public:
    TOwnStarUser();
    TOwnStarUser(const TOwnStarUser& starinfo);
    TOwnStarUser& operator =(const TOwnStarUser& starinfo);

    ~TOwnStarUser() {
        if (data_ != NULL) {
            data_->Release();
        }
    }
    const int64 uid() const {
        return data_->uid_;
    }
    const int64 ownseconds() const {
        return data_->ownseconds_;
    }
    const int32 appoint() const {
        return data_->appoint_;
    }

    const std::string& user_nickname() const {
        return data_->user_nickname_ ;
    }
    const std::string& starcode() const {
        return data_->starcode_;
    }
    const std::string& faccid() const {
        return data_->faccid_;
    }
    const std::string& headurl() const {
        return data_->head_url_;
    }
//----
    void set_uid(const int64 value) {
        data_->uid_ = value;
    }
    void set_ownseconds(const int64 value) {
        data_->ownseconds_ = value;
    }
    void set_appoint(const int32 value) {
        data_->appoint_ = value;
    }

    void set_starcode(const std::string& code) {
        data_->starcode_ = code;
    }
    void set_user_nickname(const std::string& value) {
        data_->user_nickname_ = value;
    }
    void set_faccid(const std::string& value) {
        data_->faccid_ = value;
    }
    void set_headurl(const std::string& value) {
        data_->head_url_ = value;
    }

    void ValueSerialization(base_logic::DictionaryValue* dict);

    class Data {
    public:
        Data()
            : uid_(1),
              ownseconds_(0),
              appoint_(0),
              refcount_(1) {
        }

    public:
        int64 uid_;
        int64 ownseconds_;
        int32 appoint_; //预约
        std::string starcode_; //明星代码
        std::string user_nickname_; //用户昵称
        std::string faccid_;
        std::string head_url_;//用户头像
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
}  // namespace starside_logic

#endif /* SWP_STARSIDE_INFOS_H_ */
