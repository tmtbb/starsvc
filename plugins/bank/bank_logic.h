//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef SWP_BANK_BANK_LOGIC_
#define SWP_BANK_BANK_LOGIC_

#include "bank/bank_db.h"
#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"

namespace bank_logic {

class Banklogic {
 public:
  Banklogic();
  virtual ~Banklogic();

 private:
  static Banklogic *instance_;

 public:
  static Banklogic *GetInstance();
  static void FreeInstance();

  bool OnBankConnect(struct server *srv, const int socket);

  bool OnBankMessage(struct server *srv, const int socket, const void *msg,
                     const int len);

  bool OnBankClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

 private:
  //提交提现
  bool OnWithDrawCash(struct server* srv, int socket,
                      struct PacketHead *packet);

  //获取绑定银行卡列表
  bool OnGetBindBank(struct server* srv, int socket,
                     struct PacketHead *packet);

  //绑定银行卡
  bool OnBindBankCard(struct server* srv, int socket,
                    struct PacketHead* packet);

  //获取银行卡名称
  bool OnGetBankName(struct server* srv, int socket,
                    struct PacketHead* packet);

  //解绑银行卡
  bool OnUnbindBankCard(struct server* srv, int socket,
                    struct PacketHead* packet);

 
 private:
  bool Init();
  bank_logic::BankDB* bank_db_;
  manager_schduler::SchdulerEngine* schduler_engine_;
};
}  // namespace history_logic

#endif
