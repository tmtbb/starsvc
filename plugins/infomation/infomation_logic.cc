#include "infomation_logic.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include "infomation/errno.h"
#include <string>
#include "operator_code.h"

#define DEFAULT_CONFIG_PATH "./plugins/infomation/infomation_config.xml"

#define TIME_DISTRIBUTION_TASK 10001
#define NO_CHECK_TOKEN_ERRNO -303

namespace infomation_logic {

Infomationlogic *Infomationlogic::instance_ = NULL;

Infomationlogic::Infomationlogic() {
  Init();
}

Infomationlogic::~Infomationlogic() {
  if(sqldb != NULL){
  	delete sqldb;
  	sqldb = NULL;
  }
}

bool Infomationlogic::Init() {
  bool r = false;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG_ERROR("imlogic config init error");
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG_ERROR("login config load error");
    return false;
  }
  sqldb = new infomation_mysql::Infomation_Mysql(config);
  return true;
}

Infomationlogic *Infomationlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Infomationlogic();
  return instance_;
}

void Infomationlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Infomationlogic::OnInfomationConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool Infomationlogic::OnInfomationMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {                               
  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;
  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    send_error(socket, ERROR_TYPE, ERROR_TYPE, FORMAT_ERRNO);
    return false;
  }
  switch (packet->operate_code) {
    case R_STARINFOLIST_GET: {
	  GetStarinfoList(srv,socket,packet);
      break;
    }
	case R_STARINFOLIST_ADD:{
	  AddStarinfo(srv,socket,packet);
	  break;
	}
	//»ñÈ¡ÒÑ¹ºÃ÷ÐÇÁÐ±í
	case R_GET_ORDERSTAR:{
	  GetorderStarinfo(srv,socket,packet);
	  break;
	}
	//»ñÈ¡Ã÷ÐÇ×ÊÑ¶
	case R_GET_STARNEWS:{
	  Getstarnews(srv,socket,packet);
	  break;
	}
	//»ñÈ¡bannerÐÅÏ¢
	case R_GET_BANNER:{
	  Getbannerlist(srv,socket,packet);
	  break;
	}
	//»ñÈ¡·ÛË¿ÆÀÂÛ
	case R_GET_FANSCOMMENT:{
	  Getfanscomment(srv,socket,packet);
	  break;
	}
	//»ñÈ¡Ã÷ÐÇ·þÎñÁÐ±í
	case R_GET_STAR_SERVICE:{
	  GetStarService(srv,socket,packet);
	  break;
	}
	//ÓÃ»§¶©¹ºÃ÷ÐÇ·þÎñ
	case R_ORDER_STAR_SERVICE:{
	  AddUserOrderStarService(srv,socket,packet);
	  break;
	}
	//»ñÈ¡ÓÃ»§¶©¹ºÃ÷ÐÇÊý
	case R_ORDER_STAR_NUM:{
	  GetUserStarNum(srv,socket,packet);
	  break;
	}
  //»ñÈ¡ÓÃ»§Ã÷ÐÇÊ±¼ä
	case R_ORDER_STAR_TIME:{
	  GetUserStarTime(srv,socket,packet);
	  break;
	}
  //»ñÈ¡Ã÷ÐÇÊ±¼ä
	case R_STAR_TIME:{
	  GetStarTime(srv,socket,packet);
	  break;
	}
  //获取用户约见明星详情
  case R_USER_STAR_MEETINFO:{
    GetUserStarMeetinfo(srv,socket,packet);
    break;
  }
  //获取某个明星信息，与获取明星经验配合使用
  case R_STAR_INFO:{
    GetOneStarInfo(srv,socket,packet);
    break;
  }
  case R_GET_SYS_PARAM_VALUE:{
    GetSysParamValue(srv,socket,packet);
    break;
  }
  case R_GET_BARRAGE_INFO:{
    GetBarrageInfo(srv,socket,packet);
    break;
  }
  case R_GET_USER_AND_STAR_TIME:{
    GetUserAndStarTime(srv,socket,packet);
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

bool Infomationlogic::GetUserAndStarTime(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);
  int64 uid;
  std::string starcode;
  bool r1 = packet_recv->body_->GetBigInteger(L"uid",&uid);
  bool r2 = packet_recv->body_->GetString(L"star_code",&starcode);
  if(!r1 || !r2){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  int64 starowntime = 0, userstartime = 0;
  if(!sqldb->getuserandstartime(uid,starcode,userstartime,starowntime)){
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }
  
  
  //返回
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_GET_USER_AND_STAR_TIME, INFO_TYPE, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", 1);
  dic.SetBigInteger(L"star_own_time", starowntime);
  dic.SetBigInteger(L"user_star_time", userstartime);
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 
  return true;
}

bool Infomationlogic::GetBarrageInfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  int64 begin,end;
  bool r1 = packet_recv->body_->GetBigInteger(L"pos",&begin);
  bool r2 = packet_recv->body_->GetBigInteger(L"count",&end);
  if(!r1 || !r2){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  base_logic::ListValue* ret_list;
  end += begin;
  if(!sqldb->getbarragedata(begin,end,ret_list)){
    send_error(socket, ERROR_TYPE, NO_BARRAGE_DATA, packet->session_id);
    return false;
  }
  
  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
  dic->Set(L"barrage_info", ret_list);
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_BARRAGE_INFO, INFO_TYPE, 0,packet->session_id, 0);
  packet_reply.body_ = dic;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetSysParamValue(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string paramcode, paramvalue("");
  bool r = packet_recv->body_->GetString(L"param_code",&paramcode);
  if(!r){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  if(!sqldb->OngetSysParamValue(paramcode, paramvalue)){
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }
  
  base_logic::DictionaryValue* dic = new base_logic::DictionaryValue();
#if defined(STAR_CLOUD)
  base_logic::StringValue* ret = new base_logic::StringValue(logic::SomeUtils::FindPicTail(paramvalue));
#else
  base_logic::StringValue* ret = new base_logic::StringValue(paramvalue);
#endif
  dic->Set("param_value", ret);

  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_SYS_PARAM_VALUE, INFO_TYPE, 0, packet->session_id, 0);
  packet_reply.body_ = dic;
  send_message(socket,&packet_reply);
  return true;
}

bool Infomationlogic::GetOneStarInfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string starcode;
  bool r = packet_recv->body_->GetString(L"star_code",&starcode);
  if(!r){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  DicValue* dic = new DicValue();
  if(!sqldb->OngetOnestarInfo(starcode,dic)){
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
    return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STAR_INFO, INFO_TYPE, 0,packet->session_id, 0);
  packet_reply.body_ = dic;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetUserStarMeetinfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  int64 uid, pos, count;
  bool r = packet_recv->body_->GetBigInteger(L"uid",&uid);
  bool r2 = packet_recv->body_->GetBigInteger(L"pos",&pos);
  bool r3 = packet_recv->body_->GetBigInteger(L"count",&count);
  if(!r || !r2 || !r3){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  DicValue ret_list;
  if(!sqldb->getUserStarMeetinfo(uid,pos,count,ret_list)){
    send_error(socket, ERROR_TYPE, NO_USER_STAR_MEETINFO_ERR, packet->session_id);
    return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_USER_STAR_MEETINFO, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetStarTime(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);
  std::string starcode;
  bool r = packet_recv->body_->GetString(L"starcode",&starcode);
  if(!r){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }
  
  int64 ltime = 0;
  if(!sqldb->getstartime(starcode,ltime)){
	  send_error(socket, ERROR_TYPE, NO_STAR_TIMER_DATA, packet->session_id);
	  return false;
  }
  
  //·¢ËÍÐÅÏ¢
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_STAR_TIME, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", 1);
  dic.SetBigInteger(L"star_time", ltime);
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 
  return true;
}

bool Infomationlogic::GetUserStarTime(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  int64 uid;
  std::string starcode;
  bool r1 = packet_recv->body_->GetBigInteger(L"uid",&uid);
  bool r2 = packet_recv->body_->GetString(L"starcode",&starcode);
  if(!r1 || !r2){
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }
  
  int64 ltime = 0;
  if(!sqldb->getuserstartime(uid,starcode,ltime)){
	  send_error(socket, ERROR_TYPE, NO_ORDER_STAR_DATA, packet->session_id);
	  return false;
  }
  
  //·¢ËÍÐÅÏ¢
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_ORDER_STAR_TIME, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", 1);
  dic.SetBigInteger(L"star_time", ltime);
  
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 

  return true;
}

bool Infomationlogic::GetUserStarNum(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  int64 uid;
  bool r = packet_recv->body_->GetBigInteger(L"uid",&uid);
  if(!r){
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }
  
  int64 amount;
  if(!sqldb->getuserstaramount(uid,amount)){
	  send_error(socket, ERROR_TYPE, NO_ORDER_STAR_DATA, packet->session_id);
	  return false;
  }
  
  //·¢ËÍÐÅÏ¢
  struct PacketControl packet_control_ack; 
  MAKE_HEAD(packet_control_ack,S_ORDER_STAR_NUM, 1, 0, packet->session_id, 0);
  base_logic::DictionaryValue dic; 
  dic.SetInteger(L"result", 1);
  dic.SetBigInteger(L"amount", amount);
  
  packet_control_ack.body_ = &dic; 
  send_message(socket, &packet_control_ack); 

  return true;
}

bool Infomationlogic::AddUserOrderStarService(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  int64 uid,mid;
  std::string starcode,cityname,appointtime,comment;
  int64 meettype;
  bool r1 = packet_recv->body_->GetBigInteger(L"uid",&uid);
  bool r2 = packet_recv->body_->GetString(L"starcode",&starcode);
  bool r3 = packet_recv->body_->GetBigInteger(L"mid",&mid);
  bool r4 = packet_recv->body_->GetString(L"city_name",&cityname);
  bool r5 = packet_recv->body_->GetString(L"appoint_time",&appointtime);
  bool r6 = packet_recv->body_->GetBigInteger(L"meet_type",&meettype);
  bool r7 = packet_recv->body_->GetString(L"comment",&comment);
  LOG_DEBUG2("uid[%ld],starcode[%s],mid[%d],cityname[%s ],appointtime[%s],meettype[%ld],comment[%s]",
  						uid,starcode.c_str(),mid,cityname.c_str(),appointtime.c_str(),meettype,comment.c_str());
  if(!r1 || !r2 || !r3 || !r4 || !r5 || !r6 || !r7){
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  bool r = sqldb->userorderstarservice(uid,starcode,mid,cityname,appointtime,meettype,comment);
  if(!r){
    send_error(socket, ERROR_TYPE, NO_DATABASE_ERR, packet->session_id);
	  return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_ORDER_STAR_SERVICE, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  if (!r) {
    ret.SetInteger(L"result", 0);
  }
  else{
    ret.SetInteger(L"result", 1); 
  }
  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetStarService(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string starcode;
  bool r = packet_recv->body_->GetString(L"starcode",&starcode);
  if(!r){
	  send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	  return false;
  }

  DicValue ret_list;
  if(!sqldb->getstarservicelist(starcode,ret_list)){
	  send_error(socket, ERROR_TYPE, NO_STAR_SERVICE_DATA, packet->session_id);
	  return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_GET_STAR_SERVICE, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::Getfanscomment(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string starcode;
  int64 startnum;
  int64 endnum;
  bool r1 = packet_recv->body_->GetString(L"starcode",&starcode);
  bool r2 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
  bool r3 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
  bool r = r1  && r2 && r3;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  DicValue ret_list;
  if(!sqldb->getfanscomments(starcode,ret_list,startnum,endnum)){
	send_error(socket, ERROR_TYPE, NO_STAR_SERVICE_DATA, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}
bool Infomationlogic::Getbannerlist(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  int64 all;
  bool r1 = packet_recv->body_->GetString(L"code",&code);
  bool r2 = packet_recv->body_->GetBigInteger(L"all",&all);
  bool r = r1  && r2;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  DicValue ret_list;
  if(!sqldb->getbannerinfo(code,ret_list,all)){
	send_error(socket, ERROR_TYPE, NO_BANNER_INFO_DATA, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}
bool Infomationlogic::Getstarnews(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string name;
  int64 all;
  int64 startnum;
  int64 endnum;
  bool r1 = packet_recv->body_->GetString(L"code",&code);
  bool r2 = packet_recv->body_->GetString(L"name",&name);
  bool r3 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
  bool r4 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
  bool r5 = packet_recv->body_->GetBigInteger(L"all",&all);
  bool r = r1  && r2 && r3 && r4 && r5;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  DicValue ret_list;
  if(!sqldb->getstarnews(code,name,ret_list,startnum,endnum,all)){
	send_error(socket, ERROR_TYPE, NO_STAR_NEWS_DATA, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetorderStarinfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string phone;

  bool r1 = packet_recv->body_->GetString(L"code",&code);
  bool r2 = packet_recv->body_->GetString(L"phone",&phone);

  bool r = r1  && r2;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  DicValue ret_list;
  if(!sqldb->getorderstarinfo(code,phone,ret_list)){
	send_error(socket, ERROR_TYPE, NO_ORDER_STAR_INFO_DATA, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}

bool Infomationlogic::GetStarinfoList(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string phone;
  int64 all;
  bool r1 = packet_recv->body_->GetString(L"code",&code);
  bool r2 = packet_recv->body_->GetString(L"phone",&phone);
  bool r3 = packet_recv->body_->GetBigInteger(L"all",&all);
  
  bool r = r1  && r2 && r3;
  if(!r){
    send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
    return false;
  }

  DicValue ret_list;
  if(!sqldb->getstarinfo(code,phone,ret_list,all)){
    send_error(socket, ERROR_TYPE, NO_ORDER_STAR_DATA, packet->session_id);
    return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_GET, INFO_TYPE, 0, packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}
bool Infomationlogic::AddStarinfo(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }

  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

  std::string code;
  std::string phone;
  std::string name;
  int64 gender;
  std::string brief_url;
  double price;
  std::string accid;
  std::string starpic;
  bool r1 = packet_recv->body_->GetString(L"name",&name);
  bool r2 = packet_recv->body_->GetString(L"code",&code);
  bool r3 = packet_recv->body_->GetString(L"phone",&phone);
  bool r4 = packet_recv->body_->GetBigInteger(L"gender",&gender);
  bool r5 = packet_recv->body_->GetString(L"brief_url",&brief_url);
  bool r6 = packet_recv->body_->GetReal(L"price",&price);
  bool r7 = packet_recv->body_->GetString(L"accid",&accid);
  bool r8 = packet_recv->body_->GetString(L"starpic",&starpic);
  bool r = r1  && r2&&r3&&r4&&r5&&r6&&r7&&r8;
  if(!r){
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  
  if(!sqldb->addstarinfo(code,phone,name,gender,brief_url,price,accid,starpic)){
	send_error(socket, ERROR_TYPE, NO_ADD_STAR_INFO_ERR, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_STARINFOLIST_ADD, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::DictionaryValue ret;
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret.Set(L"result",result);
  packet_reply.body_ = &ret;
  send_message(socket,&packet_reply);
 
  return true;
}

bool Infomationlogic::OnInfomationClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Infomationlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Infomationlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Infomationlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "infamation", TIME_DISTRIBUTION_TASK, 3, -1);
    }
  }
  return true;
}

bool Infomationlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    default:
      break;
  }
  return true;
}
}  // namespace
