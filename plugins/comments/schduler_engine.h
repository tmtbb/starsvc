//  Copyright (c) 2016-2017 The comments Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef COMMENTS_SCHDULER_ENGINE_H__
#define COMMENTS_SCHDULER_ENGINE_H__

#include "logic/star_infos.h"
#include "comments/comments_redis.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"

typedef std::list<star_logic::Comments> COMMENTS_LIST;
/*typedef std::map<std::string, COMMENTS_LIST> COMMENTS_MAP; /*现货ID，行情*//*平台名称:交易所:现货ID 如:JH:TJPME:AG*/
/*typedef std::map<int64, COMMENTS_MAP> COMMENTS_ALL_MAP; /*股票,现货,期货 COMMENTS_MAP*/

/*typedef std::map<std::string, star_logic::Comments> LAST_COMMENTS_MAP;  //最近一次报价
typedef std::map<int64, LAST_COMMENTS_MAP> LAST_COMMENTS_ALL_MAP;

//正在计算的K线
typedef std::map<std::string, star_logic::Comments> K_COMMENTS_MAP; /*KEY SYMBOL 外汇标识*/
/*typedef std::map<int32, K_COMMENTS_MAP> K_ALL_COMMENTS_MAP; /*KEY 时间标识*/

//
typedef std::map<std::string, COMMENTS_LIST> HIS_COMMENTS_MAP; /*key=strcode value=comments*/
//typedef std::map<int32, HIS_COMMENTS_MAP> ALL_HIS_COMMENTS_MAP; /*时间标识*/

namespace comments_logic {

enum TIMETYPE {
  ONE_MINUTE = 60,
  FIVE_MINUTE = 300,
  QUARTER_MINUTE = 900,
  HALF_HOUR = 1800,
  ONE_HOUR = 3600
};

class CommentsCache {
 public:
 HIS_COMMENTS_MAP his_comments_map_;
	 /*
  COMMENTS_ALL_MAP comments_map_;
  LAST_COMMENTS_ALL_MAP last_comments_map_;
  K_ALL_COMMENTS_MAP current_k_all_map_;
  K_ALL_HIS_COMMENTS_MAP k_history_all_map_;*/
};
  
/*
typedef std::map<std::string, COMMENTS_LIST> COMMENTS_MAP; /*现货ID，行情*/
/*typedef std::map<int64,COMMENTS_MAP> COMMENTS_ALL_MAP; /*股票,现货,期货 COMMENTS_MAP*/
/*class CommentsCache {
 public:
  COMMENTS_ALL_MAP  comments_map_;
};*/

class CommentsManager {
 public:
  CommentsManager();
  virtual ~CommentsManager();

  //void SetComments(star_logic::Comments& quotation);

  void LoginCommentsCenter(const int socket);
/*
  void SendRealTime(const int socket, const int64 session, const int32 reversed,
                    base_logic::ListValue* value);

  void SendKChartLine(const int socket, const int64 session,
                      const int32 reversed, const int32 kchar_type,
                      const std::string& exchange_name,
                      const std::string& platform_name,
                      const std::string& symbol, const int64 start_time,
                      const int64 end_time, const int32 count);

  void SendTimeLine(const int socket, const int64 session, const int32 reversed,
                    const int32 atype, const std::string& exchange_name,
                    const std::string& platform_name, const std::string& symbol,
                    const int64 start_time, const int64 end_time,
                    const int32 count);

  void TimeEvent(int opcode, int time);
*/
  bool CommentsAddData(const int socket, const int64 session, comments_logic::net_request::FansComments &fanscomments);
  void CommentsReadData();
  void CommentsSendData(const int socket, const int64 session, comments_logic::net_request::HisComments &hiscomments);
  void InitRedis(comments_logic::CommentsRedis* comments_redis);

  void InitRedisData(const std::string& hash_name);
 private:
/*
  void SetKComments(const std::string& key, const int32 time_symbol,
                      star_logic::Comments& quotation);

  void SetCommentsUnit(star_logic::Comments& quotation);

  void SetKComments(star_logic::Comments& quotation);
  void SetKCommentsUnit(const int32 time_symbol,
                          star_logic::Comments& quotation);

  void GetRealTime(const int32 atype, const std::string& symbol,
                   star_logic::Comments* comments);

  void GetTimeLine(const int32 atype, const std::string& symbol,
                   std::list<star_logic::Comments>& list,
                   const int64 end_time = 0);

  void GetKChartLine(const int32 chart_type, const std::string& key,
                     std::list<star_logic::Comments>& list,
                     const int64 end_time = 0);
*/
 private:
  void Init();
 private:
  CommentsCache *comments_cache_;
  comments_logic::CommentsRedis* comments_redis_;

  struct threadrw_t *lock_;
};

class CommentsEngine {
 private:
  static CommentsEngine* schduler_engine_;
  static CommentsManager* schduler_mgr_;

  CommentsEngine() {
  }
  virtual ~CommentsEngine() {
  }
 public:
  static CommentsManager* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new CommentsManager();
    return schduler_mgr_;
  }

  static CommentsEngine* GetCommentsEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new CommentsEngine();
    return schduler_engine_;
  }
};
}

#endif
