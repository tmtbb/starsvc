
#ifndef SWP_IMCLOUD_IM_LOGIC_
#define SWP_IMCLOUD_IM_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"

namespace im_logic {

class Imlogic {
 public:
  Imlogic();
  virtual ~Imlogic();
 private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static Imlogic *instance_;

 public:
  static Imlogic *GetInstance();
  static void FreeInstance();

  bool OnImConnect(struct server *srv, const int socket);

  bool OnImMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnImClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);


 private:
  bool OnImTrades(struct server* srv, int socket,
                       struct PacketHead *packet);

  bool OnImRecharge(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnImWithdrawals(struct server* srv, int socket,
                            struct PacketHead * packet);

  bool OnHandleWithdrawals(struct server* srv, int socket,
                           struct PacketHead* packet);

  bool OnHandleTradesPosition(struct server* srv, int socket,
                             struct PacketHead * packet);

  bool OnHisotryHandlePosition(struct server* srv, int socket,
                          struct PacketHead* packet);
 private:
  bool Init();
};
}  // namespace im_logic

#endif
