// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// share_guide.cc
// Created on: 2016年11月3日.
// Author: Paco.

#include "pub/comm/share_guide.h"
#include "glog/logging.h"

ShareTourism::ShareTourism() {
  data_ = new TourismShareData();
}

ShareTourism::~ShareTourism() {
  if (data_ != NULL) {
    data_->DelRef();
  }
}

ShareTourism::ShareTourism(const ShareTourism& share)
    : data_(share.data_) {
  if (data_ != NULL)
    data_->AddRef();
}

ShareTourism& ShareTourism::operator=(const ShareTourism& share) {
  if (data_ == share.data_)
    return *this;
  if (data_ != NULL)
    data_->DelRef();
  if (share.data_ != NULL)
    share.data_->AddRef();
  data_ = share.data_;
  return *this;
}

void ShareTourism::Serialization(DicValue* dict) {
  dict->GetBigInteger(L"share_id_", &data_->share_id_);
  dict->GetString(L"share_type_", &data_->share_type_);
  dict->GetString(L"share_theme_", &data_->share_theme_);
  dict->GetString(L"per_cash_", &data_->per_cash_);
  dict->GetString(L"addr_region_", &data_->addr_region_);
  dict->GetString(L"addr_detail_", &data_->addr_detail_);
  dict->GetString(L"summary_", &data_->summary_);
  dict->GetString(L"telephone_", &data_->telephone_);
  dict->GetString(L"detail_pic_", &data_->detail_pic_);
  dict->GetString(L"brief_pic_", &data_->brief_pic_);
  dict->GetBigInteger(L"is_recommend_", &data_->is_recommend_);
  dict->GetBigInteger(L"share_type_id_", &data_->share_type_id_);
}

void ShareTourism::SetBriefSerialization(DicValue* dict) {
  dict->SetBigInteger(L"share_id_", data_->share_id_);
  dict->SetString(L"share_type_", data_->share_type_);
  dict->SetString(L"share_theme_", data_->share_theme_);
  dict->SetString(L"per_cash_", data_->per_cash_);
  dict->SetString(L"addr_region_", data_->addr_region_);
//  dict->SetString(L"addr_detail_", data_->addr_detail_);
//  dict->SetString(L"summary_", data_->summary_);
  dict->SetString(L"telephone_", data_->telephone_);
//  dict->SetString(L"detail_pic_", data_->detail_pic_);
  dict->SetString(L"brief_pic_", data_->brief_pic_);
//  dict->SetBigInteger(L"is_recommend_", data_->is_recommend_);
}

void ShareTourism::SetDetailSerialization(DicValue* dict) {
  dict->SetBigInteger(L"share_id_", data_->share_id_);
  dict->SetString(L"share_type_", data_->share_type_);
  dict->SetString(L"share_theme_", data_->share_theme_);
  dict->SetString(L"per_cash_", data_->per_cash_);
  dict->SetString(L"addr_region_", data_->addr_region_);
  dict->SetString(L"addr_detail_", data_->addr_detail_);
  dict->SetString(L"summary_", data_->summary_);
  dict->SetString(L"telephone_", data_->telephone_);
  dict->SetString(L"detail_pic_", data_->detail_pic_);
//  dict->SetString(L"brief_pic_", data_->brief_pic_);
//  dict->SetBigInteger(L"is_recommend_", data_->is_recommend_);
}

ShareSkill::ShareSkill() {
  data_ = new SkillShareData();
}

ShareSkill::~ShareSkill() {
  if (data_ != NULL) {
    data_->DelRef();
  }
}

ShareSkill::ShareSkill(const ShareSkill& share)
    : data_(share.data_) {
  if (data_ != NULL)
    data_->AddRef();
}

ShareSkill& ShareSkill::operator=(const ShareSkill& share) {
  if (data_ == share.data_)
    return *this;
  if (data_ != NULL)
    data_->DelRef();
  if (share.data_ != NULL)
    share.data_->AddRef();
  data_ = share.data_;
  return *this;
}


void ShareSkill::SetDetailSerialization(DicValue* dict) {
  dict->SetBigInteger(L"share_id_", data_->share_id_);
  dict->SetString(L"share_theme_", data_->share_theme_);
  dict->SetString(L"share_user_", data_->share_user_);
  dict->SetString(L"user_label_", data_->user_label_);
  dict->SetBigInteger(L"share_start_", data_->share_start_);
  dict->SetBigInteger(L"share_end_", data_->share_end_);
  dict->SetBigInteger(L"share_status_", data_->share_status_);
  dict->SetString(L"detail_pic_", data_->detail_pic_);
//  dict->SetString(L"brief_pic_", data_->brief_pic_);
//  dict->SetString(L"banner_pic_", data_->banner_pic_);
//  dict->SetBigInteger(L"is_banner_", data_->is_banner_);
  dict->SetString(L"share_head_", data_->share_head_);
  dict->SetString(L"summary_", data_->summary_);
  dict->SetBigInteger(L"entry_num_", data_->entry_num_);
}

void ShareSkill::SetBriefSerialization(DicValue* dict) {
  dict->SetBigInteger(L"share_id_", data_->share_id_);
  dict->SetString(L"share_theme_", data_->share_theme_);
  dict->SetString(L"share_user_", data_->share_user_);
  dict->SetString(L"user_label_", data_->user_label_);
  dict->SetBigInteger(L"share_start_", data_->share_start_);
//  dict->SetBigInteger(L"share_end_", data_->share_end_);
  dict->SetBigInteger(L"share_status_", data_->share_status_);
//  dict->SetString(L"detail_pic_", data_->detail_pic_);
  dict->SetString(L"brief_pic_", data_->brief_pic_);
//  dict->SetString(L"banner_pic_", data_->banner_pic_);
//  dict->SetBigInteger(L"is_banner_", data_->is_banner_);
//  dict->SetString(L"share_head_", data_->share_head_);
//  dict->SetString(L"summary_", data_->summary_);
  dict->SetBigInteger(L"entry_num_", data_->entry_num_);
}

void ShareSkill::SetBannerSerialization(DicValue* dict) {
  dict->SetBigInteger(L"share_id_", data_->share_id_);
//  dict->SetString(L"share_theme_", data_->share_theme_);
//  dict->SetString(L"share_user_", data_->share_user_);
//  dict->SetString(L"user_label_", data_->user_label_);
//  dict->SetBigInteger(L"share_start_", data_->share_start_);
//  dict->SetBigInteger(L"share_end_", data_->share_end_);
//  dict->SetBigInteger(L"share_status_", data_->share_status_);
//  dict->SetString(L"detail_pic_", data_->detail_pic_);
//  dict->SetString(L"brief_pic_", data_->brief_pic_);
  dict->SetString(L"banner_pic_", data_->banner_pic_);
//  dict->SetBigInteger(L"is_banner_", data_->is_banner_);
//  dict->SetString(L"share_head_", data_->share_head_);
//  dict->SetString(L"summary_", data_->summary_);
//  dict->SetBigInteger(L"entry_num_", data_->entry_num_);
}

void ShareSkill::Serialization(DicValue* dict) {
  dict->GetBigInteger(L"share_id_", &data_->share_id_);
  dict->GetString(L"share_theme_", &data_->share_theme_);
  dict->GetString(L"share_user_", &data_->share_user_);
  dict->GetString(L"user_label_", &data_->user_label_);
  dict->GetBigInteger(L"share_start_", &data_->share_start_);
  dict->GetBigInteger(L"share_end_", &data_->share_end_);
  dict->GetBigInteger(L"share_status_", &data_->share_status_);
  dict->GetString(L"detail_pic_", &data_->detail_pic_);
  dict->GetString(L"brief_pic_", &data_->brief_pic_);
  dict->GetString(L"banner_pic_", &data_->banner_pic_);
  dict->GetBigInteger(L"is_banner_", &data_->is_banner_);
  dict->GetString(L"share_head_", &data_->share_head_);
  dict->GetString(L"summary_", &data_->summary_);
  dict->GetBigInteger(L"entry_num_", &data_->entry_num_);
}

