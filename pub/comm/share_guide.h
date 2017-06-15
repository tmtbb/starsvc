// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// share_tourism.h
// Created on: 2016年11月3日.
// Author: Paco.
#ifndef PUB_COMM_SHARE_GUIDE_H_
#define PUB_COMM_SHARE_GUIDE_H_

#include <string>
#include <list>

#include "public/basic/basictypes.h"
#include "base/logic/base_values.h"

#include "pub/comm/ref_base.h"

typedef base_logic::DictionaryValue DicValue;
class TourismShareData : public RefBase {
 public:
  TourismShareData()
      : share_id_(0),
        share_type_id_(0),
        is_recommend_(0) {
  }
  virtual ~TourismShareData() {
  }
 public:
  int64 share_id_;
  std::string share_type_;
  std::string share_theme_;
  std::string per_cash_;
  std::string addr_region_;
  std::string addr_detail_;
  std::string summary_;
  std::string telephone_;
  std::string detail_pic_;
  std::string brief_pic_;
  int64 is_recommend_;
  int64 share_type_id_;
};

class SkillShareData : public RefBase {
 public:
  SkillShareData()
      : share_id_(0),
        share_start_(0),
        share_end_(0),
        share_status_(0),
        is_banner_(0),
        entry_num_(0){

  }
  virtual ~SkillShareData() {

  }
 public:
  int64 share_id_;
  std::string share_theme_;
  std::string share_user_;
  std::string user_label_;
  int64 share_start_;
  int64 share_end_;
  int64 share_status_;
  std::string detail_pic_;
  std::string brief_pic_;
  std::string banner_pic_;
  int64 is_banner_;
  std::string share_head_;
  std::string summary_;
  int64 entry_num_;
};

class ShareTourism {
 public:
  ShareTourism();
  virtual ~ShareTourism();
  ShareTourism(const ShareTourism& share);
  ShareTourism& operator=(const ShareTourism& share);

  void SetDetailSerialization(DicValue* dict);
  void SetBriefSerialization(DicValue* dict);
  void Serialization(DicValue* dic);
  inline int64 share_id() {
    return data_->share_id_;
  }
  inline std::string share_type() {
    return data_->share_type_;
  }
  inline int64 share_type_id() {
    return data_->share_type_id_;
  }
  inline std::string share_theme() {
    return data_->share_theme_;
  }
  inline std::string per_cash() {
    return data_->per_cash_;
  }
  inline std::string addr_region() {
    return data_->addr_region_;
  }
  inline std::string addr_detail() {
    return data_->addr_detail_;
  }
  inline std::string summary() {
    return data_->summary_;
  }
  inline std::string telephone() {
    return data_->telephone_;
  }
  inline std::string detail_pic() {
    return data_->detail_pic_;
  }
  inline std::string brief_pic() {
    return data_->brief_pic_;
  }
  inline int64 is_recommend() {
    return data_->is_recommend_;
  }

  inline void set_share_id(int64 id) {
    data_->share_id_ = id;
  }
  inline void set_share_type(std::string type) {
    data_->share_type_ = type;
  }
  inline void set_share_theme(std::string them) {
    data_->share_theme_ = them;
  }
  inline void set_per_cash(std::string cash) {
    data_->per_cash_ = cash;
  }
  inline void set_addr_region(std::string region) {
    data_->addr_region_ = region;
  }
  inline void set_addr_detail(std::string addr) {
    data_->addr_detail_ = addr;
  }
  inline void summary(std::string su) {
    data_->summary_ = su;
  }
  inline void set_telephone(std::string phone) {
    data_->telephone_ = phone;
  }
  inline void set_detail_pic(std::string pic) {
    data_->detail_pic_ = pic;
  }
  inline void set_brief_pic(std::string pic) {
    data_->brief_pic_ = pic;
  }
  inline void set_is_recommend(int64 recommend) {
    data_->is_recommend_ = recommend;
  }
  inline void set_share_type_id(int64 type_id) {
    data_->share_type_id_ = type_id;
  }
 private:
  TourismShareData* data_;
};

class ShareSkill {
 public:
  ShareSkill();
  virtual ~ShareSkill();
  ShareSkill(const ShareSkill& share);
  ShareSkill& operator=(const ShareSkill& share);

  void SetDetailSerialization(DicValue* dict);
  void SetBriefSerialization(DicValue* dict);
  void SetBannerSerialization(DicValue* dict);
  void Serialization(DicValue* dic);

  inline int64 share_id() {
    return data_->share_id_;
  }
  inline std::string share_theme() {
    return data_->share_theme_;
  }
  inline std::string share_user() {
    return data_->share_user_;
  }
  inline std::string user_label() {
    return data_->user_label_;
  }
  inline int64 share_start() {
    return data_->share_start_;
  }
  inline int64 share_end() {
    return data_->share_end_;
  }
  inline int64 share_status() {
    return data_->share_status_;
  }
  inline std::string detail_pic() {
    return data_->detail_pic_;
  }
  inline std::string brief_pic() {
    return data_->brief_pic_;
  }
  inline std::string banner_pic() {
    return data_->banner_pic_;
  }
  inline int64 is_banner() {
    return data_->is_banner_;
  }
  inline std::string share_head() {
    return data_->share_head_;
  }
  inline std::string summary() {
    return data_->summary_;
  }
  inline int64 enter_num() {
    return data_->entry_num_;
  }


  inline void set_share_id(int64 id) {
    data_->share_id_ = id;
  }
  inline void set_share_theme(std::string theme) {
    data_->share_theme_ = theme;
  }
  inline void set_share_user(std::string user) {
    data_->share_user_ = user;
  }
  inline void set_user_label(std::string label) {
    data_->user_label_ = label;
  }
  inline void set_share_start(int64 start) {
    data_->share_start_ = start;
  }
  inline void set_share_end(int64 end) {
    data_->share_end_ = end;
  }
  inline void set_share_status(int64 status) {
    data_->share_status_ = status;
  }
  inline void set_detail_pic(std::string pic) {
    data_->detail_pic_ = pic;
  }
  inline void set_brief_pic(std::string pic) {
    data_->brief_pic_ = pic;
  }
  inline void set_banner_pic(std::string pic) {
    data_->banner_pic_ = pic;
  }
  inline void set_is_banner(int64 isbanner) {
    data_->is_banner_ = isbanner;
  }
  inline void set_share_head(std::string head) {
    data_->share_head_ = head;
  }
  inline void set_summary(std::string su) {
    data_->summary_ = su;
  }
  inline void set_entry_num(int64 num) {
    data_->entry_num_ = num;
  }
 private:
  SkillShareData* data_;
};

#endif  // PUB_COMM_SHARE_GUIDE_H_
