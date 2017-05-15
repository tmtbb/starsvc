#include "market_logic.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include "operator_code.h"

#define DEFAULT_CONFIG_PATH "./plugins/market/market_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace market_logic {

Marketlogic *Marketlogic::instance_ = NULL;

Marketlogic::Marketlogic() {
  Init();
}

Marketlogic::~Marketlogic() {
  if(sqldb != NULL){
  	delete sqldb;
  	sqldb = NULL;
  }
}

bool Marketlogic::Init() {
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
  sqldb = new market_mysql::Market_Mysql(config);
  return true;
}

Marketlogic *Marketlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Marketlogic();
  return instance_;
}

void Marketlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Marketlogic::OnInfomationConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool Marketlogic::OnInfomationMessage(struct server *srv, const int socket,
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
  	//获取行情分类
    case R_MARKETTYPES_GET: {
	  getmarkettypes(srv,socket,packet);
      break;
    }
	//获取分类明细行情
	case R_MARKSTARLIST_GET:{
	  getmarketstarlist(srv,socket,packet);
	  break;
	}
	//获取明星简介
	case R_MARKSTARBRIEF_GET:{
	  getstarbrief(srv,socket,packet);
	  break;
	}
	//获取明星经历
	case R_MARKETSTAREX_GET:{
	  getstarexperience(srv,socket,packet);
	  break;
	}
	//获取明星成就
	case R_MARKETSTARACHIV_GET:{
	  getstarachive(srv,socket,packet);
	  break;
	}
	case R_SEARCHSTAR_INFO:{
	  searchsatr(srv,socket,packet);
	  break;
	}
    default:
      break;
  }

  return true;
}
bool Marketlogic::getstarachive(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getstarachive(code,ret_list)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getstarexperience(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getstarexperience(code,ret_list)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::searchsatr(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
	  std::string name;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->searchstarlist(code,ret_list)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
//测试正则过滤
bool Marketlogic::searchsatr1(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
	  std::string name;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->searchstarlist(code,ret_list)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}

bool Marketlogic::getstarbrief(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getstarbrief(code,ret_list)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getmarketstarlist(struct server* srv,int socket ,struct PacketHead* packet){
	  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  int64 type,startnum,endnum;
      bool r1 = packet_recv->body_->GetBigInteger(L"type",&type);
	  bool r2 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
	  bool r3 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
	  if(!(r1&&r2&&r3)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getmarketstarlist(type,ret_list,startnum,endnum)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}	  
bool Marketlogic::getmarkettypes(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);


  DicValue ret_list;
  if(!sqldb->getmarkettypes(ret_list)){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}


bool Marketlogic::OnInfomationClose(struct server *srv, const int socket) {
  return true;
}

bool Marketlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Marketlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Marketlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Marketlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "infamation", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Marketlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
}  // namespace
