// Copyright (c) 2016 The login Authors. All rights reserved.
// login_logic.cc

#include "net/comm_head.h"
#include "comm/comm_head.h"
#include "login/login_logic.h"

#include <assert.h>
#include <dlfcn.h>
#include <openssl/sha.h>
#include <signal.h>
#include <sys/socket.h>
#include <netdb.h>
#include <sstream>

#include "core/common.h"
#include "core/network.h"
#include "logic/base_values.h"
#include "http/http_method.h"
#include "basic/basictypes.h"
#include "config/config.h"
#include "basic/native_library.h"
#include "logic/base_values.h"
#include "login/login_opcode.h"
#include "pub/share/data_share_mgr.h"
#include "login/login_proto.h"
#include "basic/md5sum.h"
#include "logic/logic_unit.h"
#include "net/packet_processing.h"
#include "util/util.h"

#define DEFAULT_CONFIG_PATH "./plugins/login/login_config.xml"

#define SHELL_SMS "./verify_code_sms.sh"
#define SMS_KEY "yd1742653sd"
namespace login {
Loginlogic* Loginlogic::instance_ = NULL;

Loginlogic::Loginlogic() {
  login_mysql_ = NULL;

  if (!Init())
    assert(0);
}

Loginlogic::~Loginlogic() {
}

bool Loginlogic::Init() {
  bool r = false;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG_ERROR("Loginlogic config init error");
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG_ERROR("login config load error");
    return false;
  }
  login_mysql_ = new LoginMysql(config);
  InitShareData();
  return true;
}

bool Loginlogic::InitShareData() {
  basic::libhandle handle = NULL;
  handle = basic::load_native_library("./data.so");
  if (handle == NULL) {
    LOG_ERROR("Can't load path data.so\n");
  }LOG_MSG("load data.so success");
  share::DataShareMgr* (*pengine)(void);
  pengine = (share::DataShareMgr *(*)(
      void))basic::get_function_pointer(handle, "GetDataShareMgr");if
(  pengine==NULL) {
    LOG_ERROR("Can't find GetDataShareMgr\n");
    return false;
  }
  data_share_mgr_ = (*pengine)();
  return false;
}

Loginlogic* Loginlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Loginlogic();
  return instance_;
}

void Loginlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Loginlogic::OnLoginConnect(struct server *srv, const int socket) {
  LOG_MSG("OnLoginConnect");
  return true;
}

bool Loginlogic::OnLoginMessage(struct server *srv, const int socket,
                                const void *msg, const int len) {
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  PacketHead *packet = NULL;
  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    //    LOG_ERROR2("UnpackStream Error socket:%d", socket);
    send_error(socket, ERROR_TYPE, ERROR_TYPE, JSON_FORMAT_ERR);
    return false;
  }

  if (packet->type == LOGIN_TYPE ) {
    switch (packet->operate_code) {
      case HEARTBEAT_REQ: {
        OnHeartbeat(socket, packet);
        break;
      }
      case REGISTER_ACCOUNT_REQ: {
        OnRegisterAccount(socket, packet);
        break;
      }
      case USER_LOGIN_REQ: {
        OnUserLogin(socket, packet);
        break;
      }
      case CHANGE_PASSWD_REQ: {
        OnChangePasswd(socket, packet);
        break;
      }
    }
  }
  return true;
}

int32 Loginlogic::OnHeartbeat(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::Heartbeat heartbeat;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    heartbeat.set_http_packet(packet_recv->body_);
    data_share_mgr_->Heartbeat(heartbeat.uid());
  } while (0);
  return err;
}

int32 Loginlogic::OnRegisterAccount(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::RegisterAccount register_account;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    register_account.set_http_packet(packet_recv->body_);
    if (time(NULL) - register_account.timestamp() > 15000 * 60) {
      err = VERIFY_CODE_OVERDUE;
      break;
    }
    std::stringstream ss;
    ss << SMS_KEY << register_account.timestamp()
        << register_account.verify_code() << register_account.phone_num();
    base::MD5Sum md5(ss.str());
    if (md5.GetHash() != register_account.verify_token()) {
      err = VERIFY_CODE_ERR;
      break;
    }
    err = OnRegisterAccountUnit(socket, packet->session_id,
                                register_account.phone_num(),
                                register_account.passwd(), packet);

    /* 用于像用户中心注册使用
     packet->reserved = socket;
     LOG_MSG2("recv socket:%d", packet->reserved);
     send_message(server_fd_, packet);*/
  } while (0);
  if (err < 0) {
    send_error(socket, ERROR_TYPE, err, packet->session_id);
  }
  return err;
}

int32 Loginlogic::OnUserLogin(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::UserLogin user_login;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    user_login.set_http_packet(packet_recv->body_);
    //if (UserIsLogin(rev.phone_num()) {
    // todo
    //}

    OnUserLoginUnit(socket, packet->session_id, user_login.phone_num(),
                    user_login.passwd(), packet);
    //向用户中心请求
    //packet->reserved = socket;
    //send_message(server_fd_, packet);
  } while (0);
  if (err < 0) {
    send_error(socket, ERROR_TYPE, err, packet->session_id);
  }LOG_MSG2("UserLogin finish err:%d", err);
  return err;
}

int32 Loginlogic::OnChangePasswd(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::ChangePasswd change_passwd;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    change_passwd.set_http_packet(packet_recv->body_);
    if (time(NULL) - change_passwd.timestamp() > 1500 * 60) {
      err = VERIFY_CODE_OVERDUE;
      break;
    }
    std::stringstream ss;
    ss << SMS_KEY << change_passwd.timestamp() << change_passwd.verify_code()
        << change_passwd.phone_num();
    base::MD5Sum md5(ss.str());
    if (md5.GetHash() != change_passwd.verify_token()) {
      err = VERIFY_CODE_ERR;
      break;
    }
    /*UserInfo* p = data_share_mgr_->GetUser(change_passwd.uid();
     if (p == NULL || !p->is_login() {
     err = USER_NOT_IN_CACHE;
     break;
     }
     LOG_MSG("pwd:" << p->passwd();
     LOG_MSG("pwd:" << p->passwd();
     if (p->passwd() != change_passwd.old_passwd() {
     err = CHANGE_OLD_PWD_ERR;
     break;
     } else */{
      int32 type = change_passwd.type();
      if (type == net_request::ChangePasswd::PASSWD_LOGIN) {
        packet->reserved = socket;
        send_message(server_fd_, packet);
      } else if (type == net_request::ChangePasswd::PASSWD_TRADE) {
        err = login_mysql_->ChangePasswdUpdate(change_passwd.phone_num(),
                                               change_passwd.passwd());
        if (err < 0)
          break;
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, CHANGE_PASSWD_RLY, LOGIN_TYPE, 0, 0, 0);
        send_message(socket, &packet_control);
      }
      //p->set_passwd(change_passwd.new_passwd();
      ////SendMsg(socket, packet, NULL, CHANGE_PASSWD_RLY, err);
    }
  } while (0);
  if (err < 0) {
    send_error(socket, ERROR_TYPE, err, 0);
  }
  return err;
}

bool Loginlogic::UserIsLogin(std::string u) {
  return false;
}

bool Loginlogic::OnLoginClose(struct server *srv, const int socket) {
  return true;
}

bool Loginlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  server_fd_ = socket;

  LOG_MSG("OnBroadcastConnect");
  return true;
}

bool Loginlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  PacketHead *packet = NULL;
  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    //    LOG_ERROR2("UnpackStream Error socket:%d", socket);
    return false;
  }

  if (packet->type == LOGIN_TYPE) {
    switch (packet->operate_code) {
      case REGISTER_ACCOUNT_RLY: {
        OnRegisterAccountReply(socket, packet);
        break;
      }
      case USER_LOGIN_RLY: {
        OnUserLoginReply(socket, packet);
        break;
      }
      case CHANGE_PASSWD_RLY: {
        //packet->type = ERROR_TYPE;
        send_message(packet->reserved, packet);
        break;
      }
    }
    return true;
  }
  return false;
}

int32 Loginlogic::OnRegisterAccountReply(const int32 socket,
                                         PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::RegisterAccount register_account;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    register_account.set_http_packet(packet_recv->body_);
    if (err < 0)
      break;
    /*if (time(NULL) - rev.timestamp() > 15 * 60) {
     err = VERIFY_CODE_OVERDUE;
     break;
     }
     std::stringstream ss;
     ss << SMS_KEY << rev.timestamp() << rev.verify_code() << rev.phone_num();
     base::MD5Sum md5(ss.str();
     if (md5.GetHash() != rev.token() {
     err = VERIFY_CODE_ERR;
     break;
     }*/
    if (register_account.phone_num() != "") {
      DicValue dic;
      err = login_mysql_->RegisterInsertAndSelect(register_account.phone_num(),
                                                  register_account.passwd(),
                                                  &dic);
      if (err < 0)
        break;
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, REGISTER_ACCOUNT_RLY, LOGIN_TYPE, 0, 0, 0);
      packet_control.body_ = &dic;
      send_message(packet->reserved, &packet_control);
      //SendMsg(packet->reserved, packet, &dic, REGISTER_ACCOUNT_RLY, err);
    } else {
      send_message(packet->reserved, packet);
    }
  } while (0);
  if (err < 0) {
    packet->type = ERROR_TYPE;
    send_error(socket, ERROR_TYPE, err, packet->session_id);
  }
  return err;
}

int32 Loginlogic::OnUserLoginReply(const int32 socket, PacketHead* packet) {
  int32 err = 0;
  do {
    net_request::UserLogin user_login;
    struct PacketControl* packet_recv = (struct PacketControl*) (packet);
    user_login.set_http_packet(packet_recv->body_);
    if (UserIsLogin(user_login.phone_num())) {
      // todo
    }
    if (user_login.phone_num() != "") {
      ////////////CHANGE
      std::string client_ip;
      util::GetIPAddress(packet->reserved, &client_ip, NULL);
      DicValue dic;
      err = login_mysql_->UserLoginSelect(user_login.phone_num(), client_ip,
                                          &dic);
      if (err < 0)
        break;LOG_MSG2("packet->reserved:%d", packet->reserved);
      if (user_login.token() != "")
        dic.SetString("token", user_login.token());
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, USER_LOGIN_RLY, LOGIN_TYPE, 0, 0, 0);
      packet_control.body_ = &dic;
      send_message(packet->reserved, &packet_control);
      //SendMsg(packet->reserved, packet, &dic, USER_LOGIN_RLY, err);
      AddUser(packet->reserved, &dic, user_login.token());
    } else {
      LOG_MSG("token err here");
      packet->type = ERROR_TYPE;
      send_message(packet->reserved, packet);
      return -1;
    }
    //SendError(packet->reserved, packet, PHONE_OR_PASSWD_ERR, USER_LOGIN_RLY, err);
  } while (0);
  if (err < 0) {
    send_error(packet->reserved, ERROR_TYPE, err, packet->session_id);
  }LOG_MSG2("UserLoginReply finish err:%d", err);
  return err;
}

int32 Loginlogic::OnRegisterAccountUnit(const int socket, const int64 session,
                                        const std::string& phone,
                                        const std::string& passwd,
                                        PacketHead* packet) {
  DicValue dic;
  int32 err = 0;
  if (phone != "") {
    DicValue dic;
    err = login_mysql_->RegisterInsertAndSelect(phone, passwd, &dic);
    if (err < 0)
      return err;
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, REGISTER_ACCOUNT_RLY, LOGIN_TYPE, 0, session, 0);
    packet_control.body_ = &dic;
    send_message(socket, &packet_control);
  } else {
    send_message(socket, packet);
  }
  return err;
}

int32 Loginlogic::OnUserLoginUnit(const int socket, const int64 session,
                                  const std::string& phone,
                                  const std::string& passwd,
                                  PacketHead* packet) {
  DicValue dic;
  int32 err = 0;
  std::string token = "3DpzOR31SZ5leUP4yurk7PuAJr6hqVFhWhxhzsQfBc";
  if (phone != "") {
    std::string client_ip;
    util::GetIPAddress(socket, &client_ip, NULL);
    DicValue dic;
    err = login_mysql_->UserLoginSelect(phone, client_ip, &dic);
    if (err < 0)
      return err;LOG_MSG2("socket:%d", socket);
    if (token != "")
      dic.SetString("token", token);
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, USER_LOGIN_RLY, LOGIN_TYPE, 0, session, 0);
    packet_control.body_ = &dic;
    send_message(socket, &packet_control);
    AddUser(socket, &dic, token);
  } else {
    LOG_MSG("token err here");
    packet->type = ERROR_TYPE;
    send_message(packet->reserved, packet);
    return -1;
  }

  return err;
}

void Loginlogic::AddUser(int32 fd, DicValue* v, std::string token) {
  UserInfo* user = NULL;
  //游客
  user = new UserInfo();
  base_logic::DictionaryValue* dic;
  v->GetDictionary(L"userinfo", &dic);
  user->Serialization(dic);
  //    delete dic;
  user->set_is_login(true);
  user->set_socket_fd(fd);
  user->set_token(token);
  LOG_MSG2("add user id:%lld fd:%d", user->uid(), fd);
  data_share_mgr_->AddUser(user);
}

bool Loginlogic::OnBroadcastClose(struct server *srv, const int socket) {
  server_fd_ = -1;

  pthread_t tid;
  if (pthread_create(&tid, 0, Loginlogic::AutoReconnectToServer, (void*) srv)
      != 0)
    LOG_ERROR("can not create thread AutoReconnectToserver");
  pthread_detach(tid);

  return true;
}

bool Loginlogic::OnInitTimer(struct server *srv) {
  srv->add_time_task(srv, "login", CONNECT_CKECK, 15, -1);
  return true;
}

bool Loginlogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
  switch (opcode) {
    case CONNECT_CKECK: {
      data_share_mgr_->CheckHeartLoss();
      break;
    }
  }
  return true;
}

void* Loginlogic::AutoReconnectToServer(void* arg) {
  struct server* srv = (struct server*) arg;
  int ret = 0;

  do {
    ret = srv->create_reconnects(srv);
    sleep(1);
  } while (ret < 0);LOG_MSG2("try reconnect remote server:%d", ret);
}

int Loginlogic::SendFull(int socket, const char *buffer, size_t nbytes) {
  ssize_t amt = 0;
  ssize_t total = 0;
  const char *buf = buffer;

  do {
    amt = nbytes;
    amt = send(socket, buf, amt, 0);
    buf = buf + amt;
    nbytes -= amt;
    total += amt;
  } while (amt != -1 && nbytes > 0);

  LOG_MSG2("SendFull:%d", total);
  return (int) (amt == -1 ? amt : total);
}

/*void Loginlogic::SendPacket(const int socket, PacketHead* packet) {

 int packet_length = packet->packet_length;
 LOG_MSG("SendPacket packet_length:%d head:%d", packet_length, HEAD_LENGTH);
 LOG_MSG((char*)(packet + HEAD_LENGTH + 5);
 char* s = new char[packet_length + 1];
 //LOG_MSG("packet body:" << packet->body_str();
 //memset(s, 0, packet->packet_length);
 memcpy(s, &packet, packet_length);
 s[packet_length] = '\0';
 //memcpy(s + HEAD_LENGTH, packet->body_str().c_str(),
 //      packet->body_str().length();
 int total = SendFull(socket, s, packet_length);
 delete[] s;
 s = NULL;
 LOG_IF(ERROR, total != packet_length) << "send packet wrong");
 }*/

/*int Loginlogic::SendFull(int socket, const char *buffer, size_t nbytes) {
 ssize_t amt = 0;
 ssize_t total = 0;
 const char *buf = buffer;

 do {
 amt = nbytes;
 amt = send(socket, buf, amt, 0);
 buf = buf + amt;
 nbytes -= amt;
 total += amt;
 } while (amt != -1 && nbytes > 0);

 return (int) (amt == -1 ? amt : total);
 }

 void Loginlogic::SendPacket(const int socket, PacketHead* packet) {

 char* s = new char[packet->packet_length()];
 LOG_MSG("packet body:" << packet->body_str();
 memset(s, 0, packet->packet_length();
 memcpy(s, &packet->head(), HEAD_LENGTH);
 memcpy(s + HEAD_LENGTH, packet->body_str().c_str(),
 packet->body_str().length();
 int total = SendFull(socket, s, packet->packet_length();
 delete[] s;
 s = NULL;
 LOG_IF(ERROR, total != packet->packet_length() << "send packet wrong");
 }

 void Loginlogic::SendError(const int socket, PacketHead* packet, int32 err,
 int16 opcode) {
 PacketErr p_err;
 p_err.set_head(packet->head();
 p_err.set_type(3);
 p_err.set_error(err);
 p_err.Serialize();
 p_err.AdapterLen();
 p_err.set_operate_code(opcode);
 SendPacket(socket, &p_err);
 }

 void Loginlogic::SendMsg(const int socket, PacketHead* packet, DicValue* dic,
 int16 opcode) {
 PacketHead send;
 send.set_head(packet->head();
 send.Serialize(dic);
 send.AdapterLen();
 send.set_operate_code(opcode);
 SendPacket(socket, &send);
 }*/

}  // namespace login
