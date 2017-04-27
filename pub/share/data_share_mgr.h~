// Copyright (c) 2016 The tourism Authors. All rights reserved.
// data_share_mgr.h
// Created on: 2016年8月16日.
// Author: Paco.
#ifndef PUB_SHARE_DATA_SHARE_MGR_H_
#define PUB_SHARE_DATA_SHARE_MGR_H_

#include <stdio.h>
#include <map>

#include "base/thread/base_thread_lock.h"
#include "public/basic/basictypes.h"

#include "pub/comm/user_info.h"
#include "pub/net/typedef.h"

namespace share {
__attribute__((visibility("default")))
 class DataShareMgr {
 public:
  static DataShareMgr* GetInstance();
  void AddUser(UserInfo* user);
  void DelUser(int64 uid);
  void UserHeart(int64 uid);
  UserInfo* GetUser(int64 uid);

  int32 AddDeviceToken(int64 uid, std::string token);  //0-新增 1-更新 -1-未修改
  int32 AddUnReadCount(int64 uid);  //返回未读条数
  void DelUnReadCount(int64 uid, int32 count);  //count -1 全读
  std::string GetDeviceToken(int64 uid);

  void AddNick(int64 uid, std::string nick);
  std::string GetNick(int64 uid);

  UserInfo* GetFreeCoordinator();  //获取当前最空闲客服信息

  void SetImgToken(std::string token, int64 time);
  void GetImgToken(std::string*token, int64* time);
  void InitShareType();
  void InitTourismShare(ListValue* list);
  void InitSkillShare(ListValue* list);
  void InitShareType(ListValue* list);
  void ClearShareTourismMap();
  void ClearShareSkillMap();
  int32 QueryRecommendShare(int64 id, int64 count, int64 type, DicValue* info);
  int32 QueryShareTourismDetail(int64 id, DicValue* dic);
  int32 QueryShareSkillDetail(int64 id, DicValue* dic);
  int32 QuerySkillShare(int64 id, int64 count, DicValue* info);

  // key 15149320909:0 value:md5(str)
  void UpdateSMSToken(std::string key, std::string token);
  void EraseSMSToken(std::string key);
  bool CheckSMSToken(std::string key, std::string token);

  void CheckHeartLoss();
  void UserOffline(int fd);

  void test();
 private:
  DataShareMgr();
  ~DataShareMgr();

  void DelGuide(int64 uid);
  void DelVisitor(int64 uid);
  void DelCoordinator(int64 uid);

 private:
  static DataShareMgr* instance_;
  threadrw_t* lock_;

  UserMap user_map_;
  GuideMap guide_map_;
  VisitorMap visitor_map_;
  CoordinatorMap coordinator_map_;
  DeviceTokenMap dt_map_;
  UnReadMap unread_map_;
  NickMap nick_map_;
  //存用户最新的token key[手机号:type] value[token]
  SMSTokenMap sms_token_map_;

  //旅游分享类别
  ShareTypeMap share_type_map_;
  //所有的旅游分享数据
  ShareTourismMap share_tourism_map_;
  //技能分享数据
  ShareSkillMap share_skill_map_;
  //旅游推荐分享
  ShareIdMap recommend_share_map_;
  //技能分享banner
  ShareIdMap banner_share_map_;

  std::string img_token_;

  int64 token_time_;
};
}  // namespace share

extern "C" share::DataShareMgr *GetDataShareMgr(void);

#endif  // PUB_SHARE_DATA_SHARE_MGR_H_
