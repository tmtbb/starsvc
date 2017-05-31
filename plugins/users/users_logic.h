//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_USER_USER_LOGIC_
#define SWP_USER_USER_LOGIC_

#include "users/users_db.h"
#include "manager/data_share_mgr.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"

#define SHELL_SMS "./verify_code_sms.sh"
#define SMS_KEY "yd1742653sd"

namespace users_logic {

class Userslogic {
 public:
  Userslogic();
  virtual ~Userslogic();
 private:
  static Userslogic *instance_;

 public:
  static Userslogic *GetInstance();
  static void FreeInstance();

  bool OnUsersConnect(struct server *srv, const int socket);

  bool OnUsersMessage(struct server *srv, const int socket, const void *msg,
                      const int len);

  bool OnUsersClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

 private:
  bool OnRegisterAccount(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnLoginAccount(struct server* srv, int socket,
                      struct PacketHead *packet);

  bool OnUserAccount(struct server* srv, int socket, struct PacketHead *packet);

  bool OnUserRealInfo(struct server* srv, int socket,
                               struct PacketHead *packet); 
  bool OnUserCheckToken(struct server* srv, int socket,
                        struct PacketHead *packet);

  bool OnRegisterVerifycode(struct server* srv, int socket,
                            struct PacketHead *packet);

  bool SendUserInfo(const int socket, const int64 session, const int32 opcode,
                    swp_logic::UserInfo& userinfo);
  
  bool OnLoginWiXin(struct server* srv, int socket,
                      struct PacketHead *packet);
  bool OnWXBindAccount(struct server* srv, int socket,
                      struct PacketHead *packet);

  bool OnUserChangePasswd(struct server* srv, int socket,
                      struct PacketHead *packet);
  //重置密码
  bool OnResetPasswd(struct server* srv, int socket,struct PacketHead *packet);
  bool OnResetPayPassWD(struct server* srv, int socket, struct PacketHead *packet); 
  bool OnCertification(struct server* srv, int socket,
                            struct PacketHead *packet); 
 private:
  bool Init();
 private:
  users_logic::UsersDB* user_db_;

  manager_schduler::SchdulerEngine* schduler_engine_;
};
}  // namespace history_logic

#endif
