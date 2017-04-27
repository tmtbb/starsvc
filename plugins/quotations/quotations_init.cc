//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "quotations/quotations_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "quotations/quotations_logic.h"

struct quotationsplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnQuotationsStart() {
  signal(SIGPIPE, SIG_IGN);
  struct quotationsplugin *quotations =
      (struct quotationsplugin *)calloc(1, sizeof(struct quotationsplugin));
  quotations->id = "quotations";
  quotations->name = "quotations";
  quotations->version = "1.0.0";
  quotations->provider = "kerry";
  if (!quotations_logic::Quotationslogic::GetInstance())
    assert(0);
  return quotations;
}

static handler_t OnQuotationsShutdown(struct server *srv, void *pd) {
  quotations_logic::Quotationslogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnQuotationsConnect(struct server *srv, int fd, void *data,
                                     int len) {
  quotations_logic::Quotationslogic::GetInstance()->OnQuotationsConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnQuotationsMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = quotations_logic::Quotationslogic::GetInstance()->OnQuotationsMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnQuotationsClose(struct server *srv, int fd) {
  quotations_logic::Quotationslogic::GetInstance()->OnQuotationsClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  quotations_logic::Quotationslogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  quotations_logic::Quotationslogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  quotations_logic::Quotationslogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  quotations_logic::Quotationslogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  quotations_logic::Quotationslogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int quotations_plugin_init(struct plugin *pl) {
  pl->init = OnQuotationsStart;
  pl->clean_up = OnQuotationsShutdown;
  pl->connection = OnQuotationsConnect;
  pl->connection_close = OnQuotationsClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnQuotationsMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
