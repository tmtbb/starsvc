//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#include "starside/starside_info.h"
#include "basic/template.h"
#include "logic/logic_comm.h"

namespace starside_logic {

TranscationStatistics::TranscationStatistics() {
  data_ = new Data();
}

TranscationStatistics::TranscationStatistics(const TranscationStatistics& transcation_statistics)
: data_(transcation_statistics.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

TranscationStatistics& TranscationStatistics::operator =(const TranscationStatistics& transcation_statistics) {
  if (transcation_statistics.data_ != NULL) {
    transcation_statistics.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = transcation_statistics.data_;
  return (*this);
}

void TranscationStatistics::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetBigInteger(L"order_number", &data_->order_num_);
  dict->GetReal(L"order_price", &data_->open_price_); //价格
  dict->GetInteger(L"order_count", &data_->order_count_);
  double tmp = 0.0;
  //dict->GetReal(L"price", &data_->price_); //收益
  dict->GetReal(L"price", &tmp); //收益
  data_->price_ = data_->open_price_ - tmp; //总价格-0.15
  dict->GetBigInteger(L"starcode", &data_->starcode_);
  dict->GetString(L"starcode2", &data_->code_); //

  dict->GetBigInteger(L"order_date", &data_->open_position_time_);
  dict->GetReal(L"max_price", &data_->max_price_);
  dict->GetReal(L"min_price", &data_->min_price_);

  //dict->GetInteger(L"buy_sell", &data_->buy_sell_);
  //dict->GetString(L"symbol", &data_->symbol_);
  //dict->GetString(L"name", &data_->name_);
  //dict->GetBigInteger(L"close_position_time", &data_->close_position_time_);

  /*
  int32 int_result = 0;
  dict->GetInteger(L"result", &int_result);
  if (int_result == 1)
    data_->result_ = true;
  else
    data_->result_ = false;
    */
}

//_______________________________________________
//_______________________________________________


StarMeetRelForFan::StarMeetRelForFan() {
  data_ = new Data();
}

StarMeetRelForFan::StarMeetRelForFan(const StarMeetRelForFan& transcation_statistics)
: data_(transcation_statistics.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

StarMeetRelForFan& StarMeetRelForFan::operator =(const StarMeetRelForFan& transcation_statistics) {
  if (transcation_statistics.data_ != NULL) {
    transcation_statistics.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = transcation_statistics.data_;
  return (*this);
}

void StarMeetRelForFan::ValueSerialization(base_logic::DictionaryValue* dict) {
    
  dict->GetBigInteger(L"id", &data_->id_);
  dict->GetBigInteger(L"uid", &data_->uid_);
  dict->GetBigInteger(L"mid", &data_->mid_);
  dict->GetBigInteger(L"meet_type", &data_->meet_type_);

  dict->GetString(L"starcode", &data_->code_); //
  dict->GetString(L"nickname", &data_->nickname_); //
  dict->GetString(L"headurl", &data_->headurl_); //
  dict->GetString(L"name", &data_->name_); //
  dict->GetString(L"meet_city", &data_->meet_city_); //
  
  dict->GetString(L"appoint_time", &data_->appoint_time_); //
  dict->GetString(L"order_time", &data_->order_time_); //
  dict->GetString(L"comment", &data_->comment_); //

}
//
//

ServiceItem::ServiceItem() {
  data_ = new Data();
}

ServiceItem::ServiceItem(const ServiceItem& item)
: data_(item.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

ServiceItem& ServiceItem::operator =(const ServiceItem& item) {
  if (item.data_ != NULL) {
    item.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = item.data_;
  return (*this);
}

void ServiceItem::ValueSerialization(base_logic::DictionaryValue* dict) {
    
  dict->GetBigInteger(L"mid", &data_->mid_);
  dict->GetReal(L"price", &data_->price_);

  dict->GetString(L"name", &data_->name_); //
  
  dict->GetString(L"showpic_url", &data_->showpic_url_); //
}

//


StarOwnService::StarOwnService() {
  data_ = new Data();
}

StarOwnService::StarOwnService(const StarOwnService& item)
: data_(item.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

StarOwnService& StarOwnService::operator =(const StarOwnService& item) {
  if (item.data_ != NULL) {
    item.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = item.data_;
  return (*this);
}

void StarOwnService::ValueSerialization(base_logic::DictionaryValue* dict) {
    
  dict->GetBigInteger(L"mid", &data_->mid_);
  //dict->GetReal(L"price", &data_->price_);

  dict->GetString(L"name", &data_->name_); //
  
  dict->GetString(L"starcode", &data_->starcode_); //
  dict->GetString(L"meet_city", &data_->meet_city_); //
  dict->GetString(L"stardate", &data_->stardate_); //
  dict->GetString(L"enddate", &data_->enddate_); //
}
//TOwnStarUser



TOwnStarUser::TOwnStarUser() {
    data_ = new Data();
}

TOwnStarUser::TOwnStarUser(const TOwnStarUser& item)
    : data_(item.data_) {
    if (data_ != NULL) {
        data_->AddRef();
    }
}

TOwnStarUser& TOwnStarUser::operator =(const TOwnStarUser& item) {
    if (item.data_ != NULL) {
        item.data_->AddRef();
    }

    if (data_ != NULL) {
        data_->Release();
    }

    data_ = item.data_;
    return (*this);
}


void TOwnStarUser::ValueSerialization(base_logic::DictionaryValue* dict) {

    dict->GetBigInteger(L"uid", &data_->uid_);
    dict->GetBigInteger(L"ownseconds", &data_->ownseconds_);
    dict->GetInteger(L"appoint", &data_->appoint_);
    dict->GetString(L"user_nickname", &data_->user_nickname_);
    dict->GetString(L"starcode", &data_->starcode_);
    dict->GetString(L"faccid", &data_->faccid_);
    dict->GetString(L"head_url", &data_->head_url_);
}

}  // namespace trades_logic
