//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "pay/pay_logic.h"
#include "pay/pay_proto_buf.h"
#include "pay/operator_code.h"
#include "pay/errno.h"
#include "pay/schduler_engine.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "http/http_api.h"
#include "net/errno.h"
#include <string>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

#define DEFAULT_CONFIG_PATH "./plugins/pay/pay_config.xml"

namespace pay_logic {

Paylogic *Paylogic::instance_ = NULL;

Paylogic::Paylogic() {
  if (!Init())
    assert(0);
}

Paylogic::~Paylogic() {
  pay_logic::PayEngine::FreeSchdulerEngine();
  pay_logic::PayEngine::FreeSchdulerManager();
  if (pay_db_) {
    delete pay_db_;
    pay_db_ = NULL;
  }
  //delete kafka_;
}

bool Paylogic::Init() {
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);
  pay_db_ = new pay_logic::PayDB(config);
  pay_logic::PayEngine::GetSchdulerManager()->InitDB(pay_db_);

  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
  logic::SomeUtils::GetLibraryFunction(
  schduler_library, schduler_func);schduler_engine_
  = (*schduler_engine)();
  if (schduler_engine_ == NULL)
    assert(0);

  pay_logic::PayEngine::GetSchdulerManager()->InitSchdulerEngine(schduler_engine_);
  //kafka_ = new pay_logic::StroagerKafka(config);
  return true;
}

Paylogic *Paylogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Paylogic();
  return instance_;
}

void Paylogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Paylogic::OnPayConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Paylogic::OnPayMessage(struct server *srv, const int socket,
                            const void *msg, const int len) {
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    SEND_UNPACKET_ERROR(socket, ERROR_TYPE, UNPACKET_ERRNO, FORMAT_ERRNO);
    return false;
  }
	
try
{
  LOG_DEBUG2("operator_code %d",packet->operate_code);

  switch (packet->operate_code) {
    case R_WEIXIN_PAY: {
      OnWXPayOrder(srv, socket, packet);
      break;
    }
    case R_WEIXIN_CLT: {
      OnWXPayClient(srv, socket, packet);
      break;
    }
    case R_WEIXIN_SVC: {
      OnWXPaySever(srv, socket, packet);
      break;
    }
//____支付宝充值
    case R_ALIPAY_PAY: {
      OnAliPayOrder(srv, socket, packet);
      break;
    }
    case R_ALIPAY_CLT: {
      OnAliPayClient(srv, socket, packet);
      break;
    }
    case R_ALIPAY_SVC: {
      OnAliPaySever(srv, socket, packet);
      break;
    }

    case R_UNION_PAY: {
      OnUnionPayOrder(srv, socket, packet);
      break;
    }

    case R_CERTIFICATION: {
      //OnCertification(srv, socket, packet);
      break;
    }
    case R_CHECK_PAY_PWD: {
      OnCheckPayPwd(srv, socket, packet);
      break;
    }
    case R_CANCLE_PAY: {
      OnCanclePay(srv, socket, packet);
      break;
    }
    case R_UNION_WITHDRAW: {
      OnUnionWithDraw(srv, socket, packet);
    }
    default:
      break;
  }
}
catch (...)
{
    LOG_ERROR2("catch operator_code[%d]__________________________________________________", packet->operate_code);
    LOG_ERROR2("catch operator_code[%d]__________________________________________________", packet->operate_code);
    LOG_ERROR2("catch operator_code[%d]__________________________________________________", packet->operate_code);
    LOG_ERROR2("catch operator_code[%d]__________________________________________________", packet->operate_code);
}
  if(packet){
      delete packet;
      packet = NULL;
  }
  return true;
}

bool Paylogic::OnPayClose(struct server *srv, const int socket) {
  return true;
}

bool Paylogic::OnBroadcastConnect(struct server *srv, const int socket,
                                  const void *msg, const int len) {
  return true;
}

bool Paylogic::OnBroadcastMessage(struct server *srv, const int socket,
                                  const void *msg, const int len) {
  return true;
}

bool Paylogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Paylogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
  }
  return true;
}

bool Paylogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}

bool Paylogic::OnWXPayOrder(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::WXPayOrder wx_pay_order;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_pay_order.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
//____________________________________________________
  //kafka_

  pay_logic::PayEngine::GetSchdulerManager()->OnWXCreateOrder(
      socket, packet->session_id, packet->reserved, wx_pay_order.uid(),
      wx_pay_order.title(), wx_pay_order.price(),wx_pay_order.pay_type(),
      wx_pay_order.open_id());

  return true;
}


bool Paylogic::OnUnionPayOrder(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::WXPayOrder wx_pay_order;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_pay_order.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnUnionPayCreateOrder(
      socket, packet->session_id, packet->reserved, wx_pay_order.uid(),
      wx_pay_order.title(), wx_pay_order.price(),wx_pay_order.pay_type(),
      wx_pay_order.open_id());

  return true;
}


bool Paylogic::OnWXPayClient(struct server* srv, int socket,
                             struct PacketHead *packet) {
  pay_logic::net_request::WXPayClient wx_pay_client;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_pay_client.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnWXClient(
      socket, packet->session_id, packet->reserved, wx_pay_client.uid(),
      wx_pay_client.rid(), wx_pay_client.pay_result());

  return true;
}

bool Paylogic::OnWXPaySever(struct server* srv, int socket,
                            struct PacketHead * packet) {
  pay_logic::net_request::WXPayServer wx_pay_sever;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_pay_sever.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    const std::string r_rt = "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
    send_full(socket, r_rt.c_str(), r_rt.length());
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnWXServer(
      socket, wx_pay_sever.appid(), wx_pay_sever.mch_id(),
      wx_pay_sever.total_fee(), wx_pay_sever.recharge_id(),
      wx_pay_sever.pay_result(), wx_pay_sever.transaction_id());
  return true;
}

bool Paylogic::OnCertification(struct server* srv, int socket,
                            struct PacketHead *packet) {

  std::string idcard = "411325199005217439";
  std::string name = "唐伟";
  //阿里云接口
  std::string strUrl = "http://idcardreturnphoto.haoservice.com/idcard/VerifyIdcardReturnPhoto";
  ////阿里云接口code
  std::string strHeader = "Authorization:APPCODE 900036feeee64ae089177dd06b25faa9";
  std::string strResult;
  base_logic::DictionaryValue dic;
  dic.SetString(L"cardNo", idcard);
  dic.SetString(L"realName", name);
  base_http::HttpRequestHeader *httphead = new base_http::HttpRequestHeader();
  httphead->AddHeaderFromString(strHeader);
  //base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, strHeader, 1);
  base_http::HttpAPI::RequestGetMethod(strUrl, &dic, httphead, strResult, 1);
  LOG_DEBUG2("strResult [%s]___________________________________________________", strResult.c_str());
  if(httphead){
    delete httphead;
    httphead = NULL;
  }

  //pay_logic::net_reply::TResult r_ret;;
  //r_ret.set_result(1);
//_________________________________________________________
  base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
  				base_logic::IMPL_JSON, &strResult, false);
  std::string err_str;
  DicValue* dicResultJosn;
  int32 err = 0;
  DicValue* dicJosn = (DicValue*)serializer->Deserialize(&err, &err_str);
  bool r = false;
  if (dicJosn != NULL){
    r = dicJosn->GetDictionary(L"result", &dicResultJosn);
    if (r)
    {
      //解析第二层
      int32 err = 0;
      bool bResultIsOk = false;
      if (dicResultJosn != NULL){
        r = dicResultJosn->GetBoolean(L"isok", &bResultIsOk);
        if (r)
        {
          //更新数据
          std::stringstream strsql;
          if (bResultIsOk)
            LOG_DEBUG("strResult [%s]___________________________________________________ok" );
          else
            LOG_DEBUG("strResult []___________________________________________________ err" );
        //mysql_engine_->WriteData(strsql.str());
        //map_IdCard_Info_.erase(l_it);
  //        r_ret.set_result(0);
        }
      }
    }
  }
  else {
    LOG_DEBUG("josn Deserialize error[]___________________________________________________ err" );
  }
//_____________________________________________________________________________________________
/*
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_CERTIFICATION, USERS_TYPE, 0, packet->session_id, 0);
  packet_control.body_ = r_ret.get();
  send_message(socket, &packet_control);
/*
*/
  return true;
}
//__________________________alipay


bool Paylogic::OnAliPayOrder(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::AliPayOrder alipay_order;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = alipay_order.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
//____________________________________________________
  //kafka_

  pay_logic::PayEngine::GetSchdulerManager()->OnAliPayCreateOrder(
      socket, packet->session_id, packet->reserved, alipay_order.uid(),
      alipay_order.title(), alipay_order.price(),alipay_order.pay_type(),
      alipay_order.open_id());

  return true;
}


bool Paylogic::OnAliPayClient(struct server* srv, int socket,
                             struct PacketHead *packet) {
  pay_logic::net_request::AliPayClient wx_pay_client;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_pay_client.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnWXClient(
      socket, packet->session_id, packet->reserved, wx_pay_client.uid(),
      wx_pay_client.rid(), wx_pay_client.pay_result());

  return true;
}

bool Paylogic::OnAliPaySever(struct server* srv, int socket,
                            struct PacketHead * packet) {
///
  LOG_DEBUG2("AliPayServer packet_length_____________________________________ %d",packet->packet_length);
  pay_logic::net_request::AliPayServer alipay_server;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  //bool r = alipay_server.set_http_packet(packet_control->body_);
  base_logic::DictionaryValue* value = packet_control->body_;
  std::string result_str;
  bool r = value->GetString(L"result", &result_str);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    const std::string r_rt = "success";
    send_full(socket, r_rt.c_str(), r_rt.length());
    return false;
  }
  
/*
  pay_logic::PayEngine::GetSchdulerManager()->OnAliPayServer(
      socket, alipay_server.appid(), alipay_server.mch_id(),
      alipay_server.total_fee(), alipay_server.recharge_id(),
      alipay_server.pay_result(), alipay_server.transaction_id());
  */
 // std::string result_str = "gmt_create=2017-06-22+13%3A43%3A08&charset=utf-8&seller_email=18072858723%40163.com&subject=%E6%98%9F%E4%BA%AB-%E5%85%85%E5%80%BC&sign=S2nbGL25tNOH%2F6Shu2BLEG9vVYXgrA%2F74595wznOF7aSWzUy7EJmXD1MgZztGpCWf1lUnsmXYfpLJDxtmQWC7tZ4kwOcMONj6xkLEZOyrsWv4%2BQZD3x7NXGsQHaBPyAPogI44hwZ7Hvk9d5%2FL893L8BLOO6oQlXDPPUcCAOZx3w%3D&buyer_id=2088602279845191&invoice_amount=0.01&notify_id=637a309f1604c69ceb8c5600cec9ba1hgu&fund_bill_list=%5B%7B%22amount%22%3A%220.01%22%2C%22fundChannel%22%3A%22ALIPAYACCOUNT%22%7D%5D&notify_type=trade_status_sync&trade_status=TRADE_SUCCESS&receipt_amount=0.01&app_id=2017060807450365&buyer_pay_amount=0.01&sign_type=RSA&seller_id=2088721186508567&gmt_payment=2017-06-22+13%3A43%3A09&notify_time=2017-06-22+15%3A08%3A17&version=1.0&out_trade_no=6085041256207535515&total_amount=0.01&trade_no=2017062221001004190250870215&auth_app_id=2017060807450365&buyer_logon_id=tan***%40163.com&point_amount=0.00";
  pay_logic::PayEngine::GetSchdulerManager()->OnAliPayServer(socket,result_str);
  return true;
}



//校验支付密码
bool Paylogic::OnCheckPayPwd(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  
  bool r1,r2,r3;
  int64 uid;
  std::string pwd;
  std::string token;
  r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  r2 = packet_control->body_->GetString(L"paypwd", &pwd);
  r3 = packet_control->body_->GetString(L"token", &token);
  
  if (!r1 || !r2 || !r3) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  star_logic::UserInfo userinfo;
  if (!schduler_engine_->GetUserInfoSchduler(uid, &userinfo)){
    LOG_DEBUG2("uid[%ld]", uid);
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }

  //check token
  if (token != userinfo.token()) {
    LOG_DEBUG2("check token[%s],userinfo token[%s]", token.c_str(), userinfo.token().c_str());
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }

  int32 flag=0;
  //校验密码
  r1 =pay_db_->OnCheckPayPwd(uid, pwd, flag);
  if (!r1) {
    LOG_DEBUG2("uid[%ld],pwd[%s]",uid, pwd.c_str());
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }
  
  //发送信息
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_CHECK_PAY_PWD, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", flag); 
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 

  return true;
}
//

bool Paylogic::OnCanclePay(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::CanclePay cancle_pay;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = cancle_pay.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnCanclePay(
      socket, cancle_pay.uid(), cancle_pay.rid());

  return true;
}
//
//

bool Paylogic::OnUnionWithDraw(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::UnionWithDraw union_withdraw;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = union_withdraw.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }


  //创建取现订单号
  //
  int64 rid = base::SysRadom::GetInstance()->GetRandomID();
  r = pay_logic::PayEngine::GetSchdulerManager()->UnionWithDraw(
      socket, union_withdraw.uid(), rid, union_withdraw.price(), packet->session_id);
  
  if (!r)
  {
    send_error(socket, ERROR_TYPE, UNIWITHDRAW_ERROR, packet->session_id);
    return false;
  }
  return true;
}

}  // namespace trades_logic
