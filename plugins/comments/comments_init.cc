//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "comments/comments_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "comments/comments_logic.h"

struct commentsplugin {
  char *id;
  char *name;
  char *version;
  char *provider;
};

static void *OnCommentsStart() {
  signal(SIGPIPE, SIG_IGN);
  struct commentsplugin *comments =
      (struct commentsplugin *)calloc(1, sizeof(struct commentsplugin));
  comments->id = "comments";
  comments->name = "comments";
  comments->version = "1.0.0";
  comments->provider = "kerry";
  if (!comments_logic::Commentslogic::GetInstance())
    assert(0);
  return comments;
}

static handler_t OnCommentsShutdown(struct server *srv, void *pd) {
  comments_logic::Commentslogic::FreeInstance();

  return HANDLER_GO_ON;
}

static handler_t OnCommentsConnect(struct server *srv, int fd, void *data,
                                     int len) {
  comments_logic::Commentslogic::GetInstance()->OnCommentsConnect(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnCommentsMessage(struct server *srv, int fd, void *data,
                                     int len) {
  bool r = comments_logic::Commentslogic::GetInstance()->OnCommentsMessage(srv, fd,
                                                                    data, len);
  if (r)
    return HANDLER_GO_ON;
  else
    return HANDLER_FINISHED;
}

static handler_t OnCommentsClose(struct server *srv, int fd) {
  comments_logic::Commentslogic::GetInstance()->OnCommentsClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
  comments_logic::Commentslogic::GetInstance()->OnBroadcastConnect(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
  comments_logic::Commentslogic::GetInstance()->OnBroadcastClose(srv, fd);
  return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
  comments_logic::Commentslogic::GetInstance()->OnBroadcastMessage(
      srv, fd, data, len);
  return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
  comments_logic::Commentslogic::GetInstance()->OnIniTimer(srv);
  return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
  comments_logic::Commentslogic::GetInstance()->OnTimeout(srv, id, opcode,
                                                              time);
  return HANDLER_GO_ON;
}

int comments_plugin_init(struct plugin *pl) {
  pl->init = OnCommentsStart;
  pl->clean_up = OnCommentsShutdown;
  pl->connection = OnCommentsConnect;
  pl->connection_close = OnCommentsClose;
  pl->connection_close_srv = OnBroadcastClose;
  pl->connection_srv = OnBroadcastConnect;
  pl->handler_init_time = OnIniTimer;
  pl->handler_read = OnCommentsMessage;
  pl->handler_read_srv = OnBroadcastMessage;
  pl->handler_read_other = OnUnknow;
  pl->time_msg = OnTimeOut;
  pl->data = NULL;
  return 0;
}
