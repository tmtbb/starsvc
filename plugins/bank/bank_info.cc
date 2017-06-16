//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#include "bank/bank_info.h"
#include "basic/template.h"
#include "logic/logic_comm.h"

namespace bank_logic {

BankCard::BankCard() {
  data_ = new Data();
}

BankCard::BankCard(const BankCard& bank_card)
: data_(bank_card.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

BankCard& BankCard::operator =(const BankCard& bank_card) {
  if (bank_card.data_ != NULL) {
    bank_card.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = bank_card.data_;
  return (*this);
}

bool BankCard::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"id", &data_->id_);
  dict->GetBigInteger(L"uid", &data_->uid_);
  dict->GetString(L"card_no", &data_->card_no_);
  dict->GetString(L"bank_username", &data_->name_);
  dict->GetInteger(L"bank_id", &data_->bank_id_);
  dict->GetString(L"branch_bank", &data_->branch_bank_);
  dict->GetString(L"card_name", &data_->card_name_);
  dict->GetString(L"bank_name", &data_->bank_name_);
  dict->GetInteger(L"is_default", &data_->is_default_); 
  
  return true;
}

}  // namespace trades_logic
