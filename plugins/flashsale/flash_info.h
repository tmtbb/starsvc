//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_STAR_FLASH_INFOS_H_
#define SWP_STAR_FLASH_INFOS_H_

#include <string>
#include <map>
#include <list>
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/radom_in.h"

namespace flash_logic {

enum TIMETYPE {
    ONE_MINUTE = 60,
    FIVE_MINUTE = 300,
    HALF_HOUR = 1800,
    ONE_HOUR = 3600
};

//0-预售 1-发售 2-流通
enum TIMETASKTYPE {
    TASK_PRE_SELL_TYPE = 0,
    TASK_SELLING_TYPE = 1,
    TASK_SELL_TYPE = 2
};


class TimeTask {
public:
    TimeTask();
    TimeTask(const TimeTask& time_task);

    TimeTask& operator =(const TimeTask& time_task);

    ~TimeTask() {
        if (data_ != NULL)
            data_->Release();
    }

    void set_symbol(const std::string& symbol) {
        data_->symbol_ = symbol;
    }

    void set_earliest_time(const int64 ll) {
        data_->earliest_time_ = ll;
    }

    void set_latest_time(const int64 latest_time) {
        data_->latest_time_ = latest_time;
    }

    void set_task_time(const int64 task_time) {
        data_->task_time_ = task_time;
    }

    void set_interval_time(const int64 interval_time) {
        data_->interval_time_ = interval_time;
    }

    void set_task_start_time(const int64 task_start_time) {
        data_->task_start_time_ = task_start_time;
    }

    void set_task_type(const int32 task_type) {
        data_->task_type_ = task_type;
    }

    const std::string& symbol() const {
        return data_->symbol_;
    }

    const int64 earliest_time() const {
        return data_->earliest_time_;
    }

    const int64 latest_time() const {
        return data_->latest_time_;
    }

    const int64 task_time() const {
        return data_->task_time_;
    }

    const int64 interval_time() const {
        return data_->interval_time_;
    }

    const int64 task_start_time() const {
        return data_->task_start_time_;
    }


    const int32 task_type() const {
        return data_->task_type_;
    }

    static bool cmp(const TimeTask& t_time_task, const TimeTask& r_time_task) {
        return Data::cmp(t_time_task.data_, r_time_task.data_);
    }
private:
    class Data {
    public:
        Data():refcount_(1),
            earliest_time_(0),
            latest_time_(0),
            task_time_(0),
            interval_time_(0),
            task_type_(0) {
        }

    public:
        std::string  symbol_;
        int64  earliest_time_; //最早时间
        int64  latest_time_; //最晚时间
        int64 task_time_; //任务执行时间
        int64 interval_time_;//任务间隔时间
        int64 task_start_time_; //任务到点时间
        int32 task_type_; //任务类型

        static bool cmp(const Data* t_data,
                        const Data* r_data) {
            return t_data->task_start_time_ < r_data->task_start_time_;
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

/*class PulishStar {

public:
    PulishStar();
    PulishStar(const PulishStar& flash_star);

    PulishStar& operator =(const PulishStar& pulishstar);

    ~PulishStar() {
        if (data_ != NULL)
            data_->Release();
    }

    void ValueSerialization(base_logic::DictionaryValue* dict);
    void set_symbol(const std::string& symbol) {
        data_->symbol_ = symbol;
    }

    void set_name(const std::string& name) {
        data_->name_ = name;
    }

    void set_pic(const std::string& pic) {
        data_->pic_ = pic;
    }

    void set_home_pic(const std::string& pic) {
        data_->back_pic_ = pic;
    }

    void set_work(const std::string& work) {
        data_->work_ = work;
    }

    void set_star_type(const int32 startype) {
        data_->star_type_ = startype;
    }

    void set_publish_type(const int32 publishtype) {
        data_->publish_type_ = publishtype;
    }

    void set_publish_time(const int64 publishtime) {
        data_->publish_time_ = publishtime;
    }

    void set_publish_last_time(const int64 publishlasttime) {
        data_->publish_last_time_ = publishlasttime;
    }

    void set_publish_begin_time(const int64 publishbegintime) {
        data_->publish_begin_time_ = publishbegintime;
    }
    void set_publish_end_time(const int64 publishendtime) {
        data_->publish_end_time_ = publishendtime;
    }
    void set_publish_price(const double publishprice) {
        data_->publish_price_ = publishprice;
    }

    const std::string symbol() const {
        return data_->symbol_;
    }

    const std::string& name() const {
        return data_->name_;
    }

    const std::string& pic() const {
        return data_->pic_;
    }

    const std::string& back_pic() const {
        return data_->back_pic_;
    }

    const std::string& work() const {
        return data_->work_;
    }

    const int32 star_type() const {
        return data_->star_type_;
    }

    const int32 publish_type() const {
        return data_->publish_type_;
    }

    const int64  publish_time() const {
        return data_->publish_time_;
    }

    const int64 publish_last_time() const {
        return data_->publish_last_time_;
    }

    const int64 publish_begin_time() const {
        return data_->publish_begin_time_;
    }

    const int64 publish_end_time() const {
        return data_->publish_end_time_;
    }

    const double publish_price() const {
        return data_->publish_price_;
    }

private:
    class Data {
    public:
        Data() : refcount_(1) {
        }

        std::string  symbol_;
        std::string  name_;
        std::string  pic_;
        std::string  back_pic_;
        std::string  work_;
        int32 star_type_;
        int32 publish_type_;
        int64 publish_time_;
        int64 publish_last_time_;
        int64 publish_begin_time_;
        int64 publish_end_time_;
        double publish_price_;

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
*/

class PulishStar {

public:
    PulishStar();

    void Depcopy(const PulishStar& pulishstar);

    ~PulishStar() {
    }

    void ValueSerialization(base_logic::DictionaryValue* dict);
    void set_symbol(const char* symbol) {
        strcpy(symbol_, symbol);
    }

    void set_name(const char* name) {
        strcpy(name_, name);
    }

    void set_pic(const char* pic) {
        strcpy(pic_, pic);
    }

    void set_home_pic(const char* pic) {
        strcpy(back_pic_, pic);
    }

    void set_work(const char* work) {
        strcpy(work_, work);
    }

    void set_star_type(const int32 startype) {
        star_type_ = startype;
    }

    void set_publish_type(const int32 publishtype) {
        publish_type_ = publishtype;
    }

    void set_publish_time(const int64 publishtime) {
        publish_time_ = publishtime;
    }

    void set_publish_last_time(const int64 publishlasttime) {
        publish_last_time_ = publishlasttime;
    }

    void set_publish_begin_time(const int64 publishbegintime) {
        publish_begin_time_ = publishbegintime;
    }
    void set_publish_end_time(const int64 publishendtime) {
        publish_end_time_ = publishendtime;
    }
    void set_publish_price(const double publishprice) {
        publish_price_ = publishprice;
    }

    const char* symbol() const {
        return symbol_;
    }

    const char* name() const {
        return name_;
    }

    const char* pic() const {
        return pic_;
    }

    const char* back_pic() const {
        return back_pic_;
    }

    const char* work() const {
        return work_;
    }

    const int32 star_type() const {
        return star_type_;
    }

    const int32 publish_type() const {
        return publish_type_;
    }

    const int64  publish_time() const {
        return publish_time_;
    }

    const int64 publish_last_time() const {
        return publish_last_time_;
    }

    const int64 publish_begin_time() const {
        return publish_begin_time_;
    }

    const int64 publish_end_time() const {
        return publish_end_time_;
    }

    const double publish_price() const {
        return publish_price_;
    }

private:
    char  symbol_[64];
    char  name_[64];
    char  pic_[128];
    char  back_pic_[128];
    char  work_[64];
    int32 star_type_;
    int32 publish_type_;
    int64 publish_time_;
    int64 publish_last_time_;
    int64 publish_begin_time_;
    int64 publish_end_time_;
    double publish_price_;

};


}  // namespace flash_logic

#endif /* SWP_STAR_FLASH_INFOS_H_ */
