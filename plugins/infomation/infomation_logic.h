
#ifndef SWP_INFOMATION_LOGIC_
#define SWP_INFOMATION_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"
#include "infomation_db.h"
namespace infomation_logic {

class Infomationlogic {
 public:
  Infomationlogic();
  virtual ~Infomationlogic();
 private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static Infomationlogic *instance_;
 public:
  static Infomationlogic *GetInstance();
  static void FreeInstance();
  bool OnInfomationConnect(struct server *srv, const int socket);

  bool OnInfomationMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnInfomationClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);

  //获取预约明星信息列表
  bool GetorderStarinfo(struct server* srv,int socket ,struct PacketHead* packet);
  
  bool GetStarinfoList(struct server* srv,int socket ,struct PacketHead* packet);
  
  bool AddStarinfo(struct server* srv,int socket ,struct PacketHead* packet);

  bool Getstarnews(struct server* srv,int socket ,struct PacketHead* packet);

  bool Getbannerlist(struct server* srv,int socket ,struct PacketHead* packet);

  bool Getfanscomment(struct server* srv,int socket ,struct PacketHead* packet);
  
  bool GetStarService(struct server* srv,int socket ,struct PacketHead* packet);
  bool AddUserOrderStarService(struct server* srv,int socket ,struct PacketHead* packet);
  bool GetUserStarNum(struct server* srv,int socket ,struct PacketHead* packet);
  bool GetUserStarTime(struct server* srv,int socket ,struct PacketHead* packet);
  bool GetStarTime(struct server* srv,int socket ,struct PacketHead* packet);
 private:
  bool Init();
  infomation_mysql::Infomation_Mysql* sqldb;
};
}  // namespace im_logic

#endif
