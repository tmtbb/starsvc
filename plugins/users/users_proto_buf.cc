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
    set_member_id(tmp_64);
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
  r = value->GetString(L"deviceId", &tmp);
  if (r)
    set_device_id(tmp);
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
  std::string device_id;


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

  r = value->GetString(L"deviceId", &device_id);
  if (r)
    set_device_id(device_id);
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



bool LoginWiXin::set_http_packet(base_logic::DictionaryValue* value) {
  bool r = false;
  std::string open_id;
  std::string device_id;

  if (value == NULL)
    return false;

  r = value->GetString(L"openId", &open_id);
  if (r)
    set_open_id(open_id);
  else
    return false;

  r = value->GetString(L"deviceId", &device_id);
  if (r)
    set_device_id(device_id);
  else
    return false;

  return true;
}


bool WXBindAccount::set_http_packet(base_logic::DictionaryValue* value) {
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

  r = value->GetString(L"openid", &tmp);
  if (r)
    set_openid(tmp);
  else
    return false;

  r = value->GetString(L"nickname", &tmp);
  if (r)
    set_nick_name(tmp);
  else
    return false;
  r = value->GetString(L"headerUrl", &tmp);
  if (r)
    set_head_url(tmp);
  else
    return false;

  int64 tmp_64;
  r = value->GetBigInteger(L"memberId", &tmp_64);
  if (r)
    set_member_id(tmp_64);
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
  r = value->GetString(L"deviceId", &tmp);
  if (r)
    set_device_id(tmp);
  else
    return false;
  //end 
  return true;
}

bool ModifyPwd::set_http_packet(base_logic::DictionaryValue* value) {
  std::string tmp;
  bool r = false;

  if (value == NULL)
    return false;

  int64 temp = 0, type;
  r = value->GetBigInteger(L"type", &type);
  if (r) 
    set_type(type);
  else
    return false;

  r = value->GetString(L"phone", &tmp);
  if (r) 
    set_phone (tmp);
  else
    set_phone ("130");
    //return false;
//type 0-设置密码1-重置密码
  r = value->GetString(L"vCode", &tmp);
  if (r) 
    set_vcode (tmp);
  else if (type == 1)
    return false;
  else 
    set_vcode ("123062");

  r = value->GetString(L"vToken", &tmp);
  if (r) 
    set_vtoken (tmp);
  else if (type == 1)
    return false;
  else 
    set_vtoken ("vToken");

  r = value->GetString(L"pwd", &tmp);
  if (r) 
    set_pwd (tmp);
  else
    return false;

  r = value->GetBigInteger(L"id", &temp);
  if (r) 
    set_uid(temp);
  else
    return false;
  r = value->GetBigInteger(L"timestamp", &temp);
  if (r) 
    set_timestamp(temp);
  else if (type == 1)
    return false;
  else 
    set_timestamp(12346);

  return true;
}

bool Certification::set_http_packet(base_logic::DictionaryValue* value) {


  if (value == NULL)
    return false;

  bool r = false;
  std::string tmp;
  int64 id;
  r = value->GetBigInteger(L"id", &id);
  if (r)
    set_uid(id);
  else
    return false;

  r = value->GetString(L"realname", &tmp);
  if (r)
    set_realname(tmp);
  else
    return false;

  r = value->GetString(L"id_card", &tmp);
  if (r)
    set_id_card(tmp);
  else
    return false;

  return true;
}

bool CheckAccountExistReq::set_http_packet(base_logic::DictionaryValue* value) {
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

}
}
