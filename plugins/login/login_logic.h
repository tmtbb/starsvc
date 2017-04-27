// Copyright (c) 2016 The login Authors. All rights reserved.
// login_logic.h
// Created on: 2016年9月1日
// Author: Paco

#ifndef PLUGINS_LOGIN_LOGIN_LOGIC_H_
#define PLUGINS_LOGIN_LOGIN_LOGIC_H_

#include "net/comm_head.h"
#include "core/common.h"
#include "login/proto_buf.h"
#include "login/login_mysql.h"
#include "share/data_share_mgr.h"
#include <list> 
#include <pthread.h>

namespace login {

class Loginlogic {
 public:
  Loginlogic();
  virtual ~Loginlogic();

 private:
  static Loginlogic *instance_;

 public:
  static Loginlogic *GetInstance();
  static void FreeInstance();

 public:
  bool OnLoginConnect(struct server *srv, const int socket);
  bool OnLoginMessage(struct server *srv, const int socket, const void *msg,
                      const int len);
  bool OnLoginClose(struct server *srv, const int socket);
  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);
  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);
  bool OnBroadcastClose(struct server *srv, const int socket);
  bool OnInitTimer(struct server *srv);
  bool OnTimeout(struct server *srv, char* id, int opcode, int time);

  static void* AutoReconnectToServer(void* arg);
  UserInfo* GetUser(int64 uid) {
    data_share_mgr_->GetUser(uid);
  }

 private:
  bool Init();
  bool InitShareData();
  void InitLog();

  int32 OnHeartbeat(const int32 socket, PacketHead* packet);
  int32 OnRegisterAccount(const int32 socket, PacketHead* packet);
  int32 OnUserLogin(const int32 socket, PacketHead* packet);
  bool UserIsLogin(std::string u);
  int32 OnChangePasswd(const int32 socket, PacketHead* packet);

  int32 OnRegisterAccountReply(const int32 socket, PacketHead* packet);
  int32 OnUserLoginReply(const int32 socket, PacketHead* packet);

  int32 OnRegisterAccountUnit(const int socket, const int64 session,
                              const std::string& phone,
                              const std::string& passwd, PacketHead* packet);

  int32 OnUserLoginUnit(const int socket, const int64 session,
                        const std::string& phone, const std::string& passwd,
                        PacketHead* packet);

  void AddUser(int32 fd, DicValue* v, std::string token);

  int SendFull(int socket, const char *buffer, size_t nbytes);
  void SendMsg(const int socket, PacketHead* packet, DicValue* dic,
               int16 opcode);
  void SendError(const int socket, PacketHead* packet, int32 err, int16 opcode);
  void SendPacket(const int socket, PacketHead* packet);

  LoginMysql* login_mysql_;
  int32 server_fd_;
  share::DataShareMgr* data_share_mgr_;
};

}  // namespace login

#endif  // PLUGINS_LOGIN_LOGIN_LOGIC_H_
