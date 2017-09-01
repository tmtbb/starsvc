//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "circle_logic.h"
#include <string>
#include "basic/native_library.h"
#include "core/common.h"
#include "config/config.h"
#include "net/errno.h"
#include "comm/comm_head.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "schduler_engine.h"
#include "circle_info.h"
#include "errno.h"
#include "operator_code.h"
#include "qiniu/rs.h"


#define DEFAULT_CONFIG_PATH "./plugins/circle/circle_config.xml"

#define TIME_DISTRIBUTION_TASK 15000
#define TIME_FETCHKAFK_TASK 15100

namespace circle_logic
{

Circlelogic *Circlelogic::instance_ = NULL;

Circlelogic::Circlelogic()
{
  if (!Init())
      assert(0);
}

Circlelogic::~Circlelogic()
{
  if (circle_db_)
  {
    delete circle_db_;
    circle_db_ = NULL;
  }

  circle_logic::CircleEngine::FreeSchdulerManager();
  circle_logic::CircleEngine::FreeCircleEngine();
}


bool Circlelogic::Init()
{
  bool r = false;
  manager_schduler::SchdulerEngine* (*schduler_engine)(void);
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
      return false;
  r = config->LoadConfig(path);
  LOG_MSG2("path : %s", path.c_str());
  circle_logic::CircleEngine::GetSchdulerManager();
  circle_db_ = new circle_logic::CircleDB(config);
  circle_logic::CircleEngine::GetSchdulerManager()->InitDB(circle_db_);
  circle_logic::CircleEngine::GetSchdulerManager()->InitData();
  circle_logic::CircleEngine::GetSchdulerManager()->InitUserAskAnswerData();
#ifndef STAR_SIDE //用户端使用
  circle_logic::CircleEngine::GetSchdulerManager()->InitUserSeeAskInfo();
#endif
  base::SysRadom::GetInstance();
  std::string schduler_library = "./data_share.so";
  std::string schduler_func = "GetManagerSchdulerEngine";
  schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                      logic::SomeUtils::GetLibraryFunction(schduler_library, 
                      schduler_func);

  manager_schduler_engine_= (*schduler_engine)();
  if (manager_schduler_engine_ == NULL)
      assert(0);
  circle_logic::CircleEngine::GetSchdulerManager()->InitManagerSchduler(manager_schduler_engine_);
//kafka
  kafka_ = new circle_logic::Kafka(config);
  return true;
}

Circlelogic *Circlelogic::GetInstance()
{
  if (instance_ == NULL)
    instance_ = new Circlelogic();
  return instance_;
}

void Circlelogic::FreeInstance()
{
  delete instance_;
  instance_ = NULL;
}

bool Circlelogic::OnCircleConnect(struct server *srv, const int socket)
{
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Circlelogic::OnCircleMessage(struct server *srv, const int socket,
                                  const void *msg, const int len)
{
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
      return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet))
  {
      LOG_ERROR2("UnpackStream Error socket %d", socket);
      return false;
  }

  switch (packet->operate_code)
  {
  case R_GET_ALL_CIRCLE_INFO:{
    OnGetAllCircleInfo(srv, socket, packet);
    break;
  }
  case R_GET_STAR_CIRCLE_INFO:{
    OnGetStarCircleInfo(srv, socket, packet);
    break;
  }
  case R_STAR_ADD_CIRCLE:{
    OnAddCircleInfo(srv, socket, packet);
    break;
  }
  case R_STAR_DEL_CIRCLE:{
    OnDelCircleInfo(srv, socket, packet);
    break;
  }
  case R_USER_APPROVE_CIRCLE:{
    OnUserApproveCircle(srv, socket, packet);
    break;
  }
  case R_USER_COMMENT_CIRCLE:{
    OnUserCommentCircle(srv, socket, packet);
    break;
  }
  case R_STAR_REPLY_COMMENT:{
    OnStarReplyCircle(srv, socket, packet);
    break;
  }
  case R_GET_USER_ASK:{ //获取用户问答信息
    OnGetUserAsk(srv, socket, packet);
    break;
  }
  case R_GET_STAR_USER_ASK:{ //获取明星的用户问答信息
    OnGetStarUserAsk(srv, socket, packet);
    break;
  }
  case R_USER_ASK:{ //用户提问
    OnUserAsk(srv, socket, packet);
    break;
  }
  case R_STAR_ANSWER:{ //明星回答
    OnStarAnswer(srv, socket, packet);
    break;
  }
  case R_USER_SEE_VOICE :{ //用户查看语音
    break;
  }
  case R_USER_SEE_VIDEO:{ //用户查看视频
    OnSeeQuestion(srv, socket, packet);
    break;
  }
  case R_GET_ALLSTAR_USER_ASK_HOT : {
    OnGetAllStarUserAskHot(srv, socket, packet);
    break;
  }
  case R_GET_QINIU_TOKEN:{
    OnGetQiniuToken(srv, socket, packet);
    break;
  }
  case R_TEST_TEST : {
    FetchTaskForKafka();
    break;
  }
  default:
      break;
  }
  if(packet){
      delete packet;
      packet = NULL;
  }
  return true;
}

bool Circlelogic::OnCircleClose(struct server *srv, const int socket)
{
  return true;
}

bool Circlelogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  return true;
}

bool Circlelogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
      return false;

/*  if (!net::PacketProsess::UnpackStream(msg, len, &packet))
  {
      LOG_ERROR2("UnpackStream Error socket %d", socket);
      return false;
  }*/

  return true;
}


bool Circlelogic::OnBroadcastClose(struct server *srv, const int socket)
{
  return true;
}

bool Circlelogic::OnIniTimer(struct server *srv)
{
  if (srv->add_time_task != NULL)
  {
      srv->add_time_task(srv, "circle", TIME_DISTRIBUTION_TASK, 5, -1);
      srv->add_time_task(srv, "circle", TIME_FETCHKAFK_TASK, 3, -1);
  }
  return true;
}

bool Circlelogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
  switch (opcode)
  {
    case TIME_DISTRIBUTION_TASK: {
      circle_logic::CircleEngine::GetSchdulerManager()->InitData();
      break;
    }
    case TIME_FETCHKAFK_TASK: {
      FetchTaskForKafka();
      break;
    }
    default:
      break;
  }
  return true;
}

bool Circlelogic::OnGetQiniuToken(struct server* srv, int socket, 
                            struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string token;
  int64 uid;
  bool r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r2 = packet_control->body_->GetString(L"token", &token);
  if (!r1 || !r2){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  const char accessKey[] = "4jvwuLa_Xcux7WQ40KMO89DfinEuI3zXizMpwnc7";
  const char secretKey[] = "8tSk8O9VS0vl9zh8jUV1mkR1GijH2KyXMLbVel_T";
  const char bucket[] = "starshareimage";
  Qiniu_Mac mac;
  mac.accessKey = accessKey;
  mac.secretKey = secretKey;

  Qiniu_RS_PutPolicy putPolicy;
  char *uptoken = NULL;

  //简单上传凭证
  Qiniu_Zero(putPolicy);
  putPolicy.scope = bucket;
  uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);
  LOG_DEBUG2("simple:\t%s\n\n", uptoken);
  std::string t_str(uptoken);
  Qiniu_Free(uptoken);

  //自定义凭证有效期（示例1小时）
  //Qiniu_Zero(putPolicy);
  //putPolicy.scope = bucket;
  //putPolicy.expires = 3600; //单位秒
  //uptoken = Qiniu_RS_PutPolicy_Token(&putPolicy, &mac);
  //LOG_DEBUG2("deadline:\t%s\n\n", uptoken);
  //std::string t_str(uptoken);
  //Qiniu_Free(uptoken);
  
  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  base_logic::StringValue* ret = new base_logic::StringValue(t_str);
  dic->Set("uptoken", ret);
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_QINIU_TOKEN, CIRCLE_TYPE, 0, packet->session_id, packet->reserved);
  packet_reply.body_ = dic;
  send_message(socket, &packet_reply);

  return true;
}

bool Circlelogic::OnGetAllCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count;
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  if (!r1 || !r2){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetAllCircle(socket,
      packet->session_id, packet->reserved, pos, count);
  return true;
}

bool Circlelogic::OnGetStarCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count;
  std::string starcode;
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  bool r3 = packet_control->body_->GetString(L"star_code", &starcode);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetSymbolAllCircle(socket,
      packet->session_id, packet->reserved, starcode, pos, count);
  return true;
}

bool Circlelogic::OnAddCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode,content,picurl;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetString(L"content", &content);
  bool r3 = packet_control->body_->GetString(L"picurl", &picurl);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->CreateCircle(socket,
      packet->session_id, packet->reserved, starcode, content, picurl);
  return true;
}

bool Circlelogic::OnDelCircleInfo(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode;
  int64 circleid;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  if (!r1 || !r2){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->DeleteCircle(socket,
      packet->session_id, packet->reserved, starcode, circleid);
  return true;
}


bool Circlelogic::OnUserApproveCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode;
  int64 circleid,uid;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  if (!r1 || !r2 || !r3){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->ApprovalCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid);
  return true;
}

bool Circlelogic::OnUserCommentCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode, content;
  int64 circleid, uid, direction;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r4 = packet_control->body_->GetBigInteger(L"direction", &direction);
  bool r5 = packet_control->body_->GetString(L"content", &content);
  if (!r1 || !r2 || !r3 || !r4 || !r5){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->UserCommentCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid, direction, content);
  return true;
}


bool Circlelogic::OnStarReplyCircle(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  std::string starcode, content;
  int64 circleid, uid, direction;
  bool r1 = packet_control->body_->GetString(L"star_code", &starcode);
  bool r2 = packet_control->body_->GetBigInteger(L"circle_id", &circleid);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r4 = packet_control->body_->GetBigInteger(L"direction", &direction);
  bool r5 = packet_control->body_->GetString(L"content", &content);
  if (!r1 || !r2 || !r3 || !r4 || !r5){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->StarReplyCircle(socket,
      packet->session_id, packet->reserved, uid, starcode, circleid, direction, content);
  return true;
}
//

bool Circlelogic::OnGetStarUserAsk(struct server* srv, int socket, struct PacketHead* packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count , aType, pType, uid;
  std::string starcode = "";
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  bool r3 = packet_control->body_->GetString(L"starcode", &starcode);
  bool r4 = packet_control->body_->GetBigInteger(L"aType", &aType); //
  bool r5 = packet_control->body_->GetBigInteger(L"pType", &pType);
  bool r6 = packet_control->body_->GetBigInteger(L"uid", &uid);
  if (!r1 || !r2 || !r3 || !r4 || !r5 || !r6){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetStarUserAsk(socket,
      packet->session_id, packet->reserved, starcode, pos, count, aType, pType, uid);
  return true;
}

bool Circlelogic::OnGetUserAsk(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 pos, count, uid = 0, aType = -1, pType = -1;
  std::string starcode;
  bool r1 = packet_control->body_->GetBigInteger(L"pos", &pos);
  bool r2 = packet_control->body_->GetBigInteger(L"count", &count);
  bool r3 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r4 = packet_control->body_->GetBigInteger(L"aType", &aType); //
  bool r5 = packet_control->body_->GetBigInteger(L"pType", &pType);
  bool r6 = packet_control->body_->GetString(L"starcode", &starcode);
  if (!r1 || !r2 || !r3 || !r4 || !r5 || !r6){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  circle_logic::CircleEngine::GetSchdulerManager()->GetUserAsk(socket,
      packet->session_id, packet->reserved, uid, pos, count, aType, pType, starcode);
  return true;
}

bool Circlelogic::OnGetAllStarUserAskHot(struct server* srv, int socket, struct PacketHead* packet) {
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 type;
  bool r1 = packet_control->body_->GetBigInteger(L"type", &type);
  if (!r1){
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  int64 pos = 0;
  int64 count = 20;
  circle_logic::CircleEngine::GetSchdulerManager()->GetAllStarUserAskHot(socket,
      packet->session_id, packet->reserved, type, pos, count);
  return true;
}

bool Circlelogic::OnUserAsk(struct server* srv, int socket, struct PacketHead* packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 uid = 0;
  int32 a_type,c_type, p_type;
  a_type, c_type ,p_type = -1;

  std::string starcode, uask, video_url;

  circle_logic::UserQustions item;
  bool flag = item.ValueSeriForUserAsk(packet_control->body_);
  /*
  bool r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r2 = packet_control->body_->GetInteger(L"a_type", &a_type);
  bool r3 = packet_control->body_->GetInteger(L"c_type", &c_type);
  bool r4 = packet_control->body_->GetInteger(L"p_type", &p_type);
  bool r5 = packet_control->body_->GetString(L"starcode", &starcode);
  bool r6 = packet_control->body_->GetString(L"uask", &uask);
  bool r7 = packet_control->body_->GetString(L"video_url", &video_url);
  if (!r1 || !r2 || !r3 || !r4 || !r5 || !r6 || !r7){
  */
  if (flag)
  {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  int64 askt = 0;
  int64 id = circle_db_->OnUserAsk(item.uid(), item.a_type(), 
    item.p_type(), item.c_type(), item.starcode(),
            item.uask(), item.video_url(), askt);
  int64 result = 0; //0提问成功,失败
  if (id != -1){//sucess 更新内存
    item.set_id(id);
    item.set_ask_t(askt);
    circle_logic::CircleEngine::GetSchdulerManager()->UpdateUserAsk(item);
    //add kafka
    
    base_logic::DictionaryValue* task_info = new base_logic::DictionaryValue();
    task_info->Set(L"type", base_logic::Value::CreateIntegerValue(circle_logic::UserAsk)); //
    task_info->Set(L"qid", base_logic::Value::CreateIntegerValue(id));
    task_info->Set(L"uid", base_logic::Value::CreateIntegerValue(item.uid()));
    task_info->Set(L"aType", base_logic::Value::CreateIntegerValue(item.a_type()));
    task_info->Set(L"pType", base_logic::Value::CreateIntegerValue(item.p_type()));
    task_info->Set(L"cType", base_logic::Value::CreateIntegerValue(item.c_type()));
    task_info->Set(L"starcode", base_logic::Value::CreateStringValue(item.starcode()));
    task_info->Set(L"uask", base_logic::Value::CreateStringValue(item.uask()));
    task_info->Set(L"videoUrl", base_logic::Value::CreateStringValue(item.video_url()));
    task_info->Set(L"askT", base_logic::Value::CreateIntegerValue(item.ask_t()));
    kafka_->AddStorageInfo(task_info);
    delete task_info;
    
  }
  else {
    result = 1;
  }
  struct PacketControl packet_control_ack;
  MAKE_HEAD(packet_control_ack,S_USER_ASK , 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic;
  dic.SetBigInteger(L"result", result);
  packet_control_ack.body_ = &dic;
  send_message(socket, &packet_control_ack);

  return true;
}
void Circlelogic::FetchTaskForKafka()
{
    std::vector<base_logic::DictionaryValue*> vec;
    kafka_->FectchBatchQuestionTask(vec);
    int64 type = -1;
    int64 uid, qid;
    std::string starcode;
    for (int i = 0; i < vec.size(); i ++)
    {
        //
        type = -1;
        vec[i]->GetBigInteger(L"type", &type);
        if (type == circle_logic::UserSeeQuestion) //用户偷看
        {
            vec[i]->GetString(L"starcode", &starcode);
            vec[i]->GetBigInteger(L"qid", &qid);
            vec[i]->GetBigInteger(L"uid", &uid);
            circle_logic::CircleEngine::GetSchdulerManager()->UpdateUserSeeMap(uid,qid, starcode);

        }
        else if (type == circle_logic::StarAnswer)
        {
            std::string sanswer;
            int64 answer_t, p_type;
            vec[i]->GetString(L"starcode", &starcode);
            vec[i]->GetString(L"sanswer", &sanswer);
            vec[i]->GetBigInteger(L"qid", &qid);
            vec[i]->GetBigInteger(L"uid", &uid);
            vec[i]->GetBigInteger(L"answert", &answer_t);
            vec[i]->GetBigInteger(L"pType", &p_type);
            circle_logic::CircleEngine::GetSchdulerManager()->UpdateStarAnswer(qid, p_type, answer_t, sanswer, uid, starcode);
        }
        else if (type == circle_logic::UserAsk)
        {
            vec[i]->GetBigInteger(L"qid", &qid);
            int64 askt;
            vec[i]->GetBigInteger(L"askT", &askt);
            circle_logic::UserQustions item;
            bool flag = item.ValueSeriForUserAsk(vec[i]);
            item.set_id(qid);
            item.set_ask_t(askt);
            circle_logic::CircleEngine::GetSchdulerManager()->UpdateUserAsk(item);
        }
        //
        delete vec[i];
    }
}

bool Circlelogic::OnStarAnswer(struct server* srv, int socket, struct PacketHead* packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 id = 0, uid = 0;
  int32 p_type;
  int64 tmp;
  p_type = -1;

  std::string sanswer, starcode;

  bool r1 = packet_control->body_->GetBigInteger(L"id", &id);
  bool r2 = packet_control->body_->GetString(L"sanswer", &sanswer);
  bool r3 = packet_control->body_->GetBigInteger(L"pType", &tmp);
  p_type = tmp;
  if (!r1 || !r2 || !r3)
  {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  int64 answer_t = circle_db_->OnStarAnswer(id, p_type, sanswer, &uid, starcode);
  int32 result = 0; //0 回答成功,失败
  if (answer_t != -1){//sucess 更新内存
    circle_logic::CircleEngine::GetSchdulerManager()->UpdateStarAnswer(id, p_type, answer_t, sanswer, uid, starcode);
    //add kafka
    
    base_logic::DictionaryValue* task_info = new base_logic::DictionaryValue();
    task_info->Set(L"type", base_logic::Value::CreateIntegerValue(circle_logic::StarAnswer)); //
    task_info->Set(L"starcode", base_logic::Value::CreateStringValue(starcode));
    task_info->Set(L"sanswer", base_logic::Value::CreateStringValue(sanswer));
    task_info->Set(L"answert", base_logic::Value::CreateIntegerValue(answer_t));
    task_info->Set(L"qid", base_logic::Value::CreateIntegerValue(id));
    task_info->Set(L"uid", base_logic::Value::CreateIntegerValue(uid));
    task_info->Set(L"pType", base_logic::Value::CreateIntegerValue(p_type)); //
    kafka_->AddStorageInfo(task_info);
    delete task_info;
    
  }
  else {
    result = 1;
  }
  struct PacketControl packet_control_ack;
  MAKE_HEAD(packet_control_ack,S_STAR_ANSWER, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic;
  dic.SetInteger(L"result", result);
  packet_control_ack.body_ = &dic;
  send_message(socket, &packet_control_ack);

  return true;
}
//user see question
bool Circlelogic::OnSeeQuestion(struct server* srv, int socket, struct PacketHead* packet) {

  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  int64 uid = 0, qid = 0;
  int32 cType;
  int64 tmp;
  cType = -1;

  std::string starcode;

  bool r1 = packet_control->body_->GetBigInteger(L"uid", &uid);
  bool r2 = packet_control->body_->GetBigInteger(L"qid", &qid);
  bool r3 = packet_control->body_->GetString(L"starcode", &starcode);
  bool r4 = packet_control->body_->GetBigInteger(L"cType", &tmp); //消费档次 0-15s, 1-30s, 2-45s
  cType = tmp;
  if (!r1 || !r2 || !r3 || !r4)
  {
      send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
      return false;
  }

  int64 ret = circle_db_->OnSeeQuestion(uid, qid, cType, starcode);
  int32 result = 0; //0 偷听成功,失败
  if (ret != -1){//sucess 更新内存
    circle_logic::CircleEngine::GetSchdulerManager()->UpdateUserSeeMap(uid,qid, starcode);
    //add kafka 通过kafk更新明星端数据
    
    base_logic::DictionaryValue* task_info = new base_logic::DictionaryValue();
    task_info->Set(L"type", base_logic::Value::CreateIntegerValue(circle_logic::UserSeeQuestion)); //
    task_info->Set(L"starcode", base_logic::Value::CreateStringValue(starcode));
    task_info->Set(L"uid", base_logic::Value::CreateIntegerValue(uid));
    task_info->Set(L"qid", base_logic::Value::CreateIntegerValue(qid));
    kafka_->AddStorageInfo(task_info);
    delete task_info;
    
  }
  else {
    result = 1;
  }
  struct PacketControl packet_control_ack;
  MAKE_HEAD(packet_control_ack,S_USER_SEE_VIDEO, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic;
  dic.SetInteger(L"result", result);
  packet_control_ack.body_ = &dic;
  send_message(socket, &packet_control_ack);

  return true;
}
}  // namespace circle_logic

