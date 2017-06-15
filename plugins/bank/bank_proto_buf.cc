//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年1月12日 Author: kerry

#include "bank_proto_buf.h"
#include "basic/basic_util.h"

namespace bank_logic {

namespace net_request {

bool BindBankCard::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  int32 bank_id = 0;
  int64 big_bank_id = 0;
  std::string bank_name;
  std::string branch_bank;
  std::string card_no;
  std::string name;

  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetBigInteger(L"bankId", &big_bank_id);
  if (r){
    bank_id = big_bank_id;
    set_bank_id(bank_id);
  }
  else 
    return false;

  r = value->GetString(L"bankName", &bank_name);
  if (r)
    set_bank_name(bank_name);
  else
    return false;

  r = value->GetString(L"branchBank", &branch_bank);
  if (r)
    set_branch_bank(branch_bank);
  else
    return false;

  r = value->GetString(L"cardNO", &card_no);
  if (r)
    set_card_no(card_no);
  else
    return false;

  r = value->GetString(L"name", &name);
  if (r)
    set_name(name);
  else
    return false;


  return true;
}

bool UnbindBankCard::set_http_packet(base_logic::DictionaryValue* value) {
   bool r = false;
   int64 id = 0;
   int64 big_bank_card_id;
   int32 bank_card_id;
   std::string token;
   std::string ver_code;
   r = value->GetBigInteger(L"id", &id);
   if (r)
     set_id(id);
   else 
     return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetBigInteger(L"bankCardId", &big_bank_card_id);
  if (r){
     bank_card_id = big_bank_card_id;
     set_bank_card_id(bank_card_id);
  }else{
     return false;
  }

  r = value->GetString(L"verCode",&ver_code);
  if (r)
   set_ver_code(ver_code);
  return true;
}

bool GetBankName::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  std::string card_no;
  r = value->GetBigInteger(L"id", &id);
  if (r)
     set_id(id);
  else 
     return false;

  r = value->GetString(L"token", &token);
  if (r)
     set_token(token);
  else 
     return false;

  r = value->GetString(L"cardNo", &card_no);
  if (r)
    set_card_no(card_no);
  else
    return false;

 return true;
 
}



bool BankCardInfo::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  std::string token;
  int64 bigstart = 0;
  int32 start = 0;
  int64 bigcount = 0;
  int32 count = 0;
  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetBigInteger(L"start", &bigstart);
  if (r) {
    start = bigstart;
    set_start(start);
  } else
    set_start(0);

  r = value->GetBigInteger(L"count", &bigcount);
  if (r) {
    count = bigcount;
    set_count(count);
  } else
    set_count(10);

  return true;
}

bool WithDrawCash::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 id = 0;
  int64 card_id = 0;
  std::string token;
  std::string comment;
  double money;
  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_id(id);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  r = value->GetBigInteger(L"cardId", &card_id);
  if (r)
    set_card_id(card_id);
  else
    return false;

  //要区分整数和小数情况
  r = value->GetReal(L"money", &money);
  if (r)
    set_money(money);
  else {
    int64 bigmoney = 0;
    r = value->GetBigInteger(L"money", &bigmoney);
    if (r) {
      money = bigmoney;
      set_money(money);
    } else
      return false;
  }

  r = value->GetString(L"comment", &comment);

  return true;

}

}

namespace net_reply {

}

}
