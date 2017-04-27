// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// UnionpayOrder.cc
// Created on: 2016年11月1日.
// Author: Paco.
#include "unionpay_order.h"

#include <iostream>
#include <sstream>
#include "basic/md5sum.h"
#include "http/http_method.h"
//#include "glog/logging.h"
#include "util/util.h"
#include "logic/logic_comm.h"

//const char* UnionpayOrder::biz_type_ = "000201";
//const char* UnionpayOrder::txn_sub_type_ = "01";
const char* UnionpayOrder::mer_id_ = "777290058110048";

UnionpayOrder::UnionpayOrder() {
  total_fee_ = 0;
  price_ = 0.0;
}

UnionpayOrder::~UnionpayOrder() {
}

/*void UnionpayOrder::InitWxVerify() {
  biz_type = 
  appid = APPID;
  mch_id = MCH_ID;
  notify_url = NOTIFY_URL;
  trade_type = TRADE_TYPE;
  package = WX_PACKAGE;
  key = KEY;
  nonce_str = util::RandomString(32);
  out_trade_no += util::RandomString(6);
  }

void UnionpayOrder::PlaceOrderSign() {
  std::stringstream ss;
  ss << "bizType=" << "mch_id=" << mch_id
     << "&nonce_str=" << nonce_str << "&notify_url=" << notify_url
     << "&out_trade_no=" << out_trade_no << "&spbill_create_ip="
     << spbill_create_ip << "&total_fee=" << total_fee << "&trade_type="
     << trade_type << "&key=" << key;
  LOG_DEBUG2("WX_ORDER_SIGN_MD5 after: %s",md5sum.GetHash().c_str());
  base::MD5Sum md5sum(ss.str());
  LOG_DEBUG2("WX_ORDER_SIGN_MD5 after: %s",md5sum.GetHash().c_str());
  sign = md5sum.GetHash();
}

void UnionpayOrder::PreSign() {
  //重新赋值 nonce_str
  nonce_str = util::RandomString(32);
  std::stringstream ss;
  ss << time(NULL);
  timestamp = ss.str();
  ss.str("");
  ss.clear();
  ss << "appid=" << appid << "&noncestr=" << nonce_str
     << "&package=Sign=WXPay" << "&partnerid=" << mch_id << "&prepayid="
     << prepayid << "&timestamp=" << timestamp << "&key=" << key;
  LOG_DEBUG2("WX_ORDER_SIGN before: %s",ss.str().c_str());
  base::MD5Sum md5sum(ss.str());
  LOG_DEBUG2("WX_ORDER_SIGN_MD5 after: %s",md5sum.GetHash().c_str());
  prepaysign = md5sum.GetHash();
}

std::string UnionpayOrder::PostFiled() {
  DicValue dic;
  dic.SetString(L"appid", appid);
  dic.SetString(L"body", body);
  dic.SetString(L"mch_id", mch_id);
  dic.SetString(L"nonce_str", nonce_str);
  dic.SetString(L"notify_url", notify_url);
  dic.SetString(L"out_trade_no", out_trade_no);
  dic.SetString(L"spbill_create_ip", spbill_create_ip);
  dic.SetBigInteger(L"total_fee", total_fee);
  dic.SetString(L"trade_type", trade_type);
  dic.SetString(L"sign", sign);
  std::string filed = "";
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
      base_logic::IMPL_XML, &filed);
  serializer->Serialize(dic);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_XML,
                                                serializer);
  return filed;
}

std::string UnionpayOrder::PlaceOrder() {
  InitWxVerify();
  PlaceOrderSign();
  http::HttpMethodPost hmp(WX_URL);
  std::string headers = "Content-Type: text/xml";
  hmp.SetHeaders(headers);
  hmp.Post(PostFiled().c_str());
  std::string result;
  hmp.GetContent(result);
  LOG(INFO)<< "http post result:" << result;
  return result;
  }*/

std::string UnionpayOrder::PlaceOrder() {
  std::stringstream req_url;
  time_t seconds = time(0);
  struct tm* now = localtime(&seconds);
  std::stringstream ss;
  char time_buf[128];
  snprintf(time_buf, 128, "%04d%02d%02d%02d%02d%02d", now->tm_year + 1900, now->tm_mon + 1, now->tm_mday,
           now->tm_hour, now->tm_min, now->tm_sec);
  //LOG(INFO) << "time now:" << time_buf;
  LOG_DEBUG2("time now: %s",time_buf);
  std::string orderId = time_buf;
  std::string time_send = orderId;
  req_url << ACP_SERVER_URL << "?merId=" << mer_id_
          << "&txnTime=" << time_send << "&orderId="
          << orderId << "&txnAmt=" << price_;
  LOG_DEBUG2("req url: %s",req_url.str().c_str());
  http::HttpMethodGet hmp(req_url.str());
  //std::string headers = "Content-Type: text/xml";
  //hmp.SetHeaders(headers);
  hmp.Get();
  std::string result;
  hmp.GetContent(result);
  //LOG(INFO)<< "http get result:" << result;
  LOG_DEBUG2("http get result:: %s",result.c_str());
  return result;
}


/*void UnionpayOrder::PreSerialize(DicValue* dic) {
  if (dic != NULL) {
    dic->SetString(L"appid", appid);
    dic->SetString(L"partnerid", mch_id);
    dic->SetString(L"prepayid", prepayid);
    dic->SetString(L"package", package);
    dic->SetString(L"noncestr", nonce_str);
    dic->SetString(L"timestamp", timestamp);
    dic->SetString(L"sign", prepaysign);
  }
  }*/

