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

typedef std::map<int64, UserQustions> USERQUS; //id ,Qustions
typedef std::map<int64, USERQUS> USERQUS_ALL_MAP; //uid,Qustions_list
typedef std::map<std::string, USERQUS> S_USERQUS_ALL_MAP; //starcode,Qustions_list

typedef std::map<int64, std::vector<int64> > USER_SEE_ALL_MAP;

class CircleManager {
public:
  CircleManager();
  virtual ~CircleManager();

public:
  void InitDB(circle_logic::CircleDB* circle_db);
  void InitData();
  void InitUserAskAnswerData();
  void InitUserSeeAskInfo();
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
  
  bool GetUserAsk(const int socket, const int64 session, 
                        const int32 reserved,const int64 uid,
                        const int64 pos, const int64 count);
  bool GetStarUserAsk(const int socket, const int64 session, 
                        const int32 reserved,const std::string &starcode,
                        const int64 pos, const int64 count);
  bool GetAllStarUserAskHot(const int socket, const int64 session, 
                        const int32 reserved,const int64 type,
                        const int64 pos, const int64 count);
  bool UpdateUserAsk(circle_logic::UserQustions &item);
  bool UpdateStarAnswer(const int64 id, const int32 p_type,
        const int64 answer_t, const std::string &sanswer);
private:
  void Init();

  base_logic::ListValue* GetCircleApproveList(Circle*& circle);
  base_logic::ListValue* GetCircleCommentList(Circle*& circle);
  std::string GetUserName(int64 uid);
//____________
  void GetUserAskNoLock(const int64 uid, std::list<UserQustions> &list);
  void GetStarUserAskNoLock(const std::string &starcode, std::list<UserQustions> &list);

private:
  manager_schduler::SchdulerEngine* manager_schduler_engine_;
  circle_logic::CircleDB* circle_db_;
  struct threadrw_t *lock_;

  CIRCLE_LIST    circle_list_;
  STAR_CIRCLE_MAP  star_circle_map_;
//__问答相关
  USERQUS_ALL_MAP userqus_all_map_;    //用户id为key 对应key的所有问答
  S_USERQUS_ALL_MAP s_userqus_all_map_;   //明星id为key 对应key的所有问答
//__明星最热语音和视频
  //std::map<std::string, UserQustions> star_user_ask_hot_voice_;
  //std::map<std::string, UserQustions> star_user_ask_hot_video_;
//___
  USER_SEE_ALL_MAP user_see_all_map_;  //用户偷听问答列表
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
