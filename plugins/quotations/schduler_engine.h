//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef QUOTATIONS_SCHDULER_ENGINE_H__
#define QUOTATIONS_SCHDULER_ENGINE_H__

#include "logic/swp_infos.h"
#include "quotations/quotations_redis.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"

typedef std::list<swp_logic::Quotations> QUOTATIONS_LIST;
typedef std::map<std::string, QUOTATIONS_LIST> QUOTATIONS_MAP; /*现货ID，行情*//*平台名称:交易所:现货ID 如:JH:TJPME:AG*/
typedef std::map<int64, QUOTATIONS_MAP> QUOTATIONS_ALL_MAP; /*股票,现货,期货 QUOTATIONS_MAP*/

typedef std::map<std::string, swp_logic::Quotations> LAST_QUOTATIONS_MAP;  //最近一次报价
typedef std::map<int64, LAST_QUOTATIONS_MAP> LAST_QUOTATIONS_ALL_MAP;

//正在计算的K线
typedef std::map<std::string, swp_logic::Quotations> K_QUOTATIONS_MAP; /*KEY SYMBOL 外汇标识*/
typedef std::map<int32, K_QUOTATIONS_MAP> K_ALL_QUOTATIONS_MAP; /*KEY 时间标识*/

//K线存储
typedef std::map<std::string, QUOTATIONS_LIST> K_HIS_QUOTATIONS_MAP; /*外汇标识*/
typedef std::map<int32, K_HIS_QUOTATIONS_MAP> K_ALL_HIS_QUOTATIONS_MAP; /*时间标识*/

namespace quotations_logic {

enum TIMETYPE {
  ONE_MINUTE = 60,
  FIVE_MINUTE = 300,
  QUARTER_MINUTE = 900,
  HALF_HOUR = 1800,
  ONE_HOUR = 3600
};

class QuotationsCache {
 public:
  QUOTATIONS_ALL_MAP quotations_map_;
  LAST_QUOTATIONS_ALL_MAP last_quotations_map_;
  K_ALL_QUOTATIONS_MAP current_k_all_map_;
  K_ALL_HIS_QUOTATIONS_MAP k_history_all_map_;
};
  
typedef std::map<std::string, QUOTATIONS_LIST> QUOTATIONS_MAP; /*现货ID，行情*/
typedef std::map<int64,QUOTATIONS_MAP> QUOTATIONS_ALL_MAP; /*股票,现货,期货 QUOTATIONS_MAP*/

/*class QuotationsCache {
 public:
  QUOTATIONS_ALL_MAP  quotations_map_;
};*/

class QuotationsManager {
 public:
  QuotationsManager();
  virtual ~QuotationsManager();

  void SetQuotations(swp_logic::Quotations& quotation);

  void LoginQuotationsCenter(const int socket);

  void SendRealTime(const int socket, const int64 session, const int32 reversed,
                    base_logic::ListValue* value);

  void SendKChartLine(const int socket, const int64 session,
                      const int32 reversed, const int32 kchar_type,
                      const std::string& exchange_name,
                      const std::string& platform_name,
                      const std::string& symbol, const int64 start_time,
                      const int64 end_time, const int32 count);

  void SendTimeLine(const int socket, const int64 session, const int32 reversed,
                    const int32 atype, const std::string& exchange_name,
                    const std::string& platform_name, const std::string& symbol,
                    const int64 start_time, const int64 end_time,
                    const int32 count);

  void TimeEvent(int opcode, int time);

  void InitRedis(quotations_logic::QuotationsRedis* quotations_redis);

  void InitGoodsData();

  void InitFoxreData();

  void InitRedisData(const std::string& hash_name, int32 atype);
 private:

  void SetKQuotations(const std::string& key, const int32 time_symbol,
                      swp_logic::Quotations& quotation);

  void SetQuotationsUnit(swp_logic::Quotations& quotation);

  void SetKQuotations(swp_logic::Quotations& quotation);
  void SetKQuotationsUnit(const int32 time_symbol,
                          swp_logic::Quotations& quotation);

  void GetRealTime(const int32 atype, const std::string& symbol,
                   swp_logic::Quotations* quotations);

  void GetTimeLine(const int32 atype, const std::string& symbol,
                   std::list<swp_logic::Quotations>& list,
                   const int64 end_time = 0);

  void GetKChartLine(const int32 chart_type, const std::string& key,
                     std::list<swp_logic::Quotations>& list,
                     const int64 end_time = 0);

 private:
  void Init();
 private:
  QuotationsCache *quotations_cache_;
  quotations_logic::QuotationsRedis* quotations_redis_;
  //void TimeEvent(int opcode, int time);
  //QuotationsCache *quotations_cache_;
  struct threadrw_t *lock_;
};

class QuotationsEngine {
 private:
  static QuotationsEngine* schduler_engine_;
  static QuotationsManager* schduler_mgr_;

  QuotationsEngine() {
  }
  virtual ~QuotationsEngine() {
  }
 public:
  static QuotationsManager* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new QuotationsManager();
    return schduler_mgr_;
  }

  static QuotationsEngine* GetQuotationsEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new QuotationsEngine();
    return schduler_engine_;
  }
};
}

#endif
