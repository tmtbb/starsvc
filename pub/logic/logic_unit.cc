//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include <sys/socket.h>
#include <time.h>
#include <sstream>
#include <string>
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"
#include "basic/md5sum.h"
#include "basic/radom_in.h"
#include "comm/user_info.h"
#include <cerrno>
#include "comm/comm_head.h"
/*#include "login/errno.h"
 #include "user/errno.h"
 #include "quotations/errno.h"
 #include "trades/errno.h"
 #include "history/errno.h"
 #include "user/user_opcode.h"*/
#include "logic/logic_comm.h"

namespace logic {

void* SomeUtils::GetLibraryFunction(const std::string& library_name,
                                    const std::string& func_name) {
  void* engine = NULL;
  basic::libhandle handle_lancher = NULL;
  handle_lancher = basic::load_native_library(library_name.c_str());

  if (handle_lancher == NULL) {
    LOG_ERROR2("Cant't load path %s", library_name.c_str());
    return false;
  }

  engine = basic::get_function_pointer(handle_lancher, func_name.c_str());

  if (engine == NULL) {
    LOG_ERROR2("Can't find %s", func_name.c_str());
    return NULL;
  }
  return engine;
}

void SomeUtils::CreateToken(const int64 uid, const std::string& password,
                            std::string* token) {
  std::stringstream os;
  os << uid << base::SysRadom::GetInstance()->GetRandomID() << password;
  base::MD5Sum md5(os.str());
  (*token) = md5.GetHash();
}

void SomeUtils::CreateToken(const int64 uid, const int64 time, const std::string& password,
                            std::string* token) {
  std::stringstream os;
  os << uid << time << password;
  base::MD5Sum md5(os.str());
  (*token) = md5.GetHash();
}

bool SomeUtils::VerifyToken(const int64 uid, const std::string& token) {
  return true;
}

int SomeUtils::SplitString(const std::string &in, const char ch, std::vector<std::string> &out) {
  if (in.size() == 0)
    return 0;
  out.clear();

  size_t old_pos = 0, new_pos = 0;
  std::string tmp;
  while (true) {
    new_pos = in.find(ch, old_pos);
    if (new_pos != std::string::npos) {
      tmp = in.substr(old_pos, new_pos - old_pos);
      if (tmp != "")
        out.push_back(tmp);
      old_pos = ++new_pos;
    } else if (old_pos <= in.size()) {
      tmp = in.substr(old_pos);
      if (tmp != "")
        out.push_back(tmp);
      break;
    } else
      break;
  }

  return 0;
}

share::DataShareMgr* SomeUtils::GetShareDataMgr() {
  basic::libhandle handle = NULL;
  handle = basic::load_native_library("./data.so");
  if (handle == NULL) {
    LOG_ERROR("Can't load path data.so\n");
  } LOG_MSG("load data.so success");
  share::DataShareMgr* (*pengine)(void);
  pengine = (share::DataShareMgr *(*)(
      void))basic::get_function_pointer(handle, "GetDataShareMgr");if
(  pengine==NULL) {
    LOG_ERROR("Can't find GetDataShareMgr\n");
    return false;
  }
  return (*pengine)();
}

bool SomeUtils::VerifyToken(PacketHead* packet) {
  base_logic::DictionaryValue* dic = ((struct PacketControl*) packet)->body_;
  int16 operate_code = packet->operate_code;
  if (operate_code == /*OBTAIN_VERIFY_CODE_REQ*/1029)
    return true;
  if (dic) {
    bool r;
    int64 uid = 0;
    r = dic->GetBigInteger(L"id", &uid);
    if (r) {
      std::string token;
      r = dic->GetString(L"token", &token);
      if (r) {
        UserInfo* user_info = GetShareDataMgr()->GetUser(uid);  //////NULL!
        if (user_info) {
          if (user_info->token() == token)
            return true;
          else {
            LOG_ERROR2("verify token not match id:%lld opcode:%d", uid, operate_code);
            return false;
          }
        } else {
          LOG_ERROR2("verify token not found UserInfo id:%lld", uid);
          return false;
        }
      } else {
        LOG_ERROR("verify token parse token error");
        return false;
      }
    } else {
      if (operate_code == /*REGISTER_ACCOUNT_REQ*/3001
          || operate_code == /*USER_LOGIN_REQ*/3003
          || operate_code == /*WXPAY_SERVER_REQ*/1037)
        return true;
      else {
        LOG_ERROR("verify token no user id found");
        return false;
      }
    }
  } else {
    std::string result;
    LOG_ERROR2("verify token body NULL, opcode:%d", operate_code);
    return false;
  }
}

base_logic::Value* BaseValue::Deserialize(std::string* str) {
  int error_code;
  std::string error_str;
  base_logic::Value* value;
  base_logic::ValueSerializer* engine = base_logic::ValueSerializer::Create(
      base_logic::IMPL_JSON);
  if (engine == NULL) {
    LOG_ERROR("engine create null");
    return false;
  }
  value = engine->Deserialize(str, &error_code, &error_str);
  if (value == NULL) {
    LOG_ERROR2("error_code %d, error_str %s", error_code, error_str.c_str());
  }
  return value;
}

bool BaseValue::StringToInt64(base_logic::Value* value, int64* out) {
  std::string temp_str;
  bool r = value->GetAsString(&temp_str);
  if (!r)
    return false;
  r = base::BasicUtil::StringUtil::StringToInt64(temp_str, out);
  return r;
}

bool BaseValue::StringToDouble(base_logic::Value* value, double* out) {
  std::string temp_str;
  bool r = value->GetAsString(&temp_str);
  if (!r)
    return false;
  r = base::BasicUtil::StringUtil::StringToDouble(temp_str, out);
  return r;
}

//////////base_logic::Value
bool BaseValue::GetBigInteger(base_logic::Value* value, int64* out_value) {
  //string in32 可转 int64
  bool r = false;
  switch (value->GetType()) {
    case base_logic::Value::TYPE_BIG_INTEGER: {
      r = value->GetAsBigInteger(out_value);
      break;
    }
    case base_logic::Value::TYPE_STRING: {
      r = StringToInt64(value, out_value);
      break;
    }
    default:
      r = false;
      break;
  }
  return r;
}

bool BaseValue::GetReal(base_logic::Value* value, double* out_value) {
  //string 可转double
  bool r = false;
  switch (value->GetType()) {
    case base_logic::Value::TYPE_REAL: {
      r = value->GetAsReal(out_value);
      break;
    }

    case base_logic::Value::TYPE_STRING: {
      r = StringToDouble(value, out_value);
      break;
    }
    default:
      r = false;
      break;
  }
  return r;
}

bool BaseValue::GetString(base_logic::Value* value, std::string* out_value) {
  bool r = false;
  switch (value->GetType()) {
    case base_logic::Value::TYPE_STRING: {
      r = value->GetAsString(out_value);
      break;
    }
    default:
      r = false;
      break;
  }
  if (value) {
    delete value;
    value = NULL;
  }
  return r;
}

//////////base_logic::DictionaryValue
bool BaseValue::GetReal(base_logic::DictionaryValue* dict_value,
                        const std::wstring& path, double* out_value) {
  //string 可转成 double
  bool r = false;
  base_logic::Value* temp_value = NULL;
  r = dict_value->Get(path, &temp_value);
  if (!r) {
    LOG_ERROR2("path %s is error", path.c_str());
    return false;
  }
  r = GetReal(temp_value, out_value);
  if (temp_value) {
    delete temp_value;
    temp_value = NULL;
  }
  return r;
}

bool BaseValue::GetString(base_logic::DictionaryValue* dict_value,
                          const std::wstring& path, std::string* out_value) {
  //int32 int64 double bool 都可转成 string
  bool r = false;
  base_logic::Value* temp_value;
  r = dict_value->Get(path, &temp_value);
  if (!r) {
    LOG_ERROR2("path %s is error", path.c_str());
    return false;
  }

  GetString(temp_value, out_value);
  return r;
}

bool BaseValue::GetBigInteger(base_logic::DictionaryValue* dict_value,
                              const std::wstring& path, int64* out_value) {

  //String,INT32,double 可转 int64
  bool r = false;
  base_logic::Value* temp_value;
  r = dict_value->Get(path, &temp_value);
  if (!r) {
    LOG_ERROR2("path %s is error", path.c_str());
    return false;
  }

  r = GetBigInteger(temp_value, out_value);
  if (temp_value) {
    delete temp_value;
    temp_value = NULL;
  }
  return r;
}

SendUtils *SendUtils::instance_ = NULL;

SendUtils::SendUtils() {
  InitThreadrw(&socket_lock_);
}
SendUtils::~SendUtils() {
  DeinitThreadrw(socket_lock_);
}

SendUtils *SendUtils::GetInstance() {
  if (instance_ == NULL)
    instance_ = new SendUtils();
  return instance_;
}

void SendUtils::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

int32 SendUtils::SendFull(int socket, const char *buffer, size_t nbytes) {
  base_logic::WLockGd lk(socket_lock_);
  ssize_t amt = 0;
  ssize_t total = 0;
  const char *buf = buffer;
  do {
    amt = nbytes;
    amt = send(socket, buf, amt, 0);
    if (-1 == amt) {
      if (11 == errno)
        continue;
      else {
        break;
      }
    }
    buf = buf + amt;
    nbytes -= amt;
    total += amt;
  } while (nbytes > 0);
  return static_cast<int32>(amt == -1 ? amt : total);
}

bool SendUtils::SendBytes(int socket, const void* bytes, int32 len,
                          const char* file, int32 line) {
  if (socket <= 0 || bytes == NULL || len <= 0)
    return false;

  int32 ret = SendFull(socket, reinterpret_cast<const char*>(bytes), len);
  if (ret != len) {
    LOG_ERROR("Send bytes failed");
    return false;
  }
  return true;
}

bool SendUtils::SendMessage(int socket, struct PacketHead* packet,
                            const char* file, int32 line) {
  bool r;
  void *packet_stream = NULL;
  int32_t packet_stream_length = 0;
  int ret = 0;
  bool r1 = false;
  if (socket <= 0 || packet == NULL)
    return false;

  if (net::PacketProsess::PacketStream(packet, &packet_stream,
                                       &packet_stream_length) == false) {
    LOG_ERROR2("Call PackStream failed in %s:%d", file, line);
    r = false;
    goto MEMFREE;
  }

  ret = SendFull(socket, reinterpret_cast<char*>(packet_stream),
                 packet_stream_length);
  //net::PacketProsess::HexEncode(packet_stream, packet_stream_length);
  if (ret != packet_stream_length) {
    LOG_ERROR2("Sent msg failed in %s:%d", file, line);
    r = false;
    goto MEMFREE;
  } else {
    r = true;
    goto MEMFREE;
  }

  MEMFREE: char* stream = reinterpret_cast<char*>(packet_stream);
  if (stream) {
    //delete[] stream;
    free((void*) stream);
    stream = NULL;
  }
  return r;
}
const static std::map<int, const char*>::value_type init_value[] = { std::map<
    int, const char*>::value_type(-100, "格式有误"),
/* std::map<int, const char*>::value_type(TOKEN_ERR_OR_NOTEXIST, "token错误或不存在"),
 std::map<int, const char*>::value_type(SQL_EXEC_ERR, "sql执行错误"),
 std::map<int, const char*>::value_type(VERIFY_CODE_OVERDUE, "验证码过期"),
 std::map<int, const char*>::value_type(VERIFY_CODE_ERR, "验证码错误"),

 std::map<int, const char*>::value_type(NO_USER, "没有此用户"),
 std::map<int, const char*>::value_type(WX_PLACE_ORDER_ERR, "微信下单失败"),
 std::map<int, const char*>::value_type(CREDIT_STATUS_ERR, "充值列表状态参数错误"),
 std::map<int, const char*>::value_type(PHONE_NUM_ERR, "不是有效手机号"),
 std::map<int, const char*>::value_type(PHONE_OR_PASSWD_ERR, "手机号或密码错误"),
 std::map<int, const char*>::value_type(NO_HAVE_REAL_TIME_DATA, "没有对应报价"),
 std::map<int, const char*>::value_type(NO_HAVE_TIME_LINE_DATA, "没有对应的行情数据"),
 std::map<int, const char*>::value_type(NO_HAVE_KCHART_DATA, "没有对应的K线数据"),
 std::map<int, const char*>::value_type(NO_HAVE_PLATFORM, "平台没有交易的商品"),
 std::map<int, const char*>::value_type(NO_HAVE_TRADES_GOODS, "交易的商品不存在"),
 std::map<int, const char*>::value_type(NO_HAVE_CHARGE, "扣费失败"),
 std::map<int, const char*>::value_type(NO_HAVE_GOODS_DATA, "没有商品数据"),
 std::map<int, const char*>::value_type(NO_HAVE_HISTROY_DATA, "没有对应的历史数据")*/
};
std::map<int, const char*> error_code_msgs(
    init_value, init_value + sizeof(init_value) / sizeof(init_value[0]));
}  //  namespace logic
