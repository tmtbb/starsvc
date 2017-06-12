//  Copyright (c) 2015-2015 The swp Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "trades/trades_logic.h"
#include "trades/schduler_engine.h"
#include "trades/trades_proto_buf.h"
#include "trades/trades_info.h"
#include "trades/errno.h"
#include "basic/native_library.h"
#include "trades/operator_code.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/trades/trades_config.xml"

#define TIME_DISTRIBUTION_TASK 10000

namespace trades_logic {

Tradeslogic *Tradeslogic::instance_ = NULL;

Tradeslogic::Tradeslogic() {
  if (!Init())
    assert(0);
}

Tradeslogic::~Tradeslogic() {
  if (trades_db_) {
    delete trades_db_;
    trades_db_ = NULL;
  }
  trades_logic::TradesEngine::FreeSchdulerManager();
  trades_logic::TradesEngine::FreeTradesEngine();
}

void Tradeslogic::Test() {
  std::list<trades_logic::TimeTask> list;
  trades_logic::TimeTask time_task1;
  time_task1.set_id(1);
  time_task1.set_start_time(time(NULL));
  time_task1.set_end_time(1486995277);
  list.push_back(time_task1);

  trades_logic::TimeTask time_task2;
  time_task2.set_id(2);
  time_task2.set_start_time(time(NULL));
  time_task2.set_end_time(1486995177);
  list.push_back(time_task2);

  trades_logic::TimeTask time_task3;
  time_task3.set_id(3);
  time_task3.set_start_time(time(NULL));
  time_task3.set_end_time(1486998140);
  list.push_back(time_task3);

  trades_logic::TimeTask time_task4;
  time_task4.set_id(4);
  time_task4.set_start_time(time(NULL));
  time_task4.set_end_time(1486997938);
  list.push_back(time_task4);

  trades_logic::TimeTask time_task5;
  time_task5.set_id(5);
  time_task5.set_start_time(time(NULL));
  time_task5.set_end_time(1486998253);
  list.push_back(time_task5);

  trades_logic::TimeTask time_task6;
  time_task6.set_id(6);
  time_task6.set_start_time(time(NULL));
  time_task6.set_end_time(1486998344);
  list.push_back(time_task6);

  trades_logic::TimeTask time_task7;
  time_task7.set_id(7);
  time_task7.set_start_time(time(NULL));
  time_task7.set_end_time(1486998014);
  list.push_back(time_task7);

  trades_logic::TimeTask time_task8;
  time_task8.set_id(8);
  time_task8.set_start_time(time(NULL));
  time_task8.set_end_time(1486998321);
  list.push_back(time_task8);
  list.sort(trades_logic::TimeTask::cmp);

  while (list.size() > 0) {
    trades_logic::TimeTask time_task = list.front();
    list.pop_front();
    LOG_DEBUG2("id %lld, end_time %lld",time_task.id(),time_task.end_time());
  }

  LOG_DEBUG("id , end_time ");
}

bool Tradeslogic::Init() {
  bool r = false;
  std::string path = DEFAULT_CONFIG_PATH;
  config::FileConfig *config = config::FileConfig::GetFileConfig();
  if (config == NULL)
    return false;
  r = config->LoadConfig(path);
  LOG_MSG2("path : %s", path.c_str());
  trades_logic::TradesEngine::GetSchdulerManager();
  trades_db_ = new trades_logic::TradesDB(config);
  trades_logic::TradesEngine::GetSchdulerManager()->InitDB(trades_db_);
  trades_logic::TradesEngine::GetSchdulerManager()->InitGoodsData();
  trades_logic::TradesEngine::GetSchdulerManager()->InitFlightInfo();
  base::SysRadom::GetInstance();
  return true;
}

Tradeslogic *Tradeslogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Tradeslogic();
  return instance_;
}

void Tradeslogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Tradeslogic::OnTradesConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
  return true;
}

bool Tradeslogic::OnTradesMessage(struct server *srv, const int socket,
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
    case R_TRADES_GOODS_DATA: {
      OnPlatformsGoods(srv, socket, packet);
      break;
    }
    case R_TRADES_OPEN_POSITION: {
      OnOpenPosition(srv, socket, packet);
      break;
    }

    case R_TRADES_CURRENT_POSITION: {
      OnCurrentPosition(srv, socket, packet);
      break;
    }

    case R_FLIGHT_INFO: {
      OnFightInfo(srv, socket, packet);
      break;
    }
    default:
      break;
  }
  return true;
}

bool Tradeslogic::OnTradesClose(struct server *srv, const int socket) {
  return true;
}

bool Tradeslogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len) {
  return true;
}

bool Tradeslogic::OnBroadcastMessage(struct server *srv, const int socket,
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
      OnQutations(srv, socket, packet);
      break;
    }
    default:
      break;
  }
  return true;
}

bool Tradeslogic::OnQutations(struct server* srv, int socket,
                              struct PacketHead *packet) {
  trades_logic::net_other::RealTime real_time;
  swp_logic::Quotations quotations;
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  real_time.set_http_packet(packet_control->body_);
  quotations.set_change(real_time.change());
  quotations.set_closed_yesterday_price(real_time.closed_yesterday_price());
  quotations.set_current_price(real_time.current_price());
  quotations.set_current_unix_time(real_time.current_unix_time());
  quotations.set_exchange_name(real_time.exchange_name());
  quotations.set_high_price(real_time.high_price());
  quotations.set_low_price(real_time.low_price());
  quotations.set_opening_today_price(real_time.opening_today_price());
  quotations.set_pchg(real_time.pchg());
  quotations.set_platform_name(real_time.platform_name());
  quotations.set_symbol(real_time.symbol());
  quotations.set_type(real_time.type());
  trades_logic::TradesEngine::GetSchdulerManager()->SetQuotations(quotations);
  return true;
}

bool Tradeslogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Tradeslogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    srv->add_time_task(srv, "trades", TIME_DISTRIBUTION_TASK, 3, -1);
  }
  return true;
}

bool Tradeslogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time) {
  switch (opcode) {
    case TIME_DISTRIBUTION_TASK: {
      trades_logic::TradesEngine::GetSchdulerManager()->DistributionTask();
      break;
    }
    default:
      break;
  }
  return true;
}

bool Tradeslogic::OnPlatformsGoods(struct server* srv, int socket,
                                   struct PacketHead *packet) {
  trades_logic::net_request::Goods goods;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = goods.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  trades_logic::TradesEngine::GetSchdulerManager()->SendGoods(
      socket, packet->session_id, packet->reserved, goods.pid(), goods.start(),
      goods.count());
  return true;

}

bool Tradeslogic::OnOpenPosition(struct server* srv, int socket,
                                 struct PacketHead* packet) {
  trades_logic::net_request::OpenPosition open_position;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = open_position.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  swp_logic::TradesPosition trades_position;
  trades_position.create_position_id();
  trades_position.set_uid(open_position.id());
  trades_position.set_buy_sell(open_position.buy_sell());
  trades_position.set_close_type(trades_logic::TIMER_TYPE);
  trades_position.set_amount(open_position.amount());
  trades_position.set_code_id(open_position.code_id());
  trades_logic::TradesEngine::GetSchdulerManager()->OnTimePosition(
      socket, packet->session_id, trades_position);
  return true;
}

bool Tradeslogic::OnCurrentPosition(struct server* srv, int socket,
                                    struct PacketHead* packet) {
  trades_logic::net_request::CurrentPosition current_position;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = current_position.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  trades_logic::TradesEngine::GetSchdulerManager()->SendCurrentPosition(
      socket, packet->session_id, packet->reserved, current_position.id(),
      current_position.start(), current_position.count());
  return true;
}

bool Tradeslogic::OnFightInfo(struct server* srv, int socket,
                              struct PacketHead *packet) {
  trades_logic::net_request::FlightInfo flight_info;
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  struct PacketControl* packet_control = (struct PacketControl*) (packet);
  bool r = flight_info.set_http_packet(packet_control->body_);
  if (!r) {
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }

  //发送航班信息
  trades_logic::TradesEngine::GetSchdulerManager()->SendFightInfo(socket,
                                                                  packet->session_id,
                                                                  packet->reserved,
                                                                  flight_info.id(),
                                                                  flight_info.gid());
  return true;
}

}  // namespace trades_logic
