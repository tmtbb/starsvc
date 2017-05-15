//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年1月12日 Author: kerry

#include "pay_proto_buf.h"
#include "basic/basic_util.h"
#include "logic/logic_comm.h"

//临时使用
std::string GetXmlValue(std::string value) {
  int npos1 = value.find("<![CDATA[");
  int npos2 = value.find("]]>");
  return value.substr(npos1 + 9, npos2 - npos1 - 9);
}

namespace pay_logic {

namespace net_request {

bool WXPayServer::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  std::string err_str;
  std::string xml_str;
  std::string appid;
  std::string mch_id;
  std::string total_fee;
  std::string transaction_id;
  std::string result_code;
  std::string return_code;
  if (value == NULL)
    return false;
  r = value->GetString(L"result", &xml_str);
  if (!r)
    return false;

  if (!r || xml_str.length() <= 5)
    return false;

  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &xml_str);
  base_logic::DictionaryValue* xml_value =
      (base_logic::DictionaryValue*) deserializer->Deserialize(&err, &err_str);

  r = xml_value->GetString(L"appid", &appid);
  if (r)
    set_appid(GetXmlValue(appid));
  else
    return false;

  r = xml_value->GetString(L"mch_id", &mch_id);
  if (r)
    set_mch_id(GetXmlValue(mch_id));
  else
    return false;

  r = xml_value->GetString(L"return_code", &return_code);
  if (!r)
    return false;
  if (return_code.find("SUCCESS") == std::string::npos)
    return false;

  r = xml_value->GetString(L"total_fee", &total_fee);
  if (r)
    set_total_fee(atoll(total_fee.c_str()));
  else
    return false;

  r = xml_value->GetString(L"transaction_id", &transaction_id);
  if (r)
    set_transaction_id(GetXmlValue(transaction_id));
  else
    return false;

  std::string out_trade_no;
  r = xml_value->GetString(L"out_trade_no", &out_trade_no);
  if (r) {
    set_recharge_id(atoll(GetXmlValue(out_trade_no).c_str()));
  }

  r = xml_value->GetString(L"result_code", &result_code);
  if (!r)
    return false;
  //支付成功标识
  if (result_code.find("SUCCESS") != std::string::npos) {
    set_pay_result(1);
  } else {
    set_pay_result(-1);
  }
  LOG_DEBUG2("%s",xml_str.c_str());
  return true;
}

bool WXPayClient::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  int64 uid;
  int64 rid;
  int64 big_pay_result;
  int32 pay_result;
  double price;
  if (value == NULL)
    return false;
  r = value->GetBigInteger(L"id", &uid);
  if (r)
    set_uid(uid);
  else
    return false;

  r = value->GetBigInteger(L"rid", &rid);
  if (r)
    set_rid(rid);
  else
    return false;

  r = value->GetBigInteger(L"payResult", &big_pay_result);
  if (r) {
    pay_result = big_pay_result;
    set_pay_result(pay_result);
  } else
    return false;

  return true;
}

bool WXPayOrder::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  int64 uid;
  std::string title;
  std::string open_id;
  int32 pay_type = 0;
  int64 big_pay_type = 0;
  double price;
  if (value == NULL)
    return false;
  r = value->GetBigInteger(L"id", &uid);
  if (r)
    set_uid(uid);
  else
    return false;

  r = value->GetString(L"title", &title);
  if (r)
    set_title(title);
  else
    return false;

  r = value->GetReal(L"price", &price);
  if (r)
    set_price(price);
  else {
    int64 big_price = 0;
    r = value->GetBigInteger(L"price", &big_price);
    if (!r)
      return false;
    price = big_price;
    set_price(price);
  }

  r = value->GetBigInteger(L"pay_type", &big_pay_type);
  if (r){
    pay_type = big_pay_type;
  }else{
    pay_type = 0;
  }
  set_pay_type(pay_type);

  r = value->GetString(L"openid", &open_id);
  if (r)
    set_open_id(open_id);

  return true;
}


bool SHFJPayOrder::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  int64 uid;
  std::string temp;
  int32 pay_type = 0;
  int64 big_pay_type = 0;
  double amount;
  if (value == NULL)
    return false;

  r = value->GetBigInteger(L"id", &uid);
  if (r)
    set_uid(uid);
  else
    return false;

  r = value->GetString(L"merchantNo", &temp);
  if (r)
    set_merchant_no(temp);
  //else
   // return false;

  r = value->GetReal(L"amount", &amount);
  if (r)
    set_amount(amount);
  else {
    int64 big_amount = 0;
    r = value->GetBigInteger(L"amount", &big_amount);
    if (!r)
      return false;
    amount = big_amount;
    set_amount(amount);
  }

  r = value->GetString(L"payType", &temp);
  if (r)
    set_pay_type(temp);
  else
    return false;
  
  r = value->GetString(L"wechatOpenId", &temp);
  if (r)
    set_wechat_openid(temp);
  //else
  //  return false;

  r = value->GetString(L"wechatAppId", &temp);
  if (r)
    set_wechat_appid(temp);
  //else
   // return false;

  r = value->GetString(L"currency", &temp);
  if (r)
    set_currency(temp);
  //else
    //return false;

  r = value->GetString(L"outTradeNo", &temp);
  if (r)
    set_out_trade_no(temp);
  //else
    //return false;
  //r = value->GetString(L"content", &temp);
  //if (r)
  temp = "航空润滑油"; 
  set_content(temp);

  return true;
}


bool SHFJCashOrder::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  int64 uid;
  std::string temp;
  int32 rec_bank_name = 0;
  int64 big_rec_bank_name = 0;
  double amount;
  if (value == NULL)
    return false;

  r = value->GetBigInteger(L"id", &uid);
  if (r)
    set_uid(uid);
  else
    return false;

  r = value->GetBigInteger(L"bid", &uid);
  if (r)
    set_bid(uid);
  else
    return false;
  r = value->GetReal(L"amount", &amount);
  if (r)
    set_amount(amount);
  else {
    int64 big_amount = 0;
    r = value->GetBigInteger(L"amount", &big_amount);
    if (!r)
      return false;
    amount = big_amount;
    set_amount(amount);
  }

  r = value->GetString(L"receiverBankName", &temp);
  if (r)
    set_rec_bank_name(temp);
  else
    return false;

  r = value->GetString(L"receiverBranchBankName", &temp);
  if (r)
    set_rec_branch_bank_name(temp);
  else
    return false;

  r = value->GetString(L"receiverCardNo", &temp);
  if (r)
    set_rec_card_no(temp);
  else
    return false;
  r = value->GetString(L"receiverAccountName", &temp);
  if (r)
    set_rec_account_name(temp);

  return true;
}


bool SHFJPayServer::set_http_packet(base_logic::DictionaryValue* value) {
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________%s", "test");
  int32 err = 0;
  bool r = false;
  std::string tmp;

  if (value == NULL)
    return false;
//
  r = value->GetString(L"merchantNo", &tmp);
  if (r)
    set_mch_id(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________merchantNo[%s]", tmp.c_str());
  //else
   // return false;
   r = value->GetString(L"tradeNo", &tmp);
   if (r)
     set_trade_no(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ tradeNo[%s]", tmp.c_str());
  // else
   //  return false;
   r = value->GetString(L"outTradeNo", &tmp);
   if (r)
     set_out_trade_no(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ outTradeNo[%s]", tmp.c_str());
  // else
   //  return false;
   r = value->GetString(L"outContext", &tmp);
   if (r)
     set_out_context(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ outContext[%s]", tmp.c_str());
   //else
    // return false;
   r = value->GetString(L"payType", &tmp);
   if (r)
     set_pay_type(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ payType[%s]", tmp.c_str());
   //else
    // return false;
   r = value->GetString(L"currency", &tmp);
   if (r)
     set_currency(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ currency[%s]", tmp.c_str());
   //else
    // return false;
   r = value->GetString(L"status", &tmp);
   if (r)
     set_status(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ status[%s]", tmp.c_str());
   //else
    // return false;     
   r = value->GetString(L"settleType", &tmp);
   if (r)
     set_settle_type(tmp);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ settleType[%s]", tmp.c_str());
  // else
    // return false;
/*
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &xml_str);
  base_logic::DictionaryValue* json_value =
      (base_logic::DictionaryValue*) deserializer->Deserialize(&err, &err_str);
*/

/*
  r = xml_value->GetString(L"return_code", &return_code);
  if (!r)
    return false;
  if (return_code.find("SUCCESS") == std::string::npos)
    return false;
*/
  int64 tmp_i = 0;
  r = value->GetBigInteger(L"amount", &tmp_i);
  if (r)
    set_amount(tmp_i);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ amount[%d]", tmp_i);
  //else
   // return false;

  r = value->GetBigInteger(L"payedAmount", &tmp_i);
  if (r && tmp.length()>0)
    set_payed_amount(tmp_i);
  LOG_DEBUG2("SHFJPayServer_____set_http_packet___________ payedAmount[%d]", tmp_i);
  //else
   // return false;

  //LOG_DEBUG2("%s",xml_str.c_str());
  return true;
}


bool SHFJCashServer::set_http_packet(base_logic::DictionaryValue* value) {
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________%s", "test");
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________%s", "test");
//
  std::string xml_str; 
  value->GetString(L"result", &xml_str);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________xml_str|%s", xml_str.c_str());
//
  int32 err = 0;
  bool r = false;
  std::string tmp;

  if (value == NULL)
    return false;
//
  r = value->GetString(L"merchantNo", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________merchantNo|%s", tmp.c_str());
  if (r)
    set_mch_id(tmp);
  //else
   // return false;
   r = value->GetString(L"payNo", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ payNo|%s", tmp.c_str());
   if (r)
     set_pay_no(tmp);
   //else
    // return false;
   r = value->GetString(L"outPayNo", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ outPayNo|%s", tmp.c_str());
   if (r)
     set_out_pay_no(tmp);
   else
     return false;
   r = value->GetString(L"errorCode", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ errorCode|%s", tmp.c_str());
   if (r)
     set_err_code(tmp);
     
   r = value->GetString(L"errorMsgString", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ errorMsg|%s", tmp.c_str());
   if (r)
     set_err_msg(tmp);
     
   r = value->GetString(L"status", &tmp);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ status|%s", tmp.c_str());
   if (r)
     set_status(tmp);
   //else
    // return false;     
/*
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &xml_str);
  base_logic::DictionaryValue* json_value =
      (base_logic::DictionaryValue*) deserializer->Deserialize(&err, &err_str);
*/

/*
  r = xml_value->GetString(L"return_code", &return_code);
  if (!r)
    return false;
  if (return_code.find("SUCCESS") == std::string::npos)
    return false;
*/
  int64 tmp_i;
  r = value->GetBigInteger(L"amount", &tmp_i);
  LOG_DEBUG2("SHFJCashServer_____set_http_packet___________ amount|%d", tmp_i);
  if (r)
    set_amount(tmp_i);
  //else
   // return false;

  r = value->GetBigInteger(L"transferAmount", &tmp_i);
  if (r)
    set_transfer_amount(tmp_i);
  //else
   // return false;

  r = value->GetBigInteger(L"fee", &tmp_i);
  if (r)
    set_fee(tmp_i);
  //else
   // return false;

  LOG_DEBUG2("________________________end_________________________%s","llllllllllllllllll");
  return true;
}

}

}
