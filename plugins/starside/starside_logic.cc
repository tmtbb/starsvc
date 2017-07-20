//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "starside/starside_logic.h"
#include "starside/starside_proto_buf.h"
#include "starside/operator_code.h"
#include "starside/errno.h"
#include "starside/schduler_engine.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/starside/starside_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace starside_logic {

StarSidelogic *StarSidelogic::instance_ = NULL;

StarSidelogic::StarSidelogic() {
  if (!Init())
    assert(0);
}

StarSidelogic::~StarSidelogic() {
  if (starside_db_) {
    delete starside_db_;
    starside_db_ = NULL;
  }
  starside_logic::StarSideEngine::FreeSchdulerManager();
  starside_logic::StarSideEngine::FreeStarSideEngine();
}

bool StarSidelogic::Init() {
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);
  starside_db_ = new starside_logic::StarSideDB(config);
  starside_logic::StarSideEngine::GetSchdulerManager()->InitDB(starside_db_);
  //starside_logic::StarSideEngine::GetSchdulerManager()->InitStarSideRechargeData();
  //starside_logic::StarSideEngine::GetSchdulerManager()->InitOwnStarData();

  starside_logic::StarSideEngine::GetSchdulerManager()->InitStarSideTransStatis();
  starside_logic::StarSideEngine::GetSchdulerManager()->InitStarMeetRelList();
  starside_logic::StarSideEngine::GetSchdulerManager()->InitServiceItem();
  starside_logic::StarSideEngine::GetSchdulerManager()->InitStarOwnService();
  starside_logic::StarSideEngine::GetSchdulerManager()->InitOwnStarUser();
  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
  logic::SomeUtils::GetLibraryFunction(
  schduler_library, schduler_func);schduler_engine_
  = (*schduler_engine)();
  if (schduler_engine_ == NULL)
    assert(0);

  starside_logic::StarSideEngine::GetSchdulerManager()->InitSchdulerEngine(
      schduler_engine_);
  return true;
}

StarSidelogic *StarSidelogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new StarSidelogic();
  return instance_;
}

void StarSidelogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool StarSidelogic::OnStarSideConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool StarSidelogic::OnStarSideMessage(struct server *srv, const int socket,
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
#ifdef _DEBUG
  struct timeval t_start,t_end;
  gettimeofday(&t_start, NULL);
  LOG_DEBUG2("recive operator[%d], star time[%ld]", packet->operate_code, t_start.tv_usec);
#endif

  switch (packet->operate_code) {
    case R_STARSIDE_TRANSSTATIS: {
      OnStarSideTransStatis(srv, socket, packet);
      break;
    }
    case R_STARSIDE_DETAIL: {
      OnStarSideDetail(srv, socket, packet);
      break;
    }
    case R_STARSIDE_GETSTARSERVICE: { //获取明星拥有的活动列表 005
    
        OnStarOwnService(srv, socket, packet);
        break;
    }
    case R_STARSIDE_GETALLSERVICE :
    {
        //获取所有活动类型
        OnServiceDefInfo(srv, socket, packet);
        break;
    }
    case R_STARSIDE_GETSTARMEETREL :
    {
        //获取预约明星列表
        OnGetStarMeetRel(srv, socket, packet); //
        break;
    }
        
    case R_STARSIDE_UPDSTARSERVICE :
    {
        OnUpdStarService(srv, socket, packet); //修改明星活动类型
        break;
    }
    case R_STARSIDE_UPDSTARMEETREL:
    {
        OnUpdStarMeetRel(srv, socket, packet); //明星同意预约
        break;
    }
    case R_STARSIDE_GETOWNSTARUSER :
    {
        OnGetOwnStarUser(srv, socket, packet);
        break;
    }

    /*
    case R_STARSIDE_RECHARGE: {
      OnStarSideRecharge(srv, socket, packet);
      break;
    }

    */

   /* 
    case R_STARSIDE_OWNSTAR: {
      OnStarSideOwnStar(srv, socket, packet);
      break;
    }
    */
     


    default:
      break;
  }
#ifdef _DEBUG
  gettimeofday(&t_end, NULL);
  LOG_DEBUG2("recive operator[%d], star time[%ld]", packet->operate_code, t_end.tv_usec);
  LOG_DEBUG2("operator[%d], Cost time[%ld]", packet->operate_code, t_end.tv_usec - t_start.tv_usec);
#endif
  return true;
}

bool StarSidelogic::OnStarSideClose(struct server *srv, const int socket) {
  return true;
}

bool StarSidelogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool StarSidelogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool StarSidelogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool StarSidelogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "starside", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool StarSidelogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    case TIME_DISTRIBUTION_TASK: {
    //  starside_logic::StarSideEngine::GetSchdulerManager()->InitStarMeetRelList();
      starside_logic::StarSideEngine::GetSchdulerManager()->InitServiceItem();
      starside_logic::StarSideEngine::GetSchdulerManager()->InitStarOwnService();
      starside_logic::StarSideEngine::GetSchdulerManager()->InitOwnStarUser();
      break;
    }
    default:
      break;
  }
  return true;
}

bool StarSidelogic::OnStarSideTransStatis(struct server* srv, int socket,
                                   struct PacketHead *packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  starside_logic::net_request::StarSideTransStatis starside_transstatis;
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = starside_transstatis.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  starside_logic::StarSideEngine::GetSchdulerManager()->SendStarSideTransStatis(
      socket, packet->session_id, 
      packet->reserved, starside_transstatis.starcode(),
      starside_transstatis.stardate(), 
      starside_transstatis.enddate());

  return true;
}

bool StarSidelogic::OnGetStarMeetRel(struct server* srv, int socket,
                                   struct PacketHead *packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode = "";
  int64 start = 0;
  int64 count = 0;
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  if (!packet_control->body_->GetBigInteger(L"starPos", &start))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  if (!packet_control->body_->GetBigInteger(L"count", &count))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  starside_logic::StarSideEngine::GetSchdulerManager()->SendStarMeetRel(
      socket, packet->session_id, starcode, start, count);

  return true;
}

bool StarSidelogic::OnStarSideDetail(struct server* srv, int socket,
                                   struct PacketHead *packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  starside_logic::net_request::ProfitDetail profitdetail;
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = profitdetail.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  starside_logic::StarSideEngine::GetSchdulerManager()->ProfitDetail(
      socket, packet->session_id, 
      profitdetail.starcode(),
      profitdetail.orderdate());


  return true;
}

bool StarSidelogic::OnStarOwnService(struct server* srv, int socket,
                                   struct PacketHead *packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode = ""; 
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  starside_logic::StarSideEngine::GetSchdulerManager()->SendStarOwnService(
      socket, packet->session_id , starcode);


  return true;
}

bool StarSidelogic::OnServiceDefInfo(struct server* srv, int socket,
                                   struct PacketHead *packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode = ""; 
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  starside_logic::StarSideEngine::GetSchdulerManager()->ServiceDefInfo(
      socket, packet->session_id );


  return true;
}
bool StarSidelogic::OnStarSideOwnStar(struct server* srv, int socket,
                                     struct PacketHead *packet) {
  starside_logic::net_request::StarSideOwnStar starside_ownstar;
  
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = starside_ownstar.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  starside_logic::StarSideEngine::GetSchdulerManager()->SendStarSideOwnStar(
      socket, packet->session_id, packet->reserved, starside_ownstar.id(),
      starside_ownstar.status(), starside_ownstar.start(),
      starside_ownstar.count());
  return true;
}


bool StarSidelogic::OnStarSideRecharge(struct server* srv, int socket,
                                     struct PacketHead *packet) {
  starside_logic::net_request::StarSideRecharge starside_recharge;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = starside_recharge.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  starside_logic::StarSideEngine::GetSchdulerManager()->SendStarSideRecharge(
      socket, packet->session_id, packet->reserved, starside_recharge.id(),
      starside_recharge.status(), starside_recharge.start(),
      starside_recharge.query_time(),starside_recharge.count());
  return true;
}

bool StarSidelogic::OnUpdStarMeetRel(struct server* srv, int socket,
                                     struct PacketHead *packet) {
  
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 meet_id = 0; 
  int64 meet_type = 4; //4同意
  std::string starcode = "";
  
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  if (!packet_control->body_->GetBigInteger(L"meetid", &meet_id))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  if (!packet_control->body_->GetBigInteger(L"meettype", &meet_type))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  starside_logic::StarSideEngine::GetSchdulerManager()->UpdStarMeetRel(
      socket, packet->session_id, meet_id, meet_type);

  starside_logic::StarSideEngine::GetSchdulerManager()->ModifyStarMeetRelLock(
  starcode, meet_id,meet_type); 
  
  return true;
}

bool StarSidelogic::OnGetOwnStarUser(struct server* srv, int socket,
                                     struct PacketHead *packet) {
  
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode = "";
  int64 start = 0;
  int64 count = 0;
  
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  if (!packet_control->body_->GetBigInteger(L"starPos", &start))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  if (!packet_control->body_->GetBigInteger(L"count", &count))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  starside_logic::StarSideEngine::GetSchdulerManager()->GetOwnStarUser(
      socket, packet->session_id, starcode, start, count);
  
  
  return true;
}
bool StarSidelogic::OnUpdStarService(struct server* srv, int socket,
                                     struct PacketHead *packet) {
  
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode ; 
  int64 mid = 4; //4同意
  
  if (!packet_control->body_->GetString(L"starcode", &starcode))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  if (!packet_control->body_->GetBigInteger(L"mid", &mid))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  int64 type = 0; //0-删除, 1-新增
  if (!packet_control->body_->GetBigInteger(L"type", &type))
  {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  starside_logic::StarSideEngine::GetSchdulerManager()->UpdStarService(
      socket, packet->session_id, starcode, mid, type);
  
  
  return true;
}
}  // namespace trades_logic
