// Copyright (c) 2016 The tourism Authors. All rights reserved.
// login_proto.h
// Created on: 2016年8月12日.
// Author: Paco.
#ifndef PLUGINS_LOGIN_LOGIN_PROTO_H_
#define PLUGINS_LOGIN_LOGIN_PROTO_H_

#include "net/proto_buf.h"
namespace login {

namespace net_request {

class Heartbeat {
 public:
  Heartbeat()
      : uid_(NULL) {
  }
  ~Heartbeat() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }
  }
  int32 set_http_packet(base_logic::DictionaryValue* value);
  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }
  const int64 uid() {
    int64 uid = 0;
    if (uid_)
      uid_->GetAsBigInteger(&uid);
    return uid;
  }
 private:
  base_logic::FundamentalValue* uid_;
};
class RegisterAccount {
 public:
  RegisterAccount()
      : verify_code_(NULL),
        timestamp_(NULL),
        verify_token_(NULL),
        phone_num_(NULL),
        passwd_(NULL),
        user_type_(NULL) {
  }

  ~RegisterAccount() {
    if (verify_code_) {
      delete verify_code_;
      verify_code_ = NULL;
    }
    if (timestamp_) {
      delete timestamp_;
      timestamp_ = NULL;
    }
    if (verify_token_) {
      delete verify_token_;
      verify_token_ = NULL;
    }
    if (phone_num_) {
      delete phone_num_;
      phone_num_ = NULL;
    }
    if (passwd_) {
      delete passwd_;
      passwd_ = NULL;
    }
    if (user_type_) {
      delete user_type_;
      user_type_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_verify_code(std::string& verify_code) {
    verify_code_ = new base_logic::StringValue(verify_code);
  }

  void set_timestamp(int64 timestamp) {
    timestamp_ = new base_logic::FundamentalValue(timestamp);
  }

  void set_verify_token(std::string& verify_token) {
    verify_token_ = new base_logic::StringValue(verify_token);
  }

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  void set_passwd(std::string& passwd) {
    passwd_ = new base_logic::StringValue(passwd);
  }

  std::string verify_code() {
    std::string verify_code;
    if (verify_code_)
      verify_code_->GetAsString(&verify_code);
    return verify_code;
  }

  int64 timestamp() {
    int64 timestamp;
    if (timestamp_)
      timestamp_->GetAsBigInteger(&timestamp);
    return timestamp;
  }

  std::string verify_token() {
    std::string verify_token;
    if (verify_token_)
      verify_token_->GetAsString(&verify_token);
    return verify_token;
  }

  std::string phone_num() {
    std::string phone_num;
    if (phone_num_)
      phone_num_->GetAsString(&phone_num);
    return phone_num;
  }

  std::string passwd() {
    std::string passwd;
    if (passwd_)
      passwd_->GetAsString(&passwd);
    return passwd;
  }

  int64 user_type() {
    int64 user_type;
    if (user_type_)
      user_type_->GetAsBigInteger(&user_type);
    return user_type;
  }

 private:
  base_logic::StringValue* verify_code_;
  base_logic::FundamentalValue* timestamp_;
  base_logic::StringValue* verify_token_;
  base_logic::StringValue* user_type_;
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
};

//recv
class UserLogin {
 public:
  UserLogin()
      : phone_num_(NULL),
        passwd_(NULL),
        token_(NULL) {
  }

  ~UserLogin() {
    if (phone_num_) {
      delete phone_num_;
      phone_num_ = NULL;
    }
    if (passwd_) {
      delete passwd_;
      passwd_ = NULL;
    }
    if (token_) {
      delete token_;
      token_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  void set_passwd(std::string& passwd) {
    passwd_ = new base_logic::StringValue(passwd);
  }

  void set_token(std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  std::string phone_num() {
    std::string phone_num;
    if (phone_num_)
      phone_num_->GetAsString(&phone_num);
    return phone_num;
  }

  std::string passwd() {
    std::string passwd;
    if (passwd_)
      passwd_->GetAsString(&passwd);
    return passwd;
  }

  std::string token() {
    std::string token;
    if (token_)
      token_->GetAsString(&token);
    return token;
  }

 private:
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
  base_logic::StringValue* token_;
};

/*class Heartbeat {
 public:
 Heartbeat(PacketHead packet);

 int64 uid() { return uid_; }
 private:
 int64 uid_;
 };*/

class ChangePasswd {
 public:
  enum PasswdType {
    PASSWD_LOGIN = 0,  //登录
    PASSWD_TRADE,  //交易 提现
  };

  ChangePasswd()
      : type_(NULL),
        phone_num_(NULL),
        verify_code_(NULL),
        timestamp_(NULL),
        verify_token_(NULL),
        passwd_(NULL) {
  }

  ~ChangePasswd() {
    if (type_) {
      delete type_;
      type_ = NULL;
    }
    if (phone_num_) {
      delete phone_num_;
      phone_num_ = NULL;
    }
    if (verify_code_) {
      delete verify_code_;
      verify_code_ = NULL;
    }
    if (timestamp_) {
      delete timestamp_;
      timestamp_ = NULL;
    }
    if (verify_token_) {
      delete verify_token_;
      verify_token_ = NULL;
    }
    if (passwd_) {
      delete passwd_;
      passwd_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_type(int64 type) {
    type_ = new base_logic::FundamentalValue(type);
  }

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  void set_verify_code(std::string& verify_code) {
    verify_code_ = new base_logic::StringValue(verify_code);
  }

  void set_timestamp(int64 timestamp) {
    timestamp_ = new base_logic::FundamentalValue(timestamp);
  }

  void set_verify_token(std::string& verify_token) {
    verify_token_ = new base_logic::StringValue(verify_token);
  }

  void set_passwd(std::string& passwd) {
    passwd_ = new base_logic::StringValue(passwd);
  }

  int64 type() {
    int64 type;
    if (type_)
      type_->GetAsBigInteger(&type);
    return type;
  }

  std::string phone_num() {
    std::string phone_num;
    if (phone_num_)
      phone_num_->GetAsString(&phone_num);
    return phone_num;
  }

  std::string verify_code() {
    std::string verify_code;
    if (verify_code_)
      verify_code_->GetAsString(&verify_code);
    return verify_code;
  }
  ;

  int64 timestamp() {
    int64 timestamp;
    if (timestamp_)
      timestamp_->GetAsBigInteger(&timestamp);
    return timestamp;
  }

  std::string verify_token() {
    std::string verify_token;
    if (verify_token_)
      verify_token_->GetAsString(&verify_token);
    return verify_token;
  }

  std::string passwd() {
    std::string passwd;
    if (passwd_)
      passwd_->GetAsString(&passwd);
    return passwd;
  }

 private:
  base_logic::FundamentalValue* type_;
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* verify_code_;
  base_logic::FundamentalValue* timestamp_;
  base_logic::StringValue* verify_token_;
  base_logic::StringValue* passwd_;
};

class ObtainVerifyCode {
 public:
  ObtainVerifyCode()
      : verify_type_(NULL),
        phone_num_(NULL) {
  }

  ~ObtainVerifyCode() {
    if (verify_type_) {
      delete verify_type_;
      verify_type_ = NULL;
    }
    if (phone_num_) {
      delete phone_num_;
      phone_num_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_verify_type(int64 verify_type) {
    verify_type_ = new base_logic::FundamentalValue(verify_type);
  }

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  int64 verify_type() {
    int64 verify_type;
    if (verify_type_)
      verify_type_->GetAsBigInteger(&verify_type);
    return verify_type;
  }

  std::string phone_num() {
    std::string phone_num;
    if (phone_num_)
      phone_num_->GetAsString(&phone_num);
    return phone_num;
  }

 private:
  base_logic::FundamentalValue* verify_type_;
  base_logic::StringValue* phone_num_;
};

class ImproveData {
 public:
  ImproveData()
      : uid_(NULL),
        gender_(NULL),
        nickname_(NULL),
        head_url_(NULL),
        address_(NULL),
        longitude_(NULL),
        latitude_(NULL) {
  }

  ~ImproveData() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }
    if (gender_) {
      delete gender_;
      gender_ = NULL;
    }
    if (nickname_) {
      delete nickname_;
      nickname_ = NULL;
    }
    if (head_url_) {
      delete head_url_;
      head_url_ = NULL;
    }
    if (address_) {
      delete address_;
      address_ = NULL;
    }
    if (longitude_) {
      delete longitude_;
      longitude_ = NULL;
    }
    if (latitude_) {
      delete latitude_;
      latitude_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_uid(int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_gender(int64 gender) {
    gender_ = new base_logic::FundamentalValue(gender);
  }

  void set_nickname(std::string& nickname) {
    nickname_ = new base_logic::StringValue(nickname);
  }

  void set_head_url(std::string& head_url) {
    head_url_ = new base_logic::StringValue(head_url);
  }

  void set_address(std::string& address) {
    address_ = new base_logic::StringValue(address);
  }

  void set_longitude(double longitude) {
    longitude_ = new base_logic::FundamentalValue(longitude);
  }

  void set_latitude(double latitude) {
    latitude_ = new base_logic::FundamentalValue(latitude);
  }

  int64 uid() {
    int64 uid;
    if (uid_)
      uid_->GetAsBigInteger(&uid);
    return uid;
  }

  int64 gender() {
    int64 gender;
    if (gender_)
      gender_->GetAsBigInteger(&gender);
    return gender;
  }

  std::string nickname() {
    std::string nickname;
    if (nickname_)
      nickname_->GetAsString(&nickname);
    return nickname;
  }

  std::string head_url() {
    std::string head_url;
    if (head_url_)
      head_url_->GetAsString(&head_url);
    return head_url;
  }

  std::string address() {
    std::string address;
    if (address_)
      address_->GetAsString(&address);
    return address;
  }

  double longitude() {
    double longitude;
    if (longitude_)
      longitude_->GetAsReal(&longitude);
    return longitude;
  }

  double latitude() {
    double latitude;
    if (latitude_)
      latitude_->GetAsReal(&latitude);
    return latitude;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* gender_;
  base_logic::StringValue* nickname_;
  base_logic::StringValue* head_url_;
  base_logic::StringValue* address_;
  base_logic::FundamentalValue* longitude_;
  base_logic::FundamentalValue* latitude_;
};

class UserDetail {
 public:
  UserDetail()
      : uid_str_(NULL) {
  }

  ~UserDetail() {
    if (uid_str_) {
      delete uid_str_;
      uid_str_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_uid_str(std::string& uid_str) {
    uid_str_ = new base_logic::StringValue(uid_str);
  }

  std::string uid_str() {
    std::string uid_str;
    if (uid_str_)
      uid_str_->GetAsString(&uid_str);
    return uid_str;
  }

 private:
  base_logic::StringValue* uid_str_;  // "1,2,3,4"
};

}  // namespace net_request

}  // namespace login

#endif  // PLUGINS_LOGIN_LOGIN_PROTO_H_
