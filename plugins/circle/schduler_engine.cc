//  Copyright (c) 2016-2017 The star Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "schduler_engine.h"
#include "basic/template.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "errno.h"
#include "operator_code.h"


namespace circle_logic {
CircleManager* CircleEngine::schduler_mgr_ = NULL;
CircleEngine* CircleEngine::schduler_engine_ = NULL;
int64 CircleManager::current_max_circle_id = 0;

CircleManager::CircleManager() {
  InitThreadrw(&lock_);
  Init();
}

CircleManager::~CircleManager() {
  DeinitThreadrw(lock_);
}

void CircleManager::InitDB(circle_logic::CircleDB* circle_db) {
  circle_db_ = circle_db;
}

void CircleManager::Init() {
    
}

void CircleManager::InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine) {
  manager_schduler_engine_ = schduler_engine;
}

void CircleManager::InitData() {
  bool r = false;
  base_logic::ListValue *listvalue;
  
  if(!circle_db_->OnFetchAllCircleInfo(listvalue))
    return;

  while (listvalue->GetSize()) {
    circle_logic::Circle* circle = new circle_logic::Circle();
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    circle->ValueSerialization(dict_result_value);
    
    
    CIRCLE_MAP t_circle_map;
    Circle* t_circle;
    r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, circle->GetSymbol(), t_circle_map);
    if(r) {
      r = base::MapGet<CIRCLE_MAP,CIRCLE_MAP::iterator, int64, Circle*>
            (t_circle_map, circle->GetCircleId(), t_circle);
    }

    if(!r){
      t_circle_map[circle->GetCircleId()] = circle;
      star_circle_map_[circle->GetSymbol()] = t_circle_map;
      circle_list_.push_back(circle);

      if(current_max_circle_id < circle->GetCircleId())
        current_max_circle_id = circle->GetCircleId();
    }

    delete dict_result_value;
    dict_result_value = NULL;
  }
  LOG_MSG2("circle list size %ld", circle_list_.size());
  
}

//发表动态
bool CircleManager::CreateCircle(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol, const std::string content, const std::string picurl){
  circle_logic::Circle* circle = new circle_logic::Circle();
  if(circle == NULL){
    send_error(socket, ERROR_TYPE, NO_HAVE_MEMMORY, session);
    return false;
  }
  ++current_max_circle_id;
  circle->SetCircleId(current_max_circle_id);
  circle->SetSymbol(symbol);
  circle->SetCreateTime(time(NULL));
  circle->SetContent(content);
  circle->SetPicUrl(picurl);
  circle->SetStatus(CIRCLE_NORMAL_STATUS);

/*  base_logic::RLockGd lk(lock_);
  CIRCLE_MAP t_circle_map;
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
        (star_circle_map_, circle->GetSymbol(), t_circle_map);


  circle_list_.push_back(circle);
  t_circle_map[current_max_circle_id] = circle;
  star_circle_map_[symbol] = t_circle_map;
  */

    {
       base_logic::RLockGd lk(lock_);
        STAR_CIRCLE_MAP::iterator it = star_circle_map_.find(circle->GetSymbol());
        if(it != star_circle_map_.end()){
            CIRCLE_MAP& t_circle_map = it->second;
            t_circle_map[current_max_circle_id] = circle;
        }else{
            CIRCLE_MAP t_circle_map;
            t_circle_map[current_max_circle_id] = circle;
            star_circle_map_[symbol] = t_circle_map;
        }
    }

  if(circle_db_->OnCreateCircleOrder(*circle)){ //可优化
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    return false;
  }


  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(current_max_circle_id);
  dic->Set("circle_id", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_STAR_ADD_CIRCLE, CIRCLE_TYPE, 0, session, reserved);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);
}

//删除动态
bool CircleManager::DeleteCircle(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol, const int64 circleid){
  CIRCLE_MAP t_circle_map;
  Circle* t_circle;
  base_logic::RLockGd lk(lock_);
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, symbol, t_circle_map);
  if(r) {
    r = base::MapGet<CIRCLE_MAP,CIRCLE_MAP::iterator, int64, Circle*>
          (t_circle_map, circleid, t_circle);
  }

  if(!r){
    send_error(socket, ERROR_TYPE, NO_CIRCLE_NO_ERR, session);
    return false;
  }

  t_circle->SetStatus(CIRCLE_DEL_STATUS);
  if(circle_db_->OnDeleteCircle(circleid)){ //可优化
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    return false;
  }

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(1);
  dic->Set("result", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_STAR_DEL_CIRCLE, CIRCLE_TYPE, 0, session, reserved);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);
}

//点赞
bool CircleManager::ApprovalCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol, const int64 circleid){
  
  CIRCLE_MAP t_circle_map;
  Circle* t_circle;
  base_logic::RLockGd lk(lock_);
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, symbol, t_circle_map);
  if(r) {
    r = base::MapGet<CIRCLE_MAP,CIRCLE_MAP::iterator, int64, Circle*>
          (t_circle_map, circleid, t_circle);
  }

  if(!r){
    send_error(socket, ERROR_TYPE, NO_CIRCLE_NO_ERR, session);
    return false;
  }

  star_logic::StarInfo star;
  r = manager_schduler_engine_->GetStarInfoSchduler(symbol, &star);
  if(!r){
    send_error(socket, ERROR_TYPE, NO_STAR_NO_EXIST, session);
    return false;
  }

  t_circle->AddApproveId(uid);
  int64 result = 1;
  if(!circle_db_->OnUpdateCircle(uid, *t_circle, star.approval_dec_time(), result)){ //可优化
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    t_circle->DelApprovId();
    return false;
  }
  if( result < 0 ) {
    send_error(socket, ERROR_TYPE, result, session);
    t_circle->DelApprovId();
    return false;
  }
  

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(result);
  dic->Set("result", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_USER_APPROVE_CIRCLE, CIRCLE_TYPE, 0, session, reserved);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);
}

//用户发表评论/回复
bool CircleManager::UserCommentCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol,const int64 circleid,const int64 direction,const std::string& content){
  
  CIRCLE_MAP t_circle_map;
  base_logic::RLockGd lk(lock_);
  Circle* t_circle;
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, symbol, t_circle_map);
  if(r) {
    r = base::MapGet<CIRCLE_MAP,CIRCLE_MAP::iterator, int64, Circle*>
          (t_circle_map, circleid, t_circle);
  }

  if(!r){
    send_error(socket, ERROR_TYPE, NO_CIRCLE_NO_ERR, session);
    return false;
  }

  star_logic::StarInfo star;
  r = manager_schduler_engine_->GetStarInfoSchduler(symbol, &star);
  if(!r){
    send_error(socket, ERROR_TYPE, NO_STAR_NO_EXIST, session);
    return false;
  }

  int64 lastpriority = 1 + t_circle->GetLastPriority();
  circle_logic::Scomment scomment;
  scomment.uid = uid;
  scomment.direction = direction;
  scomment.content = content;
  scomment.priority = lastpriority;
  t_circle->AddComment(scomment);

  int64 result = 1;
  if(!circle_db_->OnUpdateCircle(uid, *t_circle, star.comment_dec_time(), result)){ //可优化
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    t_circle->DelComment();
    return false;
  }

  if( result < 0 ) {
    send_error(socket, ERROR_TYPE, result, session);
    t_circle->DelComment();
    return false;
  }


  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(result);
  dic->Set("result", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_USER_COMMENT_CIRCLE, CIRCLE_TYPE, 0, session, reserved);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);
}

//明星回复评论
bool CircleManager::StarReplyCircle(const int socket, const int64 session, const int32 reserved,
            const int64 uid, const std::string& symbol,const int64 circleid,const int64 direction,const std::string& content){
  
  CIRCLE_MAP t_circle_map;
  Circle* t_circle;
  base_logic::RLockGd lk(lock_);
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, symbol, t_circle_map);
  if(r) {
    r = base::MapGet<CIRCLE_MAP,CIRCLE_MAP::iterator, int64, Circle*>
          (t_circle_map, circleid, t_circle);
  }

  if(!r){
    send_error(socket, ERROR_TYPE, NO_CIRCLE_NO_ERR, session);
    return false;
  }

/*  if(t_circle->m_commentList.empty())
  {
    send_error(socket, ERROR_TYPE, NO_STAR_REPLY_ERR, session);
    return false;
  }*/

  star_logic::StarInfo star;
  r = manager_schduler_engine_->GetStarInfoSchduler(symbol, &star);
  if(!r){
    send_error(socket, ERROR_TYPE, NO_STAR_NO_EXIST, session);
    return false;
  }

  int64 lastpriority = 1 + t_circle->GetLastPriority();
  circle_logic::Scomment scomment;
  scomment.uid = uid;
  scomment.direction = direction;
  scomment.content = content;
  scomment.priority = lastpriority;
  t_circle->AddComment(scomment);

  int64 result = 1;
  if(!circle_db_->OnUpdateCircle(uid, *t_circle, star.comment_dec_time(), result)){ //可优化
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    t_circle->DelComment();
    return false;
  }
  if( result < 0 ) {
    send_error(socket, ERROR_TYPE, result, session);
    t_circle->DelComment();
    return false;
  }

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(result);
  dic->Set("result", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_STAR_REPLY_COMMENT, CIRCLE_TYPE, 0, session, reserved);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);
}

//明星动态列表
bool CircleManager::GetSymbolAllCircle(const int socket, const int64 session, const int32 reserved,
                        const std::string& symbol, const int64 pos, const int64 count){
  circle_reply::CircleListReply circlereplylist;
  
  base_logic::RLockGd lk(lock_);
  CIRCLE_MAP t_circle_map;
  bool r = base::MapGet<STAR_CIRCLE_MAP,STAR_CIRCLE_MAP::iterator, std::string, CIRCLE_MAP>
          (star_circle_map_, symbol, t_circle_map);
  if(!r){
    send_error(socket, ERROR_TYPE, NO_CIRCLE_NO_ERR, session);
    return false;
  }

  int32 posnum = 0;
  int32 startnum = 0;
  CIRCLE_MAP::reverse_iterator it = t_circle_map.rbegin();
  for(; it != t_circle_map.rend() && startnum<count; ++it){
    Circle*& circle = it->second;
    if(circle->GetStatus() == CIRCLE_DEL_STATUS)
      continue;

    ++posnum;
    if(posnum > pos){
      ++startnum;
      
      circle_reply::CircleReply* circlereply = new circle_reply::CircleReply();
      circlereply->SetCircle(circle);
      circlereply->SetApproveList(GetCircleApproveList(circle));
      circlereply->SetCommentList(GetCircleCommentList(circle));

      star_logic::StarInfo star;
      r = manager_schduler_engine_->GetStarInfoSchduler(symbol, &star);
      if (!r) {
        circlereply->SetName("");
        circlereply->SetHeadUrl("");
        circlereply->SetApproveDecTime(1);
        circlereply->SetCommentDecTime(1);
      }
      else{
        circlereply->SetName(star.name());
        circlereply->SetHeadUrl(star.pic());
        circlereply->SetApproveDecTime(star.approval_dec_time());
        circlereply->SetCommentDecTime(star.comment_dec_time());
      }
      
      
      circlereplylist.set_unit(circlereply->get());
    }
  }

  LOG_DEBUG2("Get symbol[%s] circle list size:%d, pos[%ld], count[%ld]", 
          symbol.c_str(), circlereplylist.Size(), pos, count);
  if (circlereplylist.Size() != 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_GET_STAR_CIRCLE_INFO, CIRCLE_TYPE, 0,
              session, reserved);
    packet_control.body_ = circlereplylist.get();
    send_message(socket, &packet_control);
  }else {
    send_error(socket, ERROR_TYPE, NO_HAVE_STAR_CIRCLE_ERR,session); 
  }
}

//获取全量明星动态列表
bool CircleManager::GetAllCircle(const int socket, const int64 session, const int32 reserved,
                        const int64 pos, const int64 count){
  
  circle_reply::CircleListReply circlereplylist;

  bool r = false;
  int32 posnum = 0;
  int32 startnum = 0;

  base_logic::RLockGd lk(lock_);
  CIRCLE_LIST::reverse_iterator iter = circle_list_.rbegin();
  for(; iter != circle_list_.rend() && startnum<count; ++iter){
    if((*iter)->GetStatus() == CIRCLE_DEL_STATUS)
      continue;
    
    ++posnum;
    if(posnum > pos){
      ++startnum;
      circle_reply::CircleReply* circlereply = new circle_reply::CircleReply();
      circlereply->SetCircle(*iter);
      circlereply->SetApproveList(GetCircleApproveList(*iter));
      circlereply->SetCommentList(GetCircleCommentList(*iter));

      star_logic::StarInfo star;
      r = manager_schduler_engine_->GetStarInfoSchduler((*iter)->GetSymbol(), &star);
      if (!r) {
        circlereply->SetName("");
        circlereply->SetHeadUrl("");
        circlereply->SetApproveDecTime(1);
        circlereply->SetCommentDecTime(1);
      }
      else{
        circlereply->SetName(star.name());
        circlereply->SetHeadUrl(star.pic());
        circlereply->SetApproveDecTime(star.approval_dec_time());
        circlereply->SetCommentDecTime(star.comment_dec_time());
      }
      
      circlereplylist.set_unit(circlereply->get());
    }
  }


  LOG_DEBUG2("Get all symbols circle list size:%d", circlereplylist.Size());
  if (circlereplylist.Size() != 0) {
    std::string tempstring;
    {
      base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
        0, &tempstring);
      engine->Serialize(*((base_logic::Value*) circlereplylist.get()));
    }

    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_GET_ALL_CIRCLE_INFO, CIRCLE_TYPE, 0,
              session, reserved);
    packet_control.body_ = circlereplylist.get();
    send_message(socket, &packet_control);
  }else {
    send_error(socket, ERROR_TYPE, NO_HAVE_STAR_CIRCLE_ERR, session); 
  }
}

/* return
// {{"uid":123,"user_name":"abc123"}{"uid":456,"user_name":"abc456"}...}
*/
base_logic::ListValue* CircleManager::GetCircleApproveList(Circle*& circle){
  base_logic::ListValue* approvelist = new base_logic::ListValue();

  std::list<int64>::iterator it = circle->m_approveList.begin();
  for(; it!= circle->m_approveList.end(); ++it){
    base_logic::DictionaryValue* t_dir = new base_logic::DictionaryValue();
    base_logic::FundamentalValue* t_uid = new base_logic::FundamentalValue(*it);
    t_dir->Set(L"uid", t_uid);
    base_logic::StringValue* t_username = new base_logic::StringValue(GetUserName(*it));
    t_dir->Set(L"user_name", t_username);
    approvelist->Append((base_logic::Value*) (t_dir));
  }

  return approvelist;
}

/* return
// {{"uid":123,"user_name":"abc123","direction":0,"content":"abc123","priority":0}
//  {"uid":456,"user_name":"abc456","direction":1,"content":"321cba","priority":1}... }
*/
base_logic::ListValue* CircleManager::GetCircleCommentList(Circle*& circle) {
  base_logic::ListValue* commentlist = new base_logic::ListValue();
  
  circle_logic::COMMENTLIST::iterator it = circle->m_commentList.begin();
  for(; it!= circle->m_commentList.end(); ++it){
    base_logic::DictionaryValue* t_dir = new base_logic::DictionaryValue();

    base_logic::FundamentalValue* t_uid = new base_logic::FundamentalValue(it->uid);
    t_dir->Set(L"uid", t_uid);
    base_logic::StringValue* t_username = new base_logic::StringValue(GetUserName(it->uid));
    t_dir->Set(L"user_name", t_username);
    base_logic::FundamentalValue* t_direction = new base_logic::FundamentalValue(it->direction);
    t_dir->Set(L"direction", t_direction);
    base_logic::StringValue* t_content = new base_logic::StringValue(it->content);
    t_dir->Set(L"content", t_content);
    base_logic::FundamentalValue* t_priority = new base_logic::FundamentalValue(it->priority);
    t_dir->Set(L"priority", t_priority);

    commentlist->Append((base_logic::Value*) (t_dir));
  }

  return commentlist;
}

std::string CircleManager::GetUserName(int64 uid){
  star_logic::UserInfo tuserinfo;
  if(manager_schduler_engine_->GetUserInfoSchduler(uid, &tuserinfo)){
    return tuserinfo.nickname();
  }

  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  circle_db_->OnGetUserName(uid, dict);
  std::string username("");
  dict->GetString(L"user_name", &username);

  if(dict){
    delete dict;
    dict = NULL;
  }

  return username;
}


}


