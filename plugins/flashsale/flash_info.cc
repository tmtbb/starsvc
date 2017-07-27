//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#include "basic/template.h"
#include "logic/logic_comm.h"
#include "flashsale/flash_info.h"

namespace flash_logic {

/*PulishStar::PulishStar() {
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
    dict->GetString(L"work", &data_->work_);
    dict->GetInteger(L"star_type", &data_->star_type_);
    dict->GetInteger(L"publish_type", &data_->publish_type_);
    dict->GetBigInteger(L"publish_time", &data_->publish_time_);
    dict->GetBigInteger(L"publish_last_time", &data_->publish_last_time_);
    dict->GetBigInteger(L"publish_begin_time", &data_->publish_begin_time_);
    dict->GetBigInteger(L"publish_end_time", &data_->publish_end_time_);
    dict->GetReal(L"publish_price", &data_->publish_price_);
}*/

PulishStar::PulishStar() {
    memset(symbol_, 0x00, sizeof(symbol_));
    memset(name_, 0x00, sizeof(name_));
    memset(pic_, 0x00, sizeof(pic_));
    memset(back_pic_, 0x00, sizeof(back_pic_));
    memset(work_, 0x00, sizeof(work_));
}

void PulishStar::Depcopy(const PulishStar& pubstar) {
    set_symbol(pubstar.symbol());
    set_name(pubstar.name());
    set_pic(pubstar.pic());
    set_home_pic(pubstar.back_pic());
    set_work(pubstar.work());
    set_star_type(pubstar.star_type());
    set_publish_type(pubstar.publish_type());
    set_publish_time(pubstar.publish_time());
    set_publish_last_time(pubstar.publish_last_time());
    set_publish_begin_time(pubstar.publish_begin_time());
    set_publish_end_time(pubstar.publish_end_time());
    set_publish_price(pubstar.publish_price());

}

void PulishStar::ValueSerialization(base_logic::DictionaryValue* dict) {
    std::string str("");
    if(dict->GetString(L"star_code", &str))
        strcpy(symbol_, str.c_str());
    if(dict->GetString(L"star_name", &str))
        strcpy(name_, str.c_str());
    if(dict->GetString(L"star_pic", &str))
        strcpy(pic_, str.c_str());
    if(dict->GetString(L"back_pic", &str))
        strcpy(back_pic_, str.c_str());
    if(dict->GetString(L"work", &str))
        strcpy(work_, str.c_str());
    dict->GetInteger(L"star_type", &star_type_);
    dict->GetInteger(L"publish_type", &publish_type_);
    dict->GetBigInteger(L"publish_time", &publish_time_);
    dict->GetBigInteger(L"publish_last_time", &publish_last_time_);
    dict->GetBigInteger(L"publish_begin_time", &publish_begin_time_);
    dict->GetBigInteger(L"publish_end_time", &publish_end_time_);
    dict->GetReal(L"publish_price", &publish_price_);
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
