//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#include "basic/template.h"
#include "logic/logic_comm.h"
#include "circle_info.h"

namespace circle_logic {


Circle::Circle() {
  data_ = new Data();
  data_->m_status = 0;
  data_->m_lastPriority = 0;
}

Circle::Circle(const Circle& circle)
  : data_(circle.data_) {
  if (data_ != NULL) {
    data_->AddRef();
  }
}

Circle& Circle::operator =(const Circle& circle) {
  if (circle.data_ != NULL) {
    circle.data_->AddRef();
  }

  if (data_ != NULL) {
    data_->Release();
  }

  data_ = circle.data_;
  return (*this); 
}

void Circle::ValueSerialization(base_logic::DictionaryValue* dict) {
  dict->GetString(L"star_code", &data_->m_symbol);
  dict->GetBigInteger(L"circle_id", &data_->m_circleId);
  dict->GetInteger(L"create_time", &data_->m_createTime);
  dict->GetString(L"content", &data_->m_content);
  dict->GetString(L"pic_url", &data_->m_picUrl);
  
  
  //反序列化 string->Value
  m_approveList.clear();
  m_commentList.clear();
  std::string t_approvelist,t_commentlist;
  dict->GetString(L"approval_list", &t_approvelist);
  dict->GetString(L"comment_list", &t_commentlist);
  LOG_DEBUG2("t_approvelist [%s],",t_approvelist.c_str());
  
  bool r = false;
  int error_code = 0;
  std::string error_str;
  do{
    if(t_approvelist.empty()){
      break;
    }

    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
          0, &t_approvelist);
    
    base_logic::Value *value = engine->Deserialize(&error_code, &error_str);
    if (0 != error_code || NULL == value)
        break;
    base_logic::ListValue *list = (base_logic::ListValue *) value;
    while (list->GetSize()) {
      base_logic::Value *result_value;
      list->Remove(0, &result_value);
      base_logic::DictionaryValue *dict_result_value =
          (base_logic::DictionaryValue *) (result_value);
      
      int64 uid;
      r = dict_result_value->GetBigInteger(L"uid", &uid);
      if(r){
        m_approveList.push_back(uid);
      }

      delete dict_result_value;
      dict_result_value = NULL;
    }
  }while(0);
  
  do{
    if(t_commentlist.empty()){
      break;
    }

    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
          0, &t_commentlist);
    
    base_logic::Value *value = engine->Deserialize(&error_code, &error_str);
    if (0 != error_code || NULL == value)
        break;
    base_logic::ListValue *list = (base_logic::ListValue *) value;
    while (list->GetSize()) {
      base_logic::Value *result_value;
      list->Remove(0, &result_value);
      base_logic::DictionaryValue *dict_result_value =
          (base_logic::DictionaryValue *) (result_value);
      
      //{"uid":123,"direction":0,"content":"abc123","priority":0}
      int64 uid,priority;
      int32 direction;
      std::string content;
      Scomment scomment;
      r = dict_result_value->GetBigInteger(L"uid", &uid);
      if(r){
        scomment.uid = uid;
      }
      r = dict_result_value->GetInteger(L"direction", &direction);
      if(r){
        scomment.direction = direction;
      }
      r = dict_result_value->GetString(L"content", &content);
      if(r){
        scomment.content = content;
      }
      r = dict_result_value->GetBigInteger(L"priority", &priority);
      if(r){
        scomment.priority = priority;
        if(data_->m_lastPriority < priority)
          data_->m_lastPriority = priority;
      }
      m_commentList.push_back(scomment);

      delete dict_result_value;
      dict_result_value = NULL;
    }
  }while(0);

}

/* return
// { "approve_list": {{"uid":123}{"uid":456}...} }
*/
base_logic::ListValue* Circle::GetApproveList() {
  base_logic::ListValue* approvelist = new base_logic::ListValue();

  std::list<int64>::iterator it = m_approveList.begin();
  for(; it!= m_approveList.end(); ++it){
    base_logic::DictionaryValue* t_dir = new base_logic::DictionaryValue();
    base_logic::FundamentalValue* t_uid = new base_logic::FundamentalValue(*it);
    t_dir->Set(L"uid", t_uid);
    approvelist->Append((base_logic::Value*) (t_dir));
  }

  // return
  return approvelist;
}

/* return
// { "comment_list": {{"uid":123,"direction":0,"content":"abc123","priority":0}
//                     {"uid":456,"direction":1,"content":"321cba","priority":1}... }}
*/
base_logic::ListValue* Circle::GetCommentList() {
  base_logic::ListValue* commentlist = new base_logic::ListValue();
  
  COMMENTLIST::iterator it = m_commentList.begin();
  for(; it!= m_commentList.end(); ++it){
    base_logic::DictionaryValue* t_dir = new base_logic::DictionaryValue();

    base_logic::FundamentalValue* t_uid = new base_logic::FundamentalValue(it->uid);
    t_dir->Set(L"uid", t_uid);
    base_logic::FundamentalValue* t_direction = new base_logic::FundamentalValue(it->direction);
    t_dir->Set(L"direction", t_direction);
    base_logic::StringValue* t_content = new base_logic::StringValue(it->content);
    t_dir->Set(L"content", t_content);
    base_logic::FundamentalValue* t_priority = new base_logic::FundamentalValue(it->priority);
    t_dir->Set(L"priority", t_priority);

    commentlist->Append((base_logic::Value*) (t_dir));
  }

  // return
  return commentlist;
}

/*base_logic::DictionaryValue* Circle::get() {
  base_logic::DictionaryValue* value = new base_logic::DictionaryValue();
  base_logic::StringValue* t_symbol = new base_logic::StringValue(m_symbol);
  base_logic::FundamentalValue* t_circleid = new base_logic::FundamentalValue(m_circleId);
  base_logic::FundamentalValue* t_createtime = new base_logic::FundamentalValue(m_createTime);
  base_logic::StringValue* t_content = new base_logic::StringValue(m_content);
  base_logic::StringValue* t_picurl = new base_logic::StringValue(m_picUrl);
  base_logic::FundamentalValue* t_status = new base_logic::FundamentalValue(m_status);

  value->Set(L"symbol", t_symbol);
  value->Set(L"circle_id", t_circleid);
  value->Set(L"create_time", t_createtime);
  value->Set(L"content", t_content);
  value->Set(L"pic_url", t_picurl);
  value->Set(L"status", t_status);
  value->Set(L"approve_list", GetApproveList());
  value->Set(L"comment_list", GetCommentList());

  return value;
}*/



}  // namespace circle_logic
