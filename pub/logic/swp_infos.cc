//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#include "logic/swp_infos.h"
#include "basic/template.h"
#include "logic/logic_comm.h"

namespace swp_logic {

UserInfo::UserInfo() {
  data_ = new Data();
}

UserInfo::UserInfo(const UserInfo& userinfo)
    : data_(userinfo.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

UserInfo& UserInfo::operator =(const UserInfo& userinfo) {
  if (userinfo.data_ != NULL) {
    userinfo.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = userinfo.data_;
  return (*this);
}

void UserInfo::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"uid", &data_->uid_);
  dict->GetReal(L"balance", &data_->balance_);
  dict->GetString(L"phone", &data_->phone_num_);
  dict->GetInteger(L"type", &data_->type_);
}



Withdrawals::Withdrawals() {
  data_ = new Data();
}

Withdrawals::Withdrawals(const Withdrawals& withdrawals)
    : data_(withdrawals.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

Withdrawals& Withdrawals::operator =(const Withdrawals& withdrawals) {
  if (withdrawals.data_ != NULL) {
    withdrawals.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = withdrawals.data_;
  return (*this);
}

void Withdrawals::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"uid", &data_->uid_);
  dict->GetBigInteger(L"wid", &data_->wid_);
  dict->GetInteger(L"status", &data_->status_);
  dict->GetReal(L"money",&data_->amount_);
  dict->GetReal(L"charge", &data_->charge_);
  dict->GetBigInteger(L"withdraw_unix_time", &data_->withdraw_unix_time_);
  dict->GetString(L"withdraw_time", &data_->withdraw_time_);
  dict->GetString(L"handle_time", &data_->handle_time_);
  dict->GetString(L"bankName", &data_->bank_);
  dict->GetString(L"branchBank", &data_->branch_bank_);
  dict->GetString(L"account",&data_->card_no_);
  dict->GetString(L"bank_username", &data_->name_);
  dict->GetString(L"comment", &data_->comment_);
}


Recharge::Recharge() {
  data_ = new Data();
}

Recharge::Recharge(const Recharge& recharge)
    : data_(recharge.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

Recharge& Recharge::operator =(const Recharge& recharge) {
  if (recharge.data_ != NULL) {
    recharge.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = recharge.data_;
  return (*this);
}

void Recharge::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"rid", &data_->rid_);
  dict->GetBigInteger(L"uid", &data_->uid_);

  int32 status = 0;
  dict->GetInteger(L"status", &status);
  if (status == 0 ||status == 1)
    data_->status_ = 1;
  else if (status == 3)
    data_->status_ = 2;
  else
    data_->status_ = 2;

  dict->GetReal(L"amount", &data_->amount_);
  dict->GetString(L"deposit_time", &data_->deposit_time_);
  dict->GetBigInteger(L"deposit_unit_time", &data_->deposit_unix_time_);
  dict->GetInteger(L"deposit_type", &data_->deposit_type_);
  if (data_->deposit_type_ == 1)
    data_->deposit_name_ = "微信";
  else if (data_->deposit_type_ == 2)
    data_->deposit_name_ = "银联";
  else
    data_->deposit_name_ = "未知";
}

TradesPosition::TradesPosition() {
  data_ = new Data();
}

TradesPosition::TradesPosition(const TradesPosition& trades_positions)
    : data_(trades_positions.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

TradesPosition& TradesPosition::operator =(
    const TradesPosition& trades_positions) {
  if (trades_positions.data_ != NULL) {
    trades_positions.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = trades_positions.data_;
  return (*this);
}

void TradesPosition::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"tid", &data_->position_id_);
  dict->GetBigInteger(L"uid", &data_->uid_);
  dict->GetInteger(L"code_id", &data_->code_id_);
  dict->GetInteger(L"buy_sell", &data_->buy_sell_);
  dict->GetString(L"symbol", &data_->symbol_);
  dict->GetString(L"name", &data_->name_);
  dict->GetInteger(L"close_type", &data_->close_type_);
  dict->GetBigInteger(L"amount", &data_->amount_);
  dict->GetBigInteger(L"open_position_time", &data_->open_position_time_);
  dict->GetBigInteger(L"close_position_time", &data_->close_position_time_);
  dict->GetReal(L"gross_profit", &data_->gross_profit_);
  dict->GetReal(L"open_price", &data_->open_price_);
  dict->GetReal(L"open_cost", &data_->open_cost_);
  dict->GetReal(L"open_charge", &data_->open_charge_);
  dict->GetReal(L"close_price", &data_->close_price_);
  dict->GetReal(L"pos_limit", &data_->limit_);
  dict->GetReal(L"stop", &data_->stop_);
  dict->GetReal(L"deferred", &data_->deferred_);
  int32 int_result = 0;
  dict->GetInteger(L"result", &int_result);
  if (int_result == 1)
    data_->result_ = true;
  else
    data_->result_ = false;

  dict->GetInteger(L"handle", &data_->handle_);
}

Quotations::Quotations() {
  data_ = new Data();
}

Quotations::Quotations(const Quotations& quotations)
    : data_(quotations.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

Quotations& Quotations::operator =(const Quotations& quotations) {
  if (quotations.data_ != NULL) {
    quotations.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = quotations.data_;
  return (*this);
}

void Quotations::ValueDeserialize(std::string& str) {
  int32 error_code;
  std::string error_str;
  base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON);
  if (engine == NULL) {
    LOG_ERROR("engine create null");
  }
  base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*) engine
      ->Deserialize(&str, &error_code, &error_str);
  dict->GetReal(L"current_price", &data_->current_price_);
  dict->GetReal(L"high_price", &data_->high_price_);
  dict->GetReal(L"low_price", &data_->low_price_);
  dict->GetReal(L"opening_today_price", &data_->opening_today_price_);
  dict->GetReal(L"closed_yesterday_price", &data_->closed_yesterday_price_);
  dict->GetReal(L"change", &data_->change_);
  dict->GetReal(L"pchg", &data_->pchg_);
  dict->GetBigInteger(L"current_unix_time", &data_->current_unix_time_);
  dict->GetString(L"platform_name", &data_->platform_name_);
  dict->GetString(L"symbol", &data_->symbol_);
  dict->GetString(L"exchange_name", &data_->exchange_name_);
}

std::string Quotations::ValueSerialize() {
  std::string json_content;
  base_logic::DictionaryValue dict;
  if (!data_)
    return "";
  dict.SetReal(L"current_price", data_->current_price_);
  dict.SetReal(L"high_price", data_->high_price_);
  dict.SetReal(L"low_price", data_->low_price_);
  dict.SetReal(L"opening_today_price", data_->opening_today_price_);
  dict.SetReal(L"closed_yesterday_price", data_->closed_yesterday_price_);
  dict.SetReal(L"change", data_->change_);
  dict.SetReal(L"pchg", data_->pchg_);
  dict.SetBigInteger(L"current_unix_time", data_->current_unix_time_);
  if (!data_->platform_name_.empty())
    dict.SetString(L"platform_name", data_->platform_name_);
  if (!data_->symbol_.empty())
    dict.SetString(L"symbol", data_->symbol_);
  if (!data_->exchange_name_.empty())
    dict.SetString(L"exchange_name", data_->exchange_name_);

  base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON);
  if (engine == NULL) {
    LOG_ERROR("engine create null");
  }
  bool r = engine->Serialize(dict, &json_content);
  return json_content;
}

}  // namespace quotations_logic
