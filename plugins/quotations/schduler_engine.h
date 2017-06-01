//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef QUOTATIONS_SCHDULER_ENGINE_H__
#define QUOTATIONS_SCHDULER_ENGINE_H__

#include "quotations/quotations_db.h"
#include "logic/star_infos.h"
#include "quotations/quotations_redis.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"


typedef std::map<std::string,star_logic::StarInfo> BASIC_INFO_MAP;

typedef std::list<star_logic::Quotations> QUOTATIONS_LIST; /*行情存储列表，用于每个标的的历史行情，专门用于排序*/
typedef std::map<std::string, QUOTATIONS_LIST> QUOTATIONS_MAP; /*标的ID，行情*//*平台名称:交易所:现货ID 如:JH:TJPME:AG*/
typedef std::map<int64, QUOTATIONS_MAP> QUOTATIONS_ALL_MAP; /*股票,现货,期货, 明星  QUOTATIONS_MAP*/

typedef std::map<std::string, star_logic::Quotations> LAST_QUOTATIONS_MAP;  //最近一次报价 /*key: 标的的key，value:行情数据*/
typedef std::map<int64, LAST_QUOTATIONS_MAP> LAST_QUOTATIONS_ALL_MAP; //股票，现货，期货，明星的最近一次的报价存储（以最近一份为准)， key 类别标识，value，行情数据


typedef std::list<star_logic::Quotations> LAST_QUOTATIONS_SORT_LIST; //最后一次报价存储(最后三秒)，专门用于排序
typedef std::map<int64,LAST_QUOTATIONS_SORT_LIST> LAST_QUOTATIONS_SORT_MAP; //股票，现货，期货，明星，存储最近一次报价 

typedef std::map<std::string,star_logic::Quotations> LAST_SORT_MAP;//专门作为过滤器，防止在排序list中，出现重复标的
typedef std::map<int64,LAST_SORT_MAP> LAST_SORT_ALL_MAP;//股票，现货，期货，明星用于排序处理
 
//正在计算的K线
typedef std::map<std::string, star_logic::Quotations> K_QUOTATIONS_MAP; /*KEY SYMBOL 外汇标识*/
typedef std::map<int32, K_QUOTATIONS_MAP> K_ALL_QUOTATIONS_MAP; /*KEY 时间标识*/

//K线存储
typedef std::map<std::string, QUOTATIONS_LIST> K_HIS_QUOTATIONS_MAP; /*外汇标识*/
typedef std::map<int32, K_HIS_QUOTATIONS_MAP> K_ALL_HIS_QUOTATIONS_MAP; /*时间标识*/

namespace quotations_logic {

enum TIMETYPE{
  FIVE_MINUTE = 300,
  QUARTER_MINUTE = 900,
  HALF_HOUR = 1800,
  ONE_HOUR = 3600
};

class QuotationsCache {
 public:
  BASIC_INFO_MAP     basic_info_map_;
  QUOTATIONS_ALL_MAP quotations_map_;
  LAST_QUOTATIONS_ALL_MAP     last_quotations_map_;
  LAST_QUOTATIONS_SORT_MAP    last_quotations_list_;
  LAST_SORT_ALL_MAP           filer_quotations_map_;
  K_ALL_QUOTATIONS_MAP current_k_all_map_;
  K_ALL_HIS_QUOTATIONS_MAP k_history_all_map_;
};
  
typedef std::map<std::string, QUOTATIONS_LIST> QUOTATIONS_MAP; /*标的ID，行情*/
typedef std::map<int64,QUOTATIONS_MAP> QUOTATIONS_ALL_MAP; /*股票,现货,期货 QUOTATIONS_MAP*/

/*class QuotationsCache {
 public:
  QUOTATIONS_ALL_MAP  quotations_map_;
};*/

class QuotationsManager {
 public:
  QuotationsManager();
  virtual ~QuotationsManager();

  void SetQuotations(star_logic::Quotations& quotation);

  void LoginQuotationsCenter(const int socket);

  void SendRealTime(const int socket, const int64 session, const int32 reversed,
                    base_logic::ListValue* value);

  void SendKChartLine(const int socket, const int64 session,
                      const int32 reversed, const int32 kchar_type,
                      const std::string& symbol, const int64 start_time,
                      const int64 end_time, const int32 count);

  void SendTimeLine(const int socket, const int64 session, const int32 reversed,
                    const int32 atype,const std::string& symbol,
                    const int64 start_time, const int64 end_time,
                    const int32 count);

  void SendSymbolList(const int socket, const int64 session, const int32 reversed,
		      const int32 atype,
                      const int32 sort, const int32 pos, const int32 count);

  void TimeEvent(int opcode, int time);

  void InitRedis(quotations_logic::QuotationsRedis* quotations_redis);

  void InitDB(quotations_logic::QuotationsDB* quotations_db);
  
  void InitGoodsData();

  void InitFoxreData();

  void InitStarData();

  void InitRedisData(const std::string& hash_name, int32 atype);

private:

  void SetKQuotations(const std::string& key, const int32 time_symbol,
                      star_logic::Quotations& quotation);

  void SetQuotationsUnit(star_logic::Quotations& quotation);

  void SetKQuotations(star_logic::Quotations& quotation);
  void SetKQuotationsUnit(const int32 time_symbol,
                          star_logic::Quotations& quotation);

  void SetSortQuotations(star_logic::Quotations& quotation);

  void GetRealTime(const int32 atype, const std::string& symbol,
                   star_logic::Quotations* quotations);

  void GetTimeLine(const int32 atype, const std::string& symbol,
                   std::list<star_logic::Quotations>& list,
                   const int64 end_time = 0);
  
  void GetKChartLine(const int32 chart_type, const std::string& key,
                     std::list<star_logic::Quotations>& list,
                     const int64 end_time = 0);

  void GetSortQuoationList(const int32 atype, std::list<star_logic::Quotations>& list,
		    const int32 pos,const int32 count,const int32 sort);

  void SendQuotationsList(const int socket, const int64 session, const int32 reserved,
			std::list<star_logic::Quotations>& list);
 private:
  void Init();
 private:
  QuotationsCache *quotations_cache_;
  quotations_logic::QuotationsRedis* quotations_redis_;
  quotations_logic::QuotationsDB* quotations_db_;
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
