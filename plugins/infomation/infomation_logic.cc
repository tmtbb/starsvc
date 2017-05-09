#include "infomation_logic.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include "operator_code.h"

#define DEFAULT_CONFIG_PATH "./plugins/infomation/infomation_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace infomation_logic {

Infomationlogic *Infomationlogic::instance_ = NULL;

Infomationlogic::Infomationlogic() {
  Init();
}

Infomationlogic::~Infomationlogic() {
  if(sqldb != NULL){
  	delete sqldb;
  	sqldb = NULL;
  }
}

bool Infomationlogic::Init() {
  bool r = false;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG_ERROR("imlogic config init error");
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG_ERROR("login config load error");
    return false;
  }
  sqldb = new infomation_mysql::Infomation_Mysql(config);
  return true;
}

Infomationlogic *Infomationlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Infomationlogic();
  return instance_;
}

void Infomationlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Infomationlogic::OnInfomationConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool Infomationlogic::OnInfomationMessage(struct server *srv, const int socket,
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
    case R_STARINFOLIST_GET: {
	  GetStarinfoList(srv,socket,packet);
      break;
    }
	case R_STARINFOLIST_ADD:{
	  AddStarinfo(srv,socket,packet);
	  break;
	}
	case R_STARINFOLIST_DEL:{
	  break;
	}
    default:
      break;
  }

  return true;
}
bool Infomationlogic::GetStarinfoList(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string phone;
  int64 all;
  bool r1 = packet_recv->body_->GetString(L"code",&code);
  bool r2 = packet_recv->body_->GetString(L"phone",&phone);
  bool r3 = packet_recv->body_->GetBigInteger(L"all",&all);
  bool r = r1  && r2 && r3;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  DicValue ret_list;
  if(!sqldb->getstarinfo(code,phone,ret_list,all)){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}
bool Infomationlogic::AddStarinfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string phone;
  std::string name;
  int64 gender;
  std::string brief_url;
  double price;
  std::string accid;
  bool r1 = packet_recv->body_->GetString(L"name",&name);
  bool r2 = packet_recv->body_->GetString(L"code",&code);
  bool r3 = packet_recv->body_->GetString(L"phone",&phone);
  bool r4 = packet_recv->body_->GetBigInteger(L"gender",&gender);
  bool r5 = packet_recv->body_->GetString(L"brief_url",&brief_url);
  bool r6 = packet_recv->body_->GetReal(L"price",&price);
  bool r7 = packet_recv->body_->GetString(L"accid",&accid);
  
  bool r = r1  && r2&&r3&&r4&&r5&&r6&&r7;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  
  if(!sqldb->addstarinfo(code,phone,name,gender,brief_url,price,accid)){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",result);
  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);
 
  return true;
}

bool Infomationlogic::OnInfomationClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Infomationlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Infomationlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "infamation", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Infomationlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
}  // namespace
