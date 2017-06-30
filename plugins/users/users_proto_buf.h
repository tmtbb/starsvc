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
    if (token_) {
      delete token_;
      token_ = NULL;
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
	member_id_(NULL),
	agentid_(NULL),
	sub_agentid_(NULL),
	recommend_(NULL),
	device_id_(NULL){
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

    if (member_id_) {
      delete member_id_;
      member_id_ = NULL;
    }
    if (agentid_) {
      delete agentid_;
      agentid_ = NULL;
    }
    if (sub_agentid_) {
      delete sub_agentid_;
      sub_agentid_ = NULL;
    }
    if (recommend_) {
      delete recommend_;
      recommend_ = NULL;
    }
    if (device_id_) {
      delete device_id_;
      device_id_= NULL;
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
  void set_device_id(std::string& device_id) {
    device_id_ = new base_logic::StringValue(device_id);
  }
  void set_member_id(std::string& value) {
    member_id_ = new base_logic::StringValue(value);
  }
  void set_sub_agentid(std::string& sub_agentid) {
    sub_agentid_ = new base_logic::StringValue(sub_agentid);
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
  std::string device_id() {
    std::string device_id;
    if (device_id_)
      device_id_->GetAsString(&device_id);
    return device_id;
  }
  
  std::string recommend() {
    std::string recommend;
    if (recommend_)
      recommend_->GetAsString(&recommend);
    return recommend;
  }
  
  std::string member_id() {
    std::string member_id;
    if (member_id_)
      member_id_->GetAsString(&member_id);
    return member_id;
  }
 private:
  base_logic::StringValue* verify_code_;
  base_logic::FundamentalValue* timestamp_;
  base_logic::StringValue* verify_token_;
  base_logic::StringValue* user_type_;
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
  //modify bytw
 // base_logic::FundamentalValue* member_id_;
  base_logic::StringValue* member_id_;
  base_logic::StringValue* agentid_;
  base_logic::StringValue* sub_agentid_;
  base_logic::StringValue* recommend_;
  base_logic::StringValue* device_id_;
};

class LoginAccount {
 public:
  LoginAccount()
      : phone_num_(NULL),
        passwd_(NULL),
	device_id_(NULL){
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
    if (device_id_) {
      delete device_id_;
      device_id_= NULL;
    }

  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_phone_num(std::string& phone_num) {
    phone_num_ = new base_logic::StringValue(phone_num);
  }

  void set_passwd(std::string& passwd) {
    passwd_ = new base_logic::StringValue(passwd);
  }
  void set_device_id(std::string& device_id) {
    device_id_ = new base_logic::StringValue(device_id);
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
  const std::string device_id() const {
    std::string device_id;
    if (device_id_)
      device_id_->GetAsString(&device_id);
    return device_id;
  }

 private:
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
  base_logic::StringValue* device_id_;

};

class Certification {
 public:
  Certification()
      : uid_(NULL),
        realname_(NULL),
	id_card_(NULL){
  }

  ~Certification() {
    if (uid_) {
      delete uid_;
      uid_ = NULL;
    }
    if (realname_) {
      delete realname_;
      realname_ = NULL;
    }
    if (id_card_) {
      delete id_card_;
      id_card_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_uid(int64 id) {
    uid_ = new base_logic::FundamentalValue(id);
  }

  void set_realname(std::string& value) {
    realname_ = new base_logic::StringValue(value);
  }
  void set_id_card(std::string& value) {
    id_card_ = new base_logic::StringValue(value);
  }

  int64 uid() const {
    int64 uid;
    uid_->GetAsBigInteger(&uid);
    return uid;
  }

  std::string realname() const {
    std::string tmp;
    if (realname_)
      realname_->GetAsString(&tmp);
    return tmp;
  }

  std::string id_card() const {
    std::string tmp;
    if (id_card_)
      id_card_->GetAsString(&tmp);
    return tmp;
  }

 private:
  base_logic::FundamentalValue* uid_;    //用户id
  base_logic::StringValue* realname_;
  base_logic::StringValue* id_card_;

};

class LoginWiXin {
 public:
  LoginWiXin()
      : open_id_(NULL),
	device_id_(NULL){
  }

  ~LoginWiXin() {
    if (open_id_) {
      delete open_id_;
      open_id_ = NULL;
    }

    if (device_id_) {
      delete device_id_;
      device_id_= NULL;
    }

  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_open_id(std::string& open_id) {
    open_id_ = new base_logic::StringValue(open_id);
  }

  void set_device_id(std::string& device_id) {
    device_id_ = new base_logic::StringValue(device_id);
  }

  const std::string open_id() const {
    std::string open_id;
    if (open_id_)
      open_id_->GetAsString(&open_id);
    return open_id;
  }

  const std::string device_id() const {
    std::string device_id;
    if (device_id_)
      device_id_->GetAsString(&device_id);
    return device_id;
  }

 private:
  base_logic::StringValue* open_id_;
  base_logic::StringValue* device_id_;

};


class WXBindAccount {
 public:
  WXBindAccount()
      : verify_code_(NULL),
        timestamp_(NULL),
        verify_token_(NULL),
        phone_num_(NULL),
        passwd_(NULL),
	openid_(NULL),
	nick_name_(NULL),
	head_url_(NULL),
	member_id_(NULL),
	agentid_(NULL),
	sub_agentid_(NULL),
	recommend_(NULL),
	device_id_(NULL){ 
  }

  ~WXBindAccount() {
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

    if (openid_) {
      delete openid_;
      openid_ = NULL;
    }
    if (nick_name_) {
      delete nick_name_;
      nick_name_ = NULL;
    }
    if (head_url_) {
      delete head_url_;
      head_url_ = NULL;
    }

    if (member_id_) {
      delete member_id_;
      member_id_ = NULL;
    }
    if (agentid_) {
      delete agentid_;
      agentid_ = NULL;
    }
    if (sub_agentid_) {
      delete sub_agentid_;
      sub_agentid_ = NULL;
    }
    if (recommend_) {
      delete recommend_;
      recommend_ = NULL;
    }
    if (device_id_) {
      delete device_id_;
      device_id_= NULL;
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

  void set_openid(std::string& openid) {
    openid_ = new base_logic::StringValue(openid);
  }
  void set_nick_name(std::string& nick_name) {
    nick_name_ = new base_logic::StringValue(nick_name);
  }
  void set_head_url(std::string& head_url) {
    head_url_ = new base_logic::StringValue(head_url);
  }


  void set_agentid(std::string& agentid) {
    agentid_ = new base_logic::StringValue(agentid);
  }
  void set_sub_agentid(std::string& sub_agentid) {
    sub_agentid_ = new base_logic::StringValue(sub_agentid);
  }
  void set_recommend(std::string& recommend) {
    recommend_ = new base_logic::StringValue(recommend);
  }
  void set_device_id(std::string& device_id) {
    device_id_ = new base_logic::StringValue(device_id);
  }
  /*
  void set_member_id(int64 member_id) {
    member_id_ = new base_logic::FundamentalValue(member_id);
  }
  */

  void set_member_id(std::string& value) {
    member_id_ = new base_logic::StringValue(value);
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

  
  std::string openid() {
    std::string openid;
    if (openid_)
      openid_->GetAsString(&openid);
    return openid;
  }
  std::string nick_name() {
    std::string nick_name;
    if (nick_name_)
      nick_name_->GetAsString(&nick_name);
    return nick_name;
  }
  
  std::string head_url() {
    std::string head_url;
    if (head_url_)
      head_url_->GetAsString(&head_url);
    return head_url;
  }
  
  std::string agentid() {
    std::string agentid;
    if (agentid_)
      agentid_->GetAsString(&agentid);
    return agentid;
  }
  std::string sub_agentid() {
    std::string sub_agentid;
    if (sub_agentid_)
      sub_agentid_->GetAsString(&sub_agentid);
    return sub_agentid;
  }
  std::string device_id() {
    std::string device_id;
    if (device_id_)
      device_id_->GetAsString(&device_id);
    return device_id;
  }
  
  std::string recommend() {
    std::string recommend;
    if (recommend_)
      recommend_->GetAsString(&recommend);
    return recommend;
  }
  
  std::string member_id() {
    std::string member_id;
    if (member_id_)
      member_id_->GetAsString(&member_id);
    return member_id;
  }
  
 private:
  base_logic::StringValue* verify_code_;
  base_logic::FundamentalValue* timestamp_;
  base_logic::StringValue* verify_token_;
  //base_logic::StringValue* user_type_;
  base_logic::StringValue* phone_num_;
  base_logic::StringValue* passwd_;
  base_logic::StringValue* openid_;
  base_logic::StringValue* nick_name_;
  base_logic::StringValue* head_url_;
//
  //base_logic::FundamentalValue* member_id_;
  base_logic::StringValue* member_id_;
  base_logic::StringValue* agentid_;
  base_logic::StringValue* sub_agentid_;
  base_logic::StringValue* recommend_;
  base_logic::StringValue* device_id_;
};


class ModifyPwd {
 public:
  ModifyPwd()
      : phone_(NULL),vcode_(NULL),
      vtoken_(NULL),pwd_(NULL),
      type_(NULL),timestamp_(NULL),
      uid_(NULL)
      {
  }

  ~ModifyPwd() {
    if (phone_) {
      delete phone_;
    }
    if (vcode_) {
      delete vcode_;
    }
    if (vtoken_) {
      delete vtoken_;
    }
    if (pwd_) {
      delete pwd_;
    }

    if (type_) {
      delete type_;
    }

    if (timestamp_) {
      delete timestamp_;
    }

    if (uid_) {
      delete uid_;
    }
  }

  void set_phone(const std::string& phone) {
    phone_ = new base_logic::StringValue(phone);
  }

  void set_vcode(const std::string& value) {
    vcode_ = new base_logic::StringValue(value);
  }

  void set_vtoken(const std::string& value) {
    vtoken_ = new base_logic::StringValue(value);
  }

  void set_pwd(const std::string& value) {
    pwd_ = new base_logic::StringValue(value);
  }

  void set_type(const int64 type) {
    type_ = new base_logic::FundamentalValue(type);
  }

  void set_uid(const int64 value) {
    uid_ = new base_logic::FundamentalValue(value);
  }

  void set_timestamp(const int64 value) {
    timestamp_ = new base_logic::FundamentalValue(value);
  }

  int64 type() const {
    int64 type;
    type_->GetAsBigInteger(&type);
    return type;
  }

  int64 uid() const {
    int64 uid;
    uid_->GetAsBigInteger(&uid);
    return uid;
  }

  int64 timestamp() const {
    int64 timestamp;
    timestamp_->GetAsBigInteger(&timestamp);
    return timestamp;
  }

  const std::string& phone() const {
    std::string phone;
    phone_->GetAsString(&phone);
    return phone;
  }

  const std::string& vcode() const {
    std::string vcode;
    vcode_->GetAsString(&vcode);
    return vcode;
  }

  const std::string& vtoken() const {
    std::string vtoken;
    vtoken_->GetAsString(&vtoken);
    return vtoken;
  }

  const std::string& pwd() const {
    std::string pwd;
    pwd_->GetAsString(&pwd);
    return pwd;
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

 private:
  base_logic::StringValue* phone_;
  base_logic::StringValue* vcode_;
  base_logic::StringValue* vtoken_;
  base_logic::StringValue* pwd_;
  base_logic::FundamentalValue* uid_;    //用户id
  base_logic::FundamentalValue* type_;   //0：登录密码 1：交易密码，提现密码
  base_logic::FundamentalValue* timestamp_;   //时间戳
};
class CheckAccountExistReq {
 public:
  CheckAccountExistReq()
      : phone_(NULL) {
  }

  ~CheckAccountExistReq() {
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
typedef UserAccount CheckToken;
typedef UserAccount UserRealInfo;
class TGetVersion{
 public:
  TGetVersion()
      : type_(NULL) {
  }

  ~TGetVersion() {
    if (type_) {
      delete type_;
    }
  }

  void set_type(const int64 type) {
    type_ = new base_logic::FundamentalValue(type);
  }

  int64 type() const {
    int64 type;
    type_->GetAsBigInteger(&type);
    return type;
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

 private:
  base_logic::FundamentalValue* type_;
};
}

namespace net_reply {

class Balance {
 public:
  Balance()
      : balance_(NULL),market_cap_(NULL), total_amt_(NULL),is_setpwd_(NULL),
        value_(NULL),nick_name_(NULL),head_url_(NULL) {
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
    if (market_cap_ != NULL)
      value_->Set("market_cap", market_cap_);
    if (total_amt_ != NULL)
      value_->Set("total_amt", total_amt_);
    if (is_setpwd_ != NULL)
      value_->Set(L"is_setpwd", is_setpwd_);
    if (nick_name_ != NULL)
      value_->Set(L"nick_name", nick_name_);
    if (head_url_ != NULL)
      value_->Set(L"head_url", head_url_);
    return value_;
  }

  void set_balance(double balance) {
    balance_ = new base_logic::FundamentalValue(balance);
  }
  void set_market_cap(double value) {
    market_cap_ = new base_logic::FundamentalValue(value);
  }
  void set_total_amt(double value) {
    total_amt_ = new base_logic::FundamentalValue(value);
  }
  void set_is_setpwd(int32 value) {
    is_setpwd_ = new base_logic::FundamentalValue(value);
  }
  void set_nick_name(std::string nick_name) {
    nick_name_ = new base_logic::StringValue(nick_name);
  }
  void set_head_url(std::string& head_url) {
    head_url_ = new base_logic::StringValue(head_url);
  }
 private:
  base_logic::FundamentalValue* balance_;
  base_logic::FundamentalValue* market_cap_;
  base_logic::FundamentalValue* total_amt_;

  base_logic::FundamentalValue* is_setpwd_;
  base_logic::StringValue* nick_name_;
  base_logic::StringValue* head_url_;

  base_logic::DictionaryValue* value_;

};

class RealInfo{
 public:
  RealInfo()
      : realname_(NULL),id_card_(NULL),
        value_(NULL) {
  }

  ~RealInfo() {
    if (value_ != NULL) {
      delete value_;
      value_ = NULL;
    }
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (realname_ != NULL)
      value_->Set(L"realname", realname_);
    if (id_card_ != NULL)
      value_->Set(L"id_card", id_card_);
    return value_;
  }

   void set_realname(const std::string& value) {
     realname_ = new base_logic::StringValue(value);
   }
   void set_id_card(const std::string& value) {
     id_card_ = new base_logic::StringValue(value);
   }
 private:
  base_logic::StringValue* realname_;
  base_logic::StringValue* id_card_;
  base_logic::DictionaryValue* value_;
};

class UserInfo {
 public:
  UserInfo()
      : uid_(NULL),
        balance_(NULL),
        phone_(NULL),
        type_(NULL),
	member_id_(NULL),
	member_name_(NULL),
	agent_id_(NULL),
	agent_name_(NULL),
	avatar_large_(NULL),
	screen_name_(NULL),
        value_(NULL) {
  }

  ~UserInfo() {
/*
    if (value_){
     delete value_;
     value_ = NULL;
    }   
*/
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
   void set_member_id(const int32 member_id) {
     member_id_ = new base_logic::FundamentalValue(member_id);
   }
   void set_screen_name(const std::string& screen_name) {
     screen_name_ = new base_logic::StringValue(screen_name);
   }
   void set_member_name(const std::string& member_name) {
     member_name_ = new base_logic::StringValue(member_name);
   }
   void set_agent_id(const std::string& agent_id) {
     agent_id_ = new base_logic::StringValue(agent_id);
   }
   void set_agent_name(const std::string& agent_name) {
     agent_name_ = new base_logic::StringValue(agent_name);
   }
   void set_avatar_large(const std::string& avatar_large) {
     avatar_large_ = new base_logic::StringValue(avatar_large);
   }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (uid_ != NULL)
      value_->Set(L"id", uid_);
    if (balance_ != NULL)
      value_->Set(L"balance", balance_);
    if (phone_ != NULL)
      value_->Set(L"phone", phone_);
    if (type_ != NULL)
      value_->Set(L"type", type_);
	
    if (member_id_ != NULL)
      value_->Set(L"memberId", member_id_);
    if (member_name_ != NULL)
      value_->Set(L"memberName", member_name_);
    if (agent_id_ != NULL)
      value_->Set(L"agentId", agent_id_);
    if (agent_name_ != NULL)
      value_->Set(L"agentName", agent_name_);
    if (avatar_large_ != NULL)
      value_->Set(L"avatar_Large", avatar_large_);
    if (screen_name_ != NULL)
      value_->Set(L"screenName", screen_name_);
    return value_;
  }

 private:
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* balance_;
  base_logic::FundamentalValue* type_;
  base_logic::StringValue* phone_;
//add by tw
  base_logic::FundamentalValue* member_id_;
  base_logic::StringValue* member_name_;
  base_logic::StringValue* agent_id_;
  base_logic::StringValue* agent_name_;
  base_logic::StringValue* avatar_large_;
  base_logic::StringValue* screen_name_;
//end add bytw
  base_logic::DictionaryValue* value_;
};

class LoginAccount {
 public:
  LoginAccount()
      : token_(NULL),
        userinfo_(NULL),
        value_(NULL),
        token_time_(NULL) {
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

  void set_token_time(const int64 token_time) {
    token_time_ = new base_logic::FundamentalValue(token_time);
  }

  void set_userinfo(base_logic::DictionaryValue* dict) {
    userinfo_ = dict;
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
	base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
    if (token_ != NULL)
      value_->Set(L"token", token_);
    if (token_time_ != NULL)
      value_->Set(L"token_time", token_time_);
	value_->Set(L"result",result);
    if (userinfo_ != NULL)
      value_->Set(L"userinfo", userinfo_);
    return value_;
  }

 private:
  base_logic::DictionaryValue* userinfo_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* token_time_;
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
   void set_result(const int64 result) {
     result_ = new base_logic::FundamentalValue(result);
   }
   void set_token(const std::string& token) {
     token_ = new base_logic::StringValue(token);
   }
 public:
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (code_time_ != NULL)
      value_->Set(L"timeStamp", code_time_);
	if (result_ != NULL)
      value_->Set(L"result", result_);
    if (token_ != NULL)
      value_->Set(L"vToken", token_);
    return value_;
  }
 private:
  base_logic::FundamentalValue* code_time_;
  base_logic::FundamentalValue* result_;
  base_logic::StringValue* token_;
  base_logic::DictionaryValue* value_;
};


class ModifyPwd{
 public:
  ModifyPwd()
       : status_(NULL),
         value_(NULL) {
   }

   ~ModifyPwd() {

     if (value_) {
       delete value_;
       value_ = NULL;
     }
   }

   void set_status(const int64 status) {
     status_ = new base_logic::FundamentalValue(status);
   }

 public:
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (status_ != NULL)
      value_->Set(L"status", status_);

    return value_;
  }
 private:
  base_logic::FundamentalValue* status_;
  //base_logic::StringValue* token_;
  base_logic::DictionaryValue* value_;
};
class TResult{
 public:
  TResult()
      : result_(NULL),
        value_(NULL) {
  }

  ~TResult() {
    if (value_ != NULL) {
      delete value_;
      value_ = NULL;
    }
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (result_ != NULL)
      value_->Set(L"result", result_);
    return value_;
  }

  void set_result(int32 value) {
    result_ = new base_logic::FundamentalValue(value);
  }
 private:
  base_logic::FundamentalValue* result_;
  base_logic::DictionaryValue* value_;
};

class TGetVersion{
 public:
  TGetVersion()
       : type_(NULL),
         size_(NULL),
         version_code_(NULL),
         is_forceupdate_(NULL),
         app_name_(NULL),
         update_desc_(NULL),
         release_time_(NULL),
         url_(NULL),
         value_(NULL),
         version_name_(NULL)
   {
   }

   ~TGetVersion() {
     if (value_) {
       delete value_;
       value_ = NULL;
     }
   }

   void set_type(const int64 value) {
     type_ = new base_logic::FundamentalValue(value);
   }

   void set_size(const int64 value) {
     size_ = new base_logic::FundamentalValue(value);
   }
 
   void set_version_code(const int64 value) {
     version_code_ = new base_logic::FundamentalValue(value);
   }

   void set_is_forceupdate(const int64 value) {
     is_forceupdate_ = new base_logic::FundamentalValue(value);
   }

   void set_app_name(const std::string& value) {
     app_name_ = new base_logic::StringValue(value);
   }

   void set_update_desc(const std::string& value) {
     update_desc_= new base_logic::StringValue(value);
   }
   void set_version_name(const std::string& value) {
     version_name_= new base_logic::StringValue(value);
   }
   void set_release_time(const std::string& value) {
     release_time_= new base_logic::StringValue(value);
   }
   void set_url(const std::string& value) {
     url_= new base_logic::StringValue(value);
   }

 public:
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();

    if (type_!= NULL)
      value_->Set(L"type", type_);

    if (size_!= NULL)
      value_->Set(L"newAppSize", size_);

    if (version_code_ != NULL)
      value_->Set(L"newAppVersionCode", version_code_);

    if (version_name_ != NULL)
      value_->Set(L"newAppVersionName", version_name_);

    if (app_name_ != NULL)
      value_->Set(L"appName", app_name_);

    if (is_forceupdate_!= NULL)
      value_->Set(L"isForceUpdate", is_forceupdate_);

    if (update_desc_!= NULL)
      value_->Set(L"newAppUpdateDesc", update_desc_);

    if (release_time_!= NULL)
      value_->Set(L"newAppReleaseTime", release_time_);

    if (url_!= NULL)
      value_->Set(L"newAppUrl", url_);

    return value_;
  }
 private:
  base_logic::FundamentalValue* type_;
  base_logic::FundamentalValue* size_;
  base_logic::FundamentalValue* version_code_;
  base_logic::FundamentalValue* is_forceupdate_;
  base_logic::StringValue* app_name_;
  base_logic::StringValue* update_desc_;
  base_logic::StringValue* version_name_;
  base_logic::StringValue* release_time_;
  base_logic::StringValue* url_;

  base_logic::DictionaryValue* value_;
};
}

}

#endif
