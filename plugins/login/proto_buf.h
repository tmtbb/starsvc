// Copyright (c) 2016 The tourism Authors. All rights reserved.
// proto_buf.h
// Created on: 2016年8月10日.
// Author: Paco.
#ifndef PUB_NET_PROTO_BUF_H_
#define PUB_NET_PROTO_BUF_H_

#include <stdio.h>

#pragma pack(push)
#pragma pack (1)

#define HEAD_LENGTH (sizeof(int16) * 4 + sizeof(int8) * 2 + \
    sizeof(int32) * 2 + sizeof(int64))

struct BufHead {
  int16 packet_length;
  int8  is_zip_encrypt;
  int8  type;
  int16 signature;
  int16 operate_code;
  int16 data_length;
  uint32 timestamp;
  int64 session_id;
  int32 reserved;
};

/*class PacketHead {
 public:
  PacketHead();
  PacketHead(char* packet);
  virtual ~PacketHead();
  int32 Deserialize() { return 0; }
  std::string Serialize(DicValue* dic);
  std::string Serialize() { return body_str_; }


  //call after serialize
  void AdapterLen();

  void set_head(char* packet);
  void set_head(BufHead head);
  void set_body_str(char* packet);
  std::string body_str() { return body_str_; }

  BufHead head() { return head_; }

  inline int16 packet_length() { return head_.packet_length; }
  inline int8 is_zip_encrypt() { return head_.is_zip_encrypt; }
  inline int8 type() { return head_.type; }
  inline int16 signature() { return head_.signature; }
  inline int16 operate_code() { return head_.operate_code; }
  inline int16 data_length() { return head_.data_length; }
  inline uint32 timestamp() { return head_.timestamp; }
  inline int64 session_id() { return head_.session_id; }
  inline int32 reserved() { return head_.reserved; }

  inline void set_packet_length(int16 len) { head_.packet_length = len; }
  inline void set_is_zip_encrypt(int8 isz) { head_.is_zip_encrypt = isz; }
  inline void set_type(int8 type) { head_.type = type; }
  inline void set_signature(int16 sig) { head_.signature = sig; }
  inline void set_operate_code(int16 op) { head_.operate_code = op; }
  inline void set_data_length(int16 da_len) { head_.data_length = da_len; }
  inline void set_timestamp(uint32 t) { head_.timestamp = t; }
  inline void set_session_id(int64 sid) { head_.session_id = sid; }
  inline void set_reserved(int32 res) { head_.reserved = res; }
 protected:
  BufHead head_;
  std::string body_str_;
};

class PacketErr:public PacketHead {
 public:
  PacketErr() { error_ = -1000; }
  PacketErr(PacketHead head);
  std::string Serialize();

  inline void set_error(int32 err) { error_ = err; }
  inline int32 error() { return error_; }
 private:
  int32 error_;
  };*/

#pragma pack(pop)
#endif  // PUB_NET_PROTO_BUF_H_
