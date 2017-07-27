//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "netflash_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "netflash_logic.h"

struct netflashplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnNetflashStart() {
    signal(SIGPIPE, SIG_IGN);
    struct netflashplugin *netflash =
        (struct netflashplugin *)calloc(1, sizeof(struct netflashplugin));
    netflash->id = "netflash";
    netflash->name = "netflash";
    netflash->version = "1.0.0";
    netflash->provider = "kerry";
    if (!netflash_logic::Netflashlogic::GetInstance())
        assert(0);
    return netflash;
}

static handler_t OnNetflashShutdown(struct server *srv, void *pd) {
    netflash_logic::Netflashlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnNetflashConnect(struct server *srv, int fd, void *data,
                                 int len) {
    netflash_logic::Netflashlogic::GetInstance()->OnNetflashConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnNetflashMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = netflash_logic::Netflashlogic::GetInstance()->OnNetflashMessage(srv, fd, data,
             len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnNetflashClose(struct server *srv, int fd) {
    netflash_logic::Netflashlogic::GetInstance()->OnNetflashClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    netflash_logic::Netflashlogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    netflash_logic::Netflashlogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    netflash_logic::Netflashlogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    netflash_logic::Netflashlogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    netflash_logic::Netflashlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
    return HANDLER_GO_ON;
}

int netflash_plugin_init(struct plugin *pl) {
    pl->init = OnNetflashStart;
    pl->clean_up = OnNetflashShutdown;
    pl->connection = OnNetflashConnect;
    pl->connection_close = OnNetflashClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnNetflashMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
