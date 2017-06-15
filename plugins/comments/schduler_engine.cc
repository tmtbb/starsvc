//  Copyright (c) 2016-2017 The comments Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "comments/schduler_engine.h"
#include "comments/comments_proto_buf.h"
#include "comments/operator_code.h"
#include "comments/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

namespace comments_logic {

CommentsManager* CommentsEngine::schduler_mgr_ = NULL;
CommentsEngine* CommentsEngine::schduler_engine_ = NULL;

static std::string HEADCMS = "CMS:";
CommentsManager::CommentsManager() {
  comments_cache_ = new CommentsCache();
  Init();
}

CommentsManager::~CommentsManager() {
  DeinitThreadrw(lock_);
  //comments_cache_->comments_map_.clear();
  if (comments_cache_) {
    delete comments_cache_;
    comments_cache_ = NULL;
  }

}

void CommentsManager::Init() {
  InitThreadrw(&lock_);
}
/*
*/

void CommentsManager::InitRedisData(const std::string& hash_name) {

  std::list<star_logic::Comments> list;
  comments_redis_->ReadCommentsData(hash_name, list);
  list.sort(star_logic::Comments::after);
  comments_cache_->his_comments_map_[hash_name] = list;  
/*
  int32 all = 1440;
  while (list.size() > 0 && all > 0) {
    star_logic::Comments comments = list.front();
    list.pop_front();
    //comments.set_type(atype);
    //SetCommentsUnit(comments);
    //SetKComments(comments);
    all--;
  }
*/
}

void CommentsManager::InitRedis(
    comments_logic::CommentsRedis* comments_redis) {
  comments_redis_ = comments_redis;
}
bool CommentsManager::CommentsAddData(const int socket, const int64 session, comments_logic::net_request::FansComments &fanscomments)
{
  std::string key = HEADCMS + fanscomments.star_code().c_str();
  if (comments_redis_->WriteData(fanscomments)) //添加数据成功后处理
  {
    base_logic::RLockGd lk(lock_); 
//    std::string key = key + "CMS:" + base::BasicUtil::StringUtil::Int64ToString(fanscomments.id());
    LOG_DEBUG2("Comments Add Data key[%s] .",key.c_str());
    HIS_COMMENTS_MAP::iterator iter = comments_cache_->his_comments_map_.find(key);
    if (iter == comments_cache_->his_comments_map_.end()) //无加载或数据为空
    {
      InitRedisData(key);  
    }
    else //添加到缓存
    { 
      LOG_DEBUG2("Comments Add Data key    ......[%s] .",key.c_str());
      COMMENTS_LIST &lst = iter->second;
      star_logic::Comments item;
      item.set_star_code(fanscomments.star_code());
      item.set_fans_id(fanscomments.fans_id());
      item.set_comments(fanscomments.comments());
      item.set_nick_name(fanscomments.nick_name());
      item.set_head_url(fanscomments.head_url());
      item.set_current_unix_time(fanscomments.cms_time());
      lst.push_front(item);
      comments_cache_->his_comments_map_[key] = lst;
    }
    LOG_DEBUG2("Comments Add Data key    ......[%s] size[%d] .",key.c_str(), comments_cache_->his_comments_map_[key].size());

    return true;
  }
  LOG_DEBUG2("Comments Add Data key    ......[%s] failed.",key.c_str() );
  return false;
}

void CommentsManager::CommentsReadData()
{
  std::string name = "starcode:1001";
  std::list<std::string> lst;
  comments_redis_->ReadData(name, lst);
}

/*
*/
void CommentsManager::CommentsSendData(const int socket, const int64 session, comments_logic::net_request::HisComments &hiscomments)
{

    //std::string key = HEADCMS + base::BasicUtil::StringUtil::Int64ToString(hiscomments.id());
    std::string key = HEADCMS + (hiscomments.id()).c_str();

    LOG_DEBUG2("Comments Get Data key    ......[%s] ",key.c_str() );
    base_logic::RLockGd lk(lock_); 
    int64 total_count = 0;
    HIS_COMMENTS_MAP::iterator iter = comments_cache_->his_comments_map_.find(key);
    if (iter == comments_cache_->his_comments_map_.end()) //无加载或数据为空
    {
      InitRedisData(key);
      iter = comments_cache_->his_comments_map_.find(key);
      if (iter == comments_cache_->his_comments_map_.end()) //数据为空
      {
        send_error(socket, ERROR_TYPE, NO_HAVE_COMMENTS_DATA, session);
        return;
      }
    }
    total_count = comments_cache_->his_comments_map_[key].size();
    LOG_DEBUG2("Comments Get Data key    ......[%s] size[%d] .",key.c_str(), comments_cache_->his_comments_map_[key].size());

//___________
  int32 base_num = 10;
  int32 pos = hiscomments.start();
  int32 count = hiscomments.count();
  //if (revered / 1000 == HTTP)
   // base_num = count;
  //else
    base_num = base_num < count ? base_num : count;

  int32 t_start = 0;
  int32 t_count = 0;

  comments_logic::net_reply::AllComments all_net_comments;
  all_net_comments.set_total_count(total_count);
  COMMENTS_LIST &comments_list = comments_cache_->his_comments_map_[key];
  //comments_list.sort(star_logic::Recharge::close_after);
  COMMENTS_LIST::iterator iter_cms = comments_list.begin();

  for (; iter_cms != comments_list.end() && t_count < count; iter_cms++){
  //while (comments_list.size() > 0 && t_count < count){} 
    star_logic::Comments comments = (*iter_cms);//comments_list.front();
    //comments_list.pop_front();
    t_start++;
    if (t_start < pos)
      continue;
    net_reply::Comments* net_comments = new net_reply::Comments;

    net_comments->set_star_code(comments.star_code());
    net_comments->set_fans_id(comments.fans_id());
    net_comments->set_comments(comments.comments());
    net_comments->set_nick_name(comments.nick_name());
    net_comments->set_head_url(comments.head_url());
    net_comments->set_cms_time(comments.current_unix_time());


    all_net_comments.set_unit(net_comments->get());
    t_count++;
    if (all_net_comments.Size() % base_num == 0
        && all_net_comments.Size() != 0) {
      struct PacketControl packet_control;
      MAKE_HEAD(packet_control, S_COMMENTS_DATA, 1, 0, session, 0);
      packet_control.body_ = all_net_comments.get();
      send_message(socket, &packet_control);
      all_net_comments.Reset();
      all_net_comments.set_total_count(total_count);
    }
  }

  if (all_net_comments.Size() > 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_COMMENTS_DATA, 1, 0, session, 0);
    packet_control.body_ = all_net_comments.get();
    send_message(socket, &packet_control);
  }
  else
  {
    send_error(socket, ERROR_TYPE, NO_HAVE_COMMENTS_DATA, session);
  }
/*
  std::string name = "starcode:1001";
  std::list<std::string> lst;
  comments_redis_->ReadData(name, lst);
*/

}

void CommentsManager::LoginCommentsCenter(const int socket) {
/*
  comments_logic::net_other::Login login;
  login.set_aid(time(NULL));
  login.set_atype(4);
  login.set_passowrd("1234567x");
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, LOGIN_QUOTATIONS, 1, 0, 0, 0);
  packet_control.body_ = login.get();
  send_message(socket, &packet_control);
*/
}

}
