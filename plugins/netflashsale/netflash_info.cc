//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#include "basic/template.h"
#include "logic/logic_comm.h"
#include "flashsale/flash_info.h"

namespace flash_logic {

PulishStar::PulishStar() {
    data_ = new Data();
}

PulishStar::PulishStar(const PulishStar& pubstar)
    : data_(pubstar.data_) {
    if (data_ != NULL) {
        data_->AddRef();
    }
}

PulishStar& PulishStar::operator =(const PulishStar& pubstar) {
    if (pubstar.data_ != NULL) {
        pubstar.data_->AddRef();
    }

    if (data_ != NULL) {
        data_->Release();
    }

    data_ = pubstar.data_;
    return (*this);
}

void PulishStar::ValueSerialization(base_logic::DictionaryValue* dict) {
    dict->GetString(L"star_code", &data_->symbol_);
    dict->GetString(L"star_name", &data_->name_);
    dict->GetString(L"star_pic", &data_->pic_);
    dict->GetString(L"back_pic", &data_->back_pic_);
    dict->GetInteger(L"star_type", &data_->star_type_);
    dict->GetInteger(L"publish_type", &data_->publish_type_);
    dict->GetBigInteger(L"publish_time", &data_->publish_time_);
    dict->GetBigInteger(L"publish_last_time", &data_->publish_last_time_);
    dict->GetBigInteger(L"publish_begin_time", &data_->publish_begin_time_);
    dict->GetBigInteger(L"publish_end_time", &data_->publish_end_time_);
    dict->GetReal(L"publish_price", &data_->publish_price_);
}



TimeTask::TimeTask() {
    data_ = new Data();
}

TimeTask::TimeTask(const TimeTask& time_task)
    : data_(time_task.data_) {
    if (data_ != NULL) {
        data_->AddRef();
    }
}

TimeTask& TimeTask::operator =(const TimeTask& time_task) {
    if (time_task.data_ != NULL) {
        time_task.data_->AddRef();
    }

    if (data_ != NULL) {
        data_->Release();
    }

    data_ = time_task.data_;
    return (*this);
}


}  // namespace flash_logic
