//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "starside/starside_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "starside/starside_logic.h"

struct starsideplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnStarSideStart() {
  signal(SIGPIPE, SIG_IGN);
  struct starsideplugin *starside =
      (struct starsideplugin *)calloc(1, sizeof(struct starsideplugin));
  starside->id = "starside";
  starside->name = "starside";
  starside->version = "1.0.0";
  starside->provider = "kerry";
  if (!starside_logic::StarSidelogic::GetInstance())
    assert(0);
  return starside;
}

static handler_t OnStarSideShutdown(struct server *srv, void *pd) {
  starside_logic::StarSidelogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnStarSideConnect(struct server *srv, int fd, void *data,
                                     int len) {
  starside_logic::StarSidelogic::GetInstance()->OnStarSideConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnStarSideMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = starside_logic::StarSidelogic::GetInstance()->OnStarSideMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnStarSideClose(struct server *srv, int fd) {
  starside_logic::StarSidelogic::GetInstance()->OnStarSideClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  starside_logic::StarSidelogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  starside_logic::StarSidelogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  starside_logic::StarSidelogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  starside_logic::StarSidelogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  starside_logic::StarSidelogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int starside_plugin_init(struct plugin *pl) {
  pl->init = OnStarSideStart;
  pl->clean_up = OnStarSideShutdown;
  pl->connection = OnStarSideConnect;
  pl->connection_close = OnStarSideClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnStarSideMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
