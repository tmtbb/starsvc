//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "pay/pay_logic.h"
#include "pay/pay_proto_buf.h"
#include "pay/operator_code.h"
#include "pay/errno.h"
#include "pay/schduler_engine.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

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

    case R_THIRD_PAY: {
      OnSHFJPayOrder(srv, socket, packet);
      break;
    }
    case R_THIRD_CLT: {
      OnSHFJPayClient(srv, socket, packet);
      break;
    }
    case R_THIRD_SVC: {
      OnSHFJPaySever(srv, socket, packet);
      break;
    }
    case R_THIRD_CASH: {
      OnSHFJCashOrder(srv, socket, packet);
      break;
    }
    case R_THIRD_CASH_SVC: {
      OnSHFJCashServer(srv, socket, packet);
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

  pay_logic::PayEngine::GetSchdulerManager()->OnWXCreateOrder(
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



bool Paylogic::OnSHFJPayOrder(struct server* srv, int socket,
                            struct PacketHead *packet) {
 
  pay_logic::net_request::SHFJPayOrder shfj_pay_order;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = shfj_pay_order.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnSHFJCreateOrder(
      socket, packet->session_id, packet->reserved, 
      shfj_pay_order.uid(),
      shfj_pay_order.amount(),
      shfj_pay_order.pay_type(),
      shfj_pay_order.wechat_openid(),
      shfj_pay_order.wechat_appid(),
      shfj_pay_order.content());

  return true;
}


bool Paylogic::OnSHFJCashOrder(struct server* srv, int socket,
                            struct PacketHead *packet) {
  pay_logic::net_request::SHFJCashOrder shfj_cash_order;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = shfj_cash_order.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  pay_logic::PayEngine::GetSchdulerManager()->OnSHFJCreateCashOrder(
      socket, packet->session_id, packet->reserved, shfj_cash_order.uid(),
      shfj_cash_order.amount(),shfj_cash_order.bid(),shfj_cash_order.rec_bank_name(),
      shfj_cash_order.rec_branch_bank_name(),shfj_cash_order.rec_card_no(), shfj_cash_order.rec_account_name());


  return true;
}


bool Paylogic::OnSHFJPayClient(struct server* srv, int socket,
                             struct PacketHead *packet) {
/*
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
*/
  return true;
}

bool Paylogic::OnSHFJPaySever(struct server* srv, int socket,
                            struct PacketHead * packet) {

  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  pay_logic::net_request::SHFJPayServer shfj_pay_sever;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = shfj_pay_sever.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    const std::string r_rt = "{\"shfj_pay\":\"\"}";
    send_full(socket, r_rt.c_str(), r_rt.length());
    return false;
  }
//out_trade_no --> int64 status--> type
  try
  {
    int64 result = 1;//成功
    if (shfj_pay_sever.status() == "PAYED_FAILED"
       || shfj_pay_sever.status() == "SETTLED_FAILED" )
    {
      result = 2; //失败
      return false;
    }
    else if (shfj_pay_sever.status() != "PAYED"
    	&& shfj_pay_sever.status() != "SETTLED")
	result = 3; //

    pay_logic::PayEngine::GetSchdulerManager()->OnSHFJServer(
        socket, shfj_pay_sever.pay_type(), shfj_pay_sever.mch_id(),
        shfj_pay_sever.amount(), atoll(shfj_pay_sever.out_trade_no().c_str()),
        result , shfj_pay_sever.trade_no());
  }
  catch (...)
  {
    LOG_ERROR2("OnSHFJPayServer exception socket %d___________", socket);
    LOG_ERROR2("OnSHFJPayServer exception socket %d___________", socket);
    LOG_ERROR2("OnSHFJPayServer exception socket %d___________", socket);
    LOG_ERROR2("OnSHFJPayServer exception socket %d___________", socket);
  }

  return true;
}


bool Paylogic::OnSHFJCashServer(struct server* srv, int socket,
                            struct PacketHead * packet) {

  LOG_DEBUG2("OnSHFJCashServer_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  LOG_DEBUG2("_______________________packet_length________________ %d",packet->packet_length);
  pay_logic::net_request::SHFJCashServer shfj_cash_sever;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = shfj_cash_sever.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    const std::string r_rt = "<xml><return_code><![CDATA[SUCCESS]]></return_code><return_msg><![CDATA[OK]]></return_msg></xml>";
    send_full(socket, r_rt.c_str(), r_rt.length());
    return false;
  }
  int64 status = pay_logic::GetSHFJCashStatus(shfj_cash_sever.status());
  
  //if (shfj_cash_sever.status() == "PAYED")
   // status = 2;
  pay_logic::PayEngine::GetSchdulerManager()->OnSHFJCashServer(
      socket, shfj_cash_sever.mch_id(),
      shfj_cash_sever.amount(), shfj_cash_sever.pay_no(),
      status, shfj_cash_sever.out_pay_no());

  return true;
}
}  // namespace trades_logic
