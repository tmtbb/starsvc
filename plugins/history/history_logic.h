//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_HISTORY_HISTORY_LOGIC_
#define SWP_HISTORY_HISTORY_LOGIC_

#include "history/history_db.h"
#include "manager/data_share_mgr.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"

namespace history_logic {

class Historylogic {
 public:
  Historylogic();
  virtual ~Historylogic();
 private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static Historylogic *instance_;

 public:
  static Historylogic *GetInstance();
  static void FreeInstance();

  bool OnHistoryConnect(struct server *srv, const int socket);

  bool OnHistoryMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnHistoryClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);


 private:
  bool OnHistoryTrades(struct server* srv, int socket,
                       struct PacketHead *packet);

  bool OnHistoryRecharge(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnHistoryWithdrawals(struct server* srv, int socket,
                            struct PacketHead * packet);

  bool OnHandleWithdrawals(struct server* srv, int socket,
                           struct PacketHead* packet);

  bool OnHandleTradesPosition(struct server* srv, int socket,
                             struct PacketHead * packet);

  bool OnHisotryHandlePosition(struct server* srv, int socket,
                          struct PacketHead* packet);
 private:
  bool Init();
  history_logic::HistoryDB* history_db_;
  manager_schduler::SchdulerEngine* schduler_engine_;
};
}  // namespace history_logic

#endif
