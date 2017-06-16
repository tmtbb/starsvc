//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef SWP_BANK_INFOS_H_
#define SWP_BANK_INFOS_H_

#include <string>
#include <map>
#include <list>

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"

namespace bank_logic {

class BankCard {
 public:
  BankCard();
  BankCard(const BankCard& bank_card);

  BankCard& operator =(const BankCard& bank_card);

  ~BankCard() {
    if (data_ != NULL)
      data_->Release();
  }

  bool ValueSerialization(base_logic::DictionaryValue* dict);

  static bool close_after(const BankCard& t_bankcard,
                          const BankCard& r_bankcard) {
    return Data::close_after(t_bankcard.data_, r_bankcard.data_);
  }

  void set_id(const int64 id) {
    data_->id_ = id;
  }

  void set_bank_id(const int32 bank_id) {
    data_->bank_id_ = bank_id;
  }

  void set_uid(const int64 uid) {
    data_->uid_ = uid;
  }
  
  void set_is_default(const int32 is_default){
    data_->is_default_ = is_default;
  }
  void set_bank_name(const std::string& bank_name) {
    data_->bank_name_ = bank_name;
  }

  void set_branch_bank(const std::string& branch_bank) {
    data_->branch_bank_ = branch_bank;
  }

  void set_card_no(const std::string& card_no) {
    data_->card_no_ = card_no;
  }

  void set_name(const std::string& name) {
    data_->name_ = name;
  }

  const int64 id() const {
    return data_->id_;
  }

  const int32 bank_id() const {
    return data_->bank_id_;
  }

  const int64 uid() const {
    return data_->uid_;
  }

  const int32 is_default() const {
    return data_->is_default_;
  }

  const std::string& bank_name() const {
    return data_->bank_name_;
  }

  const std::string& branch_bank() const {
    return data_->branch_bank_;
  }

  const std::string& card_no() const {
    return data_->card_no_;
  }

  const std::string& card_name() const {
   return data_->card_name_;
  }

  const std::string& name() const {
    return data_->name_;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          bank_id_(0),
          uid_(0),
          id_(0),
          is_default_(0) {
    }

   public:
    int64 id_;
    int32 bank_id_;
    int64 uid_;
    int32 is_default_;
    std::string bank_name_;
    std::string branch_bank_;
    std::string card_no_;
    std::string card_name_;
    std::string name_;

    static bool close_after(const Data* t_data, const Data* r_data) {
      return t_data->id_ > r_data->id_;
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

}  // namespace bank_logic

#endif /* SWP_BANK_INFOS_H_ */
