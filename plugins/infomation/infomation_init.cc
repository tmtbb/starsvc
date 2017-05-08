
#include "infomation_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "infomation_logic.h"
#include "logic/logic_comm.h"


struct infomationplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnInfomationStart() {
  signal(SIGPIPE, SIG_IGN);
  struct infomationplugin *im =
      (struct infomationplugin *)calloc(1, sizeof(struct infomationplugin));
  im->id = "infomation";
  im->name = "infomation";
  im->version = "1.0.0";
  im->provider = "li";
  if (!infomation_logic::Infomationlogic::GetInstance())
    assert(0);
  return im;
}

static handler_t OnInfomationShutdown(struct server *srv, void *pd) {
  infomation_logic::Infomationlogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnInfomationConnect(struct server *srv, int fd, void *data,
                                     int len) {
  infomation_logic::Infomationlogic::GetInstance()->OnInfomationConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnInfomationMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = infomation_logic::Infomationlogic::GetInstance()->OnInfomationMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnInfomationClose(struct server *srv, int fd) {
  infomation_logic::Infomationlogic::GetInstance()->OnInfomationClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  infomation_logic::Infomationlogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  infomation_logic::Infomationlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  infomation_logic::Infomationlogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  infomation_logic::Infomationlogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  infomation_logic::Infomationlogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int infomation_plugin_init(struct plugin *pl) {
  pl->init = OnInfomationStart;
  pl->clean_up = OnInfomationShutdown;
  pl->connection = OnInfomationConnect;
  pl->connection_close = OnInfomationClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnInfomationMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
