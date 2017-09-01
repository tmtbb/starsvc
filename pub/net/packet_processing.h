//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_PUB_LOGIC_NET_PACKET_PROCESSING_H_
#define SWP_PUB_LOGIC_NET_PACKET_PROCESSING_H_

#include "basic/basictypes.h"
#include "net/comm_head.h"
#include "protocol/data_packet.h"
#include <stddef.h>

#define PRINT_TITLE(v)                                                         \
  j += snprintf(buf + j, DUMPPACKBUF - j, "\n-------- %s --------\n", v)

#define PRINT_END(v)                                                           \
  j += snprintf(buf + j, DUMPPACKBUF - j, "-------- %s --------\n", v)

#define PRINT_INT(v)                                                           \
  j += snprintf(buf + j, DUMPPACKBUF - j, "\t%s = %d\n", #v,                   \
                static_cast<int>(v))

#define PRINT_INT64(v)                                                         \
  j += snprintf(buf + j, DUMPPACKBUF - j, "\t%s = %lld\n", #v, (int64_t)v)

#define PRINT_STRING(v)                                                        \
  j += snprintf(buf + j, DUMPPACKBUF - j, "\t%s = %s\n", #v, (const char *)v)

#define BUILDPAKCET(DATA_LENGTH)                                               \
  data_length = DATA_LENGTH;                                                   \
  packet_length = data_length + PACKET_HEAD_LENGTH;                            \
  packet::DataOutPacket out(false, packet_length);                             \
  out.Write16(packet_length);\
  out.Write8(is_zip_encrypt);\
  out.Write8(type);                                                            \
  out.Write16(signature);                                                      \
  out.Write16(operate_code);                                                   \
  out.Write16(data_length);                                                    \
  out.Write32(timestamp);                                                      \
  out.Write64(session_id);                                                     \
  out.Write32(reserved);                                                       \
  out.WriteData(body_stream.c_str(), body_stream.length());

#define BUILDUNPACKET()                                                       \
  packet::DataInPacket in(reinterpret_cast<char*>(const_cast<void*>(packet_stream)), len); \
  int16 packet_length = in.Read16();\
  int8 is_zip_encrypt = in.Read8();\
  int8 type = in.Read8();                                                      \
  int16 signature = in.Read16();                                               \
  int16 operate_code = in.Read16();                                            \
  int16 data_length = in.Read16();                                             \
  int32 timestamp = in.Read32();                                               \
  int64 session_id = in.Read64();                                              \
  int32 reserved = in.Read32();                                                \
  std::string body_stream;                                                     \
  if (data_length > 0)                                                         \
    body_stream  = in.ReadData(data_length, temp);

#define FILLPACKET()                                                          \
  (packet_control)->packet_length = packet_length;                               \
  (packet_control)->is_zip_encrypt = is_zip_encrypt;                             \
  (packet_control)->type = type;                                                 \
  (packet_control)->signature = signature;                                       \
  (packet_control)->operate_code = operate_code;                                 \
  (packet_control)->data_length = data_length;                                   \
  (packet_control)->timestamp = timestamp;                                       \
  (packet_control)->session_id = session_id;                                     \
  (packet_control)->reserved = reserved;                                         \
  (packet_control)->body_ = value;

#define _MAKE_HEAD(head, _packet_length, _operate_code, _data_length, _type,   \
                   _is_zip_encrypt, _session_id, _reserved)                    \
  do {                                                                         \
    head.packet_length = _packet_length;                                       \
    head.is_zip_encrypt = _is_zip_encrypt;                                     \
    head.data_length = _data_length;                                           \
    head.type = _type;                                                         \
    head.signature = 0;                                                        \
    head.operate_code = _operate_code;                                         \
    head.timestamp = time(NULL);                                               \
    head.session_id = _session_id;                                             \
    head.reserved = _reserved;                                                 \
  } while (0)

#define MAKE_FAIL_HEAD(head, _operate_code, _session_id, _reserved)            \
  _MAKE_HEAD(head, PACKET_HEAD_LENGTH, _operate_code, 0, 0, 0, _session_id,    \
             _reserved)

#define MAKE_HEAD(head, _operate_code, _type, _is_zip_encrypt, _session_id,    \
                  _reserved)                                                   \
  _MAKE_HEAD(head, 0, _operate_code, 0, _type, _is_zip_encrypt, _session_id,   \
             _reserved)

namespace net {

class PacketProsess {
 public:
  //加密压缩组包
  static bool PacketStream(const struct PacketHead *packet_head,
                           void **packet_stream, int32 *packet_stream_length);

  //解加密压缩包
  static bool UnpackStream(const void *packet_stream, int32 len,
                           struct PacketHead **packet_head);

  //明文组包
  static bool PacketClearStream(const PacketHead *packet_head,
                                 void **packet_stream,
                                 int32 *packet_stream_length);

  //解明文包
  static bool UnpackClearStream(const void *packet_stream, int32 len,
                                 struct PacketHead **packet_head);

  static void HexEncode(const void *bytes, size_t size);

  static void DumpPacket(const struct PacketHead *packet_head);

  static void DeletePacket(const void *packet_stream, int32 len,
                           struct PacketHead *packet_head);

  static bool StrPacket(const int16 t_operate_code,
                        const int8 t_is_zip_encrypt, const int8 t_type,
                        int64 t_session_id, const int32 t_reserved,
                        const std::string& body_stream, void **packet_stream,
                        int32 *packet_stream_length);

  static std::string StrUnpacket(const void *packet_stream, int32 len);

  //列表发送清除内存

 private:
  static uint64 CompressionStream(const uint8 *unzip_data, uint64 unzip_len,
                                  uint8 **zip_data);  //  压缩

  static uint64 DecompressionStream(const uint8 *zip_data, uint64 zip_len,
                                    uint8 **unzip_data);  //  解压缩

};
}  //  namespace net
#endif /* SWP_NET_PACKET_PROCESSING_H_ */
