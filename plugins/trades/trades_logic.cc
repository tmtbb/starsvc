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

namespace trades_logic
{

Tradeslogic *Tradeslogic::instance_ = NULL;

Tradeslogic::Tradeslogic()
{
    if (!Init())
        assert(0);
}

Tradeslogic::~Tradeslogic()
{
    if (trades_db_)
    {
        delete trades_db_;
        trades_db_ = NULL;
    
    }

    if (trades_kafka_){
        delete trades_kafka_;
        trades_kafka_ = NULL;
    }
    trades_logic::TradesEngine::FreeSchdulerManager();
    trades_logic::TradesEngine::FreeTradesEngine();
}


bool Tradeslogic::Init()
{
    bool r = false;
    manager_schduler::SchdulerEngine* (*schduler_engine)(void);
    std::string path = DEFAULT_CONFIG_PATH;
    config::FileConfig *config = config::FileConfig::GetFileConfig();
    if (config == NULL)
        return false;
    r = config->LoadConfig(path);
    LOG_MSG2("path : %s", path.c_str());
    trades_logic::TradesEngine::GetSchdulerManager();
    trades_db_ = new trades_logic::TradesDB(config);
    trades_kafka_ = new trades_logic::TradesKafka(config);
    trades_logic::TradesEngine::GetSchdulerManager()->InitDB(trades_db_);
    trades_logic::TradesEngine::GetSchdulerManager()->InitKafka(trades_kafka_);
    trades_logic::TradesEngine::GetSchdulerManager()->InitData();
    base::SysRadom::GetInstance();
    std::string schduler_library = "./data_share.so";
    std::string schduler_func = "GetManagerSchdulerEngine";
    schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                        logic::SomeUtils::GetLibraryFunction(schduler_library, 
                        schduler_func);

    schduler_engine_= (*schduler_engine)();
    if (schduler_engine_ == NULL)
        assert(0);
    trades_logic::TradesEngine::GetSchdulerManager()->InitManagerSchduler(schduler_engine_);
    return true;
}

Tradeslogic *Tradeslogic::GetInstance()
{
    if (instance_ == NULL)
        instance_ = new Tradeslogic();
    return instance_;
}

void Tradeslogic::FreeInstance()
{
    delete instance_;
    instance_ = NULL;
}

bool Tradeslogic::OnTradesConnect(struct server *srv, const int socket)
{
    std::string ip;
    int port;
    logic::SomeUtils::GetIPAddress(socket, ip, port);
    LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);
    return true;
}

bool Tradeslogic::OnTradesMessage(struct server *srv, const int socket,
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
    case R_TRADES_OPEN_POSITION:
    {
        OnOpenPosition(srv,socket, packet);
        break;
    }
    case R_TRADES_SYMBOL_STATUS:{
        OnTradesSymbolInfo(srv, socket, packet);
        break;
    }

    case R_CONFIRM_ORDER:{
        //OnConfirmOrder(srv, socket, packet);
        break;
    }

    case R_CANCEL_ORDER:{
        OnCancelOrder(srv,socket, packet);
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

bool Tradeslogic::OnTradesClose(struct server *srv, const int socket)
{
    return true;
}

bool Tradeslogic::OnBroadcastConnect(struct server *srv, const int socket,
                                     const void *msg, const int len)
{
    return true;
}

bool Tradeslogic::OnBroadcastMessage(struct server *srv, const int socket,
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

    return true;
}


bool Tradeslogic::OnBroadcastClose(struct server *srv, const int socket)
{
    return true;
}

bool Tradeslogic::OnIniTimer(struct server *srv)
{
    if (srv->add_time_task != NULL)
    {
        srv->add_time_task(srv, "trades", TIME_DISTRIBUTION_TASK, 10, -1);
    }
    return true;
}

bool Tradeslogic::OnTimeout(struct server *srv, char *id, int opcode,
                            int time)
{
    switch (opcode)
    {
    case TIME_DISTRIBUTION_TASK:
    {
        trades_logic::TradesEngine::GetSchdulerManager()->TimeStarEvent();
    }
    default:
        break;
    }
    return true;
}

bool Tradeslogic::OnCancelOrder(struct server* srv, int socket, struct PacketHead* packet) {
    trades_logic::net_request::CancelOrder cancel_order;

    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = cancel_order.set_http_packet(packet_control->body_);
    if (!r){
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    trades_logic::TradesEngine::GetSchdulerManager()->CancelOrder(socket,packet->session_id,
        packet->reserved,cancel_order.id(),cancel_order.order_id());
}

bool Tradeslogic::OnTradesSymbolInfo(struct server* srv, int socket, struct PacketHead* packet) {
    trades_logic::net_request::TradesSymbol trades_symbol;
    if (packet->packet_length <= PACKET_HEAD_LENGTH) {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = trades_symbol.set_http_packet(packet_control->body_);
    if (!r){
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    trades_logic::TradesEngine::GetSchdulerManager()->TradesSymbolInfo(socket,
        packet->session_id,packet->reserved,trades_symbol.symbol());
    return true;
}

bool Tradeslogic::OnConfirmOrder(struct server* srv, int socket, struct PacketHead* packet) {
    trades_logic::net_request::ConfirmOrder confirm_order;
    if (packet->packet_length <= PACKET_HEAD_LENGTH)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = confirm_order.set_http_packet(packet_control->body_);
    if (!r)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

//    trades_logic::TradesEngine::GetSchdulerManager()->ConfirmOrder(socket,
//            packet->session_id, packet->reserved,confirm_order.id(),
//            confirm_order.order_id(),confirm_order.position_id());
    return true;

}

bool Tradeslogic::OnGetStarTrading(struct server* srv, int socket, struct PacketHead* packet)
{
    trades_logic::net_request::CurrentPosition current_position;
    if (packet->packet_length <= PACKET_HEAD_LENGTH)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = current_position.set_http_packet(packet_control->body_);
    if (!r)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    return true;
}

bool Tradeslogic::OnOpenPosition(struct server* srv, int socket, struct PacketHead* packet)
{
    trades_logic::net_request::OpenPosition open_position;
    if (packet->packet_length <= PACKET_HEAD_LENGTH)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }
    struct PacketControl* packet_control = (struct PacketControl*) (packet);
    bool r = open_position.set_http_packet(packet_control->body_);
    if (!r)
    {
        send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
        return false;
    }

    trades_logic::TradesEngine::GetSchdulerManager()->CreateTradesPosition(socket,
            packet->session_id,packet->reserved,open_position.id(),
            open_position.symbol(),open_position.wid(), open_position.buy_sell(),
            open_position.amount(),open_position.price());
    return true;
}

}  // namespace trades_logic
