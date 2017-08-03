//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "circle_logic.h"
#include <string>
#include "basic/native_library.h"
#include "core/common.h"
#include "config/config.h"
#include "net/errno.h"
#include "comm/comm_head.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "schduler_engine.h"
#include "circle_info.h"
#include "errno.h"
#include "operator_code.h"


#define DEFAULT_CONFIG_PATH "./plugins/circle/circle_config.xml"

#define TIME_DISTRIBUTION_TASK 15000

namespace circle_logic
{

Circlelogic *Circlelogic::instance_ = NULL;

Circlelogic::Circlelogic()
{
  if (!Init())
      assert(0);
}

Circlelogic::~Circlelogic()
{
  if (circle_db_)
  {
    delete circle_db_;
    circle_db_ = NULL;
  }

  circle_logic::CircleEngine::FreeSchdulerManager();
  circle_logic::CircleEngine::FreeCircleEngine();
}


bool Circlelogic::Init()
{
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
      return false;
  r = config->LoadConfig(path);
  LOG_MSG2("path : %s", path.c_str());
  circle_logic::CircleEngine::GetSchdulerManager();
  circle_db_ = new circle_logic::CircleDB(config);
  circle_logic::CircleEngine::GetSchdulerManager()->InitDB(circle_db_);
  circle_logic::CircleEngine::GetSchdulerManager()->InitData();
  base::SysRadom::GetInstance();
  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                      logic::SomeUtils::GetLibraryFunction(schduler_library, 
                      schduler_func);

  manager_schduler_engine_= (*schduler_engine)();
  if (manager_schduler_engine_ == NULL)
      assert(0);
  circle_logic::CircleEngine::GetSchdulerManager()->InitManagerSchduler(manager_schduler_engine_);
  return true;
}

Circlelogic *Circlelogic::GetInstance()
{
  if (instance_ == NULL)
    instance_ = new Circlelogic();
  return instance_;
}

void Circlelogic::FreeInstance()
{
  delete instance_;
  instance_ = NULL;
}

bool Circlelogic::OnCircleConnect(struct server *srv, const int socket)
{
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Circlelogic::OnCircleMessage(struct server *srv, const int socket,
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
  case R_GET_ALL_CIRCLE_INFO:{
    OnGetAllCircleInfo(srv, socket, packet);
    break;
  }
  case R_GET_STAR_CIRCLE_INFO:{
    OnGetStarCircleInfo(srv, socket, packet);
    break;
  }
  case R_STAR_ADD_CIRCLE:{
    OnAddCircleInfo(srv, socket, packet);
    break;
  }
  case R_STAR_DEL_CIRCLE:{
    OnDelCircleInfo(srv, socket, packet);
    break;
  }
  case R_USER_APPROVE_CIRCLE:{
    OnUserApproveCircle(srv, socket, packet);
    break;
  }
  case R_USER_COMMENT_CIRCLE:{
    OnUserCommentCircle(srv, socket, packet);
    break;
  }
  case R_STAR_REPLY_COMMENT:{
    OnStarReplyCircle(srv, socket, packet);
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

bool Circlelogic::OnCircleClose(struct server *srv, const int socket)
{
  return true;
}

bool Circlelogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  return true;
}

bool Circlelogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
      return false;

/*  if (!net::PacketProsess::UnpackStream(msg, len, &packet))
  {
      LOG_ERROR2("UnpackStream Error socket %d", socket);
      return false;
  }*/

  return true;
}


bool Circlelogic::OnBroadcastClose(struct server *srv, const int socket)
{
  return true;
}

bool Circlelogic::OnIniTimer(struct server *srv)
{
  if (srv->add_time_task != NULL)
  {
      srv->add_time_task(srv, "circle", TIME_DISTRIBUTION_TASK, 10, -1);
  }
  return true;
}

bool Circlelogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
  switch (opcode)
  {
  case TIME_DISTRIBUTION_TASK:
  {
    //circle_logic::CircleEngine::GetSchdulerManager()->InitData();
  }
  default:
      break;
  }
  return true;
}

bool Circlelogic::OnGetAllCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count;
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  if (!r1 || !r2){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetAllCircle(socket,
      packet->session_id, packet->reserved, pos, count);
  return true;
}

bool Circlelogic::OnGetStarCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count;
  std::string starcode;
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  bool r3 = packet_control->body_->GetString(L"star_code", &starcode);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetSymbolAllCircle(socket,
      packet->session_id, packet->reserved, starcode, pos, count);
  return true;
}

bool Circlelogic::OnAddCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode,content,picurl;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetString(L"content", &content);
  bool r3 = packet_control->body_->GetString(L"picurl", &picurl);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->CreateCircle(socket,
      packet->session_id, packet->reserved, starcode, content, picurl);
  return true;
}

bool Circlelogic::OnDelCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode;
  int64 circleid;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  if (!r1 || !r2){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->DeleteCircle(socket,
      packet->session_id, packet->reserved, starcode, circleid);
  return true;
}


bool Circlelogic::OnUserApproveCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode;
  int64 circleid,uid;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->ApprovalCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid);
  return true;
}

bool Circlelogic::OnUserCommentCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode, content;
  int64 circleid, uid, direction;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r4 = packet_control->body_->GetBigInteger(L"direction", &direction);
  bool r5 = packet_control->body_->GetString(L"content", &content);
  if (!r1 || !r2 || !r3 || !r4 || !r5){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->UserCommentCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid, direction, content);
  return true;
}


bool Circlelogic::OnStarReplyCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode, content;
  int64 circleid, uid, direction;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r4 = packet_control->body_->GetBigInteger(L"direction", &direction);
  bool r5 = packet_control->body_->GetString(L"content", &content);
  if (!r1 || !r2 || !r3 || !r4 || !r5){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->StarReplyCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid, direction, content);
  return true;
}



}  // namespace circle_logic

