//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_SATR_FLASH_LOGIC_
#define SWP_SATR_FLASH_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"
#include "flashsale/flash_db.h"
#include "flashsale/flash_kafka.h"

namespace flash_logic {

class Flashlogic {
public:
    Flashlogic();
    virtual ~Flashlogic();

private:
    static Flashlogic *instance_;

public:
    static Flashlogic *GetInstance();
    static void FreeInstance();

    bool OnFlashConnect(struct server *srv, const int socket);

    bool OnFlashMessage(struct server *srv, const int socket, const void *msg,
                         const int len);

    bool OnFlashClose(struct server *srv, const int socket);

    bool OnBroadcastConnect(struct server *srv, const int socket,
                            const void *data, const int len);

    bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                            const int len);

    bool OnBroadcastClose(struct server *srv, const int socket);

    bool OnIniTimer(struct server *srv);

    bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:

    bool OnSymbolStatus(struct server* srv, int socket, struct PacketHead* packet);

    bool OnSymbolInfo(struct server* srv, int socket, struct PacketHead* packet);

    bool OnFlashSaleConfirmOrder(struct server* srv, int socket, struct PacketHead* packet);
private:
    bool Init();
private:
    flash_logic::FlashDB*  flash_db_;
    flash_logic::FlashKafka*  flash_kafka_;
    manager_schduler::SchdulerEngine* schduler_engine_;
};
} // namespace flash_logic

#endif
