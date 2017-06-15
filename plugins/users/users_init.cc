//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "users/users_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "users/users_logic.h"
#include "logic/logic_comm.h"

struct usersplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnUsersStart() {
  signal(SIGPIPE, SIG_IGN);
  struct usersplugin *users =
      (struct usersplugin *)calloc(1, sizeof(struct usersplugin));
  users->id = "users";
  users->name = "users";
  users->version = "1.0.0";
  users->provider = "li";
  if (!users_logic::Userslogic::GetInstance())
    assert(0);
  return users;
}

static handler_t OnUsersShutdown(struct server *srv, void *pd) {
  users_logic::Userslogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnUsersConnect(struct server *srv, int fd, void *data,
                                     int len) {
  users_logic::Userslogic::GetInstance()->OnUsersConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUsersMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = users_logic::Userslogic::GetInstance()->OnUsersMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnUsersClose(struct server *srv, int fd) {
  users_logic::Userslogic::GetInstance()->OnUsersClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  users_logic::Userslogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  users_logic::Userslogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  //LOG_ERROR("===============================onbroadcastmessage");
  users_logic::Userslogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  users_logic::Userslogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  users_logic::Userslogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int users_plugin_init(struct plugin *pl) {
  pl->init = OnUsersStart;
  pl->clean_up = OnUsersShutdown;
  pl->connection = OnUsersConnect;
  pl->connection_close = OnUsersClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnUsersMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
