//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_SATR_NET_FLASH_LOGIC_
#define SWP_SATR_NET_FLASH_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
//#include "manager/data_share_mgr.h"
#include "netflash_db.h"
#include "netflash_kafka.h"

namespace netflash_logic {

class Netflashlogic {
public:
  Netflashlogic();
  virtual ~Netflashlogic();

private:
  static Netflashlogic *instance_;

public:
  static Netflashlogic *GetInstance();
  static void FreeInstance();

  bool OnNetflashConnect(struct server *srv, const int socket);

  bool OnNetflashMessage(struct server *srv, const int socket, const void *msg,
                       const int len);

  bool OnNetflashClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

private:
  //明星倒计时
  bool OnNetSymbolStatus(struct server* srv, int socket, struct PacketHead* packet);
  //明星列表
  bool OnNetSymbolList(struct server* srv, int socket, struct PacketHead* packet);
  //明星简介
  bool OnNetStarInfo(struct server* srv, int socket, struct PacketHead* packet);
  //明星发行信息
  bool OnNetSymbolPubInfo(struct server* srv, int socket, struct PacketHead* packet);
  //购买确认
  bool OnNetflashSaleConfirmOrder(struct server* srv, int socket, struct PacketHead* packet);
private:
  bool Init();
private:
  netflash_logic::NetflashDB*  netflash_db_;
  netflash_logic::NetflashKafka*  netflash_kafka_;
  //manager_schduler::SchdulerEngine* schduler_engine_;
};
} // namespace netflash_logic

#endif
