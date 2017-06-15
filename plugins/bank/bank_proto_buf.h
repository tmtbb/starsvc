//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#ifndef SWP_BANK_PROTO_BUF_H_
#define SWP_BANK_PROTO_BUF_H_

#include "basic/basictypes.h"
#include "logic/base_values.h"
#include <list>
#include <string>

namespace bank_logic {

namespace net_request {

//绑定银行卡
class BindBankCard {
 public:
   BindBankCard()
   :id_(NULL)
   ,token_(NULL)
   ,bank_id_(NULL)
   ,bank_name_(NULL)
   ,branch_bank_(NULL)
   ,card_no_(NULL)
   ,name_(NULL){}
  
    ~BindBankCard(){
      if (id_) {delete id_; id_ = NULL;}
      if (token_) {delete token_; token_ = NULL;}
      if (bank_id_) {delete bank_id_; bank_id_ = NULL;}
      if (bank_name_) {delete bank_name_; bank_name_ = NULL;}
      if (branch_bank_) {delete branch_bank_; branch_bank_ = NULL;}
      if (card_no_) {delete card_no_; card_no_ = NULL;}
      if (name_) {delete name_; name_ = NULL;}
    }


   bool set_http_packet(base_logic::DictionaryValue* value);

   void set_id(const int64 id) {
      id_ = new base_logic::FundamentalValue(id);
   }

   void set_token(const std::string& token) {
      token_ = new base_logic::StringValue(token);
   }

   void set_bank_id(const int32 bank_id) {
      bank_id_ = new base_logic::FundamentalValue(bank_id);
   }

   void set_bank_name(const std::string& bank_name) {
      bank_name_ =  new base_logic::StringValue(bank_name);
   }
   
   void set_branch_bank(const std::string& branch_bank) {
      branch_bank_ = new base_logic::StringValue(branch_bank);
   }
   
   void set_card_no(const std::string& card_no) {
      card_no_ = new base_logic::StringValue(card_no);   
   }

   void set_name(const std::string& name) {
      name_ = new base_logic::StringValue(name);
   }


   const int64 id() const {
      int64 id  = 0;
      if (id_)
        id_->GetAsBigInteger(&id);
     return id;
   }

  const std::string token() const {
      std::string token;
      if (token_)
	token_->GetAsString(&token);
      return token;
  }

  const int32 bank_id() const {
      int32 bank_id;
      if(bank_id_)
        bank_id_->GetAsInteger(&bank_id);
      return bank_id;
  }
   
  const std::string name() const{
     std::string name;
     if (name_)
        name_->GetAsString(&name);
     return name;
  }

  const std::string bank_name() const {
     std::string bank_name;
     if (bank_name_) {bank_name_->GetAsString(&bank_name);}
     return bank_name;
  }

  const std::string branch_bank() const {
     std::string branch_bank;
     if (branch_bank_){branch_bank_->GetAsString(&branch_bank);}
     return branch_bank; 
  }
  
  const std::string card_no() const {
    std::string card_no;
    if (card_no_) {card_no_->GetAsString(&card_no);}
    return card_no;
  }
 public:
   base_logic::FundamentalValue* id_;
   base_logic::StringValue* token_;
   base_logic::FundamentalValue* bank_id_;
   base_logic::StringValue*   bank_name_;
   base_logic::StringValue*   branch_bank_;
   base_logic::StringValue*   card_no_;
   base_logic::StringValue*   name_;
};

//获取银行卡名称
class GetBankName {
public:
  GetBankName()
    :id_(NULL),
     token_(NULL),
     card_no_(NULL) {}

  ~GetBankName(){
    if (id_) {delete id_; id_ = NULL;}
    if (token_) {delete token_; token_ = NULL;}
    if (card_no_) {delete card_no_; card_no_ = NULL;}

  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id){
     id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
     token_ = new base_logic::StringValue(token);
  }

  void set_card_no(const std::string& card_no) {
     card_no_  = new base_logic::StringValue(card_no);
  }

  const int64 id() const {
     int64 id = 0;
     if (id_)
         id_->GetAsBigInteger(&id);
    return  id;
  }

  const std::string token() const {
     std::string token;
     if (token_)
         token_->GetAsString(&token);
     return token;
  }

  const std::string card_no() const {
     std::string card_no;
     if(card_no_)
         card_no_->GetAsString(&card_no);
     return card_no;
  }
public:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue*      token_;
  base_logic::StringValue*      card_no_;
};

//解绑银行卡
class UnbindBankCard{
public:
  UnbindBankCard()
   :id_(NULL),
    token_(NULL),
    bank_card_id_(NULL),
    ver_code_(NULL){}

  ~UnbindBankCard() {
     if (id_){delete id_; id_ = NULL;}
     if (token_) {delete token_; token_ = NULL;}
     if (bank_card_id_) {delete bank_card_id_; bank_card_id_ = NULL;}
     if (ver_code_) {delete ver_code_; ver_code_ = NULL;}
  }

   bool set_http_packet(base_logic::DictionaryValue* value);

   void set_id(const int64 id) {
     id_ = new base_logic::FundamentalValue(id);
   }

   void set_token(const std::string& token) {
     token_ = new base_logic::StringValue(token);
   }

   void set_bank_card_id(const int64 bank_card_id) {
      bank_card_id_ = new base_logic::FundamentalValue(bank_card_id);
   }

   void set_ver_code(const std::string& ver_code) {
      ver_code_ = new base_logic::StringValue(ver_code);
   }

   const int64 id () const {
      int64 id = 0;
      if (id_) 
         id_->GetAsBigInteger(&id);
      return id;
   }

   const std::string token() const {
     std::string  token;
     if (token_)
         token_->GetAsString(&token);
     return  token;
   }

   const int64 bank_card_id() const {
      int64 bank_card_id;
      if (bank_card_id_)
        bank_card_id_->GetAsBigInteger(&bank_card_id);
      return bank_card_id;
   }

   const std::string ver_code() const {
      std::string ver_code;
      if (ver_code_)
         ver_code_->GetAsString(&ver_code);
      return ver_code;
   }

public:
  base_logic::FundamentalValue*  id_;
  base_logic::StringValue*       token_;
  base_logic::FundamentalValue*  bank_card_id_;
  base_logic::StringValue*       ver_code_;  
};

//获取银行卡列表
class BankCardInfo {
 public:
  BankCardInfo()
      : id_(NULL),
        token_(NULL),
        start_(NULL),
        count_(NULL) {
  }

  ~BankCardInfo() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }

    if (token_) {
      delete token_;
      token_ = NULL;
    }

    if (start_) {
      delete start_;
      start_ = NULL;
    }

    if (count_) {
      delete count_;
      count_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }
  void set_start(const int32 start) {
    start_ = new base_logic::FundamentalValue(start);
  }

  void set_count(const int32 count) {
    count_ = new base_logic::FundamentalValue(count);
  }

  const int64 id() const {
    int64 id = 0;
    if (id_)
      id_->GetAsBigInteger(&id);
    return id;
  }

  const std::string token() const {
    std::string token;
    if (token_)
      token_->GetAsString(&token);
    return token;
  }

  const int32 start() const {
    int32 start = 0;
    if (start_)
      start_->GetAsInteger(&start);
    return start;
  }

  const int32 count() const {
    int32 count = 0;
    if (count_)
      count_->GetAsInteger(&count);
    return count;
  }

 private:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* start_;
  base_logic::FundamentalValue* count_;
};

class WithDrawCash {
 public:
  WithDrawCash()
      : id_(NULL),
        token_(NULL),
        money_(NULL),
        card_id_(NULL),
        pwd_(NULL),
        commnet_(NULL) {
  }

  ~WithDrawCash() {
    if (id_) {
      delete id_;
      id_ = NULL;
    }
    if (token_) {
      delete token_;
      token_ = NULL;
    }
    if (money_) {
      delete money_;
      money_ = NULL;
    }
    if (card_id_) {
      delete card_id_;
      card_id_ = NULL;
    }
    if (pwd_) {
      delete pwd_;
      pwd_ = NULL;
    }
  }

  bool set_http_packet(base_logic::DictionaryValue* value);

  void set_id(const int64 id) {
    id_ = new base_logic::FundamentalValue(id);
  }

  void set_token(const std::string& token) {
    token_ = new base_logic::StringValue(token);
  }

  void set_card_id(const int64 card_id) {
    card_id_ = new base_logic::FundamentalValue(card_id);
  }

  void set_money(const double money) {
    money_ = new base_logic::FundamentalValue(money);
  }

  void set_password(const std::string& password) {
    pwd_ = new base_logic::StringValue(password);
  }

  void set_comment(const std::string& comment) {
    commnet_ = new base_logic::StringValue(comment);
  }

  const int64 id() const {
    int64 id = 0;
    if (id_)
      id_->GetAsBigInteger(&id);
    return id;
  }

  const std::string token() const {
    std::string token;
    if (token_)
      token_->GetAsString(&token);
    return token;
  }

  const int64 card_id() const {
    int64 card_id = 0;
    if (card_id_)
      card_id_->GetAsBigInteger(&card_id);
    return card_id;
  }

  const double money() const {
    double money = 0;
    if (money_)
      money_->GetAsReal(&money);
    return money;
  }

  const std::string password() const {
    std::string password;
    if (pwd_)
      pwd_->GetAsString(&password);
    return password;
  }

  const std::string comment() const {
    std::string comment;
    if (commnet_)
      commnet_->GetAsString(&comment);
    return comment;
  }

 private:
  base_logic::FundamentalValue* id_;
  base_logic::StringValue* token_;
  base_logic::FundamentalValue* money_;
  base_logic::FundamentalValue* card_id_;
  base_logic::StringValue* pwd_;
  base_logic::StringValue* commnet_;
};

}

namespace net_reply {

class BankCardInfo {
 public:
  BankCardInfo()
      : bid_(NULL),
        bank_(NULL),
        branch_bank_(NULL),
        card_no_(NULL),
        name_(NULL),
        card_name_(NULL),
        is_default_(NULL),
        value_(NULL) {
  }

  ~BankCardInfo() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_bid(const int64 bid) {
    bid_ = new base_logic::FundamentalValue(bid);
  }
  
  void set_card_name(const std::string& card_name) {
   card_name_ = new base_logic::StringValue(card_name);
  }
  void set_bank(const std::string& bank) {
    bank_ = new base_logic::StringValue(bank);
  }

  void set_branch_bank(const std::string& branch) {
    branch_bank_ = new base_logic::StringValue(branch);
  }

  void set_card_no(const std::string& card_no) {
    card_no_ = new base_logic::StringValue(card_no);
  }

  void set_name(const std::string& name) {
    name_ = new base_logic::StringValue(name);
  }

  void set_is_default(const int32 is_default) {
    is_default_ = new base_logic::FundamentalValue(is_default);
  } 

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (bid_ != NULL)
      value_->Set(L"bid", bid_);
    if (bank_ != NULL)
      value_->Set(L"bank", bank_);
    if (branch_bank_ != NULL)
      value_->Set(L"branchBank", branch_bank_);
    if (card_no_ != NULL)
      value_->Set(L"cardNo", card_no_);
   if (is_default_ != NULL)
      value_->Set(L"isDefault", is_default_); 
   if (name_ != NULL)
      value_->Set(L"name", name_);
   if (card_name_ != NULL)
      value_->Set(L"cardName", card_name_);
    return value_;
  }
 private:
  base_logic::FundamentalValue* bid_;
  base_logic::StringValue* bank_;
  base_logic::StringValue* branch_bank_;
  base_logic::StringValue* card_no_;
  base_logic::StringValue* card_name_;
  base_logic::StringValue* name_;
  base_logic::FundamentalValue* is_default_;
  base_logic::DictionaryValue* value_;

};

class AllBankCard {
 public:
  AllBankCard()
   : card_list_(NULL),
     value_(NULL){
    card_list_ = new base_logic::ListValue;
  }

  ~AllBankCard() {
    if (value_){
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    card_list_->Append((base_logic::Value*)(value));
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (!card_list_->empty()) {
      value_->Set(L"cardList", card_list_);
    } else {
      delete card_list_;
      card_list_ = NULL;
    }
    return value_;
  }

  const int32 Size() {
    return card_list_->GetSize();
  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    card_list_ = new base_logic::ListValue;
  }

 private:
  base_logic::ListValue*  card_list_;
  base_logic::DictionaryValue* value_;
};

class BindBankCard {
 public:
   BindBankCard()
      :bank_card_id_(NULL)
      ,bank_id_(NULL)
      ,bank_name_(NULL)
      ,branch_bank_name_(NULL)
      ,card_no_(NULL)
      ,card_name_(NULL)
      ,name_(NULL)
      ,value_(NULL) {}

  ~BindBankCard(){
    if (value_){
      delete value_;
      value_ = NULL;
    }   
  }

  void set_bank_card_id(const int64 card_id){
    bank_card_id_ = new base_logic::FundamentalValue(card_id);
  }

  void set_bank_id(const int32 bank_id) {
   bank_id_ = new base_logic::FundamentalValue(bank_id); 
  }

  void set_bank_name(const std::string& bank_name) {
   bank_name_ = new base_logic::StringValue(bank_name);
  }

  void set_branch_bank_name(const std::string& branch_bank_name) {
    branch_bank_name_ = new base_logic::StringValue(branch_bank_name);
  }

  void set_card_no(const std::string& card_no) {
    card_no_ = new base_logic::StringValue(card_no);
  }

  void set_name(const std::string& name) {
    name_ = new base_logic::StringValue(name);
  }

  void set_card_name(const std::string& card_name) {
   card_name_ = new base_logic::StringValue(card_name);
  }

  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue;
    if (bank_card_id_ != NULL)
       value_->Set(L"bid", bank_card_id_);
   if (bank_id_ != NULL)
       value_->Set(L"bankId", bank_id_);
   if (bank_name_ != NULL)
       value_->Set(L"bankName",bank_name_);
   if (branch_bank_name_ != NULL)
       value_->Set(L"brachName", branch_bank_name_);
   if (card_no_ != NULL)
       value_->Set(L"cardNO", card_no_);
   if (name_ != NULL)
      value_->Set(L"name", name_);
   if (card_name_ != NULL)
      value_->Set(L"cardName", card_name_);
   return value_;
  }

 private:
  base_logic::FundamentalValue*  bank_card_id_;
  base_logic::FundamentalValue*  bank_id_;
  base_logic::StringValue*       bank_name_;
  base_logic::StringValue*       branch_bank_name_;
  base_logic::StringValue*       card_no_;
  base_logic::StringValue*       card_name_;
  base_logic::StringValue*       name_;
  base_logic::DictionaryValue*   value_;
};

class BankCardNOInfo {
 public:
   BankCardNOInfo()
    :bank_id_(NULL)
    ,card_no_(NULL)
    ,card_name_(NULL)
    ,bank_name_(NULL)
    ,value_(NULL) {}
  
  ~BankCardNOInfo(){
    if (value_){delete value_; value_ = NULL;}
  }  

  void set_bank_id(const int32 bank_id){
    bank_id_ = new base_logic::FundamentalValue(bank_id);
  } 
  
  void set_card_no(const std::string& card_no) {
    card_no_ = new base_logic::StringValue(card_no);
  }

  void set_card_name(const std::string& card_name) {
    card_name_ = new base_logic::StringValue(card_name);
  }
  
  void set_bank_name(const std::string& bank_name) {
    bank_name_ = new base_logic::StringValue(bank_name);
  }

  base_logic::DictionaryValue* get() {
     value_ = new base_logic::DictionaryValue();
     if (bank_id_ != NULL)
         value_->Set(L"bankId", bank_id_);
     if (card_no_ != NULL)
         value_->Set(L"cardNO", card_no_);
     if (card_name_ != NULL)
         value_->Set(L"cardName", card_name_);
     if (bank_name_ != NULL)
         value_->Set(L"bankName", bank_name_);
     return value_;    
  }


 private:
   base_logic::FundamentalValue*  bank_id_;
   base_logic::StringValue*       card_no_;
   base_logic::StringValue*       card_name_;
   base_logic::StringValue*       bank_name_;
   base_logic::DictionaryValue*   value_;
};

class UnbindBankCard {
 public:
   UnbindBankCard()
     :result_(NULL)
     ,value_(NULL) {}
   
   ~UnbindBankCard(){
     if (value_){delete value_; value_ = NULL;}
   }

   void set_result(const int32 result) {
     result_ = new base_logic::FundamentalValue(result);
   }  

   base_logic::DictionaryValue* get() {
      value_ = new base_logic::DictionaryValue;
      if (result_ != NULL)
         value_->Set(L"result", result_);
      return value_;
   }
 private:
   base_logic::FundamentalValue* result_;
   base_logic::DictionaryValue*  value_;
};

class WithDrawCash {
 public:
  WithDrawCash()
     :result_(NULL)
     ,balance_(NULL)
     ,value_(NULL) {}

  ~WithDrawCash() {
    if (value_) {delete value_; value_ = NULL;}
   }

   void set_result(const int32 result) {
     result_ = new base_logic::FundamentalValue(result);
   }

   void set_balance(const double balance) {
     balance_ = new base_logic::FundamentalValue(balance);
   }
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue;
    if (result_ != NULL)
       value_->Set(L"result", result_);
    if (balance_ != NULL)
       value_->Set(L"balance", balance_);
    return value_;
  }

 private:
   base_logic::FundamentalValue* result_;
   base_logic::FundamentalValue* balance_;
   base_logic::DictionaryValue*  value_;
};


}

}

#endif
