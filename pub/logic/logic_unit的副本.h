//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_LOGIC_LOGIC_UNIT_H_
#define SWP_LOGIC_LOGIC_UNIT_H_

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>
#include "logic/base_values.h"
#include "basic/basictypes.h"
#include "basic/native_library.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_comm.h"
#include "basic/basic_util.h"

namespace logic {
/*
class SendUtils {
 public:
  static int32 SendFull(int socket, const char* buffer, size_t bytes);
  static bool SendBytes(int socket, const void* bytes, int32 len,
                        const char* file, int32 line);
  static bool SendMessage(int socket, struct PacketHead* packet,
                          const char* file, int32 line);
  static bool SendHeadMessage(int socket, int32 operate_code, int32 type,
                              int32 is_zip_encrypt, int64 session,
                              int32 reserved, const char* file, int32 line);

  static bool SendErronMessage(int socket, int32 operate_code, int32 type,
                               int32 is_zip_encrypt, int64 session,
                               int32 reserved, int32 error, const char* file,
                               int32 line);

  static struct threadrw_t* socket_lock_;
};*/


class SendUtils {
 public:
  SendUtils();
  virtual ~SendUtils();
 private:
  static SendUtils* instance_;
 public:
  static SendUtils* GetInstance();
  static void FreeInstance();

 public:
  int32 SendFull(int socket, const char* buffer, size_t bytes);
  bool SendBytes(int socket, const void* bytes, int32 len, const char* file,
                 int32 line);
  bool SendMessage(int socket, struct PacketHead* packet, const char* file,
                   int32 line);
  bool SendString(int socket, struct PacketHead* packet, const char* file,
                  int32 line);
 private:
  struct threadrw_t* socket_lock_;
};

class BaseValue {
 public:
  static base_logic::Value* Deserialize(std::string* str);

  static bool GetBigInteger(base_logic::DictionaryValue* dict_value,
                            const std::wstring& path, int64* out_value);

  static bool GetReal(base_logic::DictionaryValue* dict_value,
                      const std::wstring& path, double* out_value);

  static bool GetString(base_logic::DictionaryValue* dict_value,
                        const std::wstring& path, std::string* out_value);

  static bool GetString(base_logic::DictionaryValue* dict_value,
                        const std::wstring& path, std::wstring* out_value);

  static bool GetReal(base_logic::Value* value, double* out_value);

  static bool GetBigInteger(base_logic::Value* value, int64* out_value);

  static bool GetString(base_logic::Value* value, std::string* out_value);

 private:
  static bool StringToInt64(base_logic::Value* value, int64* out);
  static bool StringToDouble(base_logic::Value* value, double* out);

};


class SomeUtils {
 public:

  static void* GetLibraryFunction(const std::string& library_name,
                                  const std::string& func_name);

  static void CreateToken(const int64 uid, const std::string& password,
                          std::string* token);

  static bool VerifyToken(const int64 uid, const std::string& password,
                          const std::string& token);

  static inline int8 StringToIntChar(const char* str) {
    int8 intvalue = 0;
    base::BasicUtil::StringUtil::StringToCharInt(std::string(str), &intvalue);
    return intvalue;
  }
  static inline int16 StringToIntShort(const char* str) {
    int16 intvalue = 0;
    base::BasicUtil::StringUtil::StringToShortInt(std::string(str), &intvalue);
    return intvalue;
  }

  static inline std::string StringReplace(std::string& str,
                                          const std::string& old_value,
                                          const std::string& new_value) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos +=
        new_value.length()) {

      if ((pos = str.find(old_value, pos)) != std::string::npos)
        str.replace(pos, old_value.length(), new_value);
      else
        break;
    }
    return str;
  }

  static inline std::string StringReplaceUnit(std::string& str,
                                              const std::string& old_value,
                                              const std::string& new_value) {
    for (std::string::size_type pos(0); pos != std::string::npos; pos +=
        new_value.length()) {

      if ((pos = str.find(old_value, pos)) != std::string::npos) {
        str.replace(pos, old_value.length(), new_value);
        break;
      } else
        break;
    }
    return str;
  }

  static inline bool GetIPAddress(const int socket, std::string& ip,
                                  int& port) {
    struct sockaddr_in sa;
    socklen_t len;
    len = sizeof(sa);
    if (!getpeername(socket, (struct sockaddr *) &sa, &len)) {
      ip = inet_ntoa(sa.sin_addr);
      port = ntohs(sa.sin_port);
      return true;
    }
    return false;
  }
};

}

#define send_message(socket, packet) \
    //logic::SendUtils::SendMessage(socket, packet, __FILE__, __LINE__)\
  logic::SendUtils::GetInstance()->SendMessage(socket, packet, __FILE__, __LINE__)\


#define closelockconnect(socket) \
    shutdown(socket, SHUT_RDWR);
#endif /* LOGIC_UNIT_H_ */
