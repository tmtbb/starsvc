// Copyright (c) 2016 The tourism Authors. All rights reserved.
// proto_buf.cc
// Created on: 2016年8月10日.
// Author: Paco.

#include "pub/net/proto_buf.h"

#include <string.h>

#include "glog/logging.h"

#include "pub/comm/comm_head.h"

PacketHead::PacketHead(char* packet) {
  memset(&head_, 0, HEAD_LENGTH);
  set_head(packet);
  set_body_str(packet + HEAD_LENGTH);
}

PacketHead::PacketHead() {

}

PacketHead::~PacketHead() {

}

void PacketHead::set_head(char* packet) {
  memcpy(&head_, packet, HEAD_LENGTH);
}

void PacketHead::set_head(BufHead head) {
  head_ = head;
}

std::string PacketHead::Serialize(DicValue* dic) {
  do{
    if (dic == NULL || dic->empty())
      break;
    base_logic::ValueSerializer* serializer =
          base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
    serializer->Serialize(*dic, &body_str_);
    base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                   serializer);
  } while (0);
  return body_str_;
}

void PacketHead::set_body_str(char* packet) {
  if (packet_length() != data_length() + HEAD_LENGTH) {
    //LOG(ERROR) << "something wrong with packet set_body";
    //LOG(ERROR) << "type:" << type() << "-operate_code:" \
        << operate_code() << "-packet_length:" << packet_length() \
        << "-data_length:" << data_length();
	return;
  }
  //LOG(INFO) << "type:" << (int)type() << "-operate_code:" \
      << operate_code() << "-packet_length:" << packet_length() \
      << "-data_length:" << data_length();
  //LOG(INFO) << "is_zip_encrypt:" << (int)is_zip_encrypt() << "-signature:" \
      << signature() << "-timestamp:" << timestamp();
//  char* data = new char[data_length()+1];
//  memset(data, 0, data_length()+1);
//  memcpy(data, packet, data_length());
  body_str_ = packet;
  //LOG(INFO) << "body_str:" << body_str_;
//  delete[] data;
}

void PacketHead::AdapterLen() {
  if (body_str_.length() < 0)
    body_str_ = "";
  head_.data_length = body_str_.length();
  head_.packet_length = HEAD_LENGTH + head_.data_length;
  head_.timestamp = time(NULL);
}

//所有的错误包，type均为ERROR_TYPE
PacketErr::PacketErr(PacketHead packet) {
  head_ = packet.head();
  head_.type = ERROR_TYPE;
  error_ = -1000; // 默认错误
}

std::string PacketErr::Serialize() {
  DicValue body_dic;
  body_dic.SetBigInteger(L"error", error_);

  base_logic::ValueSerializer* serializer =
        base_logic::ValueSerializer::Create(base_logic::IMPL_JSON);
  serializer->Serialize(body_dic, &body_str_);
  base_logic::ValueSerializer::DeleteSerializer(base_logic::IMPL_JSON,
                                                 serializer);
  return body_str_;
}

