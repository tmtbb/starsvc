//  Copyright (c) 2015-2015 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "flashsale/flash_init.h"
#include "core/common.h"
#include "core/plugins.h"
#include "flashsale/flash_logic.h"

struct flashplugin {
    char *id;
    char *name;
    char *version;
    char *provider;
};

static void *OnFlashStart() {
    signal(SIGPIPE, SIG_IGN);
    struct flashplugin *flash =
        (struct flashplugin *)calloc(1, sizeof(struct flashplugin));
    flash->id = "flash";
    flash->name = "flash";
    flash->version = "1.0.0";
    flash->provider = "kerry";
    if (!flash_logic::Flashlogic::GetInstance())
        assert(0);
    return flash;
}

static handler_t OnFlashShutdown(struct server *srv, void *pd) {
    flash_logic::Flashlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnFlashConnect(struct server *srv, int fd, void *data,
                                 int len) {
    flash_logic::Flashlogic::GetInstance()->OnFlashConnect(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnFlashMessage(struct server *srv, int fd, void *data,
                                 int len) {
    bool r = flash_logic::Flashlogic::GetInstance()->OnFlashMessage(srv, fd, data,
             len);
    if (r)
        return HANDLER_GO_ON;
    else
        return HANDLER_FINISHED;
}

static handler_t OnFlashClose(struct server *srv, int fd) {
    flash_logic::Flashlogic::GetInstance()->OnFlashClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len) {
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastConnect(struct server *srv, int fd, void *data,
                                    int len) {
    flash_logic::Flashlogic::GetInstance()->OnBroadcastConnect(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastClose(struct server *srv, int fd) {
    flash_logic::Flashlogic::GetInstance()->OnBroadcastClose(srv, fd);
    return HANDLER_GO_ON;
}

static handler_t OnBroadcastMessage(struct server *srv, int fd, void *data,
                                    int len) {
    flash_logic::Flashlogic::GetInstance()->OnBroadcastMessage(srv, fd, data,
            len);
    return HANDLER_GO_ON;
}

static handler_t OnIniTimer(struct server *srv) {
    flash_logic::Flashlogic::GetInstance()->OnIniTimer(srv);
    return HANDLER_GO_ON;
}

static handler_t OnTimeOut(struct server *srv, char *id, int opcode, int time) {
    flash_logic::Flashlogic::GetInstance()->OnTimeout(srv, id, opcode, time);
    return HANDLER_GO_ON;
}

int flash_plugin_init(struct plugin *pl) {
    pl->init = OnFlashStart;
    pl->clean_up = OnFlashShutdown;
    pl->connection = OnFlashConnect;
    pl->connection_close = OnFlashClose;
    pl->connection_close_srv = OnBroadcastClose;
    pl->connection_srv = OnBroadcastConnect;
    pl->handler_init_time = OnIniTimer;
    pl->handler_read = OnFlashMessage;
    pl->handler_read_srv = OnBroadcastMessage;
    pl->handler_read_other = OnUnknow;
    pl->time_msg = OnTimeOut;
    pl->data = NULL;
    return 0;
}
