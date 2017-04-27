//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef MANAGER_DATA_SHARE_MGR_H__
#define MANAGER_DATA_SHARE_MGR_H__

#include "logic/swp_infos.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"

namespace manager_schduler {

typedef std::map<int64, swp_logic::UserInfo> USER_MAP;
typedef std::map<int, swp_logic::UserInfo> SOCKET_MAP;

class SchdulerEngine {
 public:
  virtual bool SetUserInfoSchduler(const int64 id,
                                   swp_logic::UserInfo* user) = 0;

  virtual bool GetUserInfoSchduler(const int64 id,
                                   swp_logic::UserInfo* user) = 0;

  virtual bool DelUserInfoSchduler(const int64 id) = 0;

  virtual bool FindUserInfoSchduler(const int socket,
                                    swp_logic::UserInfo* schduler) = 0;

  virtual bool CloseUserInfoSchduler(const int socket) = 0;

  virtual bool SetRecvTime(const int socket) = 0;

  virtual bool SetSendTime(const int socket) = 0;

  virtual bool CheckHeartPacket() = 0;

  virtual bool SetSendErrorCount(int socket) = 0;

  virtual bool SetRecvErrorCount(int socket) = 0;

};

class SchdulerEngineImpl : public SchdulerEngine {
 public:
  bool SetUserInfoSchduler(const int64 id, swp_logic::UserInfo* user);

  bool GetUserInfoSchduler(const int64 id,
                           swp_logic::UserInfo* user);

  bool DelUserInfoSchduler(const int64 id);

  bool FindUserInfoSchduler(const int socket, swp_logic::UserInfo* schduler);

  bool CloseUserInfoSchduler(const int socket);

  bool SetRecvTime(const int socket);

  bool SetSendTime(const int socket);

  bool CheckHeartPacket();

  bool SetSendErrorCount(int socket);

  bool SetRecvErrorCount(int socket);

};

class SchdulerCache {
 public:
  USER_MAP user_map_;
  SOCKET_MAP socket_map_;
};

__attribute__((visibility("default")))
      class ManagerSchdulerEngine {
 public:
  ManagerSchdulerEngine();
  virtual ~ManagerSchdulerEngine();
 public:
  bool SetUserInfoSchduler(const int64 id, swp_logic::UserInfo* user);

  bool GetUserInfoSchduler(const int64 id,
                           swp_logic::UserInfo* user);

  bool DelUserInfoSchduler(const int64 id);

  bool FindUserInfoSchduler(const int socket, swp_logic::UserInfo* schduler);

  bool CloseUserInfoSchduler(const int socket);

  bool SetRecvTime(const int socket);

  bool SetSendTime(const int socket);

  bool CheckHeartPacket();

  bool SetSendErrorCount(int socket);

  bool SetRecvErrorCount(int socket);

 private:
  void Init();

 public:
  SchdulerCache* GetFindCache() {
    return this->schduler_cache_;
  }

 private:
  struct threadrw_t* lock_;
  SchdulerCache* schduler_cache_;
};

class EngineSchdulerEngine {
 private:
  static ManagerSchdulerEngine *schduler_mgr_;
  static EngineSchdulerEngine *schduler_engine_;

  EngineSchdulerEngine() {
  }

  virtual ~EngineSchdulerEngine() {
  }
 public:
  __attribute__((visibility("default")))
            static ManagerSchdulerEngine* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new ManagerSchdulerEngine();
    return schduler_mgr_;
  }

  static EngineSchdulerEngine* GetSchdulerEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new EngineSchdulerEngine();

    return schduler_engine_;
  }
};
}

#ifdef __cplusplus
extern "C" {
#endif
manager_schduler::SchdulerEngine *GetManagerSchdulerEngine(void);
#ifdef __cplusplus
}
#endif

#endif

