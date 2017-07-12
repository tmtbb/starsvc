//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "pay/schduler_engine.h"
#include "pay/operator_code.h"
#include "pay/wx_order.h"
#include "pay/aplipay_clt.h"
#include "pay/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

namespace pay_logic {

PayManager* PayEngine::schduler_mgr_ = NULL;
PayEngine* PayEngine::schduler_engine_ = NULL;

PayManager::PayManager() {
  pay_cache_ = new PayCache();
  Init();
}

PayManager::~PayManager() {
  DeinitThreadrw(lock_);

}

void PayManager::Init() {
  InitThreadrw(&lock_);
}

void PayManager::InitDB(pay_logic::PayDB* pay_db) {
  pay_db_ = pay_db;
}

void PayManager::InitSchdulerEngine(
    manager_schduler::SchdulerEngine* schduler_engine) {
  schduler_engine_ = schduler_engine;
}


//--------------------------WX_ORDER--------------------------------------

bool PayManager::OnWXCreateOrder(const int socket, const int64 session,
                                 const int32 reversed, const int64 uid,
                                 const std::string& title, const double price,
                                 const int32 pay_type,const std::string& open_id) {
  //创建订单号
  pay_logic::WXOrder wx_order;
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();
  base_logic::DictionaryValue recharge_dic;
  bool r = WXOrder(socket, title, rid, price, pay_type,open_id, wx_order);
  if (!r) {
    send_error(socket, ERROR_TYPE, WX_ORDER_ERROR, session);
    return false;
  }

  r = pay_db_->OnCreateRechargeOrder(uid, rid, price, WX_APP);
  if (!r) {
    send_error(socket, ERROR_TYPE, STOAGE_ORDER_ERROR, session);
    return r;
  }
  pay_logic::net_reply::WXPayOrder r_wx_order;
  struct PacketControl packet_control;

  std::string package;
  if (0 == pay_type){
    package = wx_order.get_package();
  }else {
    package = "prepay_id=" + wx_order.get_prepayid();
  }
  MAKE_HEAD(packet_control, S_WEIXIN_PAY, USER_TYPE, 0, session, 0);
  r_wx_order.set_appid(wx_order.get_appid());
  r_wx_order.set_partnerid(wx_order.get_partnerid());
  r_wx_order.set_prepayid(wx_order.get_prepayid());
  r_wx_order.set_packetage(package);
  r_wx_order.set_noncestr(wx_order.get_nonce_str());
  r_wx_order.set_timestamp(wx_order.get_timestamp());
  r_wx_order.set_sign(wx_order.get_prepaysign());
  r_wx_order.set_rid(rid);
  packet_control.body_ = r_wx_order.get();
  send_message(socket, &packet_control);
  return true;
}

bool PayManager::OnAliPayCreateOrder(const int socket, const int64 session,
                                 const int32 reversed, const int64 uid,
                                 const std::string& title, const double price,
                                 const int32 pay_type,const std::string& open_id) {
  //创建订单号
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();

  /*
  pay_logic::WXOrder wx_order;
  base_logic::DictionaryValue recharge_dic;
  bool r = WXOrder(socket, title, rid, price, pay_type,open_id, wx_order);
  if (!r) {
    send_error(socket, ERROR_TYPE, WX_ORDER_ERROR, session);
    return false;
  }

  */
  bool r = pay_db_->OnCreateRechargeOrder(uid, rid, price, ALIPAY); //
  if (!r) {
    send_error(socket, ERROR_TYPE, STOAGE_ORDER_ERROR, session);
    return r;
  }
  pay_logic::AliPayContent  content;
  content.set_total_amount(base::BasicUtil::StringUtil::DoubleToString(price));
  content.set_product_code( "QUICK_MSECURITY_PAY");
  content.set_out_trade_no(base::BasicUtil::StringUtil::Int64ToString(rid));
  content.set_subject(title);

  std::string body_stream;
  base_logic::DictionaryValue *value = content.get();
  base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
              base_logic::IMPL_JSON);
  if (engine == NULL) {
      LOG_ERROR("engine create null");
      return false;
  }
  r = engine->Serialize((*value), &body_stream);

  if (engine) {
    delete engine;
    engine = NULL;
  }
  //__生成orderinfo
  //
  pay_logic::ApliPayOrder aplipay;
  LOG_ERROR2("body_stream[%s]", body_stream.c_str());
  //body_stream = "{\"out_trade_no\":\"5830161767537791357\",\"product_code\":\"QUICK_MSECURITY_PAY\",\"total_amount\":\"0.01\",\"subject\":\"title1\"}";
  std::string orderinfo = aplipay.PlaceOrderSign(body_stream);

  
  pay_logic::net_reply::AliPayOrder r_alipay_order;
  struct PacketControl packet_control;

  std::string package;

  MAKE_HEAD(packet_control, S_ALIPAY_PAY, PAY_TYPE, 0, session, 0);
  r_alipay_order.set_partnerid(orderinfo);
  r_alipay_order.set_prepayid(base::BasicUtil::StringUtil::Int64ToString(rid));

  /*
  r_alipay_order.set_appid(wx_order.get_appid());
  r_alipay_order.set_partnerid(wx_order.get_partnerid());
  r_alipay_order.set_packetage(package);
  r_alipay_order.set_noncestr(wx_order.get_nonce_str());
  r_alipay_order.set_timestamp(wx_order.get_timestamp());
  r_alipay_order.set_sign(wx_order.get_prepaysign());
  */
  //r_alipay_order.set_rid(rid);
  packet_control.body_ = r_alipay_order.get();
  send_message(socket, &packet_control);
  return true;
}
bool PayManager::WXOrder(const int socket, const std::string& title,
                         const int64 rid, const double price,
                         const int32 pay_type, const std::string& open_id,
                         pay_logic::WXOrder& wx_order) {

  std::string prepay_id;
  std::string ip;
  std::string app_id;
  std::string mch_id;
  std::string trade_type;
  std::string key;
  int port;
  if (logic::SomeUtils::GetIPAddress(socket, ip, port))
    wx_order.set_spbill_create_ip(ip);
  if (0 == pay_type) {
    app_id = APPID;
    mch_id = MCH_ID;
    trade_type = APP_TRADE_TYPE;
    key = APP_KEY;
  } else if (1 == pay_type) {
    app_id = MPID;
    mch_id = MPMCH_ID;
    trade_type = MP_TRADE_TYPE;
    key = MP_KEY;
  }
  wx_order.set_body(title);
  std::string str_rid = base::BasicUtil::StringUtil::Int64ToString(rid);
  wx_order.set_out_trade_no(str_rid);
  wx_order.set_total_fee(price * 100);
  std::string wx_result = wx_order.PlaceOrder(app_id, mch_id, trade_type, key,
                                              pay_type, open_id);
  bool r = ParserWXOrderResult(wx_result, prepay_id);
  if (!r)
    return false;
  wx_order.set_prepayid(prepay_id);
  wx_order.PreSign();
  return true;
}

bool PayManager::ParserWXOrderResult(std::string& result,
                                     std::string& prepay_id) {
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &result);
  std::string err_str;
  int32 err = 0;
  bool r = false;
  base_logic::DictionaryValue* dic = (DicValue*) deserializer->Deserialize(
      &err, &err_str);
  if (dic == NULL)
    return false;
  std::string return_code;
  r = dic->GetString(L"return_code", &return_code);
  if (!r)
    return false;
  //下单成功
  if (return_code.find("SUCCESS") == std::string::npos)
    return false;
  std::string result_code;
  r = dic->GetString(L"result_code", &result_code);
  if (!r)
    return false;
  if (result_code.find("SUCCESS") == std::string::npos)
    return false;

  r = dic->GetString(L"prepay_id", &prepay_id);
  if (!r)
    return false;
  int npos1 = prepay_id.find("<![CDATA[");
  int npos2 = prepay_id.find("]]>");
  prepay_id = prepay_id.substr(npos1 + 9, npos2 - npos1 - 9);

  //
  return true;
}

bool PayManager::OnWXClient(const int socket, const int64 session,
                            const int32 reversed, const int64 uid,
                            const int64 rid, const int32 pay_result) {

  bool r = pay_db_->OnUpdateRechargeOrder(uid, rid, pay_result, WX_APP);
  return true;
}

bool PayManager::OnWXServer(const int socket, const std::string& appid,
                            const std::string& mch_id, const int64 total_fee,
                            const int64 rid, const int64 result,
                            const std::string& transaction_id) {
  int64 uid = 0;
  double balance = 0.0;
  double price = ((double) total_fee) / 100;
  bool r = pay_db_->OnUpdateCallBackRechargeOrder(rid, price, transaction_id,
                                                  result, uid, balance);

  const std::string r_rt =
      "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
  send_full(socket, r_rt.c_str(), r_rt.length());

  if (r) {
    star_logic::UserInfo user;
    schduler_engine_->GetUserInfoSchduler(uid, &user);
    user.set_balance(balance);
    pay_logic::net_reply::Balance net_balance;
    net_balance.set_balance(balance);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_NOTICE_SVC, PAY_TYPE, 0, 0, 0);
    packet_control.body_ = net_balance.get();
    send_message(user.socket_fd(), &packet_control);

  }
  return true;
}
//-------------union pay 

bool PayManager::OnUnionPayCreateOrder(const int socket, const int64 session,
                                 const int32 reversed, const int64 uid,
                                 const std::string& title, const double price,
                                 const int32 pay_type,const std::string& open_id) {
  //创建订单号
  pay_logic::UnionpayOrder unionpay_order;
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();
  base_logic::DictionaryValue recharge_dic;
  bool r = UnionPayOrder(socket, title, rid, price, pay_type,open_id, unionpay_order);
  if (!r) {
    send_error(socket, ERROR_TYPE, UNIPNPAY_ORDER_ERROR, session);
    return false;
  }

  r = pay_db_->OnCreateRechargeOrder(uid, rid, price, UNIPNPAY);
  if (!r) {
    send_error(socket, ERROR_TYPE, STOAGE_ORDER_ERROR, session);
    return r;
  }
/*
  pay_logic::net_reply::WXPayOrder r_wx_order;
  struct PacketControl packet_control;

  std::string package;
  if (0 == pay_type){
    package = wx_order.get_package();
  }else {
    package = "prepay_id=" + wx_order.get_prepayid();
  }
  MAKE_HEAD(packet_control, S_WEIXIN_PAY, USER_TYPE, 0, session, 0);
  r_wx_order.set_appid(wx_order.get_appid());
  r_wx_order.set_partnerid(wx_order.get_partnerid());
  r_wx_order.set_prepayid(wx_order.get_prepayid());
  r_wx_order.set_packetage(package);
  r_wx_order.set_noncestr(wx_order.get_nonce_str());
  r_wx_order.set_timestamp(wx_order.get_timestamp());
  r_wx_order.set_sign(wx_order.get_prepaysign());
  r_wx_order.set_rid(rid);
  packet_control.body_ = r_wx_order.get();
  send_message(socket, &packet_control);
*/
  return true;
}

bool PayManager::UnionPayOrder(const int socket, const std::string& title,
                         const int64 rid, const double price,
                         const int32 pay_type, const std::string& open_id,
                         pay_logic::UnionpayOrder& unionpay_order) {

  //std::string ip;
  //std::string mch_id;
  //int port;
  //if (logic::SomeUtils::GetIPAddress(socket, ip, port))
   // wx_order.set_spbill_create_ip(ip);
  std::string str_rid = base::BasicUtil::StringUtil::Int64ToString(rid);
  unionpay_order.SetPrice(price);
  std::string result = unionpay_order.PlaceOrder();
  bool r = ParserUnionPayOrderResult(result);
  if (!r)
    return false;
  return true;
}

bool PayManager::ParserUnionPayOrderResult(std::string& result) {
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &result);
  std::string err_str;
  int32 err = 0;
  bool r = false;
  base_logic::DictionaryValue* dic = (DicValue*) deserializer->Deserialize(
      &err, &err_str);
  if (dic == NULL)
    return false;
  std::string return_code;
  r = dic->GetString(L"return_code", &return_code);
  if (!r)
    return false;
  //下单成功
  if (return_code.find("SUCCESS") == std::string::npos)
    return false;
  std::string result_code;
  r = dic->GetString(L"result_code", &result_code);
  if (!r)
    return false;
  if (result_code.find("SUCCESS") == std::string::npos)
    return false;
/*
  r = dic->GetString(L"prepay_id", &prepay_id);
  if (!r)
    return false;
  int npos1 = prepay_id.find("<![CDATA[");
  int npos2 = prepay_id.find("]]>");
  prepay_id = prepay_id.substr(npos1 + 9, npos2 - npos1 - 9);
*/
  //
  return true;
}



bool PayManager::OnAliPayServer(const int socket, std::string& result )
{
  int64 uid = 0;
  double balance = 0.0;
  double price = 0.0;
  std::string rid, transaction_id;
  pay_logic::ApliPayOrder alipay_rsaVerify;
  int status = 0;
  if (alipay_rsaVerify.DealResponse(result, transaction_id, rid, price, status)) //deal 成功
  {
    bool r = pay_db_->OnUpdateCallBackRechargeOrder(atoll(rid.c_str()), price, transaction_id,
                                                  (int32)status, uid, balance);

      const std::string r_rt = "success";
      send_full(socket, r_rt.c_str(), r_rt.length());

      if (r) {
        star_logic::UserInfo user;
        schduler_engine_->GetUserInfoSchduler(uid, &user);
        user.set_balance(balance);
        pay_logic::net_reply::Balance net_balance;
        net_balance.set_balance(balance);
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, S_NOTICE_SVC, PAY_TYPE, 0, 0, 0);
        packet_control.body_ = net_balance.get();
        send_message(user.socket_fd(), &packet_control);

      }
  }
  else
      return false;
}
bool PayManager::OnAliPayServer(const int socket, const std::string& appid,
                            const std::string& mch_id, const int64 total_fee,
                            const int64 rid, const int64 result,
                            const std::string& transaction_id) {
  int64 uid = 0;
  double balance = 0.0;
  double price = ((double) total_fee) / 100;
  bool r = pay_db_->OnUpdateCallBackRechargeOrder(rid, price, transaction_id,
                                                  result, uid, balance);

  const std::string r_rt =
      "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
  send_full(socket, r_rt.c_str(), r_rt.length());

  if (r) {
    star_logic::UserInfo user;
    schduler_engine_->GetUserInfoSchduler(uid, &user);
    user.set_balance(balance);
    pay_logic::net_reply::Balance net_balance;
    net_balance.set_balance(balance);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_NOTICE_SVC, PAY_TYPE, 0, 0, 0);
    packet_control.body_ = net_balance.get();
    send_message(user.socket_fd(), &packet_control);

  }
  return true;
}



bool PayManager::OnCanclePay(const int socket, const int64 uid, const int64 rid){
  bool r = pay_db_->OnUpdateRechargeOrder(uid, rid, 4, 0);

  double balance = 0.0;
  //if (r) 
  {
    //star_logic::UserInfo user;
    //schduler_engine_->GetUserInfoSchduler(uid, &user);
    //user.set_balance(balance);
    //pay_logic::net_reply::Balance net_balance;
    //net_balance.set_balance(balance);
    base_logic::DictionaryValue dic; 
    int flag = 1;
    dic.SetInteger(L"result", flag); 
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_CANCLE_PAY, PAY_TYPE, 0, 0, 0);
    packet_control.body_ = &dic;
    send_message(socket, &packet_control);

  }
  return true;
}
//-------------------------

bool PayManager::UnionWithDraw(const int socket, const int64 uid, 
                const int64 rid, const double price, const int session){
  
try
{
  bool r = pay_db_->OnCreateUnionWithDraw(uid, rid, price);

  if (r) //本地创建订单成功
  {
    /*
    LOG_DEBUG2("uid[%d] withdraw create sucess", uid);
    pay_logic::UnionpayOrder::GetInstance()->Init(); 
    pay_logic::UnionpayOrder::GetInstance()->Withdrawals(); 
    */
  }
  else //本地创建订单失败
  {
    LOG_DEBUG2("uid[%d] withdraw create failed", uid);
    return false;
  }

}
catch (...)
{
    return false;
}


  struct PacketControl packet_control;

  //MAKE_HEAD(packet_control, S_UNION_WITHDRAW, USER_TYPE, 0, 0, 0);
  MAKE_HEAD(packet_control, S_UNION_WITHDRAW, USER_TYPE, 0, session, 0);
  base_logic::DictionaryValue dic;
  dic.SetInteger(L"result", 1);
  packet_control.body_ = &dic;
  send_message(socket, &packet_control);

  return true;
}
}
