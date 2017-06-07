//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "search/search_logic.h"
#include "search/schduler_engine.h"
#include "search/search_proto_buf.h"
#include "search/operator_code.h"
#include "search/errno.h"
#include "comm/comm_head.h"
#include "net/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include <sstream>
#include "http/http_api.h"

#define DEFAULT_CONFIG_PATH "./plugins/search/search_config.xml"

namespace search_logic {

Searchlogic *Searchlogic::instance_ = NULL;

Searchlogic::Searchlogic() {
    if (!Init())
        assert(0);
}

Searchlogic::~Searchlogic() {
    if (search_db_) {
        delete search_db_;
        search_db_ = NULL;
    }
    search_logic::SearchEngine::FreeSchdulerManager();
    search_logic::SearchEngine::FreeSearchEngine();
}

bool Searchlogic::Init() {
    bool r = false;
    std::string path = DEFAULT_CONFIG_PATH;
    config::FileConfig *config = config::FileConfig::GetFileConfig();
    if (config == NULL)
        return false;
    r = config->LoadConfig(path);
    search_db_ = new search_logic::SearchDB(config);
    search_logic::SearchEngine::GetSchdulerManager()->InitDB(search_db_);
    search_logic::SearchEngine::GetSchdulerManager()->InitSearchData();
    search_logic::SearchEngine::GetSearchEngine();
    return true;
}

Searchlogic *Searchlogic::GetInstance() {
    if (instance_ == NULL)
        instance_ = new Searchlogic();
    return instance_;
}

void Searchlogic::FreeInstance() {
    delete instance_;
    instance_ = NULL;
}

bool Searchlogic::OnSearchConnect(struct server *srv, const int socket) {
    std::string ip;
    int port;
    logic::SomeUtils::GetIPAddress(socket, ip, port);
    LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
    return true;
}

bool Searchlogic::OnSearchMessage(struct server *srv, const int socket,
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
    case R_SEARCH_STAR: {
        OnSearchStar(srv, socket, packet);
        break;
    }
    default:
        break;
    }
    return true;
}


bool Searchlogic::OnSearchClose(struct server *srv, const int socket) {
    return true;
}

bool Searchlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len) {
    return true;
}

bool Searchlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                     const void *msg, const int len) {
    printf("OnBroadcastMessage \n");
    return true;
}

bool Searchlogic::OnBroadcastClose(struct server *srv, const int socket) {
    return true;
}

bool Searchlogic::OnIniTimer(struct server *srv) {
    if (srv->add_time_task != NULL) {
    }
    return true;
}

bool Searchlogic::OnTimeout(struct server *srv, char *id, int opcode, int time) {
    switch (opcode) {
    default:
        break;
    }
    return true;
}

bool Searchlogic::OnSearchStar(struct server* srv, int socket, struct PacketHead* packet) {
    search_logic::net_request::SearchStarInfo search_info;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = search_info.set_http_packet(packet_control->body_);
    if (!r) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    search_logic::SearchEngine::GetSchdulerManager()->SendSearchStarInfo(socket,packet->session_id,packet->reserved,search_info.message());

}

}  // namespace search_logic
