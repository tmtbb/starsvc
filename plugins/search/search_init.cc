//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "search/search_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "search/search_logic.h"
#include "logic/logic_comm.h"

struct searchplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnSearchStart() {
    signal(SIGPIPE, SIG_IGN);
    struct searchplugin *search =
        (struct searchplugin *)calloc(1, sizeof(struct searchplugin));
    search->id = "search";
    search->name = "search";
    search->version = "1.0.0";
    search->provider = "kerry";
    if (!search_logic::Searchlogic::GetInstance())
        assert(0);
    return search;
}

static handler_t OnSearchShutdown(struct server *srv, void *pd) {
    search_logic::Searchlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnSearchConnect(struct server *srv, int fd, void *data,
                                 int len) {
    search_logic::Searchlogic::GetInstance()->OnSearchConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnSearchMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = search_logic::Searchlogic::GetInstance()->OnSearchMessage(srv, fd,
             data, len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnSearchClose(struct server *srv, int fd) {
    search_logic::Searchlogic::GetInstance()->OnSearchClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    search_logic::Searchlogic::GetInstance()->OnBroadcastConnect(
        srv, fd, data, len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    search_logic::Searchlogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    //LOG_ERROR("===============================onbroadcastmessage");
    search_logic::Searchlogic::GetInstance()->OnBroadcastMessage(
        srv, fd, data, len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    search_logic::Searchlogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    search_logic::Searchlogic::GetInstance()->OnTimeout(srv, id, opcode,
            time);
    return HANDLER_GO_ON;
}

int search_plugin_init(struct plugin *pl) {
    pl->init = OnSearchStart;
    pl->clean_up = OnSearchShutdown;
    pl->connection = OnSearchConnect;
    pl->connection_close = OnSearchClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnSearchMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
