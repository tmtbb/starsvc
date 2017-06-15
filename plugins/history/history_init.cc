//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "history/history_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "history/history_logic.h"

struct historyplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnHistoryStart() {
  signal(SIGPIPE, SIG_IGN);
  struct historyplugin *history =
      (struct historyplugin *)calloc(1, sizeof(struct historyplugin));
  history->id = "history";
  history->name = "history";
  history->version = "1.0.0";
  history->provider = "kerry";
  if (!history_logic::Historylogic::GetInstance())
    assert(0);
  return history;
}

static handler_t OnHistoryShutdown(struct server *srv, void *pd) {
  history_logic::Historylogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnHistoryConnect(struct server *srv, int fd, void *data,
                                     int len) {
  history_logic::Historylogic::GetInstance()->OnHistoryConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnHistoryMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = history_logic::Historylogic::GetInstance()->OnHistoryMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnHistoryClose(struct server *srv, int fd) {
  history_logic::Historylogic::GetInstance()->OnHistoryClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  history_logic::Historylogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  history_logic::Historylogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  history_logic::Historylogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  history_logic::Historylogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  history_logic::Historylogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int history_plugin_init(struct plugin *pl) {
  pl->init = OnHistoryStart;
  pl->clean_up = OnHistoryShutdown;
  pl->connection = OnHistoryConnect;
  pl->connection_close = OnHistoryClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnHistoryMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
