//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "net/packet_processing.h"
#include "logic/logic_comm.h"
#include "net/comm_head.h"
#include "protocol/data_packet.h"
#include <list>
#include <string>

#define DUMPPACKBUF 4096 * 10

namespace net {

bool PacketProsess::PacketStream(const PacketHead *packet_head,
                                 void **packet_stream,
                                 int32 *packet_stream_length) {
  bool r = true;
  int16 packet_length = packet_head->packet_length;
  int8 is_zip_encrypt = packet_head->is_zip_encrypt;
  int8 type = packet_head->type;
  int16 signature = packet_head->signature;
  int16 operate_code = packet_head->operate_code;
  int16 data_length = packet_head->data_length;
  int32 timestamp = packet_head->timestamp;
  int64 session_id = packet_head->session_id;
  int32 reserved = packet_head->reserved;

  std::string body_stream;
  struct PacketControl *packet_control = (struct PacketControl *)(packet_head);
  base_logic::DictionaryValue *value = packet_control != NULL ? packet_control->body_ : NULL;

  base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON);
    if (engine == NULL) {
      LOG_ERROR("engine create null");
      return false;
    }
    r = engine->Serialize((*value), &body_stream);

  BUILDPAKCET(body_stream.length());

  LOG_DEBUG2("%s",body_stream.c_str());

  *packet_stream = reinterpret_cast<void *>(const_cast<char *>(out.GetData()));
  *packet_stream_length = PACKET_HEAD_LENGTH + body_stream.length();
  if (engine) {
    delete engine;
    engine = NULL;
  }
  return true;
}

bool PacketProsess::UnpackStream(const void *packet_stream, int32 len,
                                 struct PacketHead **packet_head) {

  int32 temp;
  int error_code;
  std::string error_str;

  packet::DataInPacket in(
      reinterpret_cast<char*>(const_cast<void*>(packet_stream)), len);
  int16 packet_length = in.Read16();
  if (packet_length < PACKET_HEAD_LENGTH || len < PACKET_HEAD_LENGTH
      || packet_length != len)
    return false;

  int8 is_zip_encrypt = in.Read8();
  int8 type = in.Read8();
  int16 signature = in.Read16();
  int16 operate_code = in.Read16();
  int16 data_length = in.Read16();
  int32 timestamp = in.Read32();
  int64 session_id = in.Read64();
  int32 reserved = in.Read32();

  if (packet_length > PACKET_HEAD_LENGTH) {
    std::string body_stream = in.ReadData(data_length, temp);
    bool r = false;
    int error_code;
    std::string error_str;
    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
        base_logic::IMPL_JSON);
    if (engine == NULL) {
      LOG_ERROR("engine create null");
      return false;
    }


    base_logic::DictionaryValue *value = (base_logic::DictionaryValue*) engine
        ->Deserialize(&body_stream, &error_code, &error_str);

    if (value == NULL) {
      LOG_ERROR("json error");
      return false;
    }

    struct PacketControl *packet_control = new struct PacketControl;
    FILLPACKET()
    ;
    (*packet_head) = (struct PacketHead*) (packet_control);
    if (engine) {
      delete engine;
      engine = NULL;
    }
  } else if (packet_length == PACKET_HEAD_LENGTH) {
    (*packet_head) = new struct PacketHead;
    (*packet_head)->packet_length = packet_length;
    (*packet_head)->is_zip_encrypt = is_zip_encrypt;
    (*packet_head)->type = type;
    (*packet_head)->signature = signature;
    (*packet_head)->operate_code = operate_code;
    (*packet_head)->data_length = data_length;
    (*packet_head)->timestamp = timestamp;
    (*packet_head)->session_id = session_id;
    (*packet_head)->reserved = reserved;
  }
  return true;
}

void PacketProsess::DeletePacket(const void *packet_stream, int32 len,
                                 struct PacketHead *packet_head){

}

void PacketProsess::DumpPacket(const struct PacketHead *packet_head) {
#if 1
  struct PacketControl* packet_control = (struct PacketControl*) (packet_head);
  int16 packet_length = packet_control->packet_length;
  int8 is_zip_encrypt = packet_control->is_zip_encrypt;
  int8 type = packet_control->type;
  int16 operate_code = packet_control->operate_code;
  int16 data_length = packet_control->data_length;
  int32 timestamp = packet_control->timestamp;
  int64 session_id = packet_control->session_id;
  int32 reserved = packet_control->reserved;
  int16 signature = packet_control->signature;
  base_logic::DictionaryValue* value = packet_control->body_;
  char buf[DUMPPACKBUF];
  bool r = false;
  int32 j = 0;
#endif
}

uint64 PacketProsess::CompressionStream(const uint8 *unzip_data,
                                        uint64 unzip_len, uint8 **zip_data) {
  return unzip_len;
}

uint64 PacketProsess::DecompressionStream(const uint8 *zip_data, uint64 zip_len,
                                          uint8 **unzip_data) {
  return zip_len;
}

std::string PacketProsess::StrUnpacket(const void *packet_stream, int32 len) {
  int32 temp = 0;
  std::string empty;
  packet::DataInPacket in(
      reinterpret_cast<char*>(const_cast<void*>(packet_stream)), len);
  int16 packet_length = in.Read16();
  int8 is_zip_encrypt = in.Read8();
  int8 type = in.Read8();
  int16 signature = in.Read16();
  int16 operate_code = in.Read16();
  int16 data_length = in.Read16();
  int32 timestamp = in.Read32();
  int64 session_id = in.Read64();
  int32 reserved = in.Read32();
  if (packet_length == PACKET_HEAD_LENGTH || len == PACKET_HEAD_LENGTH)
    return empty;
  /* else if (packet_length < PACKET_HEAD_LENGTH || len < PACKET_HEAD_LENGTH
   || packet_length != len)
   return empty;*/

  std::string string_stream = in.ReadData(data_length, temp);
  std::string body_stream = string_stream.substr(0, data_length);
  //LOG_DEBUG2("%s",body_stream.c_str());
  return body_stream;
}

bool PacketProsess::PacketProsess::StrPacket(const int16 t_operate_code,
                                             const int8 t_is_zip_encrypt,
                                             const int8 t_type,
                                             int64 t_session_id,
                                             const int32 t_reserved,
                                             const std::string& body_stream,
                                             void **packet_stream,
                                             int32 *packet_stream_length) {
  bool r = true;
  int16 packet_length = body_stream.length() + PACKET_HEAD_LENGTH;
  int8 is_zip_encrypt = t_is_zip_encrypt;
  int8 type = t_type;
  int16 signature = 0;
  int16 operate_code = t_operate_code;
  int16 data_length = body_stream.length();
  int32 timestamp = time(NULL);
  int64 session_id = t_session_id;
  int32 reserved = t_reserved;

  BUILDPAKCET(body_stream.length());

  //LOG_DEBUG2("%s",body_stream.c_str());

  *packet_stream = reinterpret_cast<void *>(const_cast<char *>(out.GetData()));
  *packet_stream_length = PACKET_HEAD_LENGTH + body_stream.length();
  return true;
}

void PacketProsess::HexEncode(const void *bytes, size_t size) {
#if 1
  struct PacketHead *head = (struct PacketHead *) bytes;
  static const char kHexChars[] = "0123456789ABCDEF";
  std::string sret(size * 3, '\0');
  for (size_t i = 0; i < size; ++i) {
    char b = reinterpret_cast<const char *>(bytes)[i];
    sret[(i * 3)] = kHexChars[(b >> 4) & 0xf];
    sret[(i * 3) + 1] = kHexChars[b & 0xf];
    if ((((i * 3) + 2 + 1) % 12) != 0)
      sret[(i * 3) + 2] = '\40';
    else
      sret[(i * 3) + 2] = '\n';
  }LOG_DEBUG2("===start====\nopcode[%d]:\n%s\n====end====\n", head->operate_code,
      sret.c_str());
#endif
}

}  //  namespace net
