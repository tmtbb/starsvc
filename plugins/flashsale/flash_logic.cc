//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "flashsale/flash_logic.h"
#include <string>
#include "basic/native_library.h"
#include "core/common.h"
#include "config/config.h"
#include "net/errno.h"
#include "comm/comm_head.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "flashsale/schduler_engine.h"
#include "flashsale/flash_info.h"
#include "flashsale/errno.h"
#include "flashsale/operator_code.h"


#define DEFAULT_CONFIG_PATH "./plugins/flash/flash_config.xml"

#define TIME_DISTRIBUTION_TASK 14000

namespace flash_logic
{

Flashlogic *Flashlogic::instance_ = NULL;

Flashlogic::Flashlogic()
{
  if (!Init())
      assert(0);
}

Flashlogic::~Flashlogic()
{
  if (flash_db_)
  {
    delete flash_db_;
    flash_db_ = NULL;
  }

  if (flash_kafka_){
    delete flash_kafka_;
    flash_kafka_ = NULL;
  }
  flash_logic::FlashEngine::FreeSchdulerManager();
  flash_logic::FlashEngine::FreeFlashEngine();
}


bool Flashlogic::Init()
{
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
      return false;
  r = config->LoadConfig(path);
  LOG_MSG2("path : %s", path.c_str());
  flash_logic::FlashEngine::GetSchdulerManager();
  flash_db_ = new flash_logic::FlashDB(config);
  flash_kafka_ = new flash_logic::FlashKafka(config);
  flash_logic::FlashEngine::GetSchdulerManager()->InitDB(flash_db_);
  flash_logic::FlashEngine::GetSchdulerManager()->InitKafka(flash_kafka_);
  flash_logic::FlashEngine::GetSchdulerManager()->InitData();
  base::SysRadom::GetInstance();
  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                      logic::SomeUtils::GetLibraryFunction(schduler_library, 
                      schduler_func);

  schduler_engine_= (*schduler_engine)();
  if (schduler_engine_ == NULL)
      assert(0);
  flash_logic::FlashEngine::GetSchdulerManager()->InitManagerSchduler(schduler_engine_);
  return true;
}

Flashlogic *Flashlogic::GetInstance()
{
  if (instance_ == NULL)
    instance_ = new Flashlogic();
  return instance_;
}

void Flashlogic::FreeInstance()
{
  delete instance_;
  instance_ = NULL;
}

bool Flashlogic::OnFlashConnect(struct server *srv, const int socket)
{
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Flashlogic::OnFlashMessage(struct server *srv, const int socket,
                                  const void *msg, const int len)
{
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
      return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet))
  {
      LOG_ERROR2("UnpackStream Error socket %d", socket);
      SEND_UNPACKET_ERROR(socket, ERROR_TYPE, UNPACKET_ERRNO, FORMAT_ERRNO);
      return false;
  }

  switch (packet->operate_code)
  {
  case R_GET_PUB_STAR_STATUS:{
      OnSymbolStatus(srv, socket, packet);
      break;
  }
  case R_GET_PUB_STAR_INFO:
  {
      OnSymbolInfo(srv,socket, packet);
      break;
  }
  case R_CONFIRM_FLASH_ORDER:{
      OnFlashSaleConfirmOrder(srv, socket, packet);
      break;
  }
  /*
  case R_CANCEL_ORDER:{
      OnCancelOrder(srv,socket, packet);
      break;
  }
  */
  default:
      break;
  }
  if(packet){
      delete packet;
      packet = NULL;
  }
  return true;
}

bool Flashlogic::OnFlashClose(struct server *srv, const int socket)
{
  return true;
}

bool Flashlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  return true;
}

bool Flashlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
      return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet))
  {
      LOG_ERROR2("UnpackStream Error socket %d", socket);
      return false;
  }

  return true;
}


bool Flashlogic::OnBroadcastClose(struct server *srv, const int socket)
{
  return true;
}

bool Flashlogic::OnIniTimer(struct server *srv)
{
  if (srv->add_time_task != NULL)
  {
      srv->add_time_task(srv, "flash", TIME_DISTRIBUTION_TASK, 10, -1);
  }
  return true;
}

bool Flashlogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
  switch (opcode)
  {
  case TIME_DISTRIBUTION_TASK:
  {
      flash_logic::FlashEngine::GetSchdulerManager()->TimeStarEvent();
  }
  default:
      break;
  }
  return true;
}

bool Flashlogic::OnSymbolStatus(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 uid;
  std::string token, symbol;
  bool r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r2 = packet_control->body_->GetString(L"token", &token);
  bool r3 = packet_control->body_->GetString(L"symbol", &symbol);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  flash_logic::FlashEngine::GetSchdulerManager()->FlashSymbolStatus(socket,
      packet->session_id,packet->reserved, symbol);
  return true;
}

bool Flashlogic::OnFlashSaleConfirmOrder(struct server* srv, int socket, struct PacketHead* packet) {
    
  if (packet->packet_length <= PACKET_HEAD_LENGTH)
  {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 uid, amount, lprice;
  std::string token, symbol;
  double price=0.0;
  bool r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r2 = packet_control->body_->GetString(L"token", &token);
  bool r3 = packet_control->body_->GetString(L"symbol", &symbol);
  bool r4 = packet_control->body_->GetBigInteger(L"amount", &amount);
  bool r5 = packet_control->body_->GetReal(L"price", &price);
  if(!r5){
    r5 = packet_control->body_->GetBigInteger(L"price", &lprice);
    price = lprice;
  }
  if (!r1 || !r2 || !r3 || !r4 || !r5)
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  LOG_MSG2("price:%.2f", price);

  flash_logic::FlashEngine::GetSchdulerManager()->ConfirmOrder(socket,
          packet->session_id, packet->reserved,uid,symbol,amount,price);
  return true;

}

bool Flashlogic::OnSymbolInfo(struct server* srv, int socket, struct PacketHead* packet)
{
  if (packet->packet_length <= PACKET_HEAD_LENGTH)
  {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string symbol;
  bool r = packet_control->body_->GetString(L"symbol", &symbol);
  if (!r){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  flash_logic::FlashEngine::GetSchdulerManager()->SymbolInfo(socket,
          packet->session_id,packet->reserved,symbol);
  return true;
}

}  // namespace flash_logic

