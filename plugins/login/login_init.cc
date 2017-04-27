#include "login/login_init.h"
#include <assert.h>
#include "login/login_logic.h"
#include "core/common.h"
#include "core/plugins.h"
#include "comm/comm_struct.h"
#include "logic/logic_comm.h"

static void *OnLoginStart() {
  signal(SIGPIPE, SIG_IGN);
  PluginInfo* login = reinterpret_cast<PluginInfo*>
    (calloc(1, sizeof(PluginInfo)));
  login->id = "login";
  login->name = "login";
  login->version = "1.0.0";
  login->provider = "Paco";
  if (!login::Loginlogic::GetInstance())
    assert(0);
  return login;
}

static handler_t OnLoginShutdown(struct server* srv, void* pd) {
  login::Loginlogic::FreeInstance();
  return HANDLER_GO_ON;
}

static handler_t OnLoginConnect(struct server *srv, int fd, void *data,
                                  int len) {
  login::Loginlogic::GetInstance()->OnLoginConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnLoginMessage(struct server *srv, int fd, void *data,
                                  int len) {
  LOG_ERROR("--------------------onloginmessage");
  login::Loginlogic::GetInstance()->OnLoginMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnLoginClose(struct server *srv, int fd) {
  login::Loginlogic::GetInstance()->OnLoginClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data,
                                    int len) {
  login::Loginlogic::GetInstance()->OnBroadcastConnect(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server* srv, int fd) {
  login::Loginlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data,
                                    int len) {
  LOG_ERROR("----------------onbroadcast-------------login");
  login::Loginlogic::GetInstance()->OnBroadcastMessage(srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnInitTimer(struct server* srv) {
  login::Loginlogic::GetInstance()->OnInitTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server* srv, char* id, int opcode, int time) {
  login::Loginlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
  return HANDLER_GO_ON;
}

int login_plugin_init(struct plugin *pl) {
  pl->init = OnLoginStart;
  pl->clean_up = OnLoginShutdown;
  pl->connection = OnLoginConnect;
  pl->connection_close = OnLoginClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnInitTimer;
  pl->handler_read = OnLoginMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}

