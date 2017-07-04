
#ifndef SWP_PUSH_LOGIC_
#define SWP_PUSH_LOGIC_

#include "basic/basictypes.h"
#include "core/common.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "manager/data_share_mgr.h"
#include "push_kafka.h"
#include "push_engine.h"
#include "push_db.h"

namespace push_logic {

class CPushlogic {
 public:
  CPushlogic();
  virtual ~CPushlogic();
 private:
  bool OnQutations(struct server* srv, int socket, struct PacketHead *packet);
 private:
  static CPushlogic *instance_;
  // DB
  push_logic::CPushDB *m_sqlengine;
  //kafka
  push_logic::CPushKafka *m_kafkaengine;
  //消息工厂
  push_logic::CMessageFactory* m_pMessageFactory;
  //具体消息
  push_logic::IMessage* m_pPushMessage;
  //推送工厂
  push_logic::CPushMessageImplFactory* m_pPushImplFactory;
  //推送方法
  push_logic::IPushImpl* m_pPushImpl;

 public:
  static CPushlogic *GetInstance();
  static void FreeInstance();
  bool OnPushConnect(struct server *srv, const int socket);

  bool OnPushMessage(struct server *srv, const int socket, const void *msg,
                        const int len);

  bool OnPushClose(struct server *srv, const int socket);

  bool OnBroadcastConnect(struct server *srv, const int socket,
                          const void *data, const int len);

  bool OnBroadcastMessage(struct server *srv, const int socket, const void *msg,
                          const int len);

  bool OnBroadcastClose(struct server *srv, const int socket);

  bool OnIniTimer(struct server *srv);

  bool OnTimeout(struct server *srv, char *id, int opcode, int time);
private:
  bool Init();

  void DealMessage();

private:
  std::list<base_logic::DictionaryValue*> m_kafkadatalist;
};
}  // namespace push_logic

#endif
