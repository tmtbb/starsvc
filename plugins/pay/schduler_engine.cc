//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "pay/schduler_engine.h"
#include "pay/operator_code.h"
#include "pay/wx_order.h"
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

bool PayManager::OnSHFJCreateCashOrder(const int socket, const int64 session,
                                 const int32 reversed, const int64 uid,
                                 const double price,const int64 bid,
                                 const std::string& rec_bank_name, const std::string& rec_bra_bank_name,
				 const std::string& rec_card_no, const std::string& rec_account_name) {
  pay_logic::net_reply::SHFJCashOrder r_shfj_cash_order;	//返回数据结构
  //创建取现订单号
  pay_logic::SHFJOrder shfj_order;
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();
try
{
  bool r = SHFJCashOrder(socket, rid, price, rec_bank_name, rec_bra_bank_name, rec_card_no, rec_account_name,shfj_order, r_shfj_cash_order);
  if (!r) {
    send_error(socket, ERROR_TYPE, THIRD_CASH_ORDER_ERROR, session);
    return false;
  }

//数据库操作创建取现订单记录
//
  int status = pay_logic::GetSHFJCashStatus(r_shfj_cash_order.status());  //

  r = pay_db_->OnCreateWithdrawOrder(uid, rid, price, bid, status);
  if (!r) {
    send_error(socket, ERROR_TYPE, STOAGE_ORDER_ERROR, session);
    return r;
  }
}
catch (...)
{
  LOG_DEBUG2("CreateCashOrder:exception uid[%d]", uid);
}
//返回处理
  struct PacketControl packet_control;

  MAKE_HEAD(packet_control, S_THIRD_PAY, USER_TYPE, 0, session, 0);
  packet_control.body_ = r_shfj_cash_order.get();
  send_message(socket, &packet_control);
  return true;
}


bool PayManager::OnSHFJCreateOrder(const int socket, const int64 session,
                                 const int32 reversed, const int64 uid,
                                 const double price,
                                 const std::string& pay_type, 
                         	 const std::string &wechat_openid,
                         	 const std::string &wechat_appid,
				 const std::string& content) {
  //创建订单号
  pay_logic::SHFJOrder shfj_order;
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();//
  
  bool r = SHFJOrder(socket,  rid, price, pay_type,
  			wechat_openid, wechat_appid, 
			content ,shfj_order);
  if (!r) {
    send_error(socket, ERROR_TYPE, THIRD_ORDER_ERROR, session);
    return false;
  }
  int pay_plat = UNIPNPAY;
  if (pay_type == "ALIPAY_QRCODE_PAY" || pay_type == "ALIPAY_JSAPI_PAY" )
    pay_plat = ALIPAY; 
  else if (pay_type == "WECHAT_QRCODE_PAY" || pay_type == "WECHAT_JSAPI_PAY")
    pay_plat = WX_APP; 
  r = pay_db_->OnCreateRechargeOrder(uid, rid, price, pay_plat);
  if (!r) {
    send_error(socket, ERROR_TYPE, STOAGE_ORDER_ERROR, session);
    return r;
  }
  pay_logic::net_reply::SHFJPayOrder r_shfj_order;
  struct PacketControl packet_control;

  MAKE_HEAD(packet_control, S_THIRD_PAY, USER_TYPE, 0, session, 0);
  r_shfj_order.set_payment_info(shfj_order.get_payment_info());
  packet_control.body_ = r_shfj_order.get();
  send_message(socket, &packet_control);
  return true;
}


bool PayManager::ParserSHFJOrderResult(std::string& result,
                                     std::string& prepay_id) {
  if (result.length() < 1)
    return false;
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &result);
  std::string err_str;
  int32 err = 0;
  bool r = false;
  base_logic::DictionaryValue* dic = (DicValue*) deserializer->Deserialize(
      &err, &err_str);
  if (dic == NULL)
    return false;
  std::string return_code;

  //r = dic->GetString(L"prepay_id", &prepay_id);
  r = dic->GetString(L"paymentInfo", &prepay_id);
  if (!r)
    return false;
  //
  return true;
}

bool PayManager::ParserSHFJCashOrderResult(std::string& result, pay_logic::net_reply::SHFJCashOrder &t_shfj_cash_order) {
  if (result.length() < 1)
    return false;
  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_JSON, &result);
  std::string err_str;
  int32 err = 0;
  bool r = false;
  base_logic::DictionaryValue* dic = (DicValue*) deserializer->Deserialize(
      &err, &err_str);
  if (dic == NULL)
    return false;
  //下单成功
  std::string tmp = "";
  //r = dic->GetString(L"prepay_id", &prepay_id);
  r = dic->GetString(L"merchantNo", &tmp);
  if (!r)
    return false;
  t_shfj_cash_order.set_merchant_no(tmp);
  LOG_DEBUG2("merchantNo_________: %s", tmp.c_str());
  r = dic->GetString(L"status", &tmp);
  if (!r)
    return false;
  t_shfj_cash_order.set_status(tmp);
  LOG_DEBUG2("status_________: %s", tmp.c_str());

  r = dic->GetString(L"payNo", &tmp);
  if (!r)
    return false;
  t_shfj_cash_order.set_pay_no(tmp);
  LOG_DEBUG2("payNo_________: %s", tmp.c_str());
  //
  //
  int64 tmp_i; 
  r = dic->GetBigInteger(L"amount", &tmp_i);
  if (!r)
    return false;
  t_shfj_cash_order.set_amount(tmp_i/100.0);
  LOG_DEBUG2("amount_________: %d", tmp_i);
  //
  r = dic->GetBigInteger(L"fee", &tmp_i);
  if (!r)
    return false;
  t_shfj_cash_order.set_fee(tmp_i/100.0);
  LOG_DEBUG2("fee_________: %d", tmp_i);
  //
  //
  r = dic->GetBigInteger(L"transferAmount", &tmp_i);
  if (!r)
    return false;
  t_shfj_cash_order.set_transfer_amount(tmp_i/100);
  LOG_DEBUG2("transfer_________: %d", tmp_i);
  //
  return true;
}
bool PayManager::SHFJOrder(const int socket, 
                         const int64 rid, const double price,
                         const std::string &pay_type,
                         const std::string &wechat_openid,
                         const std::string &wechat_appid,
			 const std::string& content, 
                         pay_logic::SHFJOrder& shfj_order) {

  LOG_ERROR2("tw________price[%lf],pay_type[%s],content[%s]\n", price, pay_type.c_str(), content.c_str()); //tw test
  std::string payment_info;
  std::string ip;
  std::string app_id;
  int port;
  if (logic::SomeUtils::GetIPAddress(socket, ip, port))
    shfj_order.set_spbill_create_ip(ip);

  std::string str_rid = base::BasicUtil::StringUtil::Int64ToString(rid);
  shfj_order.set_out_trade_no(str_rid);
  shfj_order.set_wechat_openid(wechat_openid);
  shfj_order.set_wechat_appid(wechat_appid);
  shfj_order.set_total_fee(price * 100);
  std::string shfj_result = shfj_order.PlaceOrder(app_id, pay_type, content);
  bool r = ParserSHFJOrderResult(shfj_result, payment_info);
  if (!r)
    return false;
  shfj_order.set_payment_info(payment_info);
  //shfj_order.PreSign();
  return true;
}


bool PayManager::SHFJCashOrder(const int socket, 
                         const int64 rid, const double price,
                         const std::string &rec_bank_name,const std::string& rec_bra_bank_name, const std::string &rec_card_no, const std::string &rec_account_name, 
                         pay_logic::SHFJOrder& shfj_order, pay_logic::net_reply::SHFJCashOrder &r_shfj_cash_order) {

  //std::string prepay_id;
  std::string ip;
  std::string app_id;
  int port;
  if (logic::SomeUtils::GetIPAddress(socket, ip, port))
    shfj_order.set_spbill_create_ip(ip);

  //shfj_order.set_body(title);
  std::string str_rid = base::BasicUtil::StringUtil::Int64ToString(rid);
  shfj_order.set_out_trade_no(str_rid);
  shfj_order.set_total_fee(price * 100);
  shfj_order.set_rec_bank_name(rec_bank_name);
  shfj_order.set_rec_branch_bank_name(rec_bra_bank_name);
  shfj_order.set_rec_card_no(rec_card_no);
  shfj_order.set_rec_account_name(rec_account_name);
  std::string shfj_result = shfj_order.CashPlaceOrder(app_id);
  bool r = ParserSHFJCashOrderResult(shfj_result, r_shfj_cash_order);
  if (!r)
    return false;
  //shfj_order.set_payment_info(prepay_id);
  //shfj_order.PreSign();
  return true;
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
    swp_logic::UserInfo user;
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
//-------------shfj callback

bool PayManager::OnSHFJServer(const int socket, const std::string& appid,
                            const std::string& mch_id, const int64 total_fee,
                            const int64 rid, const int64 result,
                            const std::string& transaction_id) {
  int64 uid = 0;
  double balance = 0.0;
  double price = ((double) total_fee) / 100;
  bool r = pay_db_->OnUpdateCallBackRechargeOrder(rid, price, transaction_id,
                                                  result, uid, balance);

  //const std::string r_rt =
   //   "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
  const std::string r_rt = "{\"returncode\":\"SUCCESS\"}";
  send_full(socket, r_rt.c_str(), r_rt.length());

  if (r) {
    swp_logic::UserInfo user;
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


bool PayManager::OnSHFJCashServer(const int socket, 
                            const std::string& mch_id, 
			    const int64 total_fee,
                            const std::string& transaction_id,
			    const int64 status, const std::string& rid)
			    {
  int64 uid = 0;
  double balance = 0.0;
  double price = ((double) total_fee) / 100;

  bool r = pay_db_->OnUpdateCallBackWithdrawOrder(rid, price, transaction_id,
                                                  status, uid, balance);

  const std::string r_rt =
      "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
  send_full(socket, r_rt.c_str(), r_rt.length());

  if (r) {
    swp_logic::UserInfo user;
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
}
