//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_USER_PROTO_BUF_H_
#define SWP_USER_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include "logic/logic_comm.h"

namespace users_logic {

namespace net_request {

class RegisterVerfiycode {
 public:
  RegisterVerfiycode()
      : phone_(NULL) {
  }

  ~RegisterVerfiycode() {
    if (phone_) {
      delete phone_;
    }
  }

  void set_phone(const std::string& phone) {
    phone_ = new base_logic::StringValue(phone);
  }

  const std::string& phone() const {
    std::string phone;
    phone_->GetAsString(&phone);
    return phone;
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

 private:
  base_logic::StringValue* phone_;
};

class UserAccount {
 public:
  UserAccount()
      : uid_(NULL),
        token_(NULL) {
  }

  ~UserAccount() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  const int64 uid() const {
    int64 uid = 0;
    uid_->GetAsBigInteger(&uid);
    return uid;
  }

  const std::string token() const {
    std::string token;
    token_->GetAsString(&token);
    return token;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::StringValue* token_;
};

class RegisterAccount {
 public:
  RegisterAccount()
      : verify_code_(NULL),
        timestamp_(NULL),
        verify_token_(NULL),
        phone_num_(NULL),
        passwd_(NULL),
        user_type_(NULL),
	memberid_(NULL),
	agentid_(NULL),
	recommend_(NULL){
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

    if (memberid_) {
      delete memberid_;
      memberid_ = NULL;
    }
    if (agentid_) {
      delete agentid_;
      agentid_ = NULL;
    }
    if (recommend_) {
      delete recommend_;
      recommend_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

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
  void set_agentid(std::string& agentid) {
    agentid_ = new base_logic::StringValue(agentid);
  }
  void set_recommend(std::string& recommend) {
    recommend_ = new base_logic::StringValue(recommend);
  }
  void set_memberid(int64 memberid) {
    memberid_ = new base_logic::FundamentalValue(memberid);
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
  
  std::string agentid() {
    std::string agentid;
    if (agentid_)
      agentid_->GetAsString(&agentid);
    return agentid;
  }
  
  std::string recommend() {
    std::string recommend;
    if (recommend_)
      recommend_->GetAsString(&recommend);
    return recommend;
  }
  
  int64 memberid() {
    int64 memberid;
    if (memberid_)
      memberid_->GetAsBigInteger(&memberid);
    return memberid;
  }
  
 private:
  base_logic::StringValue* verify_code_;
  base_logic::FundamentalValue* timestamp_;
  base_logic::StringValue* verify_token_;
  base_logic::StringValue* user_type_;
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
  //modify bytw
  base_logic::FundamentalValue* memberid_;
  base_logic::StringValue* agentid_;
  base_logic::StringValue* recommend_;
};

class LoginAccount {
 public:
  LoginAccount()
      : phone_num_(NULL),
        passwd_(NULL) {
  }

  ~LoginAccount() {
    if (phone_num_) {
      delete phone_num_;
      phone_num_ = NULL;
    }
    if (passwd_) {
      delete passwd_;
      passwd_ = NULL;
    }

  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  void set_passwd(std::string& passwd) {
    passwd_ = new base_logic::StringValue(passwd);
  }

  const std::string phone_num() const {
    std::string phone_num;
    if (phone_num_)
      phone_num_->GetAsString(&phone_num);
    return phone_num;
  }

  const std::string passwd() const {
    std::string passwd;
    if (passwd_)
      passwd_->GetAsString(&passwd);
    return passwd;
  }

 private:
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;

};

typedef UserAccount CheckToken;

}

namespace net_reply {

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

class UserInfo {
 public:
  UserInfo()
      : uid_(NULL),
        balance_(NULL),
        phone_(NULL),
        type_(NULL),
        value_(NULL) {
  }

  ~UserInfo() {
    /*if (value_){
     delete value_;
     value_ = NULL;
     }*/
  }

  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_balance(const double balanace) {
    balance_ = new base_logic::FundamentalValue(balanace);
  }

  void set_phone(const std::string& phone) {
    phone_ = new base_logic::StringValue(phone);
  }

  void set_type(const int32 type) {
    type_ = new base_logic::FundamentalValue(type);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (uid_ != NULL)
      value_->Set(L"id", uid_);
    if (balance_ != NULL)
      value_->Set(L"balance", balance_);
    if (balance_ != NULL)
      value_->Set(L"phone", phone_);
    if (type_ != NULL)
      value_->Set(L"type", type_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* balance_;
  base_logic::FundamentalValue* type_;
  base_logic::StringValue* phone_;
  base_logic::DictionaryValue* value_;
};

class LoginAccount {
 public:
  LoginAccount()
      : token_(NULL),
        userinfo_(NULL),
        value_(NULL) {
  }

  ~LoginAccount() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_userinfo(base_logic::DictionaryValue* dict) {
    userinfo_ = dict;
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (token_ != NULL)
      value_->Set(L"token", token_);
    if (userinfo_ != NULL)
      value_->Set(L"userinfo", userinfo_);
    return value_;
  }

 private:
  base_logic::DictionaryValue* userinfo_;
  base_logic::StringValue* token_;
  base_logic::DictionaryValue* value_;
};

class RegisterAccount {
 public:
  RegisterAccount()
      : uid_(NULL),
        result_(NULL),
        value_(NULL) {
  }

  ~RegisterAccount() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_uid(const int64 uid) {
    uid_ = new base_logic::FundamentalValue(uid);
  }

  void set_result(const int32 result) {
    result_ = new base_logic::FundamentalValue(result);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (uid_ != NULL)
      value_->Set(L"uid", uid_);
    if (result_ != NULL)
      value_->Set(L"result", result_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* result_;
  base_logic::DictionaryValue* value_;
};

class RegisterVerfiycode {
 public:
  RegisterVerfiycode()
       : code_time_(NULL),
         token_(NULL),
         value_(NULL) {
   }

   ~RegisterVerfiycode() {
     if (value_) {
       delete value_;
       value_ = NULL;
     }
   }

   void set_code_time(const int64 code_time) {
     code_time_ = new base_logic::FundamentalValue(code_time);
   }

   void set_token(const std::string& token) {
     token_ = new base_logic::StringValue(token);
   }
 public:
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (code_time_ != NULL)
      value_->Set(L"timeStamp", code_time_);
    if (token_ != NULL)
      value_->Set(L"vToken", token_);
    return value_;
  }
 private:
  base_logic::FundamentalValue* code_time_;
  base_logic::StringValue* token_;
  base_logic::DictionaryValue* value_;
};
}

}

#endif
