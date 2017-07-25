//  Copyright (c) 2015-2015 The star Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef STAR_QUOTATIONS_QUOTATIONS_LOGIC_
#define STAR_QUOTATIONS_QUOTATIONS_LOGIC_

#include "quotations/quotations_redis.h"
#include "quotations/quotations_db.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"

namespace quotations_logic {

class Quotationslogic {
public:
  Quotationslogic();
  virtual ~Quotationslogic();
private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
private:
  static Quotationslogic *instance_;

public:
  static Quotationslogic *GetInstance();
  static void FreeInstance();

  bool OnQuotationsConnect(struct server *srv, const int socket);

  bool OnQuotationsMessage(struct server *srv, const int socket, const void *msg,
                       const int len);

  bool OnQuotationsClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);
private:
  bool OnRealTime(struct server* srv, int socket, struct PacketHead *packet);

  bool OnTimeLine(struct server* srv, int socket, struct PacketHead *packet);

  bool OnKChartTimeLine(struct server* srv, int socket, struct PacketHead *packet);


  bool OnSymbolList(struct server* srv, int socket, struct PacketHead* packet);
  bool OnSymbolOne(struct server* srv, int socket, struct PacketHead* packet);

  bool OnHomeSymbolList(struct server* srv, int socket, struct PacketHead* packet);

  void Test();

private:
  bool Init();

private:
  QuotationsRedis* quotations_redis_;
  quotations_logic::QuotationsDB* quotations_db_;
};
} // namespace quatations_logic

#endif
