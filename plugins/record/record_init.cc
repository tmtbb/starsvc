//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "record/record_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "record/record_logic.h"

struct recordplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnRecordStart() {
    signal(SIGPIPE, SIG_IGN);
    struct recordplugin *record =
        (struct recordplugin *)calloc(1, sizeof(struct recordplugin));
    record->id = "record";
    record->name = "record";
    record->version = "1.0.0";
    record->provider = "kerry";
    if (!record_logic::Recordlogic::GetInstance())
        assert(0);
    return record;
}

static handler_t OnRecordShutdown(struct server *srv, void *pd) {
    record_logic::Recordlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnRecordConnect(struct server *srv, int fd, void *data,
                                 int len) {
    record_logic::Recordlogic::GetInstance()->OnRecordConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnRecordMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = record_logic::Recordlogic::GetInstance()->OnRecordMessage(srv, fd,
             data, len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnRecordClose(struct server *srv, int fd) {
    record_logic::Recordlogic::GetInstance()->OnRecordClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    record_logic::Recordlogic::GetInstance()->OnBroadcastConnect(
        srv, fd, data, len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    record_logic::Recordlogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    record_logic::Recordlogic::GetInstance()->OnBroadcastMessage(
        srv, fd, data, len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    record_logic::Recordlogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    record_logic::Recordlogic::GetInstance()->OnTimeout(srv, id, opcode,
            time);
    return HANDLER_GO_ON;
}

int record_plugin_init(struct plugin *pl) {
    pl->init = OnRecordStart;
    pl->clean_up = OnRecordShutdown;
    pl->connection = OnRecordConnect;
    pl->connection_close = OnRecordClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnRecordMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
