#include "infomation_logic.h"
#include "logic/swp_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include "operator_code.h"

#define DEFAULT_CONFIG_PATH "./plugins/infomation/infomation_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace infomation_logic {

Infomationlogic *Infomationlogic::instance_ = NULL;

Infomationlogic::Infomationlogic() {
  Init();
}

Infomationlogic::~Infomationlogic() {

}

bool Infomationlogic::Init() {
  bool r = false;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG_ERROR("imlogic config init error");
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG_ERROR("login config load error");
    return false;
  }
  //sqlengine = new im_mysql::Im_Mysql(config);
  return true;
}

Infomationlogic *Infomationlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Infomationlogic();
  return instance_;
}

void Infomationlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Infomationlogic::OnInfomationConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool Infomationlogic::OnInfomationMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {                               
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;
  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  switch (packet->operate_code) {
    case 1: {
      break;
    }
    default:
      break;
  }

  return true;
}
bool Infomationlogic::OnInfomationClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Infomationlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Infomationlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "infamation", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Infomationlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
}  // namespace
