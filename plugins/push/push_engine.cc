#include "push_engine.h"
#include <stdio.h>
#include <iostream>
#include <string.h>

using namespace std;

extern char *host;
extern char *appId;
extern char *appKey;
extern char *masterSecret;

namespace push_logic {

void IPushImpl::printResult(IPushResult &result){
  cout << "print result:-------------" << endl;
  for (int i = 0; i < result.size; i++) {
      cout << result.entry[i].key << ": " << result.entry[i].value << endl;
  }
  cout << "print end:----------------" << endl;
}

//CPushToAppImpl
void CPushToAppImpl::sendMessage(IMessage* pMessage, TemplateType tType){
  //准备数据
  Message msg = {0};
  msg.isOffline = 0;//是否离线下发
  msg.offlineExpireTime = GetOfflineExpireTime();//离线下发有效期 毫秒
  msg.pushNetWorkType = UNLIMIT;//0不限 1wifi 2:4G/3G/2G

  AppMessage appMsg = {0};
  appMsg.msg = msg;
      appMsg.speed = 10000;//定速
  char* appList[] = {appId};
  appMsg.appIdList = appList;
  appMsg.appIdListSize = 1;

  IPushResult result = {0};
  cout<<"TemplateType tType:"<<tType<<",appKey:"<<appKey<<endl;
  result = pushMessageToApp(appKey, &appMsg, pMessage->GetData(), tType);
  
  //打印结果
  printResult(result);
}

//CPushToSingleImpl
void CPushToSingleImpl::sendMessage(IMessage* pMessage, TemplateType tType){
  //准备数据
  Message msg = {0};
  msg.isOffline = true;//是否离线下发
  msg.offlineExpireTime = GetOfflineExpireTime();//离线下发有效期 毫秒
  msg.pushNetWorkType = UNLIMIT;//0不限 1wifi 2:4G/3G/2G
  SingleMessage singleMsg = {0};
  singleMsg.msg = msg;

  //目标用户
  Target target = {0};
  target.appId = appId;
  target.clientId = m_cid;
  //target.alias = m_alias;
  
  IPushResult result = {0};
  result = pushMessageToSingle(appKey, &singleMsg, pMessage->GetData(), tType, &target);

  //打印结果
  printResult(result);
}

//CPushToListImpl
void CPushToListImpl::sendMessage(IMessage* pMessage, TemplateType tType, int32 iSize, char** ppCid, char** ppAlias){
  //准备数据
  Message msg = {0};
  msg.isOffline = true;//是否离线下发
  msg.offlineExpireTime = GetOfflineExpireTime();//离线下发有效期 毫秒
  msg.pushNetWorkType = UNLIMIT;//0不限 1wifi 2:4G/3G/2G
  ListMessage listMsg = {0};
  listMsg.msg = msg;

  Result ret;
  int32 iPushSize = iSize < PUSH_LIST_NUM ? iSize : PUSH_LIST_NUM;
  char contentId[PUSH_LIST_NUM] = "";
  ret = getContentId(appKey, &listMsg, pMessage->GetData(), tType, contentId, sizeof(contentId));

  if (ret == SUCCESS) {
    Target* targetList = new Target[iPushSize];
    memset((void*)targetList, 0, sizeof(Target));
    for(int i=0;i<iPushSize;++i){
      char* pcid = *ppCid;
      targetList[i].appId = appId;
      targetList[i].clientId = pcid;
      // targetList[i].alias = *ppAlias;
    }

    PushDetail* details = new PushDetail[iPushSize];
    memset((void*)details, 0, sizeof(details));
    cout << "============pushMessageToList============" << endl;
    IPushResult result = {0};
    result = pushMessageToList(appKey, contentId, targetList, iPushSize, details);
    //      cout<<details->ret<<endl;
    printResult(result);
    delete []targetList;

    ret = cancelContentId(appKey, contentId);
    cout << "cancelContentId ret=" << ret << endl;
  }
}

//CPushToApnSingleImpl
void CPushToApnSingleImpl::sendMessage(IMessage* pMessage, TemplateType tType){
  //准备数据
  Message msg = {0};
  msg.isOffline = true;//是否离线下发
  msg.offlineExpireTime = GetOfflineExpireTime();//离线下发有效期 毫秒
  SingleMessage singleMsg = {0};
      singleMsg.msg = msg;

  IPushResult result = {0};

  result = pushAPNMessageToSingle(appKey, pMessage->GetData(), appId, m_iosDeviceId);
  //打印结果
  printResult(result);
}

//CPushToApnListImpl
void CPushToApnListImpl::sendMessage(IMessage* pMessage, TemplateType tType, int32 iSize, char** ppDt, char** ppAlias){
  //准备数据
  Message msg = {0};
  msg.isOffline = true;//是否离线下发
  msg.offlineExpireTime = GetOfflineExpireTime();//离线下发有效期 毫秒
  SingleMessage singleMsg = {0};
      singleMsg.msg = msg;

  //发送消息
  Result ret;
  int32 iPushSize = iSize < PUSH_LIST_NUM ? iSize : iPushSize;
  char* contentId = new char[iPushSize];
  memset((void*)contentId, 0, sizeof(contentId));
  ret = getAPNContentId(appKey, pMessage->GetData(), contentId, iPushSize);
  if (ret == SUCCESS) {
      ListItem item;
      ListInfo* targetList = new ListInfo[iPushSize];
      memset((void*)targetList, 0, sizeof(ListInfo));
      for(int i=0;i<iPushSize;++i){
        char* pDt = *ppDt;
        targetList[i].size = iPushSize;
        strcpy(item.item,pDt);
        targetList[i].item = &item;
      }

      cout << "============pushApnMessageToList============" << endl;
      IPushResult result = {0};
      result = pushAPNMessageToList(appKey, appId, targetList, contentId);
      printResult(result);
  }
}






//CPushMessageImplFactory
CPushMessageImplFactory::CPushMessageImplFactory(){
  m_pPushToAppImpl = new CPushToAppImpl();
  m_pPushToSingleImpl = new CPushToSingleImpl();
  m_pPushToListImpl = new CPushToListImpl();
  m_pPushToApnSingleImpl = new CPushToApnSingleImpl();
  m_pPushToApnListImpl = new CPushToApnListImpl();

};
CPushMessageImplFactory::~CPushMessageImplFactory(){
  delete m_pPushToAppImpl;
  m_pPushToAppImpl = NULL;
  delete m_pPushToSingleImpl;
  m_pPushToSingleImpl = NULL;
  delete m_pPushToListImpl;
  m_pPushToListImpl = NULL;
  delete m_pPushToApnSingleImpl;
  m_pPushToApnSingleImpl = NULL;
  delete m_pPushToApnListImpl;
  m_pPushToApnListImpl = NULL;
};

bool CPushMessageImplFactory::init(){
  // 注意：接口传入字符必须为UTF-8编码，因ASCII字符UTF－8编码后与原先一样，所以可以不编码，但中文等非ASCII字符必须编码
  // 如果返回的类似错误"post http data failed, code=6"，错误码可百度CURL返回的错误码是什么意思，如http://www.cnblogs.com/wainiwann/p/3492939.html
  ////程序运行前只需初始化一遍，若已经初始化过即可不用，"编码"两个字为固定写法，不需要做转换
  Result r = pushInit(host, appKey, masterSecret, "编码");
  if (r != SUCCESS){
    cout<<"pushInit for app failed: ret="<<r<<endl;
    return false;
  }

  cout<<"PushMsg  init success"<<endl;
  cout<<"host:"<<host<<",appId:"<<appId<<",appKey:"<<appKey<<endl<<",masterSecret:"<<masterSecret<<endl;
  return true;
}

IPushImpl* CPushMessageImplFactory::CreatePushMessageImpl(PushImplType tType){
  switch(tType)
  {
    case PUSH_TO_APP:
      return m_pPushToAppImpl;
      break;
    case PUSH_TO_SINGLE:
      return m_pPushToSingleImpl;
      break;
    case PUSH_TO_LIST:
      return m_pPushToListImpl;
      break;
    case PUSH_TO_APN_SINGLE:
      return m_pPushToApnSingleImpl;
      break;
    case PUSH_TO_APN_LIST:
      return m_pPushToApnListImpl;
      break;
    default:
      return NULL;
  }
}

} //push_logic
