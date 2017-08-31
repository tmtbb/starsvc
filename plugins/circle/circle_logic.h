//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_SATR_CIRCLE_LOGIC_
#define SWP_SATR_CIRCLE_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"
#include "circle_db.h"
//#include "storager_kafka.h"
#include "kafka.h"

namespace circle_logic {

class Circlelogic {
public:
  Circlelogic();
  virtual ~Circlelogic();

private:
    static Circlelogic *instance_;

public:
  static Circlelogic *GetInstance();
  static void FreeInstance();

  bool OnCircleConnect(struct server *srv, const int socket);

  bool OnCircleMessage(struct server *srv, const int socket, const void *msg,
                       const int len);

  bool OnCircleClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:
  void FetchTaskForKafka();

  bool OnGetAllCircleInfo(struct server* srv, int socket, struct PacketHead* packet);

  bool OnGetStarCircleInfo(struct server* srv, int socket, struct PacketHead* packet);

  bool OnAddCircleInfo(struct server* srv, int socket, struct PacketHead* packet);

  bool OnDelCircleInfo(struct server* srv, int socket, struct PacketHead* packet);

  bool OnUserApproveCircle(struct server* srv, int socket, struct PacketHead* packet);

  bool OnUserCommentCircle(struct server* srv, int socket, struct PacketHead* packet);

  bool OnStarReplyCircle(struct server* srv, int socket, struct PacketHead* packet);
//
  bool OnGetUserAsk(struct server* srv, int socket, struct PacketHead* packet);
  bool OnGetStarUserAsk(struct server* srv, int socket, struct PacketHead* packet);
  bool OnGetAllStarUserAskHot(struct server* srv, int socket, struct PacketHead* packet); 
  bool OnUserAsk(struct server* srv, int socket, struct PacketHead* packet); 
  bool OnStarAnswer(struct server* srv, int socket, struct PacketHead* packet); 

  bool OnGetQiniuToken(struct server* srv, int socket, struct PacketHead* packet);
  bool OnSeeQuestion(struct server* srv, int socket, struct PacketHead* packet); 
private:
  bool Init();
private:
  circle_logic::CircleDB*  circle_db_;
  manager_schduler::SchdulerEngine* manager_schduler_engine_;

  circle_logic::Kafka* kafka_;
};
} // namespace circle_logic

#endif
