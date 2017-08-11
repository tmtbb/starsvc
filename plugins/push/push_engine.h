//push_engine.h

#include "push_message.h"

#ifndef PUSH_ENGINE_IMP_H__
#define PUSH_ENGINE_IMP_H__

namespace push_logic {
  
#define PUSH_LIST_NUM 50

typedef enum push_net_work_type {
  UNLIMIT = 0,
  NETWORK_WIFI = 1,
  NETWORK_234G = 2
} PushNetWorkType;

typedef enum push_impl_type {
  PUSH_TO_APP,
  PUSH_TO_SINGLE,
  PUSH_TO_LIST,
  PUSH_TO_APN_SINGLE,
  PUSH_TO_APN_LIST
} PushImplType;

typedef enum push_device_type {
  DEVICE_IOS,
  DEVICE_ANDROID
} PushDeviceType;

class IPushImpl{
public:
  IPushImpl():m_lOfflineExpireTime(0),
    m_iPushNetWorkType(UNLIMIT) {
      
    };
  virtual ~IPushImpl(){
    
  };
  virtual void sendMessage(IMessage* pMessage, TemplateType tType) {};
  virtual void sendMessage(IMessage* pMessage, TemplateType tType, int32 iSize, char** ppCid, char** ppAlias) {};
  virtual void setCid(const char* cid) {};
  virtual void setDeviceId(const char* deviceId) {};
  
public:
  int64 GetOfflineExpireTime(){
    return m_lOfflineExpireTime;
  }
  
  int32 GetPushNetWorkType(){
    return m_iPushNetWorkType;
  }
  
  void SetOfflineExpireTime(int64 l){
    m_lOfflineExpireTime = l;
  }
  
  void SetPushNetWorkType(int32 i){
    m_iPushNetWorkType = i;
  }
  
private:
  int64 m_lOfflineExpireTime;  //离线下发有效期 毫秒
	int32 m_iPushNetWorkType;  //0不限 1wifi 2:4G/3G/2G
  
public:
  //测试打印
	void printResult(IPushResult &result);
};


class CPushToAppImpl: public IPushImpl 
{
public:
  CPushToAppImpl(){
      
    };
  ~CPushToAppImpl(){
    
  };
  
public:
  void sendMessage(IMessage*, TemplateType tType);

};


class CPushToSingleImpl: public IPushImpl 
{
public:
  CPushToSingleImpl() {
    memset(m_cid, 0, 32);
  };
  ~CPushToSingleImpl(){
    
  };
  
public:
  void sendMessage(IMessage*, TemplateType tType);
  
  void setCid(const char* cid){
    strcpy(m_cid,cid);
	}
  
private:
  char m_cid[32];  //要推送用户的cid
  
};

class CPushToListImpl: public IPushImpl 
{
public:
  CPushToListImpl(){
    
  };
  ~CPushToListImpl(){
    
  };
  
public:
  void sendMessage(IMessage* pMessage, TemplateType tType, int32 iSize, char** ppCid, char** ppAlias);
  
};

class CPushToApnSingleImpl: public IPushImpl 
{
public:
  CPushToApnSingleImpl() {
    memset(m_iosDeviceId, 0, 128);
  };
  ~CPushToApnSingleImpl(){
    
  };
  
public:
  void sendMessage(IMessage*, TemplateType tType);
  
  void setDeviceId(const char* deviceId){
    strcpy(m_iosDeviceId, deviceId);
  } 
private:
  char m_iosDeviceId[128];  //iOS设备唯一标识号
  
};

class CPushToApnListImpl: public IPushImpl 
{
public:
  CPushToApnListImpl() {
    
  };
  ~CPushToApnListImpl(){
    
  };
  
public:
  void sendMessage(IMessage* pMessage, TemplateType tType, int32 iSize, char** ppCid, char** ppAlias);
  
  
};



class CPushMessageImplFactory
{
public:
  CPushMessageImplFactory();
  ~CPushMessageImplFactory();
  
public:
  bool init();
  IPushImpl* CreatePushMessageImpl(PushImplType tType);

private:
  CPushToAppImpl* m_pPushToAppImpl;
  CPushToSingleImpl* m_pPushToSingleImpl;
  CPushToListImpl* m_pPushToListImpl;
  CPushToApnSingleImpl* m_pPushToApnSingleImpl;
  CPushToApnListImpl* m_pPushToApnListImpl;

};

} //push_logic

#endif //PUSH_ENGINE_IMP_H__
