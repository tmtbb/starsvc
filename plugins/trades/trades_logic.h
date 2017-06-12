//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_TRADES_TRADES_LOGIC_
#define SWP_TRADES_TRADES_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "trades/trades_db.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"

namespace trades_logic {

class Tradeslogic {
public:
    Tradeslogic();
    virtual ~Tradeslogic();

private:
    static Tradeslogic *instance_;

public:
    static Tradeslogic *GetInstance();
    static void FreeInstance();

    bool OnTradesConnect(struct server *srv, const int socket);

    bool OnTradesMessage(struct server *srv, const int socket, const void *msg,
                         const int len);

    bool OnTradesClose(struct server *srv, const int socket);

    bool OnBroadcastConnect(struct server *srv, const int socket,
                            const void *data, const int len);

    bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                            const int len);

    bool OnBroadcastClose(struct server *srv, const int socket);

    bool OnIniTimer(struct server *srv);

    bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:

    bool OnOpenPosition(struct server* srv, int socket, struct PacketHead* packet);

    bool OnGetStarTrading(struct server* srv, int socket, struct PacketHead* packet);
private:
    bool Init();

    void Test();
private:
    trades_logic::TradesDB*  trades_db_;
};
} // namespace trades_logic

#endif
