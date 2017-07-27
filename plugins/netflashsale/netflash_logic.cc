//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "netflash_logic.h"
#include <string>
#include "basic/native_library.h"
#include "core/common.h"
#include "config/config.h"
#include "net/errno.h"
#include "comm/comm_head.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "netflash_schduler_engine.h"
//#include "netflash_info.h" //TODO
#include "errno.h"
#include "operator_code.h"


#define DEFAULT_CONFIG_PATH "./plugins/netflash/netflash_config.xml"

#define TIME_DISTRIBUTION_TASK 17000
#define TIME_SYNCHRONIZE_TASK 17001

namespace netflash_logic
{

Netflashlogic *Netflashlogic::instance_ = NULL;

Netflashlogic::Netflashlogic()
{
  if (!Init())
    assert(0);
}

Netflashlogic::~Netflashlogic()
{
  if (netflash_db_)
  {
    delete netflash_db_;
    netflash_db_ = NULL;
  }

  if (netflash_kafka_){
    delete netflash_kafka_;
    netflash_kafka_ = NULL;
  }
  netflash_logic::NetflashEngine::FreeSchdulerManager();
  netflash_logic::NetflashEngine::FreeNetflashEngine();
}


bool Netflashlogic::Init()
{
  bool r = false;
  //manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
      return false;
  r = config->LoadConfig(path);
  LOG_MSG2("path : %s", path.c_str());
  netflash_logic::NetflashEngine::GetSchdulerManager();
  netflash_db_ = new netflash_logic::NetflashDB(config);
  netflash_kafka_ = new netflash_logic::NetflashKafka(config);
  netflash_logic::NetflashEngine::GetSchdulerManager()->InitDB(netflash_db_);
  netflash_logic::NetflashEngine::GetSchdulerManager()->InitKafka(netflash_kafka_);
  netflash_logic::NetflashEngine::GetSchdulerManager()->InitData();
  base::SysRadom::GetInstance();
  //std::string schduler_library = "./data_share.so";
  //std::string schduler_func = "GetManagerSchdulerEngine";
  //schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
  //                    logic::SomeUtils::GetLibraryFunction(schduler_library, 
  //                    schduler_func);

  //schduler_engine_= (*schduler_engine)();
  //if (schduler_engine_ == NULL)
  //    assert(0);
  //netflash_logic::NetflashEngine::GetSchdulerManager()->InitManagerSchduler(schduler_engine_);
  return true;
}

Netflashlogic *Netflashlogic::GetInstance()
{
  if (instance_ == NULL)
    instance_ = new Netflashlogic();
  return instance_;
}

void Netflashlogic::FreeInstance()
{
  delete instance_;
  instance_ = NULL;
}

bool Netflashlogic::OnNetflashConnect(struct server *srv, const int socket)
{
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Netflashlogic::OnNetflashMessage(struct server *srv, const int socket,
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

  switch (packet->operate_code)
  {
  case R_GET_PUB_STAR_STATUS:{
    OnNetSymbolStatus(srv, socket, packet);
    break;
  }
  case R_GET_STAR_PUB_INFO:
  {
    OnNetSymbolPubInfo(srv,socket, packet);
    break;
  }
  case R_CONFIRM_FLASH_ORDER:{
    OnNetflashSaleConfirmOrder(srv, socket, packet);
    break;
  }
  case R_GET_PUB_STAR_LIST:{
    OnNetSymbolList(srv,socket, packet);
    break;
  }
  case R_GET_STAR_EXPR_INFO:{
    OnNetStarInfo(srv,socket, packet);
    break;
  }
  default:
    break;
  }
  if(packet){
    delete packet;
    packet = NULL;
  }
  return true;
}

bool Netflashlogic::OnNetflashClose(struct server *srv, const int socket)
{
  return true;
}

bool Netflashlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  return true;
}

bool Netflashlogic::OnBroadcastMessage(struct server *srv, const int socket,
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


bool Netflashlogic::OnBroadcastClose(struct server *srv, const int socket)
{
  return true;
}

bool Netflashlogic::OnIniTimer(struct server *srv)
{
  if (srv->add_time_task != NULL)
  {
    srv->add_time_task(srv, "netflash", TIME_DISTRIBUTION_TASK, 10, -1);
    srv->add_time_task(srv, "netflash", TIME_SYNCHRONIZE_TASK, 3600, -1);
  }
  return true;
}

bool Netflashlogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
  switch (opcode)
  {
  case TIME_DISTRIBUTION_TASK:
  {
    netflash_logic::NetflashEngine::GetSchdulerManager()->TimeStarEvent();
    break;
  }
  case TIME_SYNCHRONIZE_TASK:
  {
    netflash_logic::NetflashEngine::GetSchdulerManager()->SynchronizeStarInfo();
    break;
  }
  default:
      break;
  }
  return true;
}

bool Netflashlogic::OnNetSymbolStatus(struct server* srv, int socket, struct PacketHead* packet) {
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

  return netflash_logic::NetflashEngine::GetSchdulerManager()->NetflashSymbolStatus(socket,
      packet->session_id, packet->reserved, symbol);
}

bool Netflashlogic::OnNetflashSaleConfirmOrder(struct server* srv, int socket, struct PacketHead* packet) {
    
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

  netflash_logic::NetflashEngine::GetSchdulerManager()->ConfirmOrder(socket,
          packet->session_id, packet->reserved,uid,symbol,amount,price);
  return true;
}

bool Netflashlogic::OnNetSymbolPubInfo(struct server* srv, int socket, struct PacketHead* packet){
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

  return netflash_logic::NetflashEngine::GetSchdulerManager()->SymbolInfo(socket,
          packet->session_id, packet->reserved, symbol);
}

bool Netflashlogic::OnNetSymbolList(struct server* srv, int socket, struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  return netflash_logic::NetflashEngine::GetSchdulerManager()->NetSymbolList(socket,
          packet->session_id, packet->reserved);
}

bool Netflashlogic::OnNetStarInfo(struct server* srv, int socket, struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH){
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

  return netflash_logic::NetflashEngine::GetSchdulerManager()->Getstarexperience(socket,
          packet->session_id, packet->reserved, symbol);
}

}  // namespace netflash_logic

