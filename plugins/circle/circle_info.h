//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef SWP_STAR_CIRCLE_INFOS_H_
#define SWP_STAR_CIRCLE_INFOS_H_

#include <string>
#include <map>
#include <list>
#include "basic/basictypes.h"
#include "logic/base_values.h"

namespace circle_logic {

enum CircleStatus {
  CIRCLE_NORMAL_STATUS = 0,
  CIRCLE_DEL_STATUS = 1
};

struct Scomment
{
  Scomment(): uid(0)
            , direction(0)
            , content("")
            , priority(0){

            }
  int64 uid;
  int32 direction;//0:用户发表评论 1:明星回复评论 2:用户回复评论
  std::string content;
  int64 priority;
};

typedef std::list<Scomment> COMMENTLIST;

class Circle {

public:
  Circle();
  Circle(const Circle& circleid);

  Circle& operator =(const Circle& circleid);

  ~Circle() {
  if (data_ != NULL)
    data_->Release();
  }

  void ValueSerialization(base_logic::DictionaryValue* dict);
  void SetSymbol(const std::string& symbol) {
    data_->m_symbol = symbol;
  }

  void SetCircleId(const int64 circleId) {
    data_->m_circleId = circleId;
  }

  void SetCreateTime(const int32 createTime) {
    data_->m_createTime = createTime;
  }

  void SetContent(const std::string& content) {
    data_->m_content = content;
  }

  void SetPicUrl(const std::string& picUrl) {
    data_->m_picUrl = picUrl;
  }

  void SetStatus(const int32 status) {
    data_->m_status = status;
  }

  void AddApproveId(const int64 uid) {
    m_approveList.push_back(uid);
  }

  void AddComment(const Scomment& sc) {
    m_commentList.push_back(sc);
    data_->m_lastPriority = sc.priority;
  }

  void DelApprovId(){
    m_approveList.pop_back();
  }

  void DelComment(){
    return m_commentList.pop_back();
  }


  const std::string& GetSymbol() const {
    return data_->m_symbol;
  }

  const int64  GetCircleId() const {
    return data_->m_circleId;
  }

  const int32 GetCreateTime() const {
    return data_->m_createTime;
  }

  const std::string& GetContent() const {
    return data_->m_content;
  }

  const std::string& GetPicUrl() const {
    return data_->m_picUrl;
  }

  const int32 GetStatus() const {
    return data_->m_status;
  }

  const int64 GetLastPriority() const {
    return data_->m_lastPriority;
  }

  base_logic::ListValue* GetApproveList();//获取点赞列表

  base_logic::ListValue* GetCommentList();//获取评论列表

//  base_logic::DictionaryValue* get();

  
private:
  class Data {
  public:
    Data() : refcount_(1) {
    }

    std::string  m_symbol;
    int64 m_circleId;//唯一id
    int32 m_createTime;
    std::string m_content;
    std::string m_picUrl;
    int32 m_status;//0-正常 1-删除
    int64 m_lastPriority;//当前最大的优先级

    void AddRef() {
      __sync_fetch_and_add(&refcount_, 1);
    }

    void Release() {
      __sync_fetch_and_sub(&refcount_, 1);
      if (!refcount_)
        delete this;
    }
    private:
      int refcount_;
  };

  Data* data_;

public:
  std::list<int64> m_approveList;//点赞列表
  COMMENTLIST m_commentList;//评论列表
};

}  // namespace circle_logic


namespace circle_reply {

class CircleReply {

public:
  CircleReply(): m_symbol(NULL)
              , m_name(NULL)
              , m_headUrl(NULL)
              , m_circleId(NULL)
              , m_createTime(NULL)
              , m_content(NULL)
              , m_picUrl(NULL)
              , m_approveList(NULL)
              , m_commentList(NULL)
              {}

  ~CircleReply() {
    if (m_value) {delete m_value; m_value = NULL;}
  }

  void SetName(const std::string& name) {
    m_name = new base_logic::StringValue(name);
  }

  void SetHeadUrl(const std::string& picurl) {
    m_headUrl = new base_logic::StringValue(picurl);
  }

  void SetApproveList(base_logic::ListValue* dic) {
    m_approveList = dic;
  }

  void SetCommentList(base_logic::ListValue* dic) {
    m_commentList = dic;
  }

  void SetCircle(circle_logic::Circle*& circle) {
    m_symbol = new base_logic::StringValue(circle->GetSymbol());
    m_circleId = new base_logic::FundamentalValue(circle->GetCircleId());
    m_createTime = new base_logic::FundamentalValue(circle->GetCreateTime());
    m_content = new base_logic::StringValue(circle->GetContent());
    m_picUrl = new base_logic::StringValue(circle->GetPicUrl());
  }

  base_logic::DictionaryValue* get(){
    m_value = new base_logic::DictionaryValue();
    if(m_symbol != NULL)
      m_value->Set(L"symbol", m_symbol);
    if(m_circleId != NULL)
      m_value->Set(L"circle_id", m_circleId);
    if(m_createTime != NULL)
      m_value->Set(L"create_time", m_createTime);
    if(m_content != NULL)
      m_value->Set(L"content", m_content);
    if(m_picUrl != NULL)
      m_value->Set(L"pic_url", m_picUrl);
    if(m_approveList != NULL)
      m_value->Set(L"approve_list", m_approveList);
    if(m_commentList != NULL)
      m_value->Set(L"comment_list", m_commentList);
    if(m_name != NULL)
      m_value->Set(L"symbol_name", m_name);
    if(m_headUrl != NULL)
      m_value->Set(L"head_url", m_headUrl);

    return m_value;
  }

  
private:
  base_logic::StringValue* m_symbol;
  base_logic::StringValue* m_name;
  base_logic::StringValue* m_headUrl;
  base_logic::FundamentalValue* m_circleId;
  base_logic::FundamentalValue* m_createTime;
  base_logic::StringValue* m_content;
  base_logic::StringValue* m_picUrl;
  base_logic::ListValue* m_approveList;
  base_logic::ListValue* m_commentList;
  base_logic::DictionaryValue* m_value;

};


class CircleListReply {
 public:
  CircleListReply()
      : m_circleList(NULL),
        value_(NULL) {
    m_circleList = new base_logic::ListValue;
  }

  ~CircleListReply() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
  }

  void set_unit(base_logic::DictionaryValue* value) {
    m_circleList->Append((base_logic::Value*) (value));
  }

  base_logic::DictionaryValue* get() {
    value_ =  new base_logic::DictionaryValue();
    if (!m_circleList->empty()) {
      value_->Set(L"circle_list", m_circleList);
      return value_;
    } else {
      delete m_circleList;
      m_circleList = NULL;
    }

  }

  void Reset() {
    if (value_) {
      delete value_;
      value_ = NULL;
    }
    
    m_circleList = new base_logic::ListValue;
  }
  const int32 Size() {
    return m_circleList->GetSize();
  }

 private:
  base_logic::ListValue* m_circleList;
  base_logic::DictionaryValue* value_;
};


}  // namespace circle_reply

#endif /* SWP_STAR_CIRCLE_INFOS_H_ */
