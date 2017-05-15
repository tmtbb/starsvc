//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_PAY_PAY_LOGIC_
#define SWP_PAY_PAY_LOGIC_

#include "pay/pay_db.h"
#include "manager/data_share_mgr.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"

namespace pay_logic {

class Paylogic {
 public:
  Paylogic();
  virtual ~Paylogic();

 private:
  static Paylogic *instance_;

 public:
  static Paylogic *GetInstance();
  static void FreeInstance();

  bool OnPayConnect(struct server *srv, const int socket);

  bool OnPayMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnPayClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

 private:
  bool OnWXPayOrder(struct server* srv, int socket,
                    struct PacketHead *packet);

  bool OnWXPayClient(struct server* srv, int socket,
                     struct PacketHead *packet);

  bool OnWXPaySever(struct server* srv, int socket,
                    struct PacketHead * packet);


  bool OnSHFJPayOrder(struct server* srv, int socket,
                    struct PacketHead *packet);

  bool OnSHFJPayClient(struct server* srv, int socket,
                     struct PacketHead *packet);

  bool OnSHFJPaySever(struct server* srv, int socket,
                    struct PacketHead * packet);

  bool OnSHFJCashOrder(struct server* srv, int socket,
                    struct PacketHead *packet);
  bool OnSHFJCashServer(struct server* srv, int socket,
                    struct PacketHead * packet);
 private:
  bool Init();
 private:
  pay_logic::PayDB* pay_db_;

  manager_schduler::SchdulerEngine* schduler_engine_;
};
}  // namespace history_logic

#endif
