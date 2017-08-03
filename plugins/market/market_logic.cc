#include "market_logic.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
#include "operator_code.h"
#include "error.h"
#include <sstream>


#define DEFAULT_CONFIG_PATH "./plugins/market/market_config.xml"

#define TIME_RELOADSTARINFO_TASK 10001

namespace market_logic {

Marketlogic *Marketlogic::instance_ = NULL;

Marketlogic::Marketlogic() {
  Init();
}

Marketlogic::~Marketlogic() {
  if(sqldb != NULL){
  	delete sqldb;
  	sqldb = NULL;
  }
  DeinitThreadrw(lock_);
}

bool Marketlogic::Init() {
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
  sqldb = new market_mysql::Market_Mysql(config);
  InitThreadrw(&lock_);

  InitStarInfo();
  return true;
}

Marketlogic *Marketlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new Marketlogic();
  return instance_;
}

void Marketlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool Marketlogic::OnInfomationConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool Marketlogic::OnInfomationMessage(struct server *srv, const int socket,
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
  	//获取行情分类
    case R_MARKETTYPES_GET: {
	  getmarkettypes(srv,socket,packet);
      break;
    }
	//获取分类明细行情
	case R_MARKSTARLIST_GET:{
	  getmarketstarlist(srv,socket,packet);
	  break;
	}
	//获取明星简介
	case R_MARKSTARBRIEF_GET:{
	  getstarbrief(srv,socket,packet);
	  break;
	}
	//获取明星经历
	case R_MARKETSTAREX_GET:{
	  getstarexperience(srv,socket,packet);
	  break;
	}
	//获取明星成就
	case R_MARKETSTARACHIV_GET:{
	  //getstarachive(srv,socket,packet);
	  break;
	}
	case R_SEARCHSTAR_INFO:{
	  searchsatr(srv,socket,packet);
	  break;
	}
	//获取自选明星列表
	case R_MARKSTAROPTION_GET:{
	  getoptionstarlist(srv,socket,packet);
	  break;
	}
	//添加明星自选
	case R_MARKSTAROPTION_ADD:{
	  addoptionstar(srv,socket,packet);
	  break;
	}
	//获取明星分时图
	case R_MARKSTARPRICE_GET:{
	  getmarketstarprice(srv,socket,packet);
	  break;
	}
	//明星转让榜单
	case R_MARKSTARTRANSFER_GET:{
	  getmarketstartransfer(srv,socket,packet);
	  break;
	}
	//明星求购榜单
	case R_MARKSTARSEEK_GET:{
	  getmarketstarseek(srv,socket,packet);
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
bool Marketlogic::getmarketstartransfer(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string starcode;
	  int64 startnum;
	  int64 endnum;
      bool r = packet_recv->body_->GetString(L"starcode",&starcode);
	  bool r1 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
	  bool r2 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
	  if(!(r&&r1&&r2)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getmarketstartransfer(starcode,startnum,endnum,ret_list)){
		send_error(socket, ERROR_TYPE, NO_GET_MARKET_STAR_TRANSFER, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getmarketstarseek(struct server* srv,int socket ,struct PacketHead* packet){
	  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string starcode;
	  int64 startnum;
	  int64 endnum;
      bool r = packet_recv->body_->GetString(L"starcode",&starcode);
	  bool r1 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
	  bool r2 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
	  if(!(r&&r1&&r2)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getmarketstarseek(starcode,startnum,endnum,ret_list)){
		send_error(socket, ERROR_TYPE, NO_GET_MARKET_STAR_SEEK, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getmarketstarprice(struct server* srv,int socket ,struct PacketHead* packet){
	  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string starcode;

	  bool r1 = packet_recv->body_->GetString(L"starcode",&starcode);
	  if(!r1){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  DicValue ret_list1;
	  
	  std::stringstream ss;
	  std::string curtime;
	  ss<<time(NULL);
	  ss>>curtime;
	  int64 _time;
	  base::BasicUtil::StringUtil::StringToInt64(curtime,&_time);
	  
	  base_logic::ListValue* stastic1 = new base_logic::ListValue();
	  base_logic::ListValue* stastic2 = new base_logic::ListValue();
	  for(int i = 0 ;i< 40 ;i++){
	  	    int key = rand()%20;
			base_logic::DictionaryValue* tmp = new base_logic::DictionaryValue();
			tmp->SetBigInteger(L"timestamp",_time-i*60*10);
			tmp->SetInteger(L"value",key);
			if(i<20){
				stastic1->Append((base_logic::Value*) tmp);
			}else
				stastic2->Append((base_logic::Value*) tmp);
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKSTARPRICE_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  ret_list.Set(L"stastic",(base_logic::Value*)stastic1);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);

	  struct PacketControl packet_reply1;
	  MAKE_HEAD(packet_reply1, S_MARKSTARPRICE_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result1 = new base_logic::FundamentalValue(1);
	  ret_list1.Set(L"result",result1);
	  ret_list1.Set(L"stastic",(base_logic::Value*)stastic2);
	  packet_reply1.body_ = &ret_list1;
	  send_message(socket,&packet_reply1);
	  return true;
}
bool Marketlogic::addoptionstar(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string phone;
	  std::string starcode;

      bool r = packet_recv->body_->GetString(L"phone",&phone);
	  bool r1 = packet_recv->body_->GetString(L"starcode",&starcode);
	  if(!r && !r1){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->addoptionstar(phone,starcode)){
		send_error(socket, ERROR_TYPE, NO_ADD_OPTION_STAR, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}

bool Marketlogic::getoptionstarlist(struct server* srv,int socket ,struct PacketHead* packet){
	  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string phone;
	  int64 startnum;
	  int64 endnum;
      bool r = packet_recv->body_->GetString(L"phone",&phone);
	  bool r1 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
	  bool r2 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getoptionstarlist(phone,startnum,endnum,ret_list)){
		send_error(socket, ERROR_TYPE, NO_GET_OPTION_STAR_LIST, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getstarachive(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getstarachive(code,ret_list)){
		send_error(socket, ERROR_TYPE, NO_GET_STAR_ACHIVE, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getstarexperience(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getstarexperience(code,ret_list)){
		send_error(socket, ERROR_TYPE, NO_GET_STAR_EXPERIENCE, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::searchsatr(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
	  std::string name;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r || code.length() == 0){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->searchstarlist(code,ret_list)){
		send_error(socket, ERROR_TYPE, NO_SEARCH_STAR_LIST, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
//测试正则过滤
bool Marketlogic::searchsatr1(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
	  std::string name;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->searchstarlist(code,ret_list)){
		send_error(socket, ERROR_TYPE, NO_SEARCH_STAR_LIST, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}

bool Marketlogic::getstarbrief(struct server* srv,int socket ,struct PacketHead* packet){
	if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  std::string code;
      bool r = packet_recv->body_->GetString(L"code",&code);
	  if(!r){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
    star_logic::StarBrief item;

    {
      base_logic::WLockGd lk(lock_);
      STARTSBRIEF_MAP::iterator iter;
      iter = starsbrief_map_.find (code);
      if (iter == starsbrief_map_.end()) //数据为空
      {
		  send_error(socket, ERROR_TYPE, NO_HAVE_DATA, packet->session_id);
          return false;
      }
      item = iter->second;

    }
	DicValue ret_list;
    ret_list.SetString(L"symbol", item.symbol());
    ret_list.SetString(L"nationality", item.nationality());
    ret_list.SetString(L"name", item.name());
    ret_list.SetBigInteger(L"gender", item.gender());

    ret_list.SetString(L"head_url", item.head_url());
    ret_list.SetString(L"nation", item.nation());
    ret_list.SetString(L"work", item.work());
    ret_list.SetInteger(L"star_vip", item.vip());
    
    ret_list.SetString(L"introduction", item.introduction());
    ret_list.SetString(L"weibo_index_id", item.weibo_index_id());
    ret_list.SetString(L"constellaction", item.constellaction());
    ret_list.SetString(L"birth", item.birth());
    ret_list.SetString(L"colleage", item.colleage());
    ret_list.SetString(L"pic_url", item.pic_url());
    ret_list.SetBigInteger(L"owntimes", item.owntimes());
      /*
	  if(!sqldb->getstarbrief(code,ret_list)){
		//send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		send_error(socket, ERROR_TYPE, NO_HAVE_DATA, packet->session_id);
		return false;
	  }
      */
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}
bool Marketlogic::getmarketstarlist(struct server* srv,int socket ,struct PacketHead* packet){
	  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  struct PacketControl* packet_recv = (struct PacketControl*) (packet);

	  int64 type,startnum,endnum,sorttype;
      bool r1 = packet_recv->body_->GetBigInteger(L"type",&type);
	  bool r2 = packet_recv->body_->GetBigInteger(L"startnum",&startnum);
	  bool r3 = packet_recv->body_->GetBigInteger(L"endnum",&endnum);
	  bool r4 = packet_recv->body_->GetBigInteger(L"sorttype",&sorttype);
	  if(!(r1&&r2&&r3&&r4)){
		send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
		return false;
	  }
	  DicValue ret_list;
	  if(!sqldb->getmarketstarlist(type,ret_list,startnum,endnum,sorttype)){
		send_error(socket, ERROR_TYPE, NO_GET_MARKET_STAR_LIST, packet->session_id);
		return false;
	  }
	  
	  struct PacketControl packet_reply;
	  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
	  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
	  ret_list.Set(L"result",result);
	  packet_reply.body_ = &ret_list;
	  send_message(socket,&packet_reply);
	
	  return true;
}	  
bool Marketlogic::getmarkettypes(struct server* srv,int socket ,struct PacketHead* packet){
  if (packet->packet_length <= PACKET_HEAD_LENGTH) {
	send_error(socket, ERROR_TYPE, FORMAT_ERRNO, packet->session_id);
	return false;
  }
  struct PacketControl* packet_recv = (struct PacketControl*) (packet);


  DicValue ret_list;
  if(!sqldb->getmarkettypes(ret_list)){
	send_error(socket, ERROR_TYPE, NO_GET_MARKET_TYPES, packet->session_id);
	return false;
  }
  
  struct PacketControl packet_reply;
  MAKE_HEAD(packet_reply, S_MARKETTYPES_GET, INFO_TYPE, 0,packet->session_id, 0);
  base_logic::FundamentalValue* result = new base_logic::FundamentalValue(1);
  ret_list.Set(L"result",result);
  packet_reply.body_ = &ret_list;
  send_message(socket,&packet_reply);

  return true;
}


bool Marketlogic::OnInfomationClose(struct server *srv, const int socket) {
  return true;
}

bool Marketlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool Marketlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool Marketlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool Marketlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    if (srv->add_time_task != NULL) {
      srv->add_time_task(srv, "infamation", TIME_RELOADSTARINFO_TASK, 60*30, -1);
    }
  }
  return true;
}

bool Marketlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    case TIME_RELOADSTARINFO_TASK:
    {
      InitStarInfo();
      break;
    }
    default:
      break;
  }
  return true;
}
void Marketlogic::InitStarInfo() {
  base_logic::WLockGd lk(lock_);
  /*
  std::list<star_logic::StarInfo> list;
  
  sqldb->OnStarsInfo(&list);
  while (list.size() > 0) {
    star_logic::StarInfo item = list.front();
    list.pop_front();
    //stars_map_[item.code()] = item;
  }
 */ 
  LOG_ERROR("init star info begin ________________________");
  std::list<star_logic::StarBrief> list;
  sqldb->OnStarsbrief(&list);
  while (list.size() > 0) {
    star_logic::StarBrief item = list.front();
    list.pop_front();
    starsbrief_map_[item.symbol()] = item;
  }
  LOG_ERROR2("init star info brief end ___[%d]", starsbrief_map_.size());
}


}  // namespace
