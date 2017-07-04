//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "push/push_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "push/push_logic.h"
#include "logic/logic_comm.h"
#include "queue/kafka_consumer.h"

struct pushplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnPushStart() {
  signal(SIGPIPE, SIG_IGN);
  struct pushplugin *push =
      (struct pushplugin *)calloc(1, sizeof(struct pushplugin));
  push->id = "push";
  push->name = "push";
  push->version = "1.0.0";
  push->provider = "li";
  if (!push_logic::CPushlogic::GetInstance())
    assert(0);
  return push;
}

static handler_t OnPushShutdown(struct server *srv, void *pd) {
  //push_logic::CPushlogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnPushConnect(struct server *srv, int fd, void *data,
                                     int len) {
  push_logic::CPushlogic::GetInstance()->OnPushConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnPushMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = push_logic::CPushlogic::GetInstance()->OnPushMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnPushClose(struct server *srv, int fd) {
  push_logic::CPushlogic::GetInstance()->OnPushClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  push_logic::CPushlogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  push_logic::CPushlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  //LOG_ERROR("===============================onbroadcastmessage");
  push_logic::CPushlogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  push_logic::CPushlogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  push_logic::CPushlogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int push_plugin_init(struct plugin *pl) {
  pl->init = OnPushStart;
  pl->clean_up = OnPushShutdown;
  pl->connection = OnPushConnect;
  pl->connection_close = OnPushClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnPushMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}



