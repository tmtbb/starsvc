//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef STAR_CIRCLE_SCHDULER_ENGINE_H__
#define STAR_CIRCLE_SCHDULER_ENGINE_H__

#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
#include "logic/time.h"
#include "logic/star_infos.h"
#include "manager/data_share_mgr.h"
#include "circle_db.h"
#include "circle_info.h"

namespace circle_logic {

typedef std::list<Circle*> CIRCLE_LIST;
typedef std::map<int64, Circle*> CIRCLE_MAP;//circle_id
typedef std::map<std::string, CIRCLE_MAP> STAR_CIRCLE_MAP;//key:symble

class CircleManager {
public:
  CircleManager();
  virtual ~CircleManager();

public:
  void InitDB(circle_logic::CircleDB* circle_db);
  void InitData();
  void InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine);

  bool CreateCircle(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol, const std::string content, const std::string picurl);

  bool DeleteCircle(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol, const int64 circleid);

  bool ApprovalCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol, const int64 circleid);

  bool UserCommentCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol,const int64 circleid,
            const int64 direction,const std::string& content);

  bool StarReplyCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol,const int64 circleid,
            const int64 direction,const std::string& content);
  
  bool GetSymbolAllCircle(const int socket, const int64 session, const int32 reserved,
                        const std::string& symbol, const int64 pos, const int64 count);
  
  bool GetAllCircle(const int socket, const int64 session, const int32 reserved,
                        const int64 pos, const int64 count);
  
private:
  void Init();

  base_logic::ListValue* GetCircleApproveList(Circle*& circle);
  base_logic::ListValue* GetCircleCommentList(Circle*& circle);
  std::string GetUserName(int64 uid);

private:
  manager_schduler::SchdulerEngine* manager_schduler_engine_;
  circle_logic::CircleDB* circle_db_;
  struct threadrw_t *lock_;

  CIRCLE_LIST    circle_list_;
  STAR_CIRCLE_MAP  star_circle_map_;

public:
  static int64 current_max_circle_id;
};

class CircleEngine {
private:
    static CircleEngine* schduler_engine_;
    static CircleManager* schduler_mgr_;

    CircleEngine() {
    }
    virtual ~CircleEngine() {
    }
public:
    static CircleManager* GetSchdulerManager() {
        if (schduler_mgr_ == NULL)
            schduler_mgr_ = new CircleManager();
        return schduler_mgr_;
    }

    static CircleEngine* GetCircleEngine() {
        if (schduler_engine_ == NULL)
            schduler_engine_ = new CircleEngine();
        return schduler_engine_;
    }

    static void FreeSchdulerManager() {
        if (schduler_mgr_) {
            delete schduler_mgr_;
            schduler_mgr_ = NULL;
        }
    }

    static void FreeCircleEngine() {
        if (schduler_engine_) {
            delete schduler_engine_;
            schduler_engine_ = NULL;
        }
    }
};
}

#endif
