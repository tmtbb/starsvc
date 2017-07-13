//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "circle_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "circle_logic.h"

struct circleplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnCircleStart() {
    signal(SIGPIPE, SIG_IGN);
    struct circleplugin *circle =
        (struct circleplugin *)calloc(1, sizeof(struct circleplugin));
    circle->id = "circle";
    circle->name = "circle";
    circle->version = "1.0.0";
    circle->provider = "kerry";
    if (!circle_logic::Circlelogic::GetInstance())
        assert(0);
    return circle;
}

static handler_t OnCircleShutdown(struct server *srv, void *pd) {
    circle_logic::Circlelogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnCircleConnect(struct server *srv, int fd, void *data,
                                 int len) {
    circle_logic::Circlelogic::GetInstance()->OnCircleConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnCircleMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = circle_logic::Circlelogic::GetInstance()->OnCircleMessage(srv, fd, data,
             len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnCircleClose(struct server *srv, int fd) {
    circle_logic::Circlelogic::GetInstance()->OnCircleClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    circle_logic::Circlelogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    circle_logic::Circlelogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    circle_logic::Circlelogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    circle_logic::Circlelogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    circle_logic::Circlelogic::GetInstance()->OnTimeout(srv, id, opcode, time);
    return HANDLER_GO_ON;
}

int circle_plugin_init(struct plugin *pl) {
    pl->init = OnCircleStart;
    pl->clean_up = OnCircleShutdown;
    pl->connection = OnCircleConnect;
    pl->connection_close = OnCircleClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnCircleMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
