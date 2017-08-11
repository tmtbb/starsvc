//  Copyright (c) 2016-2017 The star Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "netflash_schduler_engine.h"
#include "basic/template.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "logic/time.h"
#include "errno.h"
#include "operator_code.h"


namespace netflash_logic {
NetflashManager* NetflashEngine::schduler_mgr_ = NULL;
NetflashEngine* NetflashEngine::schduler_engine_ = NULL;

NetflashManager::NetflashManager() {
    InitThreadrw(&lock_);
    Init();
}

NetflashManager::~NetflashManager() {
    DeinitThreadrw(lock_);
    if(!share_memory_){
      delete share_memory_;
      share_memory_ = NULL;
    }
}

void NetflashManager::InitDB(netflash_logic::NetflashDB* db) {
    netflash_db_ = db;
}

void NetflashManager::InitKafka(netflash_logic::NetflashKafka* kafka) {
    netflash_kafka_ = kafka;
}

void NetflashManager::Init() {
  share_memory_ = new flash_logic::CShareMemory();
  if(share_memory_ == NULL){
    LOG_MSG("Create share memory failure.");
    return ;
  }
  
  if(!share_memory_->AttachMemry(1))
    LOG_MSG("Attach memory failure.");
  
}

/*void NetflashManager::InitManagerSchduler(manager_schduler::SchdulerEngine* schduler_engine) {
  manager_schduler_engine_ = schduler_engine;
}*/

void NetflashManager::InitData() {
  ClearData();
  if(share_memory_ == NULL){
    share_memory_ = new flash_logic::CShareMemory();
    share_memory_->AttachMemry(1);
  }
  int32 size = share_memory_->GetStarfromShareMemory(publish_star_map_);
  LOG_DEBUG2("publish_star_map size [%d],return size [%d].", 
              publish_star_map_.size(), size);
  
  CreateTimeTask();
}

void NetflashManager::ClearData() {
  publish_star_map_.clear();
  netflash_task_list_.clear();
  netflash_task_map_.clear();
//  STAR_EXPER_MAP::iterator it = star_experience_map_.begin();
//  for(; it != star_experience_map_.end(); ++it){
//    it->second.Clear();
//  }
//  star_experience_map_.clear();
}

void NetflashManager::CreateTimeTask() {
  
  PUBLISH_STAR_MAP::iterator it = publish_star_map_.begin();
  for (; it != publish_star_map_.end(); ++it) {
    flash_logic::PulishStar& starpub = it->second;
    flash_logic::TimeTask time_task;
    time_task.set_symbol(starpub.symbol());
    time_task.set_earliest_time(starpub.publish_begin_time());
    time_task.set_latest_time(starpub.publish_end_time());
    //time_task.set_task_time(starpub.netflash_time());
    //time_task.set_interval_time(starpub.interval_time());
    //time_task.set_task_type(starpub.publish_type());//0-预售 1-发售 2-流通
    ProcessTimeTask(time(NULL), time_task);
    netflash_task_list_.push_back(time_task);
    netflash_task_map_[starpub.symbol()] = time_task;

//    base_logic::DictionaryValue ret_list;
//    if(!netflash_db_->OnGetStarExperience(starpub.symbol(),ret_list)){
//      LOG_MSG2("DB star[%s] getstarexperience error.", starpub.symbol());
//      continue;
//    }
//    star_experience_map_[starpub.symbol()] = ret_list;
  }
}

//设置开始时间
void NetflashManager::ProcessTimeTask(const time_t current_time, flash_logic::TimeTask& task) {
  if (current_time < task.earliest_time()) { //开始时间还未到
    task.set_task_start_time(task.earliest_time());
    task.set_task_type(flash_logic::TASK_PRE_SELL_TYPE);
  } else if (current_time > task.earliest_time() and current_time < task.latest_time()) {
    task.set_task_start_time(task.latest_time());
    task.set_task_type(flash_logic::TASK_SELLING_TYPE);
  } else{
    task.set_task_start_time(2145888000);
    task.set_task_type(flash_logic::TASK_SELL_TYPE);
  }

}

void NetflashManager::SynchronizeStarInfo(){
  base_logic::RLockGd lk(lock_);
  InitData();
}

void NetflashManager::TimeStarEvent() {
  base_logic::RLockGd lk(lock_);
  time_t current_time = time(NULL);
  LOG_MSG2("netflash_task_list_ size %d", netflash_task_list_.size());
  if (netflash_task_list_.size() <= 0)
    return;

  bool r = false;
  netflash_task_list_.sort(flash_logic::TimeTask::cmp);
  TIME_TASK_LIST::iterator iter = netflash_task_list_.begin();
  for(; iter != netflash_task_list_.end(); ++iter){
    LOG_MSG2("current_time %d, time_task %d", current_time, iter->task_start_time());
    if (current_time < iter->task_start_time() || iter->task_start_time() < 1262275200)
      break;

    LOG_MSG("NEXT STAT");
    ProcessTimeTask(current_time, *(iter));

    flash_logic::PulishStar pubstar;
    r = share_memory_->GetStarfromShareMemory(iter->symbol(), pubstar);
    if (!r) {
      continue;
    }
    netflash_db_->OnUpdatePublishStarInfo(iter->symbol(),
                      iter->task_type(),pubstar.publish_last_time(),pubstar.publish_begin_time());
    
    }

}

//发行明星列表
bool NetflashManager::NetSymbolList(const int socket, const int64 session, const int32 reserved){

  base_logic::ListValue *list = new base_logic::ListValue();
  PUBLISH_STAR_MAP::iterator iter = publish_star_map_.begin();
  for(; iter != publish_star_map_.end(); ++iter){
    flash_logic::PulishStar& pubstar = iter->second;
    base_logic::DictionaryValue *ret = new base_logic::DictionaryValue();
    ret->SetString(L"star_code", pubstar.symbol());
    ret->SetString(L"head_url", pubstar.pic());
    ret->SetString(L"star_name", pubstar.name());
    ret->SetString(L"work", pubstar.work());
    list->Append((base_logic::Value *) (ret));
  }

  if(list->GetSize() < 1){
    send_error(socket, ERROR_TYPE, NO_HAVE_FLASH_STAR, session);
    return false;
  }

  base_logic::DictionaryValue ret_list;
  ret_list.Set("star_list",(base_logic::Value*)list);

  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_PUB_STAR_LIST, NETFLASH_TYPE, 0, session, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

//发送明星的交易状态
bool NetflashManager::NetflashSymbolStatus(const int socket, const int64 session, const int32 reserved,
    const std::string& symbol) {
  LOG_DEBUG2("netflash_task_map_ size[%d], symbol[%s]", 
                netflash_task_map_.size(), symbol.c_str());
  bool r = false;
  flash_logic::TimeTask time_task;
  {
    base_logic::RLockGd lk(lock_);
    r = base::MapGet<TIME_TASK_MAP,TIME_TASK_MAP::iterator, std::string, flash_logic::TimeTask>
       (netflash_task_map_,symbol, time_task);
  }
  if(!r) {
    send_error(socket, ERROR_TYPE, NO_HAVE_FLASH_STAR, session);
    return false;
  }

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::StringValue* t_symbol = new base_logic::StringValue(symbol);
  base_logic::FundamentalValue* t_remainingTime = \
          new base_logic::FundamentalValue(time_task.task_start_time() - time(NULL));
  dic->Set(L"symbol", t_symbol);
  dic->Set(L"remainingTime", t_remainingTime);
  LOG_DEBUG2("t_remainingTime [%d]", 
                time_task.task_start_time() - time(NULL));
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_GET_PUB_STAR_STATUS, NETFLASH_TYPE, 0, session, 0);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);

  return true;
}

//发行明星信息
bool NetflashManager::SymbolInfo(const int socket, const int64 session, const int32 reserved,
    const std::string& symbol){
  
  PUBLISH_STAR_MAP::iterator it = publish_star_map_.find(symbol);
  if (it == publish_star_map_.end()) {
    send_error(socket, ERROR_TYPE, NO_HAVE_FLASH_STAR, session);
    return false;
  }
  flash_logic::PulishStar& pubstar = it->second;

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::StringValue* t_symbol = new base_logic::StringValue(symbol);
  base_logic::StringValue* t_starname = new base_logic::StringValue(pubstar.name());
  base_logic::StringValue* t_pic = new base_logic::StringValue(pubstar.pic());
  base_logic::StringValue* t_backpic = new base_logic::StringValue(pubstar.back_pic());
  base_logic::FundamentalValue* t_startype = new base_logic::FundamentalValue(pubstar.star_type());
  base_logic::FundamentalValue* t_publishtype = new base_logic::FundamentalValue(pubstar.publish_type());
  base_logic::FundamentalValue* t_publishtime = new base_logic::FundamentalValue(pubstar.publish_time());
  base_logic::FundamentalValue* t_publishlasttime = new base_logic::FundamentalValue(pubstar.publish_last_time());
  base_logic::FundamentalValue* t_publishbegintime = new base_logic::FundamentalValue(pubstar.publish_begin_time());
  base_logic::FundamentalValue* t_publishendtime = new base_logic::FundamentalValue(pubstar.publish_end_time());
  base_logic::FundamentalValue* t_publishprice = new base_logic::FundamentalValue(pubstar.publish_price());


  dic->Set("star_code", t_symbol);
  dic->Set("star_name", t_starname);
  dic->Set("head_url", t_pic);
  dic->Set("back_pic_url", t_backpic);
  dic->Set("star_type", t_startype);
  dic->Set("publish_type", t_publishtype);
  dic->Set("publish_time", t_publishtime);
  dic->Set("publish_last_time", t_publishlasttime);
  dic->Set("publish_begin_time", t_publishbegintime);
  dic->Set("publish_end_time", t_publishendtime);
  dic->Set("publish_price", t_publishprice);

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_GET_STAR_PUB_INFO, NETFLASH_TYPE, 0, session, 0);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);

  return true;
}

void NetflashManager::ConfirmOrder(const int socket, const int64 session, const int32 reserved,
            const int64 uid,const std::string& symbol,const int64 amount, const double price) {
  //检查明星剩余时间
  bool r = false;
  if(!share_memory_->StarLastTimeIsEnough(symbol, amount)){
    share_memory_->JudgeStarLastTimeFinish(symbol);
    send_error(socket, ERROR_TYPE, NO_PUB_STAR_NO_TIME_ERR, session);
    return ;
  }

  star_logic::TradesOrder  netflash_trades_order;
  SetNetflashOrder(uid,symbol,amount,price,netflash_trades_order);
  int64 result = 0;
  r = netflash_db_->OnCreateNetflashOrder(netflash_trades_order, result);
  if(!r){
    share_memory_->JudgeStarLastTimeFinish(symbol);
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, session);
    return ;
  }
  if(result == -1){
    share_memory_->JudgeStarLastTimeFinish(symbol);
    netflash_db_->OnUpdateNetflashsaleResult(netflash_trades_order.order_id(), MONEY_LESS_THAN);
    //用户余额不足
    send_error(socket, ERROR_TYPE, NO_USER_BALANCE_ERR, session);
    return ;
  }
  
  //更新明星剩余时间
  int64 t_last_time = share_memory_->DeductStarLastTime(symbol, amount);
  if(-1 == t_last_time){
    //找不到明星，应该不会出现
    send_error(socket, ERROR_TYPE, NO_HAVE_FLASH_STAR, session);
    return ;
  }
  LOG_MSG2("star publish_last_time[%d]", t_last_time);
  

  netflash_trades_order.set_handle_type(COMPLETE_ORDER);
  netflash_trades_order.set_buy_handle_type(COMPLETE_ORDER);
  netflash_trades_order.set_sell_handle_type(COMPLETE_ORDER);
  //通知确认
  //SendNoiceMessage(uid, netflash_trades_order.order_id(), netflash_trades_order.handle_type(), session);
  //发送kafka
  netflash_kafka_->SetNetflashOrder(netflash_trades_order);
  //更新用户余额，更新明星时间
  //double totlePrice = amount * price;
  netflash_db_->OnUpdateNetflashsaleResult(netflash_trades_order.order_id(), COMPLETE_ORDER);

  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* ret = new base_logic::FundamentalValue(1);
  dic->Set("result", ret);
  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_CONFIRM_FLASH_ORDER, NETFLASH_TYPE, 0, session, 0);
  packet_control.body_ = dic;
  send_message(socket, &packet_control);

}

void NetflashManager::SetNetflashOrder(const int64 uid,const std::string& symbol,const int64 amount,const double price,
                              star_logic::TradesOrder& netflash_trades_order) {
    
  netflash_trades_order.create_order_id();
  netflash_trades_order.set_buy_position_id(-1);
  netflash_trades_order.set_sell_position_id(-1);
  netflash_trades_order.set_buy_uid(uid);
  netflash_trades_order.set_sell_uid(-1);
  netflash_trades_order.set_amount(amount);
  netflash_trades_order.set_symbol(symbol);
  netflash_trades_order.set_open_position_time(time(NULL));
  netflash_trades_order.set_close_position_time(0);
  netflash_trades_order.set_open_price(price);
  netflash_trades_order.set_match_type();
  //netflash_trades_order.set_handle_type(COMPLETE_ORDER);

  //user_order_map_[uid] = netflash_trades_order;
}

/*void NetflashManager::SendNoiceMessage(const int64 uid, const int64 order_id,const int32 result, const int64 session) {
  star_logic::UserInfo user;
  manager_schduler_engine_->GetUserInfoSchduler(uid, &user);
  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::FundamentalValue* t_orderid = new base_logic::FundamentalValue(order_id);
  base_logic::FundamentalValue* t_result = new base_logic::FundamentalValue(result);
  
  dic->Set("orderId", t_orderid);
  dic->Set("result", t_result);

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_ORDER_RESULT, NETFLASH_TYPE, 0, session, 0);
  packet_control.body_ = dic;
  send_message(user.socket_fd(), &packet_control);
}*/

bool NetflashManager::Getstarexperience(const int socket, const int64 session, const int32 reserved,
            const std::string& symbol) {
  //STAR_EXPER_MAP::iterator it = star_experience_map_.find(symbol);
  //if(it == star_experience_map_.end()){
  //  send_error(socket, ERROR_TYPE, NO_GET_STAR_EXPERIENCE, session);
  //  return false;
  //}
  //base_logic::DictionaryValue ret_list = it->second;

  base_logic::DictionaryValue ret_list;
  if(!netflash_db_->OnGetStarExperience(symbol, ret_list)){
    send_error(socket, ERROR_TYPE, NO_GET_STAR_EXPERIENCE, session);
    return false;
  }

  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_STAR_EXPR_INFO, NETFLASH_TYPE, 0, session, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

}


