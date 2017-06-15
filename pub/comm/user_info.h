// Copyright (c) 2016 The tourism Authors. All rights reserved.
// user.h
// Created on: 2016年8月19日.
// Author: Paco.
#ifndef PUB_COMM_USER_INFO_H_
#define PUB_COMM_USER_INFO_H_

#include <string>
#include <list>

#include "basic/basictypes.h"
#include "logic/base_values.h"

#include "comm/ref_base.h"

typedef base_logic::DictionaryValue DicValue;

class UserData : public RefBase {
 public:
  UserData():uid_(-1),
    socket_fd_(-1), heart_loss_(0),
    gender_(0), is_login_(false) {
  }
  virtual ~UserData() {}
 public:
  int64 uid_;
  std::string phone_num_;
  std::string nickname_;
  //  std::string passwd_;
  std::string token_;
  std::string head_url_;
  int32 socket_fd_;
  int32 heart_loss_;
  int64 gender_;
  bool is_login_;
};

class UserInfo {
 public:
  UserInfo();
  virtual ~UserInfo();
  UserInfo(const UserInfo& user);
  UserInfo& operator=(const UserInfo& user);

  void Serialization(DicValue* dic);
  inline int64 uid() const { return data_->uid_; }
  inline std::string phone_num() const { return data_->phone_num_; }
  inline std::string nickname() const { return data_->nickname_; }
  //inline std::string passwd() const { return data_->passwd_; }
  inline std::string token() const { return data_->token_; }
  inline std::string head_url() const { return data_->head_url_; }
  inline bool is_login() const { return data_->is_login_; }
  inline int32 socket_fd() const { return data_->socket_fd_; }
  inline int32 heart_loss() const { return data_->heart_loss_; }
  inline int64 gender() const { return data_->gender_; }

  //inline void set_uid(int64 id) { data_->uid_ = id; }
  inline void set_phone_num(std::string mob) { data_->phone_num_ = mob; }
  inline void set_nickname(std::string nm) { data_->nickname_ = nm; }
  //inline void set_passwd(std::string pwd) { data_->passwd_ = pwd; }
  inline void set_token(std::string& token) { data_->token_ = token; }
  inline void set_head_url(std::string h) { data_->head_url_ = h; }
  inline void set_is_login(bool login) { data_->is_login_ = login; }
  inline void set_socket_fd(int32 fd) { data_->socket_fd_ = fd; }
  inline void set_heart_loss(int32 loss) { data_->heart_loss_ = loss; }
  inline void set_gender(int64 sex) { data_->gender_ = sex; }
 public:
  UserData* data_;
};

#endif  // PUB_COMM_USER_INFO_H_
