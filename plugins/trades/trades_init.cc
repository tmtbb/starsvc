//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "trades/trades_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "trades/trades_logic.h"

struct tradesplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnTradesStart() {
  signal(SIGPIPE, SIG_IGN);
  struct tradesplugin *trades =
      (struct tradesplugin *)calloc(1, sizeof(struct tradesplugin));
  trades->id = "trades";
  trades->name = "trades";
  trades->version = "1.0.0";
  trades->provider = "kerry";
  if (!trades_logic::Tradeslogic::GetInstance())
    assert(0);
  return trades;
}

static handler_t OnTradesShutdown(struct server *srv, void *pd) {
  trades_logic::Tradeslogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnTradesConnect(struct server *srv, int fd, void *data,
                                 int len) {
  trades_logic::Tradeslogic::GetInstance()->OnTradesConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnTradesMessage(struct server *srv, int fd, void *data,
                                 int len) {
  bool r = trades_logic::Tradeslogic::GetInstance()->OnTradesMessage(srv, fd, data,
                                                                  len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnTradesClose(struct server *srv, int fd) {
  trades_logic::Tradeslogic::GetInstance()->OnTradesClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  trades_logic::Tradeslogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
                                                               len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  trades_logic::Tradeslogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  trades_logic::Tradeslogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
                                                               len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  trades_logic::Tradeslogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  trades_logic::Tradeslogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int trades_plugin_init(struct plugin *pl) {
  pl->init = OnTradesStart;
  pl->clean_up = OnTradesShutdown;
  pl->connection = OnTradesConnect;
  pl->connection_close = OnTradesClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnTradesMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
