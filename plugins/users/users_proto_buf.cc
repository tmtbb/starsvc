// Copyright (c) 2016 The tourism Authors. All rights reserved.
// login_proto.cc

#include "users/users_proto_buf.h"

#include <typeinfo>

#include "logic/base_values.h"
#include "basic/md5sum.h"

#include "comm/comm_head.h"
#include "logic/logic_comm.h"

namespace users_logic {

namespace net_request {

bool RegisterVerfiycode::set_http_packet(base_logic::DictionaryValue* value) {
  std::string phone;
  bool r = false;

  if (value == NULL)
    return false;

  r = value->GetString(L"phone", &phone);
  if (r)
    set_phone (phone);
  else
    return false;

  return true;
}



bool RegisterAccount::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  std::string verify_code;
  int64 timestamp;
  std::string verify_token;
  std::string phone_num;
  std::string passwd;

  if (value == NULL)
    return false;

  r = value->GetString(L"vCode", &verify_code);
  if (r)
    set_verify_code(verify_code);
  else
    return false;

  r = value->GetBigInteger(L"timeStamp", &timestamp);
  if (r)
    set_timestamp(timestamp);
  else
    return false;

  r = value->GetString(L"vToken", &verify_token);
  if (r)
    set_verify_token(verify_token);
  else
    return false;

  r = value->GetString(L"phone", &phone_num);
  if (r)
    set_phone_num(phone_num);
  else
    return false;

  r = value->GetString(L"pwd", &passwd);
  if (r)
    set_passwd(passwd);
  else
    return false;

  //add bytw
  std::string tmp;
  int64 tmp_64;
  r = value->GetBigInteger(L"memberId", &tmp_64);
  if (r)
    set_memberid(tmp_64);
  else
    return false;

  r = value->GetString(L"agentId", &tmp);
  if (r)
    set_agentid(tmp);
  else
    return false;

  r = value->GetString(L"recommend", &tmp);
  if (r)
    set_recommend(tmp);
  else
    return false;
  //end 
  return true;
}

bool UserAccount::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  int64 uid = 0;
  std::string token;

  if (value == NULL)
    return false;

  r = value->GetBigInteger(L"id", &uid);
  if (r)
    set_uid(uid);
  else
    return false;

  r = value->GetString(L"token", &token);
  if (r)
    set_token(token);
  else
    return false;

  return true;
}

bool LoginAccount::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  std::string passwd;
  std::string phone_num;
  std::string token;

  if (value == NULL)
    return false;

  r = value->GetString(L"pwd", &passwd);
  if (r)
    set_passwd(passwd);
  else
    return false;

  r = value->GetString(L"phone", &phone_num);
  if (r)
    set_phone_num(phone_num);
  else
    return false;

  /*if (phone_num.length() < 11) {
   LOG_ERROR("phone is wrong");
   err = PHONE_NUM_ERR;
   break;
   }*/

  r = value->GetString(L"token", &token);
  //if (r)
  //set_token(token);
  //else
  //return false;

  return true;
}

}
}
