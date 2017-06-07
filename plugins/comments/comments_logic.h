//  Copyright (c) 2015-2015 The star Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_COMMENTS_COMMENTS_LOGIC_
#define SWP_COMMENTS_COMMENTS_LOGIC_

#include "comments/comments_redis.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"

namespace comments_logic {

class Commentslogic {
public:
  Commentslogic();
  virtual ~Commentslogic();
private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
private:
  static Commentslogic *instance_;

public:
  static Commentslogic *GetInstance();
  static void FreeInstance();

  bool OnCommentsConnect(struct server *srv, const int socket);

  bool OnCommentsMessage(struct server *srv, const int socket, const void *msg,
                       const int len);

  bool OnCommentsClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);
private:
	/*
	*/
  void Test();

  bool OnCommentsAddData(struct server* srv, int socket, struct PacketHead *packet); 
  bool OnCommentsGetData(struct server* srv, int socket, struct PacketHead *packet); 
private:
  bool Init();

private:
  CommentsRedis* comments_redis_;
};
} // namespace quatations_logic

#endif
