
#ifndef SWP_MARKET_LOGIC_
#define SWP_MARKET_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"
#include "market_db.h"
#include "thread/base_thread_lock.h"
namespace market_logic {

typedef std::map<std::string, swp_logic::StarInfo> STARTS_MAP;/*明星ID为key*/

class Marketlogic {
 public:
  Marketlogic();
  virtual ~Marketlogic();
 private:
  //bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static Marketlogic *instance_;
 public:
  static Marketlogic *GetInstance();
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
  ///////////////add
  bool getmarkettypes(struct server* srv,int socket ,struct PacketHead* packet);

  bool getmarketstarlist(struct server* srv,int socket ,struct PacketHead* packet);

  bool getstarbrief(struct server* srv,int socket ,struct PacketHead* packet);

  bool searchsatr(struct server* srv,int socket ,struct PacketHead* packet);

  bool searchsatr1(struct server* srv,int socket ,struct PacketHead* packet);

  bool getstarexperience(struct server* srv,int socket ,struct PacketHead* packet);

  bool getstarachive(struct server* srv,int socket ,struct PacketHead* packet);

  bool getoptionstarlist(struct server* srv,int socket ,struct PacketHead* packet);

  bool addoptionstar(struct server* srv,int socket ,struct PacketHead* packet);

  bool getmarketstarprice(struct server* srv,int socket ,struct PacketHead* packet);

  bool getmarketstartransfer(struct server* srv,int socket ,struct PacketHead* packet);

  bool getmarketstarseek(struct server* srv,int socket ,struct PacketHead* packet);
 private:
  bool Init();
  void InitStarInfo(); 

  market_mysql::Market_Mysql* sqldb;

  struct threadrw_t* lock_;
  STARTS_MAP stars_map_;
};
}  // namespace im_logic

#endif
