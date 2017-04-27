//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月8日 Author: kerry

#include"manager/data_share_mgr.h"
#include "net/comm_head.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

manager_schduler::SchdulerEngine *GetManagerSchdulerEngine(void) {
  return new manager_schduler::SchdulerEngineImpl();
}

namespace manager_schduler {

bool SchdulerEngineImpl::SetUserInfoSchduler(const int64 id,
                                             swp_logic::UserInfo* user) {
  ManagerSchdulerEngine* schduler_mgr =
      EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->SetUserInfoSchduler(id, user);
}

bool SchdulerEngineImpl::GetUserInfoSchduler(const int64 id,
                                             swp_logic::UserInfo* user) {
  ManagerSchdulerEngine* schduler_mgr =
      EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->GetUserInfoSchduler(id, user);
}

bool SchdulerEngineImpl::DelUserInfoSchduler(const int64 id) {
  ManagerSchdulerEngine* schduler_mgr =
      EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->DelUserInfoSchduler(id);
}

bool SchdulerEngineImpl::FindUserInfoSchduler(const int socket,
                                              swp_logic::UserInfo* schduler) {
  ManagerSchdulerEngine* schduler_mgr =
      EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->FindUserInfoSchduler(socket, schduler);
}

bool SchdulerEngineImpl::CloseUserInfoSchduler(const int socket) {
  ManagerSchdulerEngine* schduler_mgr =
      EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->CloseUserInfoSchduler(socket);
}

bool SchdulerEngineImpl::SetRecvTime(const int socket) {
  ManagerSchdulerEngine* schduler_mgr =
        EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->SetRecvTime(socket);
}

bool SchdulerEngineImpl::SetSendTime(const int socket) {
  ManagerSchdulerEngine* schduler_mgr =
        EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->SetSendTime(socket);
}

bool SchdulerEngineImpl::CheckHeartPacket() {
  ManagerSchdulerEngine* schduler_mgr =
        EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->CheckHeartPacket();
}

bool SchdulerEngineImpl::SetSendErrorCount(const int socket) {
  ManagerSchdulerEngine* schduler_mgr =
        EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->SetSendErrorCount(socket);
}

bool SchdulerEngineImpl::SetRecvErrorCount(const int socket) {
  ManagerSchdulerEngine* schduler_mgr =
        EngineSchdulerEngine::GetSchdulerManager();
  return schduler_mgr->SetRecvErrorCount(socket);
}


ManagerSchdulerEngine* EngineSchdulerEngine::schduler_mgr_ = NULL;
EngineSchdulerEngine* EngineSchdulerEngine::schduler_engine_ = NULL;

ManagerSchdulerEngine::ManagerSchdulerEngine() {
  schduler_cache_ = new SchdulerCache();
  Init();
}

void ManagerSchdulerEngine::Init() {
  InitThreadrw (&lock_);
}

ManagerSchdulerEngine::~ManagerSchdulerEngine() {
  DeinitThreadrw (lock_);
  schduler_cache_->user_map_.clear();
  schduler_cache_->socket_map_.clear();
  if (schduler_cache_) {
    delete schduler_cache_;
    schduler_cache_ = NULL;
  }
}

bool ManagerSchdulerEngine::SetUserInfoSchduler(const int64 id,
                                                swp_logic::UserInfo* user){
  base_logic::WLockGd lk(lock_);
  int socket = user->socket_fd();
  SOCKET_MAP::iterator it = schduler_cache_->socket_map_.find(socket);
  /*if (schduler_cache_->socket_map_.end() != it) {
    LOG_ERROR2("find old socket reconnected, socket=%d", socket);
    return false;
  }*/
  base::MapAdd<SOCKET_MAP, int, swp_logic::UserInfo>(
      schduler_cache_->socket_map_, user->socket_fd(), (*user));
  return base::MapAdd<USER_MAP, int64, swp_logic::UserInfo>(
      schduler_cache_->user_map_, id, (*user));
}

bool ManagerSchdulerEngine::GetUserInfoSchduler(const int64 id,
                                                swp_logic::UserInfo* user) {
  base_logic::RLockGd lk(lock_);
  return base::MapGet<USER_MAP, USER_MAP::iterator, int64,
      swp_logic::UserInfo>(schduler_cache_->user_map_, id,
                                            (*user));
}

bool ManagerSchdulerEngine::FindUserInfoSchduler(const int socket,
                                                 swp_logic::UserInfo* user) {
  base_logic::RLockGd lk(lock_);
  return base::MapGet<SOCKET_MAP, SOCKET_MAP::iterator, int,
      swp_logic::UserInfo>(schduler_cache_->socket_map_,
                                            socket, (*user));
}

bool ManagerSchdulerEngine::CloseUserInfoSchduler(int socket) {
  base_logic::WLockGd lk(lock_);
  swp_logic::UserInfo user;
  base::MapGet<SOCKET_MAP, SOCKET_MAP::iterator, int,
  swp_logic::UserInfo>(schduler_cache_->socket_map_,
                                            socket, user);
  user.set_is_effective(false);
  base::MapDel<SOCKET_MAP, SOCKET_MAP::iterator, int>(
      schduler_cache_->socket_map_, socket);
  return base::MapDel<USER_MAP, USER_MAP::iterator, int64>(
      schduler_cache_->user_map_, user.uid());
}

bool ManagerSchdulerEngine::SetSendTime(int socket) {
  base_logic::WLockGd lk(lock_);
  swp_logic::UserInfo& user =
      schduler_cache_->socket_map_[socket];
  user.set_send_last_time(time(NULL));
  return true;
}

bool ManagerSchdulerEngine::SetRecvTime(int socket) {
  base_logic::WLockGd lk(lock_);
  swp_logic::UserInfo& user =
      schduler_cache_->socket_map_[socket];
  user.set_recv_last_time(time(NULL));
  return true;
}

bool ManagerSchdulerEngine::SetSendErrorCount(int socket) {
  base_logic::WLockGd lk(lock_);
  swp_logic::UserInfo& user =
      schduler_cache_->socket_map_[socket];
  user.add_send_error_count();
  return true;
}

bool ManagerSchdulerEngine::SetRecvErrorCount(int socket) {
  base_logic::WLockGd lk(lock_);
  swp_logic::UserInfo& user =
      schduler_cache_->socket_map_[socket];
  user.add_recv_error_count();
  return true;
}



bool ManagerSchdulerEngine::CheckHeartPacket() {
  time_t current_time = time(NULL);
  bool r = false;
  base_logic::WLockGd lk(lock_);

  USER_MAP::iterator it = schduler_cache_->user_map_.begin();
  for (; it != schduler_cache_->user_map_.end(); it++) {
    swp_logic::UserInfo& user = it->second;
    if ((current_time - user.recv_last_time() > 300)) {
      user.add_send_error_count();
      //LOG_DEBUG2("location of schduler=%p current_time=%d crawler_schduler out of time %d socket=%d send_error_count=%d",
      //&schduler, (int)current_time, (int)schduler.recv_last_time(), schduler.socket(), schduler.send_error_count());
    }

    if (user.send_error_count() > 3) {
      LOG_MSG("close connection");
      user.set_is_effective(false);
      base::MapDel<SOCKET_MAP, SOCKET_MAP::iterator, int>(
          schduler_cache_->socket_map_, user.socket_fd());
      base::MapDel<USER_MAP, USER_MAP::iterator, int64>(
          schduler_cache_->user_map_,user.uid());
      //closelockconnect(schduler.socket());
      continue;
    }
  }
  return true;
}

}
