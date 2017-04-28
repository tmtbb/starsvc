
#include "imcloud/im_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "imcloud/im_logic.h"
#include "logic/logic_comm.h"
#include "im_process.h"

struct imcloudplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnImStart() {
  signal(SIGPIPE, SIG_IGN);
  struct imcloudplugin *im =
      (struct imcloudplugin *)calloc(1, sizeof(struct imcloudplugin));
  im->id = "imcloud";
  im->name = "imcloud";
  im->version = "1.0.0";
  im->provider = "li";
  if (!im_logic::Imlogic::GetInstance())
    assert(0);
  return im;
}

static handler_t OnImShutdown(struct server *srv, void *pd) {
  im_logic::Imlogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnImConnect(struct server *srv, int fd, void *data,
                                     int len) {
  LOG_ERROR("-------------------------------------------OnImConnect");
  im_process::ImProcess *test = new im_process::ImProcess();
  test->gettoken();

  im_logic::Imlogic::GetInstance()->OnImConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnImMessage(struct server *srv, int fd, void *data,
                                     int len) {
  LOG_ERROR("-------------------------------------------IMMessage get");
  bool r = im_logic::Imlogic::GetInstance()->OnImMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnImClose(struct server *srv, int fd) {
  im_logic::Imlogic::GetInstance()->OnImClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  im_logic::Imlogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  im_logic::Imlogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  im_logic::Imlogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  im_logic::Imlogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  im_logic::Imlogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int imcloud_plugin_init(struct plugin *pl) {
  pl->init = OnImStart;
  pl->clean_up = OnImShutdown;
  pl->connection = OnImConnect;
  pl->connection_close = OnImClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnImMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
