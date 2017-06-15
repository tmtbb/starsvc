//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef STAR_RECORD_RECORD_LOGIC_
#define STAR_RECORD_RECORD_LOGIC_

#include "record/record_db.h"
#include "record/record_kafka.h"
#include "manager/data_share_mgr.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"

namespace record_logic {

class Recordlogic {
public:
    Recordlogic();
    virtual ~Recordlogic();
private:
    bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
private:
    static Recordlogic *instance_;

public:
    static Recordlogic *GetInstance();
    static void FreeInstance();

    bool OnRecordConnect(struct server *srv, const int socket);

    bool OnRecordMessage(struct server *srv, const int socket, const void *msg,
                         const int len);

    bool OnRecordClose(struct server *srv, const int socket);

    bool OnBroadcastConnect(struct server *srv, const int socket,
                            const void *data, const int len);

    bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                            const int len);

    bool OnBroadcastClose(struct server *srv, const int socket);

    bool OnIniTimer(struct server *srv);

    bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:
    bool OnTodayPosition(struct server* srv, int socket, struct PacketHead* packet);
    bool OnHisPosition(struct server* srv, int socket, struct PacketHead* packet);
    bool OnTodayOrder(struct server* srv, int socket, struct PacketHead* packet);
    bool OnHisOrder(struct server* srv, int socket, struct PacketHead* packet);

private:
    bool Init();
    record_logic::RecordDB* record_db_;
    record_logic::RecordKafka*  record_kafka_;
};
}  // namespace record_logic

#endif
