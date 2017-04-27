

#include "im_logic.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/imcloud/im_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace im_logic {

Imlogic *Imlogic::instance_ = NULL;

Imlogic::Imlogic() {
}

Imlogic::~Imlogic() {

}

bool Imlogic::Init() {
  return true;
}

Imlogic *Imlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Imlogic();
  return instance_;
}

void Imlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Imlogic::OnImConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("==========================ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Imlogic::OnImMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {
  
  return true;
}

bool Imlogic::OnImClose(struct server *srv, const int socket) {
  return true;
}

bool Imlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Imlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Imlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Imlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "im", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Imlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {

  return true;
}

bool Imlogic::OnImTrades(struct server* srv, int socket,
                                   struct PacketHead *packet) {
 
  return true;
}

bool Imlogic::OnImRecharge(struct server* srv, int socket,
                                     struct PacketHead *packet) {
 
  return true;
}

bool Imlogic::OnImWithdrawals(struct server* srv, int socket,
                                        struct PacketHead * packet) {
  
  return true;
}

bool Imlogic::OnHandleTradesPosition(struct server* srv, int socket,
                                          struct PacketHead * packet) {
  

  return true;
}

bool Imlogic::OnHisotryHandlePosition(struct server* srv, int socket,
                                           struct PacketHead* packet) {
  
  return true;
}

bool Imlogic::OnHandleWithdrawals(struct server* srv, int socket,
                                       struct PacketHead* packet) {
  
  return true;
}

}  // namespace trades_logic
