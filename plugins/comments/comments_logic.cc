//  Copyright (c) 2015-2015 The star Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "comments/comments_logic.h"
#include "comments/comments_proto_buf.h"
#include "comments/operator_code.h"
#include "comments/errno.h"
#include "comments/schduler_engine.h"
#include "logic/xml_parser.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
//#include "net/comm_head.h"
#include "comm/comm_head.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/comments/comments_config.xml"


namespace comments_logic {

Commentslogic *Commentslogic::instance_ = NULL;

Commentslogic::Commentslogic() {
  if (!Init())
    assert(0);
}

Commentslogic::~Commentslogic() {
}

bool Commentslogic::Init() {
  bool r = false;
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);

  comments_logic::CommentsEngine::GetSchdulerManager();
  comments_redis_ = new comments_logic::CommentsRedis(config);
  comments_logic::CommentsEngine::GetSchdulerManager()->InitRedis(
      comments_redis_);
//初始化数据
  //comments_logic::CommentsEngine::GetSchdulerManager()->InitCommentsData();
  return true;
}

Commentslogic *Commentslogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Commentslogic();
  return instance_;
}

void Commentslogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Commentslogic::OnCommentsConnect(struct server *srv,
                                          const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Commentslogic::OnCommentsMessage(struct server *srv, const int socket,
                                          const void *msg, const int len) {
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }

  if (packet->type == COMMENTS_TYPE) {
    switch (packet->operate_code) {
      case R_COMMENTS_ADD_DATA:
      {
        OnCommentsAddData(srv, socket, packet);
        break;
      }
      case R_COMMENTS_DATA:
      {
        OnCommentsGetData(srv, socket, packet);
        break;
      }

      default:
        break;
    }
  }
  if(packet){
      delete packet;
      packet = NULL;
  }
  return true; /////////////////
}

bool Commentslogic::OnCommentsClose(struct server *srv, const int socket) {
  return true;
}

bool Commentslogic::OnBroadcastConnect(struct server *srv, const int socket,
                                         const void *msg, const int len) {
  //像行情数据中心注册
  /*
  comments_logic::CommentsEngine::GetSchdulerManager()
      ->LoginCommentsCenter(socket);
	  */
  return true;
}

bool Commentslogic::OnBroadcastMessage(struct server *srv, const int socket,
                                         const void *msg, const int len) {
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;

  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    return false;
  }

  switch (packet->operate_code) {
    case 1001: {
      //OnQutations(srv, socket, packet);
      break;
    }
    default:
      break;
  }
  return true;
}

bool Commentslogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Commentslogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
  }
  return true;
}

bool Commentslogic::OnTimeout(struct server *srv, char *id, int opcode,
                                int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
/*
*/
void Commentslogic::Test() {

  base_logic::ListValue* goodsinfos = new base_logic::ListValue;
  base_logic::DictionaryValue* unit = new base_logic::DictionaryValue;
  unit->SetString(L"platformName", "JH");
  unit->SetString(L"exchangeName", "DEFAULT");
  unit->SetString(L"goodType", "AG");
  goodsinfos->Append(unit);

  base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON);
  if (engine == NULL) {
    LOG_ERROR("engine create null");
    return;
  }
  std::string body_stream;
  engine->Serialize((*goodsinfos), &body_stream);

  //comments_logic::CommentsEngine::GetSchdulerManager()->SendRealTime(
   //   13, 0, 0, goodsinfos);
  if (goodsinfos) {
    delete goodsinfos;
    goodsinfos = NULL;
  }
}
//______________________________________

bool Commentslogic::OnCommentsAddData(struct server* srv, int socket,
                                 struct PacketHead *packet) {

  comments_logic::net_request::FansComments fanscomments;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  bool r = fanscomments.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);

    return false;
  }
  LOG_DEBUG2("Comments Add Data[%d] begin.",packet->packet_length);

  r = comments_logic::CommentsEngine::GetSchdulerManager()->CommentsAddData(socket, packet->session_id, fanscomments);

  LOG_DEBUG2("Comments Add Data[%d] end .",packet->packet_length);
  if (!r)
    send_error(socket, ERROR_TYPE, COMMENTS_ADD_DATA , packet->session_id);
  else
  {
    //send_error(socket, ERROR_TYPE, 999, packet->session_id); //999
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_COMMENTS_ADD_DATA, 1, 0, packet->session_id, 0);
    //packet_control.body_ = all_net_comments.get();
    send_message(socket, &packet_control);
  }

  return true;
}

bool Commentslogic::OnCommentsGetData(struct server* srv, int socket,
                                 struct PacketHead *packet) {

  comments_logic::net_request::HisComments hiscomments;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);

  bool r = hiscomments.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  LOG_DEBUG2("Comments Get Data[%d] begin.",packet->packet_length);
  comments_logic::CommentsEngine::GetSchdulerManager()->CommentsSendData(socket, packet->session_id, hiscomments);

  LOG_DEBUG2("Comments Get Data[%d] end.",packet->packet_length);
  return true;
}


}  // namespace comments_logic
