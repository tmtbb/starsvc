

#include "im_logic.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include "operator_code.h"
#include "im_proto.h"
#include "im_process.h"

#define DEFAULT_CONFIG_PATH "./plugins/imcloud/imcoud_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace im_logic {

Imlogic *Imlogic::instance_ = NULL;

Imlogic::Imlogic() {
  Init();
}

Imlogic::~Imlogic() {

}

bool Imlogic::Init() {
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
  sqlengine = new im_mysql::Im_Mysql(config);
  return true;
}

Imlogic *Imlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Imlogic();
  return instance_;
}

void Imlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Imlogic::OnImConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  //test
  // im_process::ImProcess tokenfun;
  // std::string tokenvalue = tokenfun.gettoken("15306559323","15306559323");
  // LOG_MSG2("tokenvalue ============ %s ,length = %d\n",tokenvalue.c_str(),tokenvalue.length());
  // if(tokenvalue.length()<=0){
  //   tokenvalue = tokenfun.refreshtoken("15306559323");
  // }
  // LOG_MSG2("===========%s\n",tokenvalue.c_str());

  // std::string cli;
  // std::string arg = "123";
  // DicValue *dic = new base_logic::DictionaryValue();

  // int64 userid = 3333333333;
  // int64 phonenum = 1234;
  // std::string name = "testname";
  // std::string accid = "testaccid";
  // sqlengine->SetUserInfo(userid,phonenum,name,accid,dic);
  // sqlengine->GetStaticInfo(phonenum, dic);

  return true;
}

bool Imlogic::OnImMessage(struct server *srv, const int socket,
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
    case R_IMCLOUD_GETTOKEN: {
      OnGetTokenImcloud(srv, socket, packet);
      break;
    }
    case R_IMCLOUD_REFRESHTOKEN: {
      OnRefreshTokenImcloud(srv, socket, packet);
      break;
    }
    case R_IMCLOUD_ADDFRIEND:{
      OnAddCloudFriend(srv, socket, packet);	
      break;
    }
    case R_IMCLOUD_DELFRIEND:{
      OnDelCloudFriend(srv, socket, packet);
      break;
    }
    case R_IMCLOUD_LOGIN:{
      OnLoginImcloud(srv, socket, packet);
      break;
    }
	case R_IMCLOUD_GETFRIENDLIST:{
	  OnGetFriendList(srv, socket, packet);
	  break;
	}
	case R_IMCLOUD_EDITFRIENDINFO:{
	  OnEditFriendInfo(srv, socket, packet);
	  break;
	}
	case R_IMCLOUD_SENDMESSAGE:{
	  OnStarSendMessage(srv, socket, packet);
	  break;
	}
    default:
      break;
  }

  return true;
}
bool Imlogic::OnStarSendMessage(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string phone;
  std::string starcode;

  bool r1 = packet_recv->body_->GetString(L"phone",&phone);
  bool r2 = packet_recv->body_->GetString(L"starcode",&starcode);
  bool r = r1 && r2 ;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  //获取持有该明星时间，如果为1，则删除该记录；如果大于1，则做减1操作
  int64 times;
  std::string accid;
  std::string faccid;
  if(!sqlengine->gettalkingtimes(phone,starcode,times,accid,faccid)){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  if(times > 1){
	if(!sqlengine->ReduceTalkingtimes(phone,starcode)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	 }
  }else{
	if(!sqlengine->delorderrecord(phone,starcode)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	}
	//删除云信关联关系
	im_process::ImProcess im_pro;
	im_pro.delfriend(accid,faccid);
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_SENDMESSAGE, IM_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",result);
  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);
 
  return true;
}

bool Imlogic::OnEditFriendInfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string accid;
  std::string createtime;
  std::string faccid;
  std::string alias;
  std::string ex;
  bool r1 = packet_recv->body_->GetString(L"accid",&accid);
  bool r2 = packet_recv->body_->GetString(L"faccid",&faccid);
  bool r3 = packet_recv->body_->GetString(L"alias",&alias);
  bool r4 = packet_recv->body_->GetString(L"ex",&ex);
  bool r = r1 && r2 &&r3 && r4;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  im_process::ImProcess im_pro;

  r = im_pro.editfriendinfo(accid,faccid,alias,ex);
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_GETFRIENDLIST, IM_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",result);

  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);
 //sqlengine->SetUserInfo(userid,phonenum,tokencode.name(),tokencode.accid(),tokenvalue,dic);

  return true;
}

bool Imlogic::OnGetFriendList(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string accid;
  std::string createtime;
  bool r1 = packet_recv->body_->GetString(L"accid",&accid);
  bool r2 = packet_recv->body_->GetString(L"createtime",&createtime);
  bool r = r1 && r2;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  im_process::ImProcess im_pro;

  base_logic::DictionaryValue ret;
  r = im_pro.getfriendlist(accid,createtime,ret);
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_GETFRIENDLIST, IM_TYPE, 0,packet->session_id, 0);
  //base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",result);

  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);
 //sqlengine->SetUserInfo(userid,phonenum,tokencode.name(),tokencode.accid(),tokenvalue,dic);

  return true;
}

bool Imlogic::OnDelCloudFriend(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string accid;
  std::string faccid;
  std::string msg;
  int64 type;
  bool r1 = packet_recv->body_->GetString(L"accid",&accid);
  bool r2 = packet_recv->body_->GetString(L"faccid",&faccid);
  bool r = r1 && r2;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  im_process::ImProcess im_pro;
  r = im_pro.delfriend(accid,faccid);
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_ADDFRIEND, IM_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result",result);
  packet_reply.body_ = ret;
  send_message(socket,&packet_reply);
 //sqlengine->SetUserInfo(userid,phonenum,tokencode.name(),tokencode.accid(),tokenvalue,dic);

  return true;
}

bool Imlogic::OnAddCloudFriend(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string accid;
  std::string faccid;
  std::string msg;
  int64 type;
  bool r1 = packet_recv->body_->GetString(L"accid",&accid);
  bool r2 = packet_recv->body_->GetString(L"faccid",&faccid);
  bool r3 = packet_recv->body_->GetString(L"msg",&msg);
  bool r4 = packet_recv->body_->GetBigInteger(L"type",&type);
  bool r = r1 && r2 && r3 && r4;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  im_process::ImProcess im_process;
  r = im_process.addfriend(accid,faccid,msg,type);
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_DELFRIEND, IM_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result",result);
  packet_reply.body_ = ret;
  send_message(socket,&packet_reply);
  

  return true;
}
bool Imlogic::OnGetTokenImcloud(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  im_logic::net_request::tokencode tokencode;
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  std::string name;
  std::string accid;
  std::string phone;
  bool r1 = packet_control->body_->GetString(L"name_value", &name);
  bool r2 = packet_control->body_->GetString(L"accid_value", &accid);
  bool r3 = packet_control->body_->GetString(L"phone", &phone);

  bool r = (r1 && r2 && r3);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }	
  /*
  im_logic::net_request::tokencode tokencode;
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);
  tokencode.set_http_packet(packet_recv->body_);

  LOG_MSG2("getmessage-----:%s,%s",tokencode.name().c_str(),tokencode.accid().c_str());
  */
  im_process::ImProcess tokenfun;
  std::string tokenvalue = tokenfun.gettoken(name,accid);
  if(tokenvalue.length()<=0){
    tokenvalue = tokenfun.refreshtoken(accid);
  }

  if(tokenvalue.length() > 0){
    //构建回复包
    im_logic::net_reply::tokenreply reply;
    reply.set_token(tokenvalue);
    std::string code = "success";
    reply.set_result(code);
    struct PacketControl packet_reply;
    MAKE_HEAD(packet_reply, S_IMCLOUD_GETTOKEN, IM_TYPE, 0,packet->session_id, 0);
    packet_reply.body_ = reply.get();
    send_message(socket, &packet_reply);
    
    //写入数据
    DicValue *dic = new base_logic::DictionaryValue();
    int64 userid = 0;
    int64 phonenum = 0;
    sqlengine->SetUserInfo(phone,accid,tokenvalue,dic);
  }else{
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  return true;
}
bool Imlogic::OnRefreshTokenImcloud(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  im_logic::net_request::tokencode tokencode;
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);
  tokencode.set_http_packet(packet_recv->body_);

  im_process::ImProcess tokenfun;
  std::string tokenvalue = tokenfun.refreshtoken(tokencode.accid());
  if(sizeof(tokenvalue)<=0){
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }
	  
  //构建回复包
  im_logic::net_reply::tokenreply reply;
  reply.set_token(tokenvalue);
  std::string code = "success";
  reply.set_result(code);
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_IMCLOUD_REFRESHTOKEN, IM_TYPE, 0,packet->session_id, 0);
  packet_reply.body_ = reply.get();
  send_message(socket, &packet_reply);
  
  return true;
}
bool Imlogic::OnRegisterImcloud(struct server* srv,int socket ,struct PacketHead* packet){
  return true;
}
bool Imlogic::OnLoginImcloud(struct server* srv,int socket ,struct PacketHead* packet){
  return true;
}

bool Imlogic::OnImClose(struct server *srv, const int socket) {
  return true;
}

bool Imlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Imlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Imlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Imlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "im", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Imlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
}  // namespace
