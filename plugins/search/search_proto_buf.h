//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry
//
#ifndef STAR_SEARCH_PROTO_BUF_H_
#define STAR_SEARCH_PROTO_BUF_H_
#include "basic/basictypes.h"
#include "logic/base_values.h"

namespace search_logic {

namespace net_request {

class SearchStarInfo {
 public:
    SearchStarInfo()
        :id_(NULL),
        token_(NULL),
        message_(NULL) {}

    ~SearchStarInfo() {
        if (id_){delete id_; id_ = NULL;}
        if (token_) {delete token_; token_ = NULL;}
        if (message_) {delete message_; message_ = NULL;}
    }

    bool set_http_packet(base_logic::DictionaryValue* value);

    void set_id(const int64 id) {
        id_ = new base_logic::FundamentalValue(id);
    }

    void set_token(const std::string& token) {
        token_ = new base_logic::StringValue(token);
    }

    void set_message(const std::string& message) {
        message_ = new base_logic::StringValue(message);
    }

    const int64 id() const {
        int64 id = 0;
        id_->GetAsBigInteger(&id);
        return id;
    }

    const std::string token() const {
        std::string token;
        token_->GetAsString(&token);
        return token;
    }

    const std::string message() const {
        std::string message;
        message_->GetAsString(&message);
        return message;
    }


 public:
    base_logic::FundamentalValue* id_;
    base_logic::StringValue* token_;
    base_logic::StringValue* message_;
};

}

namespace net_reply{
    
class StarUnit {
 public:
   StarUnit()
        :symbol_(NULL)
        ,gender_(NULL)
        ,name_(NULL)
        ,pic_(NULL)
        ,wid_(NULL){}
    
    ~StarUnit(){
        if (value_) {delete value_; value_ = NULL;}
    }

    void set_symbol(const std::string& symbol) {
        symbol_ = new base_logic::StringValue(symbol);
    }

    void set_gender(const int32 gender) {
        gender_ = new base_logic::FundamentalValue(gender);
    }

    void set_name(const std::string& name) {
        name_ = new base_logic::StringValue(name);
    }

    void set_pic(const std::string& pic) {
        pic_ = new base_logic::StringValue(pic);
    }

    void set_wid(const std::string& wid) {
        wid_ = new base_logic::StringValue(wid);
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (symbol_ != NULL)
            value_->Set(L"symbol",symbol_);
        if (gender_ != NULL)
            value_->Set(L"gender", gender_);
        if (name_ != NULL)
            value_->Set(L"name", name_);
        if (pic_ != NULL)
            value_->Set(L"pic", pic_);
        if (wid_ != NULL)
            value_->Set(L"wid", wid_);
        return value_;
    }
 private:
    base_logic::StringValue*         symbol_;
    base_logic::FundamentalValue*    gender_;
    base_logic::StringValue*         name_;
    base_logic::StringValue*         pic_;
    base_logic::StringValue*         wid_;
    base_logic::DictionaryValue*     value_;
};

class SearchAllStar {
public:
    SearchAllStar()
        : stars_info_(NULL),
          value_(NULL) {
              stars_info_ = new base_logic::ListValue;
    }

    ~SearchAllStar() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
    }

    void set_unit(base_logic::DictionaryValue* value) {
        stars_info_->Append((base_logic::Value*) (value));
    }

    base_logic::DictionaryValue* get() {
        value_ = new base_logic::DictionaryValue();
        if (!stars_info_->empty()) {
            value_->Set(L"starsinfo", stars_info_);
        } else {
            delete stars_info_;
            stars_info_ = NULL;
        }
        return value_;
    }

    const int32 Size() {
        return stars_info_->GetSize();
    }

    void Reset() {
        if (value_) {
            delete value_;
            value_ = NULL;
        }
        stars_info_ = new base_logic::ListValue;
    }
private:
    base_logic::ListValue* stars_info_;
    base_logic::DictionaryValue* value_;
};

}


}

#endif
