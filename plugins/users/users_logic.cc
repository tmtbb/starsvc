//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "users/users_logic.h"
#include "users/users_proto_buf.h"
#include "users/operator_code.h"
#include "users/errno.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "net/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include <sstream>

#define DEFAULT_CONFIG_PATH "./plugins/users/users_config.xml"

namespace users_logic {

Userslogic *Userslogic::instance_ = NULL;

Userslogic::Userslogic() {
  if (!Init())
    assert(0);
}

Userslogic::~Userslogic() {
  if (user_db_) {
    delete user_db_;
    user_db_ = NULL;
  }
}

bool Userslogic::Init() {
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);

  user_db_ = new users_logic::UsersDB(config);

  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
  logic::SomeUtils::GetLibraryFunction(
  schduler_library, schduler_func);schduler_engine_
  = (*schduler_engine)();
  if (schduler_engine_ == NULL)
    assert(0);
  return true;
}

Userslogic *Userslogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Userslogic();
  return instance_;
}

void Userslogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Userslogic::OnUsersConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  
  /*测试用户注册
  const std::string testphonenum = "testphonenum";
  const std::string testpasswd = "testpasswd";
  const std::string agentid = "agentid";
  const std::string recommend = "recommend";
  int64 uid = 1;
  int64 memid = 1;
  int32 type = 0;
  int32 result = 1;
  bool r = user_db_->RegisterAccount(testphonenum,
                                    testpasswd, type,
                                    uid, result,agentid, 
                                    recommend,memid);
  if (!r) {  //用户已经存在
    LOG_ERROR("user has in sql==================");
  }
  */
  /*用户登录token
  const std::string phonenum = "phonenumtest";
  const std::string passwd = "passwdtest";
  const std::string ipaddr = "0.0.0.0";
  swp_logic::UserInfo userinfo;
  bool r = user_db_->LoginAccount(phonenum, passwd,
                             ipaddr, userinfo);
  if (!r || userinfo.uid() == 0) {
    LOG_ERROR("user login failed==================");
  }
  std::string token;
  logic::SomeUtils::CreateToken(userinfo.uid(), passwd, &token);
  userinfo.set_token(token);
  LOG_ERROR2("=========token===========%s",token.c_str());
  */
  return true;
}

bool Userslogic::OnUsersMessage(struct server *srv, const int socket,
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
    case R_ACCOUNT_REGISTER: {
      OnRegisterAccount(srv, socket, packet);
      break;
    }
    case R_ACCOUNT_LOGIN: {
      OnLoginAccount(srv, socket, packet);
      break;
    }
    case R_ACCOUNT_ASSET: {
      OnUserAccount(srv, socket, packet);
      break;
    }
    case R_ACCOUNT_CHECK: {
      OnUserCheckToken(srv, socket, packet);
      break;
    }
	
    case R_REGISTER_VERFIY_CODE:{
      OnRegisterVerifycode(srv, socket, packet);
      break;
    }
	case R_USRES_RESET_PASSWD:{
      OnResetPasswd(srv, socket, packet);
      break;
    }
    case R_WX_LOGIN:{
      OnLoginWiXin(srv, socket, packet);
      break;
    }
    case R_WX_BIND_ACCOUNT:{
      OnWXBindAccount(srv, socket, packet);
      break;
    }
	case R_ACCOUNT_CHANGEPASSWD:{
	  OnUserChangePasswd(srv, socket, packet);
	  break;
	}
    default:
      break;
  }
  return true;
}

bool Userslogic::OnUsersClose(struct server *srv, const int socket) {
  schduler_engine_->CloseUserInfoSchduler(socket);
  return true;
}

bool Userslogic::OnBroadcastConnect(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  return true;
}

bool Userslogic::OnBroadcastMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  printf("OnBroadcastMessage \n");
  return true;
}

bool Userslogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Userslogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
  }
  return true;
}

bool Userslogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
bool Userslogic::OnLoginWiXin(struct server* srv, int socket,
                                struct PacketHead *packet) {
  users_logic::net_request::LoginWiXin login_wixin;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  // bool r = login_wixin.set_http_packet(packet_control->body_);
  // if (!r) {
  //   LOG_DEBUG2("packet_length %d",packet->packet_length);
  //   send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
  //   return false;
  // }

  std::string ip, passwd = "";
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  swp_logic::UserInfo userinfo;
  
  std::string openid;
  std::string deviceid;
  packet_control->body_->GetString(L"openid",&openid);
  packet_control->body_->GetString(L"deviceId",&deviceid);

  LOG_ERROR2("get request value  openid : %s,deviceid: %s",openid.c_str(),deviceid.c_str());

  bool r = user_db_->LoginWiXin(openid, deviceid,
                            ip, userinfo, passwd);
  if (!r) {
    send_error(socket, ERROR_TYPE, NO_PASSWORD_ERRNOR, packet->session_id);
    return false;
  }

  //token 计算
  std::string token;
  logic::SomeUtils::CreateToken(userinfo.uid(), passwd, &token);
  userinfo.set_token(token);
  //发送用信息
  SendUserInfo(socket, packet->session_id, S_WX_LOGIN, userinfo);
  return true;
}
bool Userslogic::OnWXBindAccount(struct server* srv, int socket,
                                   struct PacketHead *packet) {
  users_logic::net_request::WXBindAccount wx_bind_account;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = wx_bind_account.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  int64 uid = 0;
  int32 result = 0;
  //注册数据库
  //
  
  r = user_db_->WXBindAccount(wx_bind_account.phone_num(),
                                wx_bind_account.passwd(), 0, uid, result,
				wx_bind_account.openid(), 
				wx_bind_account.nick_name(),
				wx_bind_account.head_url(),
				wx_bind_account.agentid(),
				wx_bind_account.recommend(),
				wx_bind_account.device_id(),
				wx_bind_account.member_id());
				//
  if (!r || result == 0) {  //用户已经存在
    send_error(socket, ERROR_TYPE, NO_USER_EXIST, packet->session_id);
    return false;
  }

  //返回绑定信息
  users_logic::net_reply::RegisterAccount net_register_account;
  net_register_account.set_result(1);
  net_register_account.set_uid(uid);

  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_ACCOUNT_REGISTER, 1, 0, packet->session_id,
            0);
  net_packet_control.body_ = net_register_account.get();
  send_message(socket, &net_packet_control);
  return true;
}

bool Userslogic::OnUserChangePasswd(struct server* srv, int socket,
                                   struct PacketHead *packet){

  bool r = false;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string phone_num;
  std::string old_passwd;
  std::string new_passwd;
  bool r1  = packet_control->body_->GetString(L"phone",&phone_num);
  bool r2  = packet_control->body_->GetString(L"oldpasswd",&old_passwd);
  bool r3  = packet_control->body_->GetString(L"newpasswd",&new_passwd);
  r = r1 && r2 && r3;
  if(!r){
  	LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  r = user_db_->UserChangePasswd(phone_num,old_passwd,new_passwd);
				
  if (!r) { 
    send_error(socket, ERROR_TYPE, NO_USER_EXIST, packet->session_id);
    return false;
  }

  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret->Set(L"result", result);
  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_ACCOUNT_REGISTER, 1, 0, packet->session_id,
            0);
  net_packet_control.body_ = ret;
  send_message(socket, &net_packet_control);
  return true;
  
}


bool Userslogic::OnRegisterAccount(struct server* srv, int socket,
                                   struct PacketHead *packet) {
  users_logic::net_request::RegisterAccount register_account;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  // bool r = register_account.set_http_packet(packet_control->body_);
  // if (!r) {
  //   LOG_DEBUG2("packet_length %d",packet->packet_length);
  //   send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
  //   return false;
  // }

  std::string passwd;
  std::string phonenum;
  std::string agentId;
  int64 memberId;
  std::string recommend;
  bool r1 = packet_control->body_->GetString(L"pwd", &passwd);
  bool r2 = packet_control->body_->GetString(L"phone", &phonenum);
  bool r3 = packet_control->body_->GetString(L"agentId", &agentId);
  bool r4 = packet_control->body_->GetString(L"recommend", &recommend);
  bool r5 = packet_control->body_->GetBigInteger(L"memberId", &memberId);
  LOG_ERROR2("---------------------%d,%d,%d,%d,%d",r1,r2,r3,r4,r5);
  bool r = (r1 && r2 && r3 && r4 && r5);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  int64 uid = 1;
  int32 result = 0;

  //注册数据库
  r = user_db_->RegisterAccount(phonenum,
                                    passwd, 0, 
                                    uid, result,agentId, 
                                    recommend,memberId);
  if (!r) {  //用户已经存在
    send_error(socket, ERROR_TYPE, NO_USER_EXIST, packet->session_id);
    return false;
  }

  //返回注册信息
  users_logic::net_reply::RegisterAccount net_register_account;
  net_register_account.set_result(1);
  net_register_account.set_uid(uid);

  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_ACCOUNT_REGISTER, 1, 0, packet->session_id,
            0);
  net_packet_control.body_ = net_register_account.get();
  send_message(socket, &net_packet_control);
  return true;
}

bool Userslogic::OnUserAccount(struct server* srv, int socket,
                               struct PacketHead *packet) {
  users_logic::net_request::UserAccount user_account;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = user_account.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  //数据库获取用户余额
  double balance = 0.0;
  swp_logic::UserInfo userinfo;
  users_logic::net_reply::Balance net_balance;
  r = schduler_engine_->GetUserInfoSchduler(user_account.uid(), &userinfo);
  if (!r)
    return r;

  r = user_db_->AccountBalance(user_account.uid(), balance);
  userinfo.set_balance(balance);

  net_balance.set_balance(userinfo.balance());
  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_ACCOUNT_ASSET, USERS_TYPE, 0,
            packet->session_id, 0);
  net_packet_control.body_ = net_balance.get();
  send_message(socket, &net_packet_control);

  return true;
}

bool Userslogic::OnLoginAccount(struct server* srv, int socket,
                                struct PacketHead *packet) {
  users_logic::net_request::LoginAccount login_account;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = login_account.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);

  swp_logic::UserInfo userinfo;
  r = user_db_->LoginAccount(login_account.phone_num(), login_account.passwd(),
                             ip, userinfo);
  if (!r) {
    send_error(socket, ERROR_TYPE, NO_PASSWORD_ERRNOR, packet->session_id);
    return false;
  }

  //token 计算
  std::string token;
  logic::SomeUtils::CreateToken(userinfo.uid(), login_account.passwd(), &token);
  userinfo.set_token(token);
  userinfo.set_phone_num(login_account.phone_num());
  
  //发送用信息
  SendUserInfo(socket, packet->session_id, S_ACCOUNT_LOGIN, userinfo);
  return true;
}

bool Userslogic::OnUserCheckToken(struct server* srv, int socket,
                                  struct PacketHead *packet) {
  users_logic::net_request::CheckToken check_token;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = check_token.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  swp_logic::UserInfo userinfo;
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  userinfo.set_token(check_token.token());
  //check token
  r = logic::SomeUtils::VerifyToken(check_token.uid(), check_token.token());
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }

  //获取用户信息
  r = user_db_->GetUserInfo(check_token.uid(), ip, userinfo);
  //发送用信息
  SendUserInfo(socket, packet->session_id, S_ACCOUNT_CHECK, userinfo);
  return true;
}
bool Userslogic::OnResetPasswd(struct server* srv, int socket,
                                      struct PacketHead *packet) {
  users_logic::net_request::RegisterAccount register_account;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);


  std::string phonenum;
  std::string passwd;
  bool r1 = packet_control->body_->GetString(L"pwd", &passwd);
  bool r2 = packet_control->body_->GetString(L"phone", &phonenum);

  bool r = (r1 && r2);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  int64 uid = 1;
  int32 result = 0;

  r = user_db_->ResetAccount(phonenum,passwd);
  if (!r) {
    send_error(socket, ERROR_TYPE, NO_USER_EXIST, packet->session_id);
    return false;
  }


  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_USRES_RESET_PASSWD, USERS_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  base_logic::FundamentalValue* m_result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",m_result);
  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);                   

}
void executeCMD(const char *cmd, char *result){       
	char buf_ps[1024];      
	char ps[1024]={0};      
	FILE *ptr;       
	strcpy(ps, cmd);     
	if((ptr=popen(ps, "r"))!=NULL)      
	{    	   
		while(fgets(buf_ps, 1024, ptr)!=NULL)    		
		{    		   
			strcat(result, buf_ps);    		   
			if(strlen(result)>1024)   			   
			break;    		
		}    		
		pclose(ptr);    		
		ptr = NULL;    	
	}    	
	else   	
	{    		
		printf("popen %s error\n", ps);   	
	}    
}
bool Userslogic::OnRegisterVerifycode(struct server* srv, int socket,
                                      struct PacketHead *packet) {
  users_logic::net_request::RegisterVerfiycode register_vercode;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = register_vercode.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  
  
  std::string phone = register_vercode.phone();

  /*
  ////检测号码是否已经注册
  r =user_db_->CheckAccountExist(phone);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, NO_USER_EXIST_REGISTER, packet->session_id);
    return false;
  }
  */
  
  int64 rand_code = 100000 + rand() % (999999 - 100000 + 1);
  std::string shell_sms = SHELL_SMS;
  std::stringstream ss;
  ss << SHELL_SMS << " " << phone << " "
      <<rand_code<<" "
      << 0;

  std::string sysc = ss.str();
  //system(sysc.c_str());
  char m_ret[1024] = {0};
  executeCMD(sysc.c_str(),m_ret);
  LOG_MSG2("send shell : %s,result = %s",sysc.c_str(),m_ret);
  if(strstr(m_ret,"\"success\":false")!=NULL){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
  }
  
  //发送信息
  int64 code_time =  time(NULL);
  std::string v_token = SMS_KEY + base::BasicUtil::StringUtil::Int64ToString(code_time) +
      base::BasicUtil::StringUtil::Int64ToString(rand_code) + phone;
  base::MD5Sum md5(v_token.c_str());
  std::string token = md5.GetHash();
  LOG_MSG2("====v_token = %s",v_token.c_str());
  users_logic::net_reply::RegisterVerfiycode register_verfiy;
  register_verfiy.set_code_time(code_time);
  register_verfiy.set_token(token);
  int64 result_ = 1;
  register_verfiy.set_result(result_);
  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control,S_REGISTER_VERFIY_CODE, 1, 0, packet->session_id, 0);
  net_packet_control.body_ = register_verfiy.get();
  send_message(socket, &net_packet_control);

  return true;
}

bool Userslogic::SendUserInfo(const int socket, const int64 session,
                              const int32 opcode,
                              swp_logic::UserInfo& userinfo) {
  userinfo.set_socket_fd(socket);
  userinfo.set_is_effective(true);

  //写入共享数据库中
  users_logic::net_reply::LoginAccount net_login_account;
  users_logic::net_reply::UserInfo net_userinfo;
  net_userinfo.set_balance(userinfo.balance());
  net_userinfo.set_phone(userinfo.phone_num());
  net_userinfo.set_uid(userinfo.uid());
  net_userinfo.set_type(userinfo.type());
  net_login_account.set_userinfo(net_userinfo.get());
  net_login_account.set_token(userinfo.token());
  schduler_engine_->SetUserInfoSchduler(userinfo.uid(), &userinfo);

  swp_logic::UserInfo tuserinfo;
  schduler_engine_->GetUserInfoSchduler(userinfo.uid(), &tuserinfo);
  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, opcode, 1, 0, session, 0);
  net_packet_control.body_ = net_login_account.get();
  send_message(socket, &net_packet_control);
  return true;
}
}  // namespace trades_logic
