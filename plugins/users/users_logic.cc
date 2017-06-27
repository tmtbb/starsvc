//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "users/users_logic.h"
#include "users/users_proto_buf.h"
#include "users/operator_code.h"
#include "users/errno.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "net/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include <sstream>
#include "http/http_api.h"

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
    //delete kafka_;
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
                          schduler_library, schduler_func);
    schduler_engine_ = (*schduler_engine)();
    //kafka_ = new users_logic::StroagerKafka(config);
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
    star_logic::UserInfo userinfo;
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
    try
    {
        switch (packet->operate_code) {
        case R_ACCOUNT_REGISTER: {
            OnRegisterAccount(srv, socket, packet);
            break;
        }
        case R_ACCOUNT_LOGIN: {
            OnLoginAccount(srv, socket, packet);
            break;
        }
        case R_RESET_PAY_PASS: { //重置支付密码
            OnResetPayPassWD(srv, socket, packet);
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

        case R_REGISTER_VERFIY_CODE: {
            OnRegisterVerifycode(srv, socket, packet);
            break;
        }
        case R_USRES_RESET_PASSWD: {
            OnResetPasswd(srv, socket, packet);
            break;
        }
        case R_WX_LOGIN: {
            OnLoginWiXin(srv, socket, packet);
            break;
        }
        case R_WX_BIND_ACCOUNT: {
            OnWXBindAccount(srv, socket, packet);
            break;
        }
        case R_ACCOUNT_CHANGEPASSWD: {
            OnUserChangePasswd(srv, socket, packet);
            break;
        }
        case R_CERTIFICATION: {
            OnCertification(srv, socket, packet);
            break;
        }
        case R_ACCOUNT_REALINFO :
        {
            OnUserRealInfo(srv, socket, packet);
            break;
        }
        case R_CHECK_ACCOUNT_EXIST :
        {
            OnCheckAccountExist(srv, socket, packet);
            break;
        }
        case R_USRES_RESET_NICK_NAME :
        {
            OnResetNickName(srv, socket, packet);
            break;
        }

	case R_GET_VERSION:{
      OnGetVersion(srv, socket, packet);
      break;
    }
    default:
      break;
  }
}
catch(...)
{
  LOG_ERROR2("catch : operator[%d]", packet->operate_code);
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
    star_logic::UserInfo userinfo;

    std::string openid;
    std::string deviceid;
    packet_control->body_->GetString(L"openid",&openid);
    packet_control->body_->GetString(L"deviceId",&deviceid);

    LOG_ERROR2("get request value  openid : %s,deviceid: %s",openid.c_str(),deviceid.c_str());
    base_logic::DictionaryValue ret_list;
    bool r = user_db_->LoginWiXin(openid, deviceid,ip, ret_list);
    if (!r) {
        send_error(socket, ERROR_TYPE, NO_PASSWORD_ERRNOR, packet->session_id);
        return false;
    }

    //userinfo.set_token(token);
    //发送用信息
    //SendUserInfo(socket, packet->session_id, S_WX_LOGIN, userinfo);

    struct PacketControl packet_reply;
    //base_logic::DictionaryValue ret_list;
    MAKE_HEAD(packet_reply, S_WX_LOGIN, INFO_TYPE, 0,packet->session_id, 0);
    base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
    //token 计算
    std::string token;
    logic::SomeUtils::CreateToken(userinfo.uid(), passwd, &token);
    int64 ret = 1;
    ret_list.SetBigInteger(L"result",ret);
    ret_list.SetString(L"token",token);

    packet_reply.body_ = &ret_list;
    //send_message(socket,&packet_reply);
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
                                wx_bind_account.member_id(),
                                wx_bind_account.sub_agentid());
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
                                    struct PacketHead *packet) {

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
    if(!r) {
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
    std::string sub_agentId;
    std::string memberId;
    std::string recommend;
    bool r1 = packet_control->body_->GetString(L"pwd", &passwd);
    bool r2 = packet_control->body_->GetString(L"phone", &phonenum);
    bool r3 = packet_control->body_->GetString(L"agentId", &agentId);
    bool r4 = packet_control->body_->GetString(L"recommend", &recommend);
    bool r5 = packet_control->body_->GetString(L"memberId", &memberId);
    bool r6 = packet_control->body_->GetString(L"sub_agentId", &sub_agentId);
    LOG_ERROR2("---------------------%d,%d,%d,%d,%d",r1,r2,r3,r4,r5);
    bool r = (r1 && r2 && r3 && r4 && r5 && r6);
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
                                  recommend,memberId,sub_agentId);
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
  star_logic::UserInfo userinfo;
  users_logic::net_reply::Balance net_balance;
  r = schduler_engine_->GetUserInfoSchduler(user_account.uid(), &userinfo);
  if (!r){
    LOG_DEBUG2("uid[%ld]",user_account.uid());
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return r;
  }
  
  std::string pwd;
  r = user_db_->AccountBalance(user_account.uid(), balance, pwd);
  if (!r){
    LOG_DEBUG2("uid[%ld]",user_account.uid());
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return r;
  }
  userinfo.set_balance(balance);

  net_balance.set_nick_name(userinfo.nickname());
  std::string t_sUserHeadUrl = userinfo.head_url();
  net_balance.set_head_url(t_sUserHeadUrl);
  net_balance.set_balance(userinfo.balance());
  net_balance.set_total_amt(0.0);
  net_balance.set_market_cap(0.0);
  if (pwd.length() > 0)
    net_balance.set_is_setpwd(0);
  else
    net_balance.set_is_setpwd(1);

  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_ACCOUNT_ASSET, USERS_TYPE, 0,
            packet->session_id, 0);
  net_packet_control.body_ = net_balance.get();
  send_message(socket, &net_packet_control);

  return true;
}

bool Userslogic::OnUserRealInfo(struct server* srv, int socket,
                               struct PacketHead *packet) {
  users_logic::net_request::UserRealInfo user_real_info;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = user_real_info.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  //
  //double balance = 0.0;
  star_logic::UserInfo userinfo;
  users_logic::net_reply::RealInfo net_real_info;
  //获取用户信息
  r = schduler_engine_->GetUserInfoSchduler(user_real_info.uid(), &userinfo);
  if (!r) {
    LOG_DEBUG2("uid[%ld]",user_real_info.uid());
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  if (userinfo.id_card().length() < 10) //如果没有实名认证信息则获取
  {
    std::string r_name = "" , id_card = "";
    r = user_db_->AccountRealNameInfo(user_real_info.uid(), r_name, id_card);
    if (r_name != "")
    {
        userinfo.set_realname(r_name);
        userinfo.set_id_card(id_card);
    }
        LOG_DEBUG2("realname[%s], id_card[%s], realname2[%s], id_card2[%s], ",
                   r_name.c_str(), id_card.c_str(), userinfo.realname().c_str(), userinfo.id_card().c_str());
  }
  
// net_real_info.set_balance(userinfo.balance());
    net_real_info.set_realname(userinfo.realname());
    net_real_info.set_id_card(userinfo.id_card());
    struct PacketControl net_packet_control;
    MAKE_HEAD(net_packet_control, S_ACCOUNT_REALINFO, USERS_TYPE, 0,
              packet->session_id, 0);
    net_packet_control.body_ = net_real_info.get();
    send_message(socket, &net_packet_control);

    return true;
}

bool Userslogic::CheckUserIsLogin(star_logic::UserInfo &userinfo) {
    star_logic::UserInfo tuserinfo;
    LOG_DEBUG2("_______________ uid%ld, socket[%d]", userinfo.uid(), userinfo.socket_fd());
    if (schduler_engine_->GetUserInfoSchduler(userinfo.uid(), &tuserinfo))
    {
        //已登陆发送退出消息
        struct PacketControl packet_reply;
        base_logic::DictionaryValue ret_list;
        MAKE_HEAD(packet_reply, S_LOGIN_EXISTS, USERS_TYPE, 0,0, 0);
        packet_reply.body_ = &ret_list;
        int64 ret = 1;
        ret_list.SetBigInteger(L"result",ret);
        send_message(tuserinfo.socket_fd(),&packet_reply);
        schduler_engine_->CloseUserInfoSchduler(tuserinfo.socket_fd());
        LOG_DEBUG2("close__________________ uid%ld, socket[%d]", userinfo.uid(), userinfo.socket_fd());
    }
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

    star_logic::UserInfo userinfo;
    r = user_db_->LoginAccount(login_account.phone_num(), login_account.passwd(),
                               ip, userinfo);
    CheckUserIsLogin(userinfo);
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

  star_logic::UserInfo userinfo;
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  userinfo.set_token(check_token.token());
  //check token
  r = logic::SomeUtils::VerifyToken(check_token.uid(), check_token.token());
  if (!r) {
    LOG_DEBUG2("uid[%ld]",check_token.uid());
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }

  //获取用户信息
  r = user_db_->GetUserInfo(check_token.uid(), ip, userinfo);
  if(!r){
    LOG_DEBUG2("uid[%ld],ip[%s]",check_token.uid(), ip.c_str());
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }
  
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
    LOG_DEBUG2("phonenum[%s],passwd[%s]",phonenum.c_str(),passwd.c_str());
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
  return r;
}
void executeCMD(const char *cmd, char *result) {
    char buf_ps[1024];
    char ps[1024]= {0};
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
  
  std::string phone = register_vercode.phone().c_str();
  
/*  
  ////检测号码是否已经注册
  if(check_account_flag == CHECK_ACCOUNT_Y){
	  r =user_db_->CheckAccountExist(phone);
	  if (!r) {
	    LOG_DEBUG2("packet_length %d",packet->packet_length);
	    send_error(socket, ERROR_TYPE, NO_USER_EXIST_REGISTER, packet->session_id);
	    return true;
	  }
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
    return false;
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
                              star_logic::UserInfo& userinfo) {
    userinfo.set_socket_fd(socket);
    userinfo.set_is_effective(true);

    //写入共享数据库中
    users_logic::net_reply::LoginAccount net_login_account;
    users_logic::net_reply::UserInfo net_userinfo;
    net_userinfo.set_balance(userinfo.balance());
    net_userinfo.set_phone(userinfo.phone_num());
    net_userinfo.set_uid(userinfo.uid());
    net_userinfo.set_type(userinfo.type());
    net_userinfo.set_agent_name(userinfo.nickname());
    net_userinfo.set_avatar_large(userinfo.head_url());
    net_login_account.set_userinfo(net_userinfo.get());
    net_login_account.set_token(userinfo.token());
    schduler_engine_->SetUserInfoSchduler(userinfo.uid(), &userinfo);

    star_logic::UserInfo tuserinfo;
    schduler_engine_->GetUserInfoSchduler(userinfo.uid(), &tuserinfo);
    struct PacketControl net_packet_control;
    MAKE_HEAD(net_packet_control, opcode, 1, 0, session, 0);
    net_packet_control.body_ = net_login_account.get();
    send_message(socket, &net_packet_control);
    return true;
}

bool Userslogic::OnResetPayPassWD(struct server* srv, int socket,
                                struct PacketHead *packet) {
  users_logic::net_request::ModifyPwd modifypwd;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  } 
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = modifypwd.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  } 
  std::string phone = modifypwd.phone();
  std::string v_token = SMS_KEY + base::BasicUtil::StringUtil::Int64ToString(modifypwd.timestamp()) +
      modifypwd.vcode().c_str() + phone;
  base::MD5Sum md5(v_token.c_str());
  std::string token = md5.GetHash();
  
  users_logic::net_reply::ModifyPwd net_modifypwd;
  int status = 1; //0 sucess ,1 failed
  LOG_DEBUG2("v_token[%s]token[%s]vtoken[%s]", v_token.c_str(), token.c_str(),modifypwd.vtoken().c_str());
  LOG_DEBUG2("type [%d]", modifypwd.type());
  if (!strcmp(token.c_str(), modifypwd.vtoken().c_str())
  || modifypwd.type() == 0) //验证token type 0-设置密码1-重置密码
  {  
    LOG_DEBUG2("phone[%s]token[%s]vtoken[%s]___________________", modifypwd.phone().c_str(), token.c_str(),modifypwd.vtoken().c_str());
    LOG_DEBUG2("pwd[%s]___________________", modifypwd.pwd().c_str());
    //std::string phone = modifypwd.phone() ;
    std::string pwd = modifypwd.pwd() ;
    r = user_db_->ModifyPwd(modifypwd.uid(), pwd);
    LOG_DEBUG2("pwd[%s]",pwd.c_str());
    if (r) status = 0;
  } 
  net_modifypwd.set_status(status);
  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_RESET_PAY_PASS, USERS_TYPE, 0,
            packet->session_id, 0);
  net_packet_control.body_ = net_modifypwd.get();
  send_message(socket, &net_packet_control);

  return true;
}

bool Userslogic::OnCertification(struct server* srv, int socket,
                            struct PacketHead *packet) {

  users_logic::net_request::Certification cerfic;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_= (struct PacketControl*) (packet);
  bool r = cerfic.set_http_packet(packet_->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  std::string idcard = cerfic.id_card();//"411325199005217439";
  std::string name = cerfic.realname();//"唐伟";
  
  //std::string idcard = "411325199005217439";
  //std::string name = "唐伟";
  //阿里云接口
  std::string strUrl = "http://idcardreturnphoto.haoservice.com/idcard/VerifyIdcardReturnPhoto";
  ////阿里云接口code
  //std::string strHeader = "Authorization:APPCODE 900036feeee64ae089177dd06b25faa9";
  std::string strHeader = "Authorization:APPCODE e361298186714a6faea52316ff1d5c32";
  std::string strResult;
  base_logic::DictionaryValue dic;
  dic.SetString(L"cardNo", idcard);
  dic.SetString(L"realName", name);
  base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, strHeader, 1);
  //base_http::HttpAPI::RequestGetMethod(strUrl, &dic, strResult, 1);
  LOG_DEBUG2("strResult [%s]___________________________________________________", strResult.c_str());

  users_logic::net_reply::TResult r_ret;;
  r_ret.set_result(1);
  //r_ret.set_result(0);
//_________________________________________________________
    base_logic::ValueSerializer* serializer = base_logic::ValueSerializer::Create(
                base_logic::IMPL_JSON, &strResult, false);
    std::string err_str;
    DicValue* dicResultJosn;
    int32 err = 0;
    DicValue* dicJosn = (DicValue*)serializer->Deserialize(&err, &err_str);
    r = false;
    if (dicJosn != NULL) {
        r = dicJosn->GetDictionary(L"result", &dicResultJosn);
        if (r)
        {
            //解析第二层
            int32 err = 0;
            bool bResultIsOk = false;
            if (dicResultJosn != NULL) {
                r = dicResultJosn->GetBoolean(L"isok", &bResultIsOk);
                if (r)
                {
                    //更新数据
                    std::stringstream strsql;
                    if (bResultIsOk)
                    {
                        LOG_DEBUG("strResult ___________________________________________________ok" );
                        //mysql_engine_->WriteData(strsql.str());
                        //map_IdCard_Info_.erase(l_it);
                        r = user_db_->Certification(cerfic.uid(), cerfic.id_card(), cerfic.realname());
                        if (r)
                            r_ret.set_result(0);
                    }
                    else
                        LOG_DEBUG("strResult ___________________________________________________ err" );
                }
            }
        }
    }
    else {
        LOG_DEBUG("josn Deserialize error[]___________________________________________________ err" );
    }
//_____________________________________________________________________________________________

//
  if(!r){
    //code 3 curl_easy_perform failed: HTTP response code said error
    //网站报错, 特殊处理
    r_ret.set_result(0);
    r = user_db_->Certification(cerfic.uid(), cerfic.id_card(), cerfic.realname());
  }

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_CERTIFICATION, USERS_TYPE, 0, packet->session_id, 0);
  packet_control.body_ = r_ret.get();
  send_message(socket, &packet_control);
/*
*/
  return true;
}

bool Userslogic::OnCheckAccountExist(struct server* srv, int socket,
                                      struct PacketHead *packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  users_logic::net_request::CheckAccountExistReq check_acount_exist_req;
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = check_acount_exist_req.set_http_packet(packet_control->body_);
  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  std::string phone = check_acount_exist_req.phone().c_str();
  int32 existFlag = 0;
  //检测号码是否已经注册
  r =user_db_->CheckAccountExist(phone, existFlag);
  if(!r){
    LOG_DEBUG2("phone[%s]",phone.c_str());
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }
  
  //发送信息
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_CHECK_ACCOUNT_EXIST, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", existFlag); /*0表示不存在未注册*/
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 

  return true;
}

bool Userslogic::OnResetNickName(struct server* srv, int socket,
                                      struct PacketHead *packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  bool r1,r2,r3;
  int64 uid;
  std::string token;
  std::string nickname;
  r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  r2 = packet_control->body_->GetString(L"token", &token);
  r3 = packet_control->body_->GetString(L"nickname", &nickname);
  
  bool r = (r1 && r2 && r3);
  if (!r1 || !r2 || !r3) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  //check token
  r = logic::SomeUtils::VerifyToken(uid, token);
  if (!r) {
    LOG_DEBUG2("uid[%ld], token[%s]",uid, token.c_str());
    send_error(socket, ERROR_TYPE, NO_CHECK_TOKEN_ERRNO, packet->session_id);
    return false;
  }

  int32 flag = 0;
  r = user_db_->ModifyNickName(uid,nickname,flag);
  if (!r) {
    LOG_DEBUG2("uid[%ld], nickname[%s]",uid, nickname.c_str());
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }  

  //发送信息
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_USRES_RESET_NICK_NAME, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", 0);
  if (r) {
    //获取用户信息
	  star_logic::UserInfo userinfo;
	  r = user_db_->GetUserInfo(uid, "", userinfo);
	  if(!r){
	    LOG_DEBUG2("GetUserInfo error, uid[%ld]",uid);
	  }
	  else{
	    userinfo.set_socket_fd(socket);
      userinfo.set_is_effective(true);
      schduler_engine_->SetUserInfoSchduler(userinfo.uid(), &userinfo);
      dic.SetInteger(L"result", 1);
	  }
  }
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 

  return true;
}
bool Userslogic::OnGetVersion(struct server* srv, int socket,
                                struct PacketHead *packet) {
  users_logic::net_request::TGetVersion get_version;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = get_version.set_http_packet(packet_control->body_);

  if (!r) {
    LOG_DEBUG2("packet_length %d",packet->packet_length);
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
/*
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);

  swp_logic::UserInfo userinfo;
*/
  users_logic::net_reply::TGetVersion net_get_version;
  r = user_db_->GetVersion(get_version.type(), net_get_version);
  if (!r) {
    send_error(socket, ERROR_TYPE, NO_VERSION_INFO, packet->session_id);
    return false;
  }

  struct PacketControl net_packet_control;
  MAKE_HEAD(net_packet_control, S_GET_VERSION, 1, 0, packet->session_id, 0);
  net_packet_control.body_ = net_get_version.get();
  send_message(socket, &net_packet_control);

  return true;
}
}  // namespace trades_logic
