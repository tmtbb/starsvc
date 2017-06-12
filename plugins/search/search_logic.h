//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef STAR_SEARCH_SEARCH_LOGIC_
#define STAR_SEARCH_SEARCH_LOGIC_

#include "search/search_db.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"


namespace search_logic {

class Searchlogic {
public:
    Searchlogic();
    virtual ~Searchlogic();
private:
    static Searchlogic *instance_;

public:
    static Searchlogic *GetInstance();
    static void FreeInstance();

    bool OnSearchConnect(struct server *srv, const int socket);

    bool OnSearchMessage(struct server *srv, const int socket, const void *msg,
                         const int len);

    bool OnSearchClose(struct server *srv, const int socket);

    bool OnBroadcastConnect(struct server *srv, const int socket,
                            const void *data, const int len);

    bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                            const int len);

    bool OnBroadcastClose(struct server *srv, const int socket);

    bool OnIniTimer(struct server *srv);

    bool OnTimeout(struct server *srv, char *id, int opcode, int time);
private:
    bool OnSearchStar(struct server* srv, int socket, struct PacketHead* packet);
private:
    bool Init();
private:
    search_logic::SearchDB* search_db_;
};
}  // namespace search_logic

#endif
