//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "bank/bank_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "bank/bank_logic.h"

struct bankplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnBankStart() {
  signal(SIGPIPE, SIG_IGN);
  struct bankplugin *bank = (struct bankplugin *) calloc(
      1, sizeof(struct bankplugin));
  bank->id = "bank";
  bank->name = "bank";
  bank->version = "1.0.0";
  bank->provider = "kerry";
  if (!bank_logic::Banklogic::GetInstance())
    assert(0);
  return bank;
}

static handler_t OnBankShutdown(struct server *srv, void *pd) {
  bank_logic::Banklogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnBankConnect(struct server *srv, int fd, void *data,
                               int len) {
  bank_logic::Banklogic::GetInstance()->OnBankConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBankMessage(struct server *srv, int fd, void *data,
                               int len) {
  bool r = bank_logic::Banklogic::GetInstance()->OnBankMessage(srv, fd, data,
                                                               len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnBankClose(struct server *srv, int fd) {
  bank_logic::Banklogic::GetInstance()->OnBankClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  bank_logic::Banklogic::GetInstance()->OnBroadcastConnect(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  bank_logic::Banklogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  bank_logic::Banklogic::GetInstance()->OnBroadcastMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  bank_logic::Banklogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  bank_logic::Banklogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int bank_plugin_init(struct plugin *pl) {
  pl->init = OnBankStart;
  pl->clean_up = OnBankShutdown;
  pl->connection = OnBankConnect;
  pl->connection_close = OnBankClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnBankMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
