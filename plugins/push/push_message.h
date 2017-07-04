//push_engine.h

#include "IGtPush.h"
#include <string>
#include <string.h>

#ifndef PUSH_ENGINE_MESSAGE_H__
#define PUSH_ENGINE_MESSAGE_H__

typedef int int32;
typedef long int int64;

#define PUSH_APN_LIST 100

namespace push_logic {

class IMessage{
public:
  IMessage(){
    
  };
  virtual ~IMessage(){
    
  };
  virtual void* GetData() {};
  virtual void SetData(char* title, char* text, char* logo, char* logoUrl, char* content) {};
  virtual void SetData(char* title, char* text, char* logo, char* logoUrl, char* url, char* content) {};
};

class CTransmissionMessage: public IMessage
{
public:
  CTransmissionMessage();
  ~CTransmissionMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text, char* logo, char* logoUrl, char* content);

private:
  TransmissionTemplate* templ;
};


class CPopupTransmissionMessage: public IMessage
{
public:
  CPopupTransmissionMessage();
  ~CPopupTransmissionMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text, char* img,
                        char* confirmButtonText, char* cancelButtonText, char* content);

private:
  PopupTransmissionTemplate* templ;
};


class CNotificationMessage: public IMessage
{
public:
  CNotificationMessage();
  ~CNotificationMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text,char* logo, char* logoUrl, char* content);

private:
  NotificationTemplate* templ;
};

class CLinkMessage: public IMessage
{
public:
  CLinkMessage();
  ~CLinkMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text,char* logo, char* logoUrl, char* url, char* content);

private:
  LinkTemplate* templ;
};

class CNotyPopLoadMessage: public IMessage
{
public:
  CNotyPopLoadMessage();
  ~CNotyPopLoadMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text,char* logo, char* logoUrl, char* url, char* content);

private:
  NotyPopLoadTemplate* templ;
};

class CAPNMessage: public IMessage
{
public:
  CAPNMessage();
  ~CAPNMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text,char* logo, char* logoUrl, char* url, char* content);

private:
  APNTemplate* templ;
};

class CAPNListMessage: public IMessage
{
public:
  CAPNListMessage();
  ~CAPNListMessage();
  
public:
  void* GetData();
  void SetData(char* title, char* text,char* logo, char* logoUrl, char* url, char* content);

private:
  APNTemplate* templ;
};





class CMessageFactory
{
public:
  CMessageFactory();
  ~CMessageFactory();
  
public:
  IMessage* CreateMessage(TemplateType tType);
  
private:
  CTransmissionMessage* m_pTransmissionMsg;
  CNotificationMessage* m_pNotificationMsg;
  CLinkMessage* m_pLinkMessage;
  CNotyPopLoadMessage* m_pNotyPopLoadMessage;
  CPopupTransmissionMessage* m_pPopupTransmissionMessage;
  CAPNMessage* m_pAPNMessage;
  CAPNListMessage* m_pAPNListMessage;

};

} //push_logic

#endif //PUSH_ENGINE_MESSAGE_H__
