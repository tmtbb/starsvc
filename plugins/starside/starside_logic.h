//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_STARSIDE_STARSIDE_LOGIC_
#define SWP_STARSIDE_STARSIDE_LOGIC_

#include "starside/starside_db.h"
#include "manager/data_share_mgr.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"

namespace starside_logic {

class StarSidelogic {
 public:
  StarSidelogic();
  virtual ~StarSidelogic();
 private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static StarSidelogic *instance_;

 public:
  static StarSidelogic *GetInstance();
  static void FreeInstance();

  bool OnStarSideConnect(struct server *srv, const int socket);

  bool OnStarSideMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnStarSideClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);


 private:
  bool OnStarSideTransStatis(struct server* srv, int socket,
                       struct PacketHead *packet);
  bool OnGetStarMeetRel(struct server* srv, int socket,
                       struct PacketHead *packet);
  bool OnStarSideDetail(struct server* srv, int socket,
                       struct PacketHead *packet);
  bool OnServiceDefInfo(struct server* srv, int socket,
                       struct PacketHead *packet);
  bool OnStarOwnService(struct server* srv, int socket,
                           struct PacketHead *packet); 

  bool OnStarSideRecharge(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnStarSideOwnStar(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnUpdStarMeetRel(struct server* srv, int socket,
                         struct PacketHead *packet);

  bool OnUpdStarService(struct server* srv, int socket,
                         struct PacketHead *packet);
  bool OnGetOwnStarUser(struct server* srv, int socket,
                        struct PacketHead *packet) ;

  bool OnUpdStarMeetDate(struct server* srv, int socket,
                        struct PacketHead *packet) ;
 private:
  bool Init();
  starside_logic::StarSideDB* starside_db_;
  manager_schduler::SchdulerEngine* schduler_engine_;
};
}  // namespace starside_logic

#endif
