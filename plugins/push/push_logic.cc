

#include "push_logic.h"
#include "logic/star_infos.h"
#include "comm/comm_head.h"
#include "config/config.h"
#include "core/common.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "net/errno.h"
#include <string>
//#include "operator_code.h"
//#include "errno.h"

#define DEFAULT_CONFIG_PATH "./plugins/push/push_config.xml"

#define TIME_PUSH_MESSAGE_TASK 14001

// http的域名
//static char *host ="http://sdk.open.api.igexin.com/apiex.htm";
// https的域名
char *host ="https://api.getui.com/apiex.htm";

//定义常量, appId、appKey、masterSecret 采用本文档 "第二步 获取访问凭证 "中获得的应用配置
//char *appId = "LLYkPyVPygAHNcLneaqWG5";
//char *appKey = "hT1mIVaLa1AcW1rMbowSL7";
//char *masterSecret = "mN3Jdr93xB7TdYPeujlO61";

//Star
char *appId = "j6hP8vFv4Q8juUqTmnPPy6";
char *appKey = "5Tb2US9CEg8RsqqYlamnP1";
char *masterSecret = "gfCZcQ8ycU67o7RNt7IZG3";


namespace push_logic {

CPushlogic *CPushlogic::instance_ = NULL;

CPushlogic::CPushlogic() {
  Init();
}

CPushlogic::~CPushlogic() {
  FreeInstance();

  if(m_pPushMessage){
    delete m_pPushMessage;
    m_pPushMessage = NULL;
  }
  if(m_pPushImpl){
    delete m_pPushImpl;
    m_pPushImpl = NULL;
  }
  if(m_pPushImplFactory){
    delete m_pPushImplFactory;
    m_pPushImplFactory = NULL;
  }
  if(m_pMessageFactory){
    delete m_pMessageFactory;
    m_pMessageFactory = NULL;
  }

}

bool CPushlogic::Init() {
  bool r = false;
  config::FileConfig* config = config::FileConfig::GetFileConfig();
  std::string path = DEFAULT_CONFIG_PATH;
  if (config == NULL) {
    LOG_ERROR("CPushlogic config init error");
    return false;
  }
  r = config->LoadConfig(path);
  if (!r) {
    LOG_ERROR("login config load error");
    return false;
  }
  m_sqlengine = new push_logic::CPushDB(config);

  m_kafkaengine = new push_logic::CPushKafka(config);

  m_pMessageFactory = new push_logic::CMessageFactory();
  m_pPushImplFactory = new push_logic::CPushMessageImplFactory();
  if(m_pPushImplFactory == NULL)
  {
    LOG_ERROR("PushMessageImplFactory create error.");
    return false;
  }
  if(!m_pPushImplFactory->init())
  {
    LOG_ERROR("PushImplFactory init failure.");
    return false;
  }
  
  return true;
}

CPushlogic *CPushlogic::GetInstance() {
  if (instance_ == NULL)
    instance_ = new CPushlogic();
  return instance_;
}

void CPushlogic::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

bool CPushlogic::OnPushConnect(struct server *srv, const int socket) {
  std::string ip;
  int port;
  logic::SomeUtils::GetIPAddress(socket, ip, port);
  LOG_MSG2("ip {%s} prot {%d}", ip.c_str(), port);

  return true;
}

bool CPushlogic::OnPushMessage(struct server *srv, const int socket,
                                    const void *msg, const int len) {                               
  LOG_DEBUG2("OnPushMessage in [%d]", 3333);
  std::list<base_logic::DictionaryValue*> *list;
  m_kafkaengine->FectchBatchTask(list);
  LOG_DEBUG2("OnPushMessage FectchBatchTask size[%d]", list->size());

  bool r = false;
  struct PacketHead *packet = NULL;
  if (srv == NULL || socket < 0 || msg == NULL || len < PACKET_HEAD_LENGTH)
    return false;
  if (!net::PacketProsess::UnpackStream(msg, len, &packet)) {
    LOG_ERROR2("UnpackStream Error socket %d", socket);
    SEND_UNPACKET_ERROR(socket, ERROR_TYPE, UNPACKET_ERRNO, FORMAT_ERRNO);
    return false;
  }
  switch (packet->operate_code) {
	case 0:{
//	  OnStarSendMessage(srv, socket, packet);
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

bool CPushlogic::OnPushClose(struct server *srv, const int socket) {
  return true;
}

bool CPushlogic::OnBroadcastConnect(struct server *srv, const int socket,
                                      const void *msg, const int len) {
  return true;
}

bool CPushlogic::OnBroadcastMessage(struct server *srv, const int socket,
                                      const void *msg, const int len) {

  return true;
}

bool CPushlogic::OnBroadcastClose(struct server *srv, const int socket) {
  return true;
}

bool CPushlogic::OnIniTimer(struct server *srv) {
  if (srv->add_time_task != NULL) {
    srv->add_time_task(srv, "push", TIME_PUSH_MESSAGE_TASK, 5, -1);
  }
  return true;
}

bool CPushlogic::OnTimeout(struct server *srv, char *id, int opcode,
                             int time) {
  switch (opcode) {
    case TIME_PUSH_MESSAGE_TASK:
      DealMessage();
      break; 
    default:
      break;
  }
  return true;
}

void CPushlogic::DealMessage(){
  //1. get message from kafka;
  m_kafkaengine->FectchBatchTask(&m_kafkadatalist);
  LOG_DEBUG2("Get data size from kafka [%d]", m_kafkadatalist.size());

  while(m_kafkadatalist.size() > 0) {
    base_logic::DictionaryValue* dict_value = m_kafkadatalist.front();
    m_kafkadatalist.pop_front();


    //2. query infro from db
    int64 iUid = 166;
    std::string s1,s2,s3,s4,s5;
    char cTitle[40] = {0};
    char cTitleText[600] = {0};
    char cLogo[40] = {0};
    char cLogoUrl[100] = {0};
    char cContent[2048] = {0};

    int32 deviceTpye = -1;
    std::string deviceId = "";
    //
    bool r1 = dict_value->GetBigInteger(L"uid", &iUid);
    bool r2 = dict_value->GetString(L"title", &s1);
    bool r3 = dict_value->GetString(L"text", &s2);
    bool r4 = dict_value->GetString(L"log", &s3);
    bool r5 = dict_value->GetString(L"logurl", &s4);
    bool r6 = dict_value->GetString(L"content", &s5);
    if (!r1 || !r2 || !r3 || !r6) {
      LOG_ERROR2("Get message infor error,uid[%d],title[%s],text[%s],log[%s],logurl[%s],content[%s]",
              iUid,s1.c_str(),s2.c_str(),s3.c_str(),s4.c_str(),s5.c_str());
      continue;
    }
    strncpy(cTitle, s1.c_str(), 39);
    cTitle[39] = '\0';
    strncpy(cTitleText, s2.c_str(), 599);
    cTitleText[599] = '\0';
    strncpy(cLogo, s3.c_str(), 39);
    cLogo[39] = '\0';
    strncpy(cLogoUrl, s4.c_str(), 99);
    cLogoUrl[99] = '\0';
    strncpy(cContent, s5.c_str(), 2047);
    cContent[2047] = '\0';
    
    if(iUid > 0){
      if( !m_sqlengine->onGetUserDeviceId(iUid, deviceId, deviceTpye) ){
        LOG_ERROR2("Data base error, [%d]", -1);
        continue;
      }
    }
    LOG_DEBUG2("onGetUserDeviceId deviceId[%s],deviceTpye[%d]", deviceId.c_str(), deviceTpye);
    

    //3. push message to users.
    //具体消息
    
    if(iUid <= 0){
      //群推
      //透传提醒
      m_pPushMessage = m_pMessageFactory->CreateMessage(Transmission);
      m_pPushMessage->SetData(cTitle, cTitleText, 
                               cLogo, cLogoUrl, cContent);

      m_pPushImpl = m_pPushImplFactory->CreatePushMessageImpl(push_logic::PUSH_TO_APP);
      m_pPushImpl->sendMessage(m_pPushMessage, Transmission);
      LOG_DEBUG2("Push to app finish.[%ld]", iUid);
    }
    else{
      if(DEVICE_IOS == deviceTpye){
        //apn 单推
        m_pPushMessage = m_pMessageFactory->CreateMessage(Transmission);
        m_pPushMessage->SetData(cTitle, cTitleText, 
                                 cLogo, cLogoUrl, cContent);

        m_pPushImpl = m_pPushImplFactory->CreatePushMessageImpl(push_logic::PUSH_TO_APN_SINGLE);
        m_pPushImpl->setDeviceId(deviceId.c_str());
        m_pPushImpl->sendMessage(m_pPushMessage, Transmission);
        LOG_DEBUG2("Push to apn user finish.[%ld],[%s]", iUid, deviceId.c_str());
      }else{
        //android 单推
        //透传模版
        //m_pPushMessage = m_pMessageFactory->CreateMessage(Notification);
        m_pPushMessage = m_pMessageFactory->CreateMessage(Transmission);
        m_pPushMessage->SetData(cTitle, cTitleText, 
                                cLogo, cLogoUrl, cContent);

        m_pPushImpl = m_pPushImplFactory->CreatePushMessageImpl(push_logic::PUSH_TO_SINGLE);
        m_pPushImpl->SetOfflineExpireTime(30000);
        m_pPushImpl->setCid(deviceId.c_str());
        //m_pPushImpl->sendMessage(m_pPushMessage, Notification);
        m_pPushImpl->sendMessage(m_pPushMessage, Transmission);
        LOG_DEBUG2("Push to android user finish.[%ld],[%s]", iUid, deviceId.c_str());
      }
    }
    

    if (dict_value) {
      delete dict_value;
      dict_value = NULL;
    }
  }
}




}  // namespace
