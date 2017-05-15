
#include "market_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "market_logic.h"
#include "logic/logic_comm.h"


struct marketplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnStart() {
  signal(SIGPIPE, SIG_IGN);
  struct marketplugin *im =
      (struct marketplugin *)calloc(1, sizeof(struct marketplugin));
  im->id = "market";
  im->name = "market";
  im->version = "1.0.0";
  im->provider = "li";
  if (!market_logic::Marketlogic::GetInstance())
    assert(0);
  return im;
}

static handler_t OnShutdown(struct server *srv, void *pd) {
  market_logic::Marketlogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnConnect(struct server *srv, int fd, void *data,
                                     int len) {
  market_logic::Marketlogic::GetInstance()->OnInfomationConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = market_logic::Marketlogic::GetInstance()->OnInfomationMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnClose(struct server *srv, int fd) {
  market_logic::Marketlogic::GetInstance()->OnInfomationClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  market_logic::Marketlogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  market_logic::Marketlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  market_logic::Marketlogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  market_logic::Marketlogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  market_logic::Marketlogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int market_plugin_init(struct plugin *pl) {
  pl->init = OnStart;
  pl->clean_up = OnShutdown;
  pl->connection = OnConnect;
  pl->connection_close = OnClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
