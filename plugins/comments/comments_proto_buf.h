//  Copyright (c) 2015-2015 The comments Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_COMMENTS_PROTO_BUF_H_
#define SWP_COMMENTS_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include "logic/logic_comm.h"

namespace comments_logic {

namespace net_other {
/*
class RealTime {
 public:
  RealTime()
      : change_(NULL),
        pchg_(NULL),
        opening_today_price_(NULL),
        closed_yesterday_price_(NULL),
        current_price_(NULL),
        current_unix_time_(NULL),
        high_price_(NULL),
        low_price_(NULL),
        type_(NULL),
        exchange_name_(NULL),
        platform_name_(NULL),
        symbol_(NULL) {
  }

  ~RealTime() {
    if (change_) {
      delete change_;
      change_ = NULL;
    }
    if (pchg_) {
      delete pchg_;
      pchg_ = NULL;
    }
    if (opening_today_price_) {
      delete opening_today_price_;
      opening_today_price_ = NULL;
    }
    if (closed_yesterday_price_) {
      delete closed_yesterday_price_;
      closed_yesterday_price_ = NULL;
    }
    if (current_price_) {
      delete current_price_;
      current_price_ = NULL;
    }
    if (current_unix_time_) {
      delete current_unix_time_;
      current_unix_time_ = NULL;
    }
    if (high_price_) {
      delete high_price_;
      high_price_ = NULL;
    }
    if (low_price_) {
      delete low_price_;
      low_price_ = NULL;
    }
    if (type_) {
      delete type_;
      type_ = NULL;
    }
    if (exchange_name_) {
      delete exchange_name_;
      exchange_name_ = NULL;
    }
    if (platform_name_) {
      delete platform_name_;
      platform_name_ = NULL;
    }
    if (symbol_) {
      delete symbol_;
      symbol_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_change(const double change) {
    change_ = new base_logic::FundamentalValue(change);
  }

  void set_pchg(const double pchg) {
    pchg_ = new base_logic::FundamentalValue(pchg);
  }

  void set_opening_today_price(const double opening_today_price) {
    opening_today_price_ = new base_logic::FundamentalValue(
        opening_today_price);
  }

  void set_closed_yesterday_price(const double closed_yesterday_price) {
    closed_yesterday_price_ = new base_logic::FundamentalValue(
        closed_yesterday_price);
  }

  void set_current_price(const double current_price) {
    current_price_ = new base_logic::FundamentalValue(current_price);
  }

  void set_current_unix_time(const int64 current_unix_time) {
    current_unix_time_ = new base_logic::FundamentalValue(current_unix_time);
  }

  void set_high_price(const double high_price) {
    high_price_ = new base_logic::FundamentalValue(high_price);
  }

  void set_low_price(const double low_price) {
    low_price_ = new base_logic::FundamentalValue(low_price);
  }

  void set_type(const int32 type) {
    type_ = new base_logic::FundamentalValue(type);
  }

  void set_exchange_name(const std::string& exchange_name) {
    exchange_name_ = new base_logic::StringValue(exchange_name);
  }

  void set_platform_name(const std::string& platform_name) {
    platform_name_ = new base_logic::StringValue(platform_name);
  }

  void set_symbol(const std::string& symbol) {
    symbol_ = new base_logic::StringValue(symbol);
  }

  const double change() const {
    double change = 0;
    change_->GetAsReal(&change);
    return change;
  }

  const double pchg() const {
    double pchg = 0;
    pchg_->GetAsReal(&pchg);
    return pchg;
  }

  const double opening_today_price() const {
    double opening_today_price = 0;
    opening_today_price_->GetAsReal(&opening_today_price);
    return opening_today_price;
  }

  const double closed_yesterday_price() const {
    double closed_yesterday_price = 0;
    closed_yesterday_price_->GetAsReal(&closed_yesterday_price);
    return closed_yesterday_price;
  }

  const double current_price() const {
    double current_price = 0;
    current_price_->GetAsReal(&current_price);
    return current_price;
  }

  const int64 current_unix_time() const {
    int64 current_unix_time = 0;
    current_unix_time_->GetAsBigInteger(&current_unix_time);
    return current_unix_time;
  }

  const double high_price() const {
    double high_price = 0;
    high_price_->GetAsReal(&high_price);
    return high_price;
  }

  const double low_price() const {
    double low_price = 0;
    low_price_->GetAsReal(&low_price);
    return low_price;
  }

  const int32 type() const {
    int32 type = 0;
    type_->GetAsInteger(&type);
    return type;
  }

  const std::string exchange_name() const {
    std::string exchange_name;
    exchange_name_->GetAsString(&exchange_name);
    return exchange_name;
  }

  const std::string platform_name() const {
    std::string platform_name;
    platform_name_->GetAsString(&platform_name);
    return platform_name;
  }

  const std::string symbol() const {
    std::string symbol;
    symbol_->GetAsString(&symbol);
    return symbol;
  }

 public:
  base_logic::FundamentalValue* change_;
  base_logic::FundamentalValue* pchg_;
  base_logic::FundamentalValue* opening_today_price_;
  base_logic::FundamentalValue* closed_yesterday_price_;
  base_logic::FundamentalValue* current_price_;
  base_logic::FundamentalValue* current_unix_time_;
  base_logic::FundamentalValue* high_price_;
  base_logic::FundamentalValue* low_price_;
  base_logic::FundamentalValue* type_;
  base_logic::StringValue* exchange_name_;
  base_logic::StringValue* platform_name_;
  base_logic::StringValue* symbol_;
};

*/
}

namespace net_request {

class FansComments{
 public:
  FansComments()
      : star_code_(NULL),
        fans_id_(NULL),
        nick_name_(NULL),
        comments_(NULL),
        value_(NULL),
        cms_time_(NULL),
        total_count_(NULL),
        head_url_(NULL) {

  }

  ~FansComments() {
/*
    if (star_code_) {delete star_code_;    star_code_ = NULL; }   
    if (fans_id_)   {delete fans_id_;      fans_id_ = NULL;    }   
    if (nick_name_) {delete nick_name_;    nick_name_ = NULL;    }   
    if (comments_)  {delete comments_;     comments_ = NULL;    }   
    if (head_url_)  {delete head_url_;     head_url_ = NULL;    }   
*/
    if (value_)  {delete value_;     value_ = NULL;    }   
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_star_code(const std::string& value) {
    star_code_ = new base_logic::StringValue(value);
  }
  void set_fans_id(const std::string& value) {
    fans_id_ = new base_logic::StringValue(value);
  }
  void set_nick_name(const std::string& value) {
    nick_name_ = new base_logic::StringValue(value);
  }
  void set_comments(const std::string& value) {
    comments_ = new base_logic::StringValue(value);
  }
  void set_head_url(const std::string& value) {
    head_url_ = new base_logic::StringValue(value);
  }
  void set_cms_time(const int64 value) {
    cms_time_ = new base_logic::FundamentalValue(value);
  }
  void set_total_count(const int64 value) {
    total_count_ = new base_logic::FundamentalValue(value);
  }

  const std::string star_code() const {
    std::string tmp;
    star_code_->GetAsString(&tmp);
    return tmp;
  }
  const std::string fans_id() const {
    std::string tmp;
    fans_id_->GetAsString(&tmp);
    return tmp;
  }

  const std::string nick_name() const {
    std::string tmp;
    nick_name_->GetAsString(&tmp);
    return tmp;
  }

  const std::string comments() const {
    std::string tmp;
    comments_->GetAsString(&tmp);
    return tmp;
  }

  const std::string head_url() const {
    std::string tmp;
    head_url_->GetAsString(&tmp);
    return tmp;
  }
  int64 cms_time() const {
    int64 tmp;
    cms_time_->GetAsBigInteger(&tmp);
    return tmp;
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (star_code_ != NULL)
      value_->Set(L"star_code", star_code_);
    if (fans_id_ != NULL)
      value_->Set(L"fans_id", fans_id_);
    if (nick_name_ != NULL)
      value_->Set(L"nick_name", nick_name_);

    if (comments_ != NULL)
      value_->Set(L"comments", comments_);

    if (head_url_ != NULL)
      value_->Set(L"head_url", head_url_);

    if (cms_time_ != NULL)
      value_->Set(L"cms_time", cms_time_);

    if (total_count_ != NULL)
      value_->Set(L"count", total_count_);

    return value_;
 }
 public:
  base_logic::StringValue* star_code_;
  base_logic::StringValue* fans_id_;
  base_logic::StringValue* nick_name_;
  base_logic::StringValue* comments_;
  base_logic::StringValue* head_url_; //头像

  base_logic::DictionaryValue* value_;
  base_logic::FundamentalValue* cms_time_; //评论时间

  base_logic::FundamentalValue* total_count_; //评论记录总数 返回客户端使用
};


class HisComments{
 public:
  HisComments()
      : id_(NULL),
        token_(NULL),
        start_(NULL),
        count_(NULL) {
  }

  ~HisComments() {
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
/*
  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }
*/

  void set_id(const std::string& value) {
    id_ = new base_logic::StringValue(value);
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
/*
  const int64 id() const {
    int64 id = 0;
    id_->GetAsBigInteger(&id);
    return id;
  }
*/

  const std::string id() const {
    std::string value;
    id_->GetAsString(&value);
    return value;
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

 public:
  //base_logic::FundamentalValue* id_;
  base_logic::StringValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* start_;
  base_logic::FundamentalValue* count_;
};
}

namespace net_reply {
typedef net_request::FansComments Comments;
class AllComments{
 public:
  AllComments()
      : deposits_info_(NULL),
        total_count_(NULL),
        value_(NULL) {
    deposits_info_ = new base_logic::ListValue;
  }

  ~AllComments() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    deposits_info_->Append((base_logic::Value*) (value));
  }
  void set_total_count(const int64 value) {
    total_count_ = new base_logic::FundamentalValue(value);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!deposits_info_->empty()) {
      value_->Set(L"commentsinfo", deposits_info_);
    } else {
      delete deposits_info_;
      deposits_info_ = NULL;
    }

    if (total_count_ != NULL)
      value_->Set(L"total_count", total_count_);

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
  base_logic::FundamentalValue* total_count_; //评论记录总数 
  base_logic::DictionaryValue* value_;
};

/*

*/
}

}
#endif
