//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#include "history/history_info.h"
#include "basic/template.h"
#include "logic/logic_comm.h"

namespace history_logic {
/*
TradesPosition::TradesPosition() {
  data_ = new Data();
}

TradesPosition::TradesPosition(const TradesPosition& trades_positions)
: data_(trades_positions.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

TradesPosition& TradesPosition::operator =(const TradesPosition& trades_positions) {
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
}*/

}  // namespace trades_logic
