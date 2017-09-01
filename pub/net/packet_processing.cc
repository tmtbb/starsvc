//  Copyright (c) 2016-2017 The KID Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "net/packet_processing.h"
#include "logic/logic_comm.h"
#include "net/comm_head.h"
#include "net/packet_process_assist.h"
#include "protocol/data_packet.h"
#include <list>
#include <string>

#define DUMPPACKBUF 4096 * 10

namespace net {

void printstream(void* pc, int16 len) {
  void* tempCP = pc;
  for(int i=1; i<=len; ++i) {
    LOG_DEBUG2("[%x]", (*((char*)tempCP) & 0xFF));
    tempCP = (char*)tempCP+1;
  }
    
}

bool PacketProsess::PacketClearStream(const PacketHead *packet_head,
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

bool PacketProsess::UnpackClearStream(const void *packet_stream, int32 len,
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


bool PacketProsess::PacketStream(const PacketHead *packet_head,
                                 void **packet_stream,
                                 int32 *packet_stream_length) {
  bool r = true;
  int16 packet_length = packet_head->packet_length;
  //int8 is_zip_encrypt = (TEA<<4) + ZIP_TYPE; //高4位加密类型  低4位压缩类型  TODO:一定加密, 选择性压缩
  int8 is_zip_encrypt = (TEA<<4) ;
  int8 type = packet_head->type;
  //uint16 signature = packet_head->signature;
  uint16 signature = 0;
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

  //加密算法会补全8 位，导致checksum 结果变化，提前补足
  int32 packetBody_length = PACKET_HEAD_LENGTH + body_stream.length() - sizeof(int16) - sizeof(int8);
  if (packetBody_length%8 != 0) {
    packetBody_length = (packetBody_length/8 + 1) * 8 + sizeof(int16) + sizeof(int8) - PACKET_HEAD_LENGTH;
    body_stream.resize(packetBody_length, '\0');
  } else {
    packetBody_length = body_stream.length();
  }
  LOG_DEBUG2("PacketStream packetBody_length:%d, content:%s", packetBody_length, body_stream.c_str());
  
  //1.组明文包
  //BUILDPAKCET(packetBody_length);
  data_length = packetBody_length;
  packet_length = data_length + PACKET_HEAD_LENGTH;
  packet::DataOutPacket out(false, packet_length);
  out.Write16(packet_length);
  out.Write8(is_zip_encrypt);
  out.Write8(type);
  out.Write16(signature);
  out.Write16(operate_code);
  out.Write16(data_length);
  out.Write32(timestamp);
  out.Write64(session_id);
  out.Write32(reserved);
  out.WriteData(body_stream.c_str(), packetBody_length);

  char* packetBody_stream;
  //2.对明文包checksum，写入signature
  char* checksum_packetBody = reinterpret_cast<char *>(const_cast<char *>(out.GetData()));
  if(!checksum(checksum_packetBody, signature)) {
    LOG_ERROR("PacketStream checksum failure.");
  }
  out.Reset();
  out.Skip( sizeof(int16) + sizeof(int8) + sizeof(int8) );
  out.Write16(signature);
  LOG_DEBUG2("PacketStream packet_length[%d] is_zip_encrypt[%d] signature [%d]", 
                packet_length, is_zip_encrypt, signature);


  //4.加密
  char* encrypt_packetBody = \
      reinterpret_cast<char *>(const_cast<char *>(out.GetData())) + sizeof(int16) + sizeof(int8);
  //LOG_ERROR2("====befor encrypt str[%s]", encrypt_packetBody);
  int32 temp_data_len = packet_length - sizeof(int16) - sizeof(int8);
  packetBody_stream = str_en_8byte(encrypt_packetBody, temp_data_len, 16, packetBody_length);//Don't forget free memory
  //LOG_DEBUG2("====encrypt in len[%d], return length[%d]", temp_data_len, packetBody_length);
  

  //3.压缩 TODO: 包体小的时候不压缩，节省时间. 先不压缩
/*  unsigned char* zipData;
  unsigned char* zip_packetBody = reinterpret_cast<unsigned char *>(packetBody_stream);
  MZip* zip_handle = new MZip();//TODO: create once
  unsigned long zipSize = zip_handle->ZipData(zip_packetBody, packetBody_length, &zipData);
  delete zip_handle;
  zip_handle = NULL;
  LOG_DEBUG2("after zip, data size[%d]", zipSize);

  packetBody_stream = reinterpret_cast<char *>(zipData);
  packetBody_length = zipSize;
  LOG_ERROR2("after zip, body length[%d]", packetBody_length);
  */

  //5.重新组包
  packet_length = packetBody_length + sizeof(int16) + sizeof(int8);
  packet::DataOutPacket out_packet(false, packet_length);
  out_packet.Write16(packet_length);
  out_packet.Write8(is_zip_encrypt);
  out_packet.WriteData(packetBody_stream, packetBody_length);

  *packet_stream = reinterpret_cast<void *>(const_cast<char *>(out_packet.GetData()));
  *packet_stream_length = packet_length;
  LOG_DEBUG2("PacketStream after packet length[%d]", packet_length);
  //printstream(*packet_stream, packet_length);

  if (packetBody_stream) {
    delete packetBody_stream;
    packetBody_stream = NULL;
  }
  if (engine) {
    delete engine;
    engine = NULL;
  }

  return true;
}

bool PacketProsess::UnpackStream(const void *packet_stream, int32 len,
                                 struct PacketHead **packet_head) {

  bool r = false;
  int32 temp;

  packet::DataInPacket in(
      reinterpret_cast<char*>(const_cast<void*>(packet_stream)), len);
  int16 packet_length = in.Read16();
  int8 is_zip_encrypt = in.Read8();
  LOG_DEBUG2("UnpackStream len[%d],packet length[%d],is_zip_encrypt[%d]", len, packet_length, is_zip_encrypt);

  char* body_stream;
  int32 body_length = packet_length - sizeof(int16) - sizeof(int8);
  //1.判断是否需要解压
  if((is_zip_encrypt & 0xF) > 0) {
    unsigned char* zip_packetBody = \
                  reinterpret_cast<unsigned char*>(const_cast<char *>(in.ReadData(temp)));
    unsigned char* unzipData;
    MUnZip* unzip_handle = new MUnZip();//TODO:Create once
    body_length = unzip_handle->UnZipData(zip_packetBody, body_length, &unzipData);
    body_stream = reinterpret_cast<char *>(unzipData);
    delete unzip_handle;
    unzip_handle = NULL;
  } else {
    body_stream = reinterpret_cast<char *>(const_cast<char *>(in.ReadData(temp)));
  }
  //LOG_DEBUG2("UnpackStream after unzip body length[%d]", body_length);

  //2.判断是否需要解密
  if(((is_zip_encrypt>>4) & 0xF) > 0) {
    char* decrypt_packetBody = reinterpret_cast<char *>(body_stream);
    body_stream = str_de_8byte(decrypt_packetBody, body_length, 16);//Don't forget free memory
  } else {
    //body_stream = reinterpret_cast<char *>(const_cast<char *>(in.ReadData(temp)));
    //body_length = len - sizeof(int16) - sizeof(int8);
  }
  //LOG_DEBUG2("UnpackStream after decrypt body length[%d]", body_length);

  
  //3.解包
  packet::DataInPacket in_body_stream(reinterpret_cast<char*>(body_stream), body_length);
  int8 type = in_body_stream.Read8();
  uint16 signature = in_body_stream.Read16();//原包中签名
  int16 operate_code = in_body_stream.Read16();
  int16 data_length = in_body_stream.Read16();
  int32 timestamp = in_body_stream.Read32();
  int64 session_id = in_body_stream.Read64();
  int32 reserved = in_body_stream.Read32();
  LOG_DEBUG2("===test type[%d], signature[%d], operate_code[%d], data_length[%d],timestamp[%d],session_id[%ld],reserved[%d]", 
        type,signature,operate_code,data_length,timestamp,session_id,reserved);


  //4.重新组包 TODO:如果包大小没有变化，可以不组
  packet_length = body_length + sizeof(int16) + sizeof(int8);
  packet::DataOutPacket checksum_packet(false, packet_length);
  checksum_packet.Write16(packet_length);
  checksum_packet.Write8(is_zip_encrypt);
  checksum_packet.WriteData(body_stream, body_length);
  //签名重置为0
  checksum_packet.Reset();
  checksum_packet.Skip( sizeof(int16) + sizeof(int8) + sizeof(int8) );
  checksum_packet.Write16(0);
  

  //5.checksum 校验
  uint16 server_signature;
  char* checksum_packetBody = reinterpret_cast<char *>(const_cast<char *>(checksum_packet.GetData()));
  checksum(checksum_packetBody, server_signature);
  //LOG_DEBUG2("Original signature[%d], myself signature[%d]", signature, server_signature);
  if(0 != signature && server_signature != signature) {
    LOG_ERROR2("Original signature[%d], server signature[%d], it's not correct packet.", signature, server_signature);
    r = false;
    goto UNPAKBACK;
  }

  
  //6.解析包体, 返回明文
  if (packet_length > PACKET_HEAD_LENGTH) {
    std::string body_stream_str = in_body_stream.ReadData(data_length, temp);
    LOG_DEBUG2("UnpackStream body_stream[%s]", body_stream_str.c_str());
    int error_code;
    std::string error_str;
    base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
        base_logic::IMPL_JSON);
    if (engine == NULL) {
      LOG_ERROR("engine create null");
      r = false;
      goto UNPAKBACK;
    }


    base_logic::DictionaryValue *value = (base_logic::DictionaryValue*) engine
        ->Deserialize(&body_stream_str, &error_code, &error_str);

    if (value == NULL) {
      LOG_ERROR2("json error, error_code[%d]", error_code);
      r = false;
      goto UNPAKBACK;
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

  r = true;
  goto UNPAKBACK;

UNPAKBACK: if (((is_zip_encrypt>>4) & 0xF) > 0 && body_stream) {
    delete body_stream;
    body_stream = NULL;
  }
  return r;
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
  //TODO: 参照 UnpackStream 写个解密返回包体函数
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


