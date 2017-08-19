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

enum AskType{
  at_text = 0, //文字
  at_voice = 1, //语音
  at_video = 2  //视频
};
enum PriType{
  pt_private = 0, //私有
  pt_pub = 1    //公开
};
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
    if(!m_approveList.empty())
        m_approveList.pop_back();
  }

  void DelComment(){
    if(!m_commentList.empty())
        m_commentList.pop_back();
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
//_____________________

//用户问答
class UserQustions {
 public:
  UserQustions();
  UserQustions(const UserQustions& userqustions);

  UserQustions& operator =(const UserQustions& userqustions);

  ~UserQustions() {
    if (data_ != NULL) {
      data_->Release();
    }
  }
/*
  static bool close_after(const UserQustions& t_trades_position, const UserQustions& r_trades_position){
    return Data::close_after(t_trades_position.data_, r_trades_position.data_);
  }
  */

  void ValueSerialization(base_logic::DictionaryValue* dict);
  bool ValueSeriForUserAsk(base_logic::DictionaryValue* dict); 

  const int64 id() const {
    return data_->id_;
  }
  const int64 uid() const {
    return data_->uid_;
  }
  const std::string& starcode() const {
    return data_->starcode_;
  }
  const int64 ask_t() const {
    return data_->ask_t_;
  }
  const int64 answer_t() const {
    return data_->answer_t_;
  }

  const int32 s_total() const {
    return data_->s_total_;
  }
  const int32 a_type() const {
    return data_->a_type_;
  }
  const int32 p_type() const {
    return data_->p_type_;
  }
  const int32 c_type() const {
    return data_->c_type_;
  }
  const std::string& uask() const {
    return data_->uask_;
  }
  const std::string& sanswer() const {
    return data_->sanswer_;
  }
  const std::string& video_url() const {
    return data_->video_url_;
  }
  /*
  void set_starcode(const std::string &type) {
    data_->starcode_ = type;
  }
  */
  void set_id(const int64 id) {
    data_->id_ = id;
  }
  void set_answer_t(const int64 value) {
    data_->answer_t_= value;
  }
  
  void set_p_type(const int32 value) {
    if (data_->p_type_ != 0)
      data_->p_type_ = value;
      
  }
  void set_sanswer(const std::string &value){
    data_->sanswer_ = value;
  }

 private:
  class Data {
   public:
    Data()
        : refcount_(1),
          id_(0),
          uid_(0),
          ask_t_(0),
          answer_t_(0),
          s_total_(0),
          a_type_(0),
          p_type_(-1),
          c_type_(0)
          {
    }
/*
    static bool close_after(const Data* t_data,
                      const Data* r_data) {
      return t_data->close_position_time_ > r_data->close_position_time_;
    }
    */

   public:
    int64 id_;            //数据库自增id
    int64 uid_;            //
    std::string starcode_;            //

    int64 ask_t_;            //用户提问时间
    int64 answer_t_;            //明星回答时间
    int32 s_total_;            //阅读记录
    int32 a_type_;            //0-文字 1-视频 2-语音
    int32 p_type_;            //0-私有 1-公开
    int32 c_type_;            //消费时间类型, 0-15s 1-30s

    std::string uask_;      //用户提问
    std::string sanswer_;
    std::string video_url_;     //视频url


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
              , m_approveDecTime(NULL)
              , m_commentDecTime(NULL)
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

  void SetApproveDecTime(const int32 i) {
    m_approveDecTime = new base_logic::FundamentalValue(i);
  }

  void SetCommentDecTime(const int32 i) {
    m_commentDecTime = new base_logic::FundamentalValue(i);
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
    if(m_approveDecTime != NULL)
      m_value->Set(L"approve_dec_time", m_approveDecTime);
    if(m_commentDecTime != NULL)
      m_value->Set(L"comment_dec_time", m_commentDecTime);

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
  base_logic::FundamentalValue* m_approveDecTime;
  base_logic::FundamentalValue* m_commentDecTime;
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

typedef CircleListReply AskAnswerList;


class AskAnswer {

public:
  AskAnswer(): id_(NULL)
              , uid_(NULL)
              , starcode_(NULL)
              , ask_t_(NULL)
              , answer_t_(NULL)
              , s_total_(NULL)
              , a_type_(NULL)
              , p_type_(NULL)
              , c_type_(NULL)
              , uask_(NULL)
              , sanswer_(NULL)
              , video_url_(NULL)
              {}

  ~AskAnswer() {
    if (m_value) {delete m_value; m_value = NULL;}
  }


  void set_id(const int64 i) {
    id_ = new base_logic::FundamentalValue(i);
  }
  void set_uid(const int64 i) {
    uid_ = new base_logic::FundamentalValue(i);
  }
  void set_ask_t(const int64 i) {
    ask_t_ = new base_logic::FundamentalValue(i);
  }
  void set_answer_t(const int64 i) {
    answer_t_ = new base_logic::FundamentalValue(i);
  }

  void set_s_total(const int32 i) {
    s_total_ = new base_logic::FundamentalValue(i);
  }

  void set_a_type(const int32 i) {
    a_type_ = new base_logic::FundamentalValue(i);
  }
  void set_p_type(const int32 i) {
    p_type_ = new base_logic::FundamentalValue(i);
  }
  void set_c_type(const int32 i) {
    c_type_ = new base_logic::FundamentalValue(i);
  }

  void set_starcode(const std::string& value) {
    starcode_ = new base_logic::StringValue(value);
  }
  void set_uask(const std::string& value) {
    uask_ = new base_logic::StringValue(value);
  }
  void set_sanswer(const std::string& value) {
    sanswer_ = new base_logic::StringValue(value);
  }
  void set_video_url(const std::string& value) {
    video_url_ = new base_logic::StringValue(value);
  }

  base_logic::DictionaryValue* get(){
    m_value = new base_logic::DictionaryValue();
    if(id_ != NULL)
      m_value->Set(L"id", id_);
    if(uid_ != NULL)
      m_value->Set(L"uid", uid_);
    if(starcode_ != NULL)
      m_value->Set(L"starcode", starcode_);
    if(ask_t_!= NULL)
      m_value->Set(L"ask_t", ask_t_);
    if(answer_t_!= NULL)
      m_value->Set(L"answer_t", answer_t_);
    if(s_total_!= NULL)
      m_value->Set(L"s_total", s_total_);
    if(a_type_!= NULL)
      m_value->Set(L"a_type", a_type_);
    if(p_type_ != NULL)
      m_value->Set(L"p_type", p_type_);
    if(c_type_ != NULL)
      m_value->Set(L"c_type", c_type_);
    if(uask_ != NULL)
      m_value->Set(L"uask", uask_);
    if(sanswer_ != NULL)
      m_value->Set(L"sanswer", sanswer_);
    if(video_url_ != NULL)
      m_value->Set(L"video_url", video_url_);

    return m_value;
  }

  
private:
  base_logic::FundamentalValue* id_;
  base_logic::FundamentalValue* uid_;
  base_logic::FundamentalValue* ask_t_;
  base_logic::FundamentalValue* answer_t_;

  base_logic::FundamentalValue* s_total_;
  base_logic::FundamentalValue* a_type_;
  base_logic::FundamentalValue* p_type_;
  base_logic::FundamentalValue* c_type_;

  base_logic::StringValue* starcode_;
  base_logic::StringValue* uask_;
  base_logic::StringValue* sanswer_;
  base_logic::StringValue* video_url_;

  base_logic::DictionaryValue* m_value;

};
}  // namespace circle_reply

#endif /* SWP_STAR_CIRCLE_INFOS_H_ */
