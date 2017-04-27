//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "bank/bank_logic.h"
#include "bank/bank_proto_buf.h"
#include "bank/operator_code.h"
#include "bank/errno.h"
#include "bank/bank_info.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/bank/bank_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace bank_logic {

Banklogic *Banklogic::instance_ = NULL;

Banklogic::Banklogic() {
  if (!Init())
    assert(0);
}

Banklogic::~Banklogic() {
  if (bank_db_) {
    delete bank_db_;
    bank_db_ = NULL;
  }
  //history_logic::HistoryEngine::FreeSchdulerManager();
  //history_logic::HistoryEngine::FreeHistoryEngine();
}

bool Banklogic::Init() {
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);
  bank_db_ = new bank_logic::BankDB(config);

  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
  logic::SomeUtils::GetLibraryFunction(
  schduler_library, schduler_func);schduler_engine_
  = (*schduler_engine)();
  if (schduler_engine_ == NULL)
    assert(0);

  //history_logic::HistoryEngine::GetSchdulerManager()->InitSchdulerEngine(
  //  schduler_engine_);
  return true;
}

Banklogic *Banklogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Banklogic();
  return instance_;
}

void Banklogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Banklogic::OnBankConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Banklogic::OnBankMessage(struct server *srv, const int socket,
                              const void *msg, const int len) {
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }

  switch (packet->operate_code) {
    case R_WITH_DRAW_CASH: {
      OnWithDrawCash(srv, socket, packet);
      break;
    }

    case R_GET_BIND_BANK: {
      OnGetBindBank(srv, socket, packet);
      break;
    }

    case R_BIND_BANK_CARD: {
      OnBindBankCard(srv, socket, packet);
      break;
    }

    case R_UNBIND_BANK_CARD: {
      OnUnbindBankCard(srv, socket, packet);
      break;
    }

    case R_GET_BANK_CARD_INFO: {
      OnGetBankName(srv, socket, packet);
      break;
    }
    default:
      break;
  }
  return true;
}

bool Banklogic::OnBankClose(struct server *srv, const int socket) {
  return true;
}

bool Banklogic::OnBroadcastConnect(struct server *srv, const int socket,
                                   const void *msg, const int len) {
  return true;
}

bool Banklogic::OnBroadcastMessage(struct server *srv, const int socket,
                                   const void *msg, const int len) {

  return true;
}

bool Banklogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Banklogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      //srv->add_time_task(srv, "history", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Banklogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}

//获取绑定银行卡
bool Banklogic::OnGetBindBank(struct server* srv, int socket,
                              struct PacketHead *packet) {
  bank_logic::net_request::BankCardInfo bank_card;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = bank_card.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  std::list<bank_logic::BankCard> bank_card_list;

  r  = bank_db_->OnGetBankCard(bank_card.id(),bank_card_list);
  if (!r||bank_card_list.size() <= 0){
    send_error(socket, ERROR_TYPE, NO_HAVE_BIND_BANK, packet->session_id);
    return false;
  }
 
  int32 base_num = 10;
  if (packet->reserved / 1000 == HTTP)
    base_num = bank_card.count();
  else
    base_num = base_num < bank_card.count() ? base_num : bank_card.count();
 
  int32 t_start = 0;
  int32 t_count = 0;

  bank_logic::net_reply::AllBankCard net_all_bank_card;
  bank_card_list.sort(bank_logic::BankCard::close_after);
  while (bank_card_list.size() > 0 && t_count < bank_card.count()) {
    bank_logic::BankCard card = bank_card_list.front();
    bank_card_list.pop_front();
    t_start++;
    if (t_start < bank_card.start())
        continue;
    bank_logic::net_reply::BankCardInfo* r_bank_card = new bank_logic::net_reply::BankCardInfo;
    r_bank_card->set_bid(card.id());
    r_bank_card->set_bank(card.bank_name());
    r_bank_card->set_branch_bank(card.branch_bank());
    r_bank_card->set_card_no(card.card_no());
    r_bank_card->set_card_name(card.card_name());    
    r_bank_card->set_name(card.name());
    r_bank_card->set_is_default(card.is_default());
    t_count++;
    net_all_bank_card.set_unit(r_bank_card->get());
    if (net_all_bank_card.Size() % base_num == 0 && net_all_bank_card.Size() != 0) {
        struct PacketControl r_packet_control;
        MAKE_HEAD(r_packet_control,S_GET_BIND_BANK,1,0,packet_control->session_id,0);
        r_packet_control.body_ = net_all_bank_card.get();
        send_message(socket, &r_packet_control);
        net_all_bank_card.Reset();
    }
  }
  
  if (net_all_bank_card.Size() > 0){ 
    struct PacketControl r_packet_control;
    MAKE_HEAD(r_packet_control,S_GET_BIND_BANK,1,0,packet_control->session_id,0);
    r_packet_control.body_ = net_all_bank_card.get();
    send_message(socket, &r_packet_control);
    net_all_bank_card.Reset();
  }
  return true;
}

//提现申请
bool Banklogic::OnWithDrawCash(struct server* srv, int socket,
                               struct PacketHead *packet) {

  bank_logic::net_request::WithDrawCash with_draw_cash;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = with_draw_cash.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  double charge = 0.0;
  int32 result = 0;
  double balance = 0.0;
  int64 wid = base::SysRadom::GetInstance()->GetRandomID();
  //提交提现 数据库操作
  r = bank_db_->OnWithDrawCash(with_draw_cash.id(), wid, with_draw_cash.card_id(),
            with_draw_cash.money(), charge, result, balance);
  
  bank_logic::net_reply::WithDrawCash r_with_draw_cash;
  r_with_draw_cash.set_result(result);
  r_with_draw_cash.set_balance(balance);
  struct PacketControl r_packet_control;
  MAKE_HEAD(r_packet_control,S_WITH_DRAW_CASH,1,0,packet_control->session_id,0);
  r_packet_control.body_ = r_with_draw_cash.get();
  send_message(socket, &r_packet_control);
  return true;
}


//绑定银行卡
bool Banklogic::OnBindBankCard(struct server* srv, int socket, struct PacketHead* packet) {

        bank_logic::net_request::BindBankCard bind_bank_card;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = bind_bank_card.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  //int64 bank_card_id = 0;
  //int32 bank_id = 0;
  bank_logic::BankCard  bank_card;
  r = bank_db_->OnBindBankCard(bind_bank_card.id(), bind_bank_card.card_no(),
                           bind_bank_card.branch_bank(),bind_bank_card.name(),bank_card);
 
  if (!r){//绑定失败
   send_error(socket, ERROR_TYPE, BIND_BANK_CARD_ERROR, packet->session_id); 
   return false;
  }
  
  bank_logic::net_reply::BindBankCard r_bind_bank_card;
  r_bind_bank_card.set_bank_card_id(bank_card.id());
  r_bind_bank_card.set_bank_id(bank_card.bank_id());
  r_bind_bank_card.set_bank_name(bank_card.bank_name());
  r_bind_bank_card.set_branch_bank_name(bank_card.branch_bank());
  r_bind_bank_card.set_card_no(bank_card.card_no());
  r_bind_bank_card.set_name(bank_card.name());
 
  struct PacketControl r_packet_control;
  MAKE_HEAD(r_packet_control,S_GET_BIND_BANK,1,0,packet_control->session_id,0);
  r_packet_control.body_ = r_bind_bank_card.get();
  send_message(socket, &r_packet_control);
}

//获取绑定银行名称
bool Banklogic::OnGetBankName(struct server* srv, int socket, struct PacketHead* packet) {
    
  bank_logic::net_request::GetBankName get_bank_name;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = get_bank_name.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  int32 bank_id = 0;
  std::string card_name;
  std::string bank_name;
  r = bank_db_->OnGetBankCardInfo(get_bank_name.card_no(),bank_id, card_name, bank_name);
  if (!r || bank_id == 0){
    send_error(socket, ERROR_TYPE, BANK_CARD_NOT_EXIST, packet->session_id);
    return false;
  }
  bank_logic::net_reply::BankCardNOInfo r_bank_card_info;
  r_bank_card_info.set_bank_id(bank_id);
  r_bank_card_info.set_card_no(get_bank_name.card_no());
  r_bank_card_info.set_card_name(card_name);
  r_bank_card_info.set_bank_name(bank_name);

  struct PacketControl r_packet_control;
  MAKE_HEAD(r_packet_control,S_GET_BIND_BANK,1,0,packet_control->session_id,0);
  r_packet_control.body_ = r_bank_card_info.get();
  send_message(socket, &r_packet_control);
}

//解绑银行卡
bool Banklogic::OnUnbindBankCard(struct server* srv, int socket, struct PacketHead* packet) {
  bank_logic::net_request::UnbindBankCard unbind_bank_card;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = unbind_bank_card.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  r = bank_db_->OnUnbindBankCard(unbind_bank_card.id(), unbind_bank_card.bank_card_id());
  if (!r) {
     send_error(socket, ERROR_TYPE, UNBIND_BANK_CARD_ERROR, packet->session_id);
     return false;
  }
  bank_logic::net_reply::UnbindBankCard r_unbind_bank_card;
  r_unbind_bank_card.set_result(1);
  struct PacketControl r_packet_control;
  MAKE_HEAD(r_packet_control,S_GET_BIND_BANK,1,0,packet_control->session_id,0);
  r_packet_control.body_ = r_unbind_bank_card.get();
  send_message(socket, &r_packet_control);
}

}  // namespace trades_logic
