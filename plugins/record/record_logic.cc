//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "record/record_logic.h"
#include "record/record_proto_buf.h"
#include "record/operator_code.h"
#include "record/errno.h"
#include "record/schduler_engine.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>

#define DEFAULT_CONFIG_PATH "./plugins/record/record_config.xml"

#define TIME_DISTRIBUTION_TASK 10001

namespace record_logic {

Recordlogic *Recordlogic::instance_ = NULL;

Recordlogic::Recordlogic() {
    if (!Init())
        assert(0);
}

Recordlogic::~Recordlogic() {
    if (record_db_) {
        delete record_db_;
        record_db_ = NULL;
    }

    if (record_kafka_) {
        delete record_kafka_;
        record_kafka_ = NULL;
    }
    record_logic::RecordEngine::FreeSchdulerManager();
    record_logic::RecordEngine::FreeRecordEngine();
}

bool Recordlogic::Init() {
    bool r = false;
    manager_schduler::SchdulerEngine* (*schduler_engine)(void);
    std::string path = DEFAULT_CONFIG_PATH;
    config::FileConfig *config = config::FileConfig::GetFileConfig();
    if (config == NULL)
        return false;
    r = config->LoadConfig(path);
    record_db_ = new record_logic::RecordDB(config);
    record_kafka_ = new record_logic::RecordKafka(config);
    record_logic::RecordEngine::GetSchdulerManager()->InitDB(record_db_);
    record_logic::RecordEngine::GetSchdulerManager()->InitKafka(record_kafka_);
    record_logic::RecordEngine::GetSchdulerManager()->InitData();
    return true;
}

Recordlogic *Recordlogic::GetInstance() {
    if (instance_ == NULL)
        instance_ = new Recordlogic();
    return instance_;
}

void Recordlogic::FreeInstance() {
    delete instance_;
    instance_ = NULL;
}

bool Recordlogic::OnRecordConnect(struct server *srv, const int socket) {
    std::string ip;
    int port;
    logic::SomeUtils::GetIPAddress(socket, ip, port);
    LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
    return true;
}

bool Recordlogic::OnRecordMessage(struct server *srv, const int socket,
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

    switch (packet->operate_code) {
    case R_TODAY_POSITION_RECORD: {
        OnTodayPosition(srv, socket, packet);
        break;
    }
    case R_TODAY_ORDER_RECORD: {
        OnTodayOrder(srv, socket, packet);
        break;
    }

    case R_ALL_POSITION_RECORD: {
        OnHisPosition(srv, socket, packet);
        break;
    }

    case R_ALL_ORDER_RECORD: {
        OnHisOrder(srv, socket, packet);
        break;
    }

    case R_FANS_POSITION: {
        OnFansPosition(srv, socket, packet);
        break;
    }
    case R_FANS_ORDER: {
        OnFansOrder(srv, socket, packet);
    }
    case R_POSITION_COUNT: {
        OnPositionCount(srv, socket, packet);
    }
    default:
        break;
    }
    return true;
}

bool Recordlogic::OnRecordClose(struct server *srv, const int socket) {
    return true;
}

bool Recordlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len) {
    return true;
}

bool Recordlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len) {

    return true;
}

bool Recordlogic::OnBroadcastClose(struct server *srv, const int socket) {
    return true;
}

bool Recordlogic::OnIniTimer(struct server *srv) {
    if (srv->add_time_task != NULL) {
        if (srv->add_time_task != NULL) {
            srv->add_time_task(srv, "record", TIME_DISTRIBUTION_TASK, 3, -1);
        }
    }
    return true;
}

bool Recordlogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time) {
    switch (opcode) {
    case TIME_DISTRIBUTION_TASK: {
        record_logic::RecordEngine::GetSchdulerManager()->DistributionRecord();
        break;
    }
    default:
        break;
    }
    return true;
}

bool Recordlogic::OnTodayPosition(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::TodayPosition today_position;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = today_position.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    record_logic::RecordEngine::GetSchdulerManager()->TodayPosition(socket, packet->session_id,
                    packet->reserved,today_position.id(),today_position.start(),
                    today_position.count());
    return true;
}


bool Recordlogic::OnTodayOrder(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::TodayOrder today_order;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = today_order.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    record_logic::RecordEngine::GetSchdulerManager()->TodayOrder(socket,packet->session_id,
                    packet->reserved, today_order.id(),today_order.start(),today_order.count());
    return true;
}

bool Recordlogic::OnHisPosition(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::HisPosition his_position;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = his_position.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    record_logic::RecordEngine::GetSchdulerManager()->HisPosition(socket, packet->session_id,
                    packet->reserved,his_position.id(),his_position.start(),
                    his_position.count());
    return true;
}



bool Recordlogic::OnHisOrder(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::HisOrder his_order;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = his_order.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    record_logic::RecordEngine::GetSchdulerManager()->HisOrder(socket,packet->session_id,
                    packet->reserved, his_order.id(),his_order.status(),
                    his_order.start(),his_order.count());
    return true;
}


bool Recordlogic::OnFansPosition(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::FansPosition fans_position;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = fans_position.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    record_logic::RecordEngine::GetSchdulerManager()->FansPosition(socket,
        packet->session_id, packet->reserved, fans_position.symbol(),
        fans_position.buy_sell(), fans_position.start(), fans_position.count());
    return true;
}


bool Recordlogic::OnFansOrder(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::FansOrder fans_order;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = fans_order.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    record_logic::RecordEngine::GetSchdulerManager()->FansOrder(socket,
        packet->session_id, packet->reserved, fans_order.symbol(),
        fans_order.start(), fans_order.count());
    return true;
}


bool Recordlogic::OnPositionCount(struct server* srv, int socket, struct PacketHead* packet) {
    record_logic::net_request::PositionCount position_count;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = position_count.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    record_logic::RecordEngine::GetSchdulerManager()->SendPositionCount(socket,
        packet->session_id, packet->reserved,position_count.symbol());
    return true;
}
}  // namespace trades_logic
