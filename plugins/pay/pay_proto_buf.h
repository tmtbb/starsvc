//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_PAY_PAY_PROTO_BUF_
#define SWP_PAY_PAY_PROTO_BUF_

#include "basic/basic_util.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace pay_logic {

namespace net_request {

class WXPayServer {
 public:
  WXPayServer()
      : appid_(NULL),
        mch_id_(NULL),
        total_fee_(NULL),
        recharge_id_(NULL),
        pay_result_(NULL),
        transaction_id_(NULL),
        time_end_(NULL) {
  }

  ~WXPayServer() {
    if (appid_) {
      delete appid_;
      appid_ = NULL;
    }
    if (mch_id_) {
      delete mch_id_;
      mch_id_ = NULL;
    }
    if (total_fee_) {
      delete total_fee_;
      total_fee_ = NULL;
    }
    if (recharge_id_) {
      delete recharge_id_;
      recharge_id_ = NULL;
    }
    if (pay_result_) {
      delete pay_result_;
      pay_result_ = NULL;
    }
    if (transaction_id_) {
      delete transaction_id_;
      transaction_id_ = NULL;
    }
    if (time_end_) {
      delete time_end_;
      time_end_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_appid(std::string appid) {
    appid_ = new base_logic::StringValue(appid);
  }

  void set_mch_id(std::string mch_id) {
    mch_id_ = new base_logic::StringValue(mch_id);
  }

  void set_total_fee(int64 total_fee) {
    total_fee_ = new base_logic::FundamentalValue(total_fee);
  }

  void set_recharge_id(int64 recharge_id) {
    recharge_id_ = new base_logic::FundamentalValue(recharge_id);
  }

  void set_pay_result(int64 pay_result) {
    pay_result_ = new base_logic::FundamentalValue(pay_result);
  }

  void set_transaction_id(std::string transaction_id) {
    transaction_id_ = new base_logic::StringValue(transaction_id);
  }

  void set_time_end(std::string& time_end) {
    time_end_ = new base_logic::StringValue(time_end);
  }

  std::string appid() {
    std::string appid;
    if (appid_)
      appid_->GetAsString(&appid);
    return appid;
  }

  std::string mch_id() {
    std::string mch_id;
    if (mch_id_)
      mch_id_->GetAsString(&mch_id);
    return mch_id;
  }

  std::string transaction_id() const {
    std::string transaction_id;
    if (transaction_id_)
      transaction_id_->GetAsString(&transaction_id);
    return transaction_id;
  }

  int64 total_fee() {
    int64 total_fee;
    if (total_fee_)
      total_fee_->GetAsBigInteger(&total_fee);
    return total_fee;
  }

  int64 recharge_id() {
    int64 recharge_id;
    if (recharge_id_)
      recharge_id_->GetAsBigInteger(&recharge_id);
    return recharge_id;
  }

  int64 pay_result() {
    int64 pay_result;
    if (pay_result_)
      pay_result_->GetAsBigInteger(&pay_result);
    return pay_result;
  }

 private:
  base_logic::StringValue* appid_;
  base_logic::StringValue* mch_id_;
  base_logic::FundamentalValue* total_fee_;
  base_logic::FundamentalValue* recharge_id_;
  base_logic::FundamentalValue* pay_result_;  //1 - 支付成功
  base_logic::StringValue* transaction_id_;
  base_logic::StringValue* time_end_;
};

class WXPayClient {
 public:
  WXPayClient()
      : uid_(NULL),
        rid_(NULL),
        pay_result_(NULL) {
  }

  ~WXPayClient() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }

    if (rid_) {
      delete rid_;
      rid_ = NULL;
    }

    if (pay_result_) {
      delete pay_result_;
      pay_result_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_rid(const int64 rid) {
    rid_ = new base_logic::FundamentalValue(rid);
  }

  void set_pay_result(const int32 pay_result) {
    pay_result_ = new base_logic::FundamentalValue(pay_result);
  }

  const int32 uid() const {
    int64 uid = 0;
    uid_->GetAsBigInteger(&uid);
    return uid;
  }

  const int64 rid() const {
    int64 rid = 0;
    rid_->GetAsBigInteger(&rid);
    return rid;
  }

  const int32 pay_result() const {
    int32 pay_result = 0;
    pay_result_->GetAsInteger(&pay_result);
    return pay_result;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* rid_;
  base_logic::FundamentalValue* pay_result_;
};

class WXPayOrder {
 public:
  WXPayOrder()
      : uid_(NULL),
        title_(NULL),
        price_(NULL),
        pay_type_(NULL),
        open_id_(NULL){
  }

  ~WXPayOrder() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }
    if (title_) {
      delete title_;
      title_ = NULL;
    }
    if (price_) {
      delete price_;
      price_ = NULL;
    }

    if (pay_type_) {
      delete pay_type_;
      pay_type_ = NULL;
    }

    if (open_id_) {
      delete open_id_;
      open_id_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_uid(int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_title(std::string& title) {
    title_ = new base_logic::StringValue(title);
  }

  void set_price(double price) {
    price_ = new base_logic::FundamentalValue(price);
  }

  void set_pay_type(int32 pay_type) {
    pay_type_ = new base_logic::FundamentalValue(pay_type);
  }

  void set_open_id(const std::string& open_id) {
    open_id_ = new base_logic::StringValue(open_id);
  }

  int64 uid() {
    int64 uid;
    if (uid_)
      uid_->GetAsBigInteger(&uid);
    return uid;
  }

  std::string title() {
    std::string title;
    if (title_)
      title_->GetAsString(&title);
    return title;
  }

  double price() {
    double price;
    if (price_)
      price_->GetAsReal(&price);
    return price;
  }

  int32 pay_type() {
    int32 pay_type;
    pay_type_->GetAsInteger(&pay_type);
    return pay_type;
  }

  const std::string open_id() {
    std::string open_id;
    if (open_id_)
      open_id_->GetAsString(&open_id);
    return open_id;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::StringValue* title_;  //应用名-商品名 eg.交易支付
  base_logic::FundamentalValue* price_;  // 订单总价  单位 分
  base_logic::FundamentalValue* pay_type_;  //1为微信APP 0 微信公众号
  base_logic::StringValue* open_id_;//微信公众号支付必填
};

}

namespace net_reply {

class WXPayOrder {
 public:
  WXPayOrder()
      : appid_(NULL),
        partnerid_(NULL),
        prepayid_(NULL),
        packetage_(NULL),
        noncestr_(NULL),
        timestamp_(NULL),
        sign_(NULL),
        rid_(NULL),
        value_(NULL) {
  }

  ~WXPayOrder() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }
  void set_appid(const std::string& appid) {
    appid_ = new base_logic::StringValue(appid);
  }

  void set_partnerid(const std::string& partnerid) {
    partnerid_ = new base_logic::StringValue(partnerid);
  }

  void set_prepayid(const std::string& prepayid) {
    prepayid_ = new base_logic::StringValue(prepayid);
  }

  void set_packetage(const std::string& packetage) {
    packetage_ = new base_logic::StringValue(packetage);
  }

  void set_noncestr(const std::string& noncestr) {
    noncestr_ = new base_logic::StringValue(noncestr);
  }

  void set_timestamp(const std::string& timestamp) {
    timestamp_ = new base_logic::StringValue(timestamp);
  }

  void set_sign(const std::string& sign) {
    sign_ = new base_logic::StringValue(sign);
  }

  void set_rid(const int64 rid) {
    std::string str_rid = base::BasicUtil::StringUtil::Int64ToString(rid);
    rid_ = new base_logic::StringValue(str_rid);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (appid_ != NULL)
      value_->Set(L"appid", appid_);
    if (appid_ != NULL)
      value_->Set(L"partnerid", partnerid_);
    if (appid_ != NULL)
      value_->Set(L"prepayid", prepayid_);
    if (appid_ != NULL)
      value_->Set(L"package", packetage_);
    if (appid_ != NULL)
      value_->Set(L"noncestr", noncestr_);
    if (appid_ != NULL)
      value_->Set(L"timestamp", timestamp_);
    if (appid_ != NULL)
      value_->Set(L"sign", sign_);
    if (appid_ != NULL)
      value_->Set(L"rid", rid_);

    return value_;
  }

 private:
  base_logic::StringValue* appid_;
  base_logic::StringValue* partnerid_;
  base_logic::StringValue* prepayid_;
  base_logic::StringValue* packetage_;
  base_logic::StringValue* noncestr_;
  base_logic::StringValue* timestamp_;  //为兼容暂时使用string
  base_logic::StringValue* sign_;
  base_logic::StringValue* rid_;  //为兼容暂时使用string
  base_logic::DictionaryValue* value_;
};

class Balance {
 public:
  Balance()
      : balance_(NULL),
        value_(NULL) {
  }

  ~Balance() {
    if (value_ != NULL) {
      delete value_;
      value_ = NULL;
    }
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (balance_ != NULL)
      value_->Set("balance", balance_);
    return value_;
  }

  void set_balance(double balance) {
    balance_ = new base_logic::FundamentalValue(balance);
  }
 private:
  base_logic::FundamentalValue* balance_;
  base_logic::DictionaryValue* value_;
};

}

}
#endif
