#include "push_message.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

extern char *host;
extern char *appId;
extern char *appKey;
extern char *masterSecret;

namespace push_logic {

//CTransmissionMessage
CTransmissionMessage::CTransmissionMessage(){
  templ = new TransmissionTemplate();
}

CTransmissionMessage::~CTransmissionMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CTransmissionMessage::GetData(){
  return (void*)templ;
}

void CTransmissionMessage::SetData(char* title, char* text, char* logo, char* logoUrl, char* content){
  templ->t.appId = appId; 
  templ->t.appKey = appKey; 
   //应用启动类型，1：强制应用启动 2：等待应用启动
  templ->transmissionType = 2;          
  //透传内容  
  templ->transmissionContent = content; 
  //templ->t.duration_start="2015-07-10 18:00:00";
  //templ->t.duration_end="2015-07-10 19:00:00";

  //APN推送,加入pushinfo参数
  templ->t.pushInfo.body = text;
  //templ->t.pushInfo.actionLocKey = "actionLocKey";
  //templ->t.pushInfo.locKey = "locKey";
  templ->t.pushInfo.launchImage = logo;
  templ->t.pushInfo.title = title;
  //templ->t.pushInfo.titleLocKey = "titleLocKey";
  templ->t.pushInfo.category=content;//客户端解析字段
  templ->t.pushInfo.badge = 1;
  templ->t.pushInfo.contentAvailable = 1;
  //templ->t.pushInfo.sound = "";
  ListInfo locargslist = {0};
  locargslist.size = 1;
  ListItem argitem = {0};
  strcpy(argitem.item,"argitem");
  locargslist.item = &argitem;
  templ->t.pushInfo.locArgs = locargslist;
  ListInfo titleargslist = {0};
  titleargslist.size = 1;
  ListItem titlelist = {0};
  strcpy(titlelist.item,"titleargitem");
  titleargslist.item = &titlelist;
  templ->t.pushInfo.titleLocArgs = titleargslist;
}


//CPopupTransmissionMessage
CPopupTransmissionMessage::CPopupTransmissionMessage(){
  templ = new PopupTransmissionTemplate();
}

CPopupTransmissionMessage::~CPopupTransmissionMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CPopupTransmissionMessage::GetData(){
  return (void*)templ;
}

void CPopupTransmissionMessage::SetData(char* title, char* text, char* img,
                              char* confirmButtonText, char* cancelButtonText, char* content){
  templ->t.appId = appId; 
  templ->t.appKey = appKey; 
  templ->title = title; 
  templ->text = text; 
  templ->img = img; 
  templ->confirmButtonText = confirmButtonText; 
  templ->cancelButtonText = cancelButtonText;
  
  //应用启动类型，1：强制应用启动 2：等待应用启动
  templ->transmissionType = 1;          
  //透传内容  
  templ->transmissionContent = content;
}


//CNotificationMessage
CNotificationMessage::CNotificationMessage(){
  templ = new NotificationTemplate();
}

CNotificationMessage::~CNotificationMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CNotificationMessage::GetData(){
  return (void*)templ;
}

void CNotificationMessage::SetData(char* title, char* text, 
                              char* logo, char* logoUrl, char* content){
  templ->t.appId = appId; 
  templ->t.appKey = appKey; 
  templ->title = title; 
  templ->text = text; 
  templ->logo = logo; 
  templ->logoUrl = logoUrl;
  
  //应用启动类型，1：强制应用启动 2：等待应用启动
  templ->transmissionType = 1;          
  //透传内容  
  templ->transmissionContent = content;
  //templ->t.duration_start="2015-07-10 18:00:00";
  //templ->t.duration_end="2015-07-10 19:00:00";
  //接收消息是否响铃, GT_ON: 响铃  GT_OFF: 不响铃
  templ->isRing = GT_ON;
  //震动
  templ->isVibrate = GT_ON;
  //接收到消息是否可清除
  templ->isClearable = GT_ON;
}

//CLinkMessage
CLinkMessage::CLinkMessage(){
  templ = new LinkTemplate();
}

CLinkMessage::~CLinkMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CLinkMessage::GetData(){
  return (void*)templ;
}

void CLinkMessage::SetData(char* title, char* text, char* logo, 
                              char* logoUrl, char* url, char* content){
  templ->t.appId = appId;
  templ->t.appKey = appKey;
  templ->title = title;
  templ->text = text;
  templ->logo = logo;
  templ->logoUrl = logoUrl;
  templ->url = url;   
  //templ->t.duration_start="2015-07-10 18:00:00";
  //templ->t.duration_end="2015-07-10 19:00:00";
  //接收消息是否响铃, GT_ON: 响铃  GT_OFF: 不响铃
  templ->isRing = GT_OFF;
  //震动
  templ->isVibrate = GT_OFF;
  //接收到消息是否可清除
  templ->isClearable = GT_ON;
}


//CNotyPopLoadMessage
CNotyPopLoadMessage::CNotyPopLoadMessage(){
  templ = new NotyPopLoadTemplate();
}

CNotyPopLoadMessage::~CNotyPopLoadMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CNotyPopLoadMessage::GetData(){
  return (void*)templ;
}

void CNotyPopLoadMessage::SetData(char* title, char* text, char* logo, 
                              char* logoUrl, char* url, char* content){
  templ->t.appId = appId;
  templ->t.appKey = appKey;
  templ->notyTitle = title;
  templ->notyContent = text;
  templ->notyIcon = "icon.png";
  templ->logoUrl = "http://tva2.sinaimg.cn/crop.0.0.512.512.180/4e39d498jw8f9zphpwmorj20e80e80t6.jpg";
  templ->popTitle = "NotyPopLoadTemplate 弹框标题";
  templ->popContent = "NotyPopLoadTemplate 弹框内容";
  templ->popImage = "icon.png";
  templ->popButton1 = "下载";
  templ->popButton2 = "取消";
  templ->loadTitle = "NotyPopLoadTemplate 下载标题";
  templ->loadIcon = "file://push.png";
  templ->loadUrl = "http://www.appchina.com/market/d/425201/cop.baidu_0/com.gexin.im.apk";
  //应用安装完成后是否自动启动
  templ->isActived = true;
  //下载之后是否弹出安装界面， true: 弹出安装 false: 手动点击弹出安装
  templ->isAutoInstall = true;
  //templ->t.duration_start="2015-07-10 18:00:00";
  //templ->t.duration_end="2015-07-10 19:00:00";
  //接收消息是否响铃, GT_ON: 响铃  GT_OFF: 不响铃
  templ->isBelled = GT_OFF;
  //震动
  templ->isVibrationed = GT_OFF;
  //接收到消息是否可清除
  templ->isCleared = GT_ON;
}

//CAPNMessage
CAPNMessage::CAPNMessage(){
  templ = new APNTemplate();
}

CAPNMessage::~CAPNMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CAPNMessage::GetData(){
  return (void*)templ;
}

void CAPNMessage::SetData(char* title, char* text, char* logo, 
                              char* logoUrl, char* url, char* content){
  //IOS自定义消息
  Entry cmsg = {0};
  strcpy(cmsg.key,"cmsg");
  strcpy(cmsg.value,"cmsg");

  //title-loc-args赋值
  ListItem titlelocargs[2]={"titlelocargs1","titlelocargs2"};

  //locargs赋值
  ListItem locargs[2]={"locargs1","locargs2"};

  //APN模板，只包含了父类模板
  templ->t.appId = appId;
  templ->t.appKey = appKey;

  templ->t.pushInfo.badge=4;
  //templ->t.pushInfo.sound="test1.wav";
  //templ->t.pushInfo.contentAvailable=1;
  templ->t.pushInfo.category="";
  //templ->t.pushInfo.cmsg.size=1;
  //templ->t.pushInfo.cmsg.map = &cmsg;

  templ->t.pushInfo.body="body";
  templ->t.pushInfo.actionLocKey="";

  templ->t.pushInfo.locKey="";
  //templ->t.pushInfo.locArgs.size=2;
  //templ->t.pushInfo.locArgs.item=locargs;

  templ->t.pushInfo.launchImage="";
  //IOS8.2以上版本支持
  templ->t.pushInfo.title=title;
  templ->t.pushInfo.titleLocKey="";

  //templ->t.pushInfo.titleLocArgs.size=2;
  //templ->t.pushInfo.titleLocArgs.item=titlelocargs;

}

//CAPNListMessage
CAPNListMessage::CAPNListMessage(){
  templ = new APNTemplate();
}

CAPNListMessage::~CAPNListMessage(){
  if(templ){
    delete templ;
    templ = NULL;
  }
}

void* CAPNListMessage::GetData(){
  return (void*)templ;
}

void CAPNListMessage::SetData(char* title, char* text, char* logo, 
                              char* logoUrl, char* url, char* content){
  //模板
  //IOS自定义消息
  Entry cmsg = {0};
  strcpy(cmsg.key,"cmsg");
  strcpy(cmsg.value,"cmsg");

  //title-loc-args赋值
  ListItem titlelocargs[2]={"titlelocargs1","titlelocargs2"};

  //locargs赋值
  ListItem locargs[2]={"locargs1","locargs2"};

  //APN推送模板，只包含了父类模板
  templ->t.appId = appId;
  templ->t.appKey = appKey;

  templ->t.pushInfo.badge=4;
  templ->t.pushInfo.sound="";
  templ->t.pushInfo.contentAvailable=1;
  templ->t.pushInfo.category="";
  templ->t.pushInfo.cmsg.size=1;
  templ->t.pushInfo.cmsg.map = &cmsg;

  templ->t.pushInfo.body="body";
  templ->t.pushInfo.actionLocKey="";

  templ->t.pushInfo.locKey="lockey";
  templ->t.pushInfo.locArgs.size=2;
  templ->t.pushInfo.locArgs.item=locargs;

  templ->t.pushInfo.launchImage="launchimage";
  //IOS8.2以上版本支持
  templ->t.pushInfo.title="title";
  templ->t.pushInfo.titleLocKey="titlelockey";

  templ->t.pushInfo.titleLocArgs.size=2;
  templ->t.pushInfo.titleLocArgs.item=titlelocargs;
  

}





//CMessageFactory
CMessageFactory::CMessageFactory(){
  m_pTransmissionMsg = new CTransmissionMessage();
  m_pNotificationMsg = new CNotificationMessage();
  m_pLinkMessage = new CLinkMessage();
  m_pNotyPopLoadMessage = new CNotyPopLoadMessage();
  m_pPopupTransmissionMessage = new CPopupTransmissionMessage();
  m_pAPNMessage = new CAPNMessage();
  m_pAPNListMessage = new CAPNListMessage();
}

CMessageFactory::~CMessageFactory(){
  delete m_pTransmissionMsg;
  m_pTransmissionMsg = NULL;
  delete m_pNotificationMsg;
  m_pNotificationMsg = NULL;
  delete m_pLinkMessage;
  m_pLinkMessage = NULL;
  delete m_pNotyPopLoadMessage;
  m_pNotyPopLoadMessage = NULL;
  delete m_pAPNMessage;
  m_pAPNMessage = NULL;
  delete m_pAPNListMessage;
  m_pAPNListMessage = NULL;
}

IMessage* CMessageFactory::CreateMessage(TemplateType tType){
  switch(tType)
  {
    case Transmission:
      return m_pTransmissionMsg;
    case PopupTransmission:
      return m_pPopupTransmissionMessage;
    case NotyPopLoad:
      return m_pNotyPopLoadMessage;
    case Notification:
      return m_pNotificationMsg;
    case Link:
      return m_pLinkMessage;
    case APN:
      return m_pAPNMessage;
    case PUSH_APN_LIST:
      return m_pAPNListMessage;
    default:
      return NULL;
  }
}

} //push_logic
