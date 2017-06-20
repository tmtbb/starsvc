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
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
	
try
{
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
  //base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, strHeader, 1);
  base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, 1);
  LOG_DEBUG2("strResult [%s]___________________________________________________", strResult.c_str());

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
  //check token
  r1 = logic::SomeUtils::VerifyToken(uid, token);
  if (!r1) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
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

}  // namespace trades_logic
