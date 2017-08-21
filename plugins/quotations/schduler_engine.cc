//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "quotations/schduler_engine.h"
#include "quotations/quotations_proto_buf.h"
#include "quotations/operator_code.h"
#include "quotations/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"

namespace quotations_logic {

QuotationsManager* QuotationsEngine::schduler_mgr_ = NULL;
QuotationsEngine* QuotationsEngine::schduler_engine_ = NULL;

QuotationsManager::QuotationsManager() {
    quotations_cache_ = new QuotationsCache();
    Init();
}

QuotationsManager::~QuotationsManager() {
    DeinitThreadrw(lock_);
    quotations_cache_->quotations_map_.clear();
    if (quotations_cache_) {
        delete quotations_cache_;
        quotations_cache_ = NULL;
    }

}

void QuotationsManager::Init() {
    InitThreadrw(&lock_);

    manager_schduler::SchdulerEngine* (*schduler_engine)(void);
    std::string schduler_library = "./data_share.so";
    std::string schduler_func = "GetManagerSchdulerEngine";
    schduler_engine = (manager_schduler::SchdulerEngine* (*)(void))
                      logic::SomeUtils::GetLibraryFunction(
                          schduler_library, schduler_func);
    manager_schduler_engine_ = (*schduler_engine)();
    if (manager_schduler_engine_ == NULL)
        assert(0);
}

void QuotationsManager::InitFoxreData() {
    InitRedisData("SINA:SINA:fx_seurcnh", FORXE_TYPE);
    InitRedisData("SINA:SINA:fx_sjpycnh", FORXE_TYPE);
    InitRedisData("SINA:SINA:fx_susdcnh", FORXE_TYPE);
}

void QuotationsManager::InitGoodsData() {
    InitRedisData("JH:DEFAULT:AG", GOODS_TYPE);
}

void QuotationsManager::InitStarData() {
  //InitRedisData("STAR:STAR")
  BASIC_INFO_MAP star_info_list;
  quotations_db_->OnGetStarInfo(star_info_list);
  BASIC_INFO_MAP::iterator it = star_info_list.begin();
  for(; it != star_info_list.end(); ++it) {
    star_logic::StarInfo& star = it->second;
    if(0 == star.status()){
      std::string key = "star_index:" + star.weibo_index_id();
      InitRedisData(key, STAR_TYPE);

      manager_schduler_engine_->SetStarInfoSchduler(star.symbol(),
                                &star);
    } else {
      //delete
      manager_schduler_engine_->DelStarInfoSchduler(star.symbol());
    }
    
  }

  quotations_db_->OngetSysParamValue(quotations_cache_->sys_param_map_);
  LOG_MSG2("Get sys param size [%d]", quotations_cache_->sys_param_map_.size());
}


void QuotationsManager::InitRedisData(const std::string& hash_name,
                                      int32 atype) {
    std::list<star_logic::Quotations> list;
    quotations_redis_->ReadHisTimeGoodsData(hash_name, list);
    list.sort(star_logic::Quotations::after);
    int32 all = 1440;
    while (list.size() > 0 && all > 0) {
        star_logic::Quotations quotations = list.front();
        list.pop_front();
        quotations.set_type(atype);
        SetQuotationsUnit(quotations);
        SetKQuotations(quotations);
        SetSortQuotations(quotations);
        all--;
    }
}

void QuotationsManager::InitRedis(
    quotations_logic::QuotationsRedis* quotations_redis) {
    quotations_redis_ = quotations_redis;
}

void QuotationsManager::InitDB(
    quotations_logic::QuotationsDB* quotations_db) {
    quotations_db_ = quotations_db;
}

void QuotationsManager::SetKQuotations(star_logic::Quotations& quotation) {
    SetKQuotationsUnit(FIVE_MINUTE, quotation);
    SetKQuotationsUnit(QUARTER_MINUTE, quotation);
    SetKQuotationsUnit(HALF_HOUR, quotation);
    SetKQuotationsUnit(ONE_HOUR, quotation);
}

void QuotationsManager::SetKQuotationsUnit(const int32 time_symbol,
        star_logic::Quotations& quotation) {
    int64 current_time = (quotation.current_unix_time() / time_symbol)
                         * time_symbol;
    std::string key = "star_index:"
                      + quotation.symbol();
    K_QUOTATIONS_MAP k_quotations_map;
    star_logic::Quotations tquotation;

    bool r = base::MapGet<K_ALL_QUOTATIONS_MAP, K_ALL_QUOTATIONS_MAP::iterator,
         int32, K_QUOTATIONS_MAP>(quotations_cache_->current_k_all_map_,
                                  time_symbol, k_quotations_map);
    r = base::MapGet<K_QUOTATIONS_MAP, K_QUOTATIONS_MAP::iterator, std::string,
    star_logic::Quotations>(k_quotations_map, key, tquotation);

    //判断时间是否相等，如果相当调整价格，如果不相等将一下个放入历史列表中，放入新的
    tquotation.set_current_price(quotation.current_price());
    if (!r) {  //不存在 说明该行情数据为开始存当前K线
        k_quotations_map[key] = quotation;
    } else {  //存在
        if ((quotation.current_unix_time() / time_symbol)
                == (tquotation.current_unix_time() / time_symbol)) {
            if (quotation.current_price() < tquotation.low_price())
                tquotation.set_low_price(quotation.current_price());
            else if (quotation.current_price() > tquotation.high_price())
                tquotation.set_high_price(quotation.current_price());
            k_quotations_map[key] = tquotation;
        } else {
            star_logic::Quotations new_quotation;
            tquotation.set_closed_yesterday_price(tquotation.current_price());
            new_quotation.set_opening_today_price(quotation.current_price());
            new_quotation.set_high_price(quotation.current_price());
            new_quotation.set_low_price(quotation.current_price());
            new_quotation.set_current_unix_time(current_time);
            new_quotation.set_symbol(quotation.symbol());
            k_quotations_map[key] = new_quotation;
            //放入历史列表
            SetKQuotations(key, time_symbol, tquotation);
        }
    }
    quotations_cache_->current_k_all_map_[time_symbol] = k_quotations_map;
}

void QuotationsManager::SetKQuotations(const std::string& key,
                                       const int32 time_symbol,
                                       star_logic::Quotations& quotation) {

    K_HIS_QUOTATIONS_MAP his_quotations_map;
    QUOTATIONS_LIST his_quotations_list;
    bool r = base::MapGet<K_ALL_HIS_QUOTATIONS_MAP,
         K_ALL_HIS_QUOTATIONS_MAP::iterator, const int32, K_HIS_QUOTATIONS_MAP>(
             quotations_cache_->k_history_all_map_, time_symbol, his_quotations_map);

    r = base::MapGet<K_HIS_QUOTATIONS_MAP, K_HIS_QUOTATIONS_MAP::iterator,
    std::string, QUOTATIONS_LIST>(his_quotations_map, key,
                                  his_quotations_list);
    his_quotations_list.push_back(quotation);
    his_quotations_map[key] = his_quotations_list;
    quotations_cache_->k_history_all_map_[time_symbol] = his_quotations_map;
}

void QuotationsManager::SetQuotationsUnit(star_logic::Quotations& quotation) {
    bool r = false;
    QUOTATIONS_MAP exchange_quotations;
    QUOTATIONS_LIST quotations_list;

    LAST_QUOTATIONS_MAP last_exchange_quotations;
    star_logic::Quotations temp_quotations;

    std::string key;
    key = "star_index:"+ quotation.symbol();
    //读取上一分钟报价
    r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
    int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                quotation.type(), last_exchange_quotations);
    r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
    std::string, star_logic::Quotations>(last_exchange_quotations, key,
                                         temp_quotations);
    //存储最近一次报价，存储实时报价数据，但list中存储分时数据
    int64 last_unix_time = temp_quotations.current_unix_time() / 60 * 60;
    int64 current_unix_time = quotation.current_unix_time() / 60 * 60;
    last_exchange_quotations[key] = quotation;
    quotations_cache_->last_quotations_map_[quotation.type()] =
        last_exchange_quotations;
    if (last_unix_time == current_unix_time&&r) {
        //LOG_DEBUG2("last time %lld",temp_quotations.current_unix_time()/60*60);
        return;
    }

    r = base::MapGet<QUOTATIONS_ALL_MAP, QUOTATIONS_ALL_MAP::iterator, int32,
    QUOTATIONS_MAP>(quotations_cache_->quotations_map_, quotation.type(),
                    exchange_quotations);
    r = base::MapGet<QUOTATIONS_MAP, QUOTATIONS_MAP::iterator, std::string,
    QUOTATIONS_LIST>(exchange_quotations, key, quotations_list);

    /*LOG_MSG2("exchange_name:{%s} platform:{%s} symbol:{%s} change:{%f}  pchg:{%f} high_price:{%f} low_price:{%f} opening_today_price:{%f} closed_yesterday_price:{%f} current_price:{%f} current_unix_time:{%lld}",
     quotation.exchange_name().c_str(), quotation.platform().c_str(),quotation.symbol().c_str(),
     quotation.change(),quotation.pchg(),quotation.high_price(),quotation.low_price(),
     quotation.opening_today_price(),quotation.closed_yesterday_price(),quotation.current_price(),
     quotation.current_unix_time());*/

    quotations_list.push_back(quotation);
    quotations_list.sort(star_logic::Quotations::cmp);
    exchange_quotations[key] = quotations_list;
    quotations_cache_->quotations_map_[quotation.type()] = exchange_quotations;
}

void QuotationsManager::SetSortQuotations(star_logic::Quotations& quotation) {
    bool r = false;
    LAST_QUOTATIONS_SORT_LIST last_quotations_list;
    LAST_SORT_MAP   filer_sort_map;
    star_logic::Quotations temp_quotations;
    //利用共享内存的机制，不需要对list节点修改
    r = base::MapGet<LAST_SORT_ALL_MAP,LAST_SORT_ALL_MAP::iterator,int64>(quotations_cache_->filer_quotations_map_,
            quotation.type(),filer_sort_map);

    //查找是否已经有这个标的的报价
    r = base::MapGet<LAST_SORT_MAP,LAST_SORT_MAP::iterator,std::string>(filer_sort_map,
            quotation.symbol(),temp_quotations);

    if (r) { //存在，利用共享内存机制，修改行情信息，无需修改list节点
        temp_quotations.set_change(quotation.change());
        temp_quotations.set_pchg(quotation.pchg());
        temp_quotations.set_symbol(quotation.symbol());
        temp_quotations.set_type(quotation.type());
        temp_quotations.set_current_price(quotation.current_price());
        temp_quotations.set_pchg(quotation.pchg());
        temp_quotations.set_current_unix_time(quotation.current_unix_time());
    } else {
        base::MapGet<LAST_QUOTATIONS_SORT_MAP,LAST_QUOTATIONS_SORT_MAP::iterator,LAST_QUOTATIONS_SORT_LIST>(
            quotations_cache_->last_quotations_list_,quotation.type(),last_quotations_list);
        last_quotations_list.push_back(quotation);
        filer_sort_map[quotation.symbol()] = quotation;
        quotations_cache_->filer_quotations_map_[quotation.type()] = filer_sort_map;
        quotations_cache_->last_quotations_list_[quotation.type()] = last_quotations_list;

    }
}

void QuotationsManager::SetQuotations(star_logic::Quotations& quotation) {
    base_logic::WLockGd lk(lock_);
    SetQuotationsUnit(quotation);
    SetKQuotations(quotation);
    SetSortQuotations(quotation);

}

void QuotationsManager::TimeEvent(int opcode, int time) {
    switch (opcode) {
    default:
        break;
    }
}


void QuotationsManager::SendSymbolList(const int socket, const int64 session, const int32 reversed,
                                       const int32 atype, const int32 sort, const int32 pos,
                                       const int32 count) {
  std::list<star_logic::StarInfo> starlist;

  //    GetSortQuoationList(atype, list, pos, count, sort);
  manager_schduler_engine_->GetAllStarInfoSchduler(starlist);
  if (starlist.size() <= 0) {
      send_error(socket, ERROR_TYPE, NO_HAVE_KCHART_DATA,session);
      return;
  }

  bool r = false;
  if(sort == 0)//明星上线时间排序
      starlist.sort(star_logic::StarInfo::sort_add_time);
  else //热度排序
      starlist.sort(star_logic::StarInfo::sort_hot_priority);

  LOG_DEBUG2("Get symbol list size before:%d", starlist.size());
  int32 t_count = 0, curpos = 0;
  std::list<star_logic::StarInfo>::iterator iter = starlist.begin();
  for(; iter != starlist.end(); ){
      if(curpos++ < pos || t_count == count){
          iter = starlist.erase(iter);
          continue;
      }
      ++t_count;
      ++iter;
  }
  LOG_DEBUG2("Get symbol list size after:%d", starlist.size());
  //发送行情列表
  SendQuotationsList(socket, session, reversed, atype, starlist);
}

void QuotationsManager::SendSymbolOne(const int socket, 
                const int64 session, const int32 reversed,
                const std::string &starcode, const int32 atype) {
  //std::list<star_logic::StarInfo> starlist;
  star_logic::StarInfo star;

  //manager_schduler_engine_->GetAllStarInfoSchduler(starlist);
  manager_schduler_engine_->GetStarInfoSchduler(starcode, &star);//获取单个明星信息
  /*
  if (starlist.size() <= 0) {
      send_error(socket, ERROR_TYPE, NO_HAVE_KCHART_DATA,session);
      return;
  }
  */

//  LOG_DEBUG2("Get symbol list size after:%d", starlist.size());
  //发送行情列表
 // SendQuotationsList(socket, session, reversed, atype, starlist);

      
      LAST_QUOTATIONS_MAP last_exchange_quotations;
      star_logic::Quotations temp_quotations;

      std::string key;
      key = "star_index:"+ star.weibo_index_id();
      //读取上一分钟报价
      bool r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
      int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                  atype, last_exchange_quotations);
      r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
      std::string, star_logic::Quotations>(last_exchange_quotations, key,
                                           temp_quotations);


      net_reply::SymbolUnit* r_symbol_unit = new net_reply::SymbolUnit;
      r_symbol_unit->set_wid(star.weibo_index_id());
      r_symbol_unit->set_name(star.name());
      r_symbol_unit->set_star_type(star.type());
      r_symbol_unit->set_pic(star.pic());
      r_symbol_unit->set_home_pic(star.home_pic());
      r_symbol_unit->set_symbol(star.symbol());
      r_symbol_unit->set_system_unix_time(time(NULL));
      r_symbol_unit->set_work(star.work());
      if(r){
        r_symbol_unit->set_current_price(temp_quotations.current_price());
        r_symbol_unit->set_current_unix_time(temp_quotations.current_unix_time());
        r_symbol_unit->set_change(temp_quotations.change());
        r_symbol_unit->set_pchg(temp_quotations.pchg());
        LOG_MSG2("current_unix_time %lld symbol %s current_price %f, change %f,pchg %f",
                temp_quotations.current_unix_time(),star.symbol().c_str(), temp_quotations.current_price(),
                temp_quotations.change(),temp_quotations.pchg());
      }else{
        r_symbol_unit->set_current_price(0);
        r_symbol_unit->set_current_unix_time(time(NULL));
        r_symbol_unit->set_change(0);
        r_symbol_unit->set_pchg(0);
      }
      //r_symbol_unit->set_cur_pulish_type(star.cur_pulish_type());
      r_symbol_unit->set_pushlish_type(star.home_pulish_type());
      r_symbol_unit->set_home_button_pic(star.home_button_pic());
      
      //symbol_list.set_unit(r_symbol_unit->get());
      
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_SYMBOL_ONLY, QUOTATIONS_TYPE, 0,
              session, reversed);
    packet_control.body_ = r_symbol_unit->get();
    send_message(socket, &packet_control);
}
void QuotationsManager::SendHomeSymbolList(const int socket, const int64 session, const int32 reversed,
                                       const int32 atype) {
    
  std::list<star_logic::StarInfo> starlist;
  manager_schduler_engine_->GetAllStarInfoSchduler(starlist);
  if (starlist.size() <= 0) {
    send_error(socket, ERROR_TYPE, NO_HAVE_SYMBOL_DATA,session);
    return;
  }

  std::list<star_logic::StarInfo>::iterator iter = starlist.begin();
  for(; iter != starlist.end(); ){
    if(1 != iter->display_on_home()){
        iter = starlist.erase(iter);
        continue;
    }
    ++iter;
  }
  LOG_DEBUG2("Get home symbol list size:%d", starlist.size());
  //发送行情列表
  SendQuotationsList(socket, session, reversed, atype, starlist);
}


void QuotationsManager::SendQuotationsList(const int socket, const int64 session, const int32 reversed, 
        const int32 atype, std::list<star_logic::StarInfo>& starlist) {
  net_reply::SymbolList symbol_list;
  bool r = false;

  {
    base_logic::RLockGd lk(lock_);
    while(starlist.size() > 0) {
      star_logic::StarInfo star = starlist.front();
      starlist.pop_front();
      
      
      LAST_QUOTATIONS_MAP last_exchange_quotations;
      star_logic::Quotations temp_quotations;

      std::string key;
      key = "star_index:"+ star.weibo_index_id();
      //读取上一分钟报价
      r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
      int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                  atype, last_exchange_quotations);
      r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
      std::string, star_logic::Quotations>(last_exchange_quotations, key,
                                           temp_quotations);


      net_reply::SymbolUnit* r_symbol_unit = new net_reply::SymbolUnit;
      r_symbol_unit->set_wid(star.weibo_index_id());
      r_symbol_unit->set_name(star.name());
      r_symbol_unit->set_star_type(star.type());
      r_symbol_unit->set_pic(star.pic());
      r_symbol_unit->set_home_pic(star.home_pic());
      r_symbol_unit->set_symbol(star.symbol());
      r_symbol_unit->set_system_unix_time(time(NULL));
      r_symbol_unit->set_work(star.work());
      if(r){
        r_symbol_unit->set_current_price(temp_quotations.current_price());
        r_symbol_unit->set_current_unix_time(temp_quotations.current_unix_time());
        r_symbol_unit->set_change(temp_quotations.change());
        r_symbol_unit->set_pchg(temp_quotations.pchg());
        LOG_MSG2("current_unix_time %lld symbol %s current_price %f, change %f,pchg %f",
                temp_quotations.current_unix_time(),star.symbol().c_str(), temp_quotations.current_price(),
                temp_quotations.change(),temp_quotations.pchg());
      }else{
        r_symbol_unit->set_current_price(0);
        r_symbol_unit->set_current_unix_time(time(NULL));
        r_symbol_unit->set_change(0);
        r_symbol_unit->set_pchg(0);
      }
      //r_symbol_unit->set_cur_pulish_type(star.cur_pulish_type());
      r_symbol_unit->set_pushlish_type(star.home_pulish_type());
      r_symbol_unit->set_home_button_pic(star.home_button_pic());
      
      symbol_list.set_unit(r_symbol_unit->get());
    }
  }

  std::map<std::string, std::string>::iterator iter
    = quotations_cache_->sys_param_map_.find("HOME_LAST_PIC");
  if(iter != quotations_cache_->sys_param_map_.end())
    symbol_list.set_home_last_pic(iter->second);

  if (symbol_list.Size() != 0) {
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_SYMBOL_LIST, QUOTATIONS_TYPE, 0,
              session, reversed);
    packet_control.body_ = symbol_list.get();
    send_message(socket, &packet_control);
  }else {
     send_error(socket, ERROR_TYPE, NO_HAVE_SYMBOL_DATA,session); 
  }
}

void QuotationsManager::SendKChartLine(const int socket, const int64 session,
                                       const int32 reversed,
                                       const int32 kchar_type,
                                       const std::string& symbol,
                                       const int64 start_time,
                                       const int64 end_time,
                                       const int32 count) {
    net_reply::KChartLine kchart_line;
    std::list<star_logic::Quotations> list;
    int32 base_num = 50;
    if (reversed /1000 == HTTP)
        base_num = count;
    else
        base_num = base_num < count ? base_num : count;
    std::string key = "star_index:" + symbol;
    GetKChartLine(kchar_type, key, list, end_time);
    if (list.size() <= 0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_KCHART_DATA,session);
        return;
    }
    list.sort(star_logic::Quotations::after);
    int32 t_count = 0;
    while (list.size() > 0 && t_count < count) {
        star_logic::Quotations quotations = list.front();
        list.pop_front();
        if (quotations.current_unix_time() > start_time)
            continue;
        net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
        r_real_time_unit->set_change(quotations.change());
        r_real_time_unit->set_pchg(quotations.pchg());
        r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
        r_real_time_unit->set_closed_yesterday_price(
            quotations.closed_yesterday_price());
        r_real_time_unit->set_current_price(quotations.current_price());
        r_real_time_unit->set_high_price(quotations.high_price());
        r_real_time_unit->set_low_price(quotations.low_price());
        r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
        r_real_time_unit->set_symbol(quotations.symbol());
        kchart_line.set_unit(r_real_time_unit->get());
        t_count++;
        if (kchart_line.Size() % base_num == 0 && kchart_line.Size() != 0) {
            kchart_line.set_stype(kchar_type);
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, S_KCHART_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                      session, 0);
            packet_control.body_ = kchart_line.get();
            send_message(socket, &packet_control);
            kchart_line.Reset();
        }
    }

    if (kchart_line.Size() > 0) {
        kchart_line.set_stype(kchar_type);
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, S_KCHART_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                  session, 0);
        packet_control.body_ = kchart_line.get();
        send_message(socket, &packet_control);
    }

}

void QuotationsManager::SendTimeLine(const int socket, const int64 session,
                                     const int32 reversed, const int32 atype,
                                     const std::string& symbol,
                                     const int64 start_time,
                                     const int64 end_time,
                                     const int32 count) {
    net_reply::TimeLine time_line;
    std::list<star_logic::Quotations> list;
    int32 base_num = 50;
    if (reversed /1000 == HTTP)
        base_num = count;
    else
        base_num = base_num < count ? base_num : count;
    std::string key = "star_index:" + symbol;
    GetTimeLine(atype, key, list, end_time);
    if (list.size() <= 0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_TIME_LINE_DATA,session);
        return;
    }
    list.sort(star_logic::Quotations::cmp);
    int32 t_count = 0;
    while (list.size() > 0 && t_count < count) {
        star_logic::Quotations quotations = list.front();
        list.pop_front();
        if (quotations.current_unix_time() > start_time)
            continue;
        net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
        r_real_time_unit->set_change(quotations.change());
        r_real_time_unit->set_pchg(quotations.pchg());
        r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
        r_real_time_unit->set_closed_yesterday_price(
            quotations.closed_yesterday_price());
        r_real_time_unit->set_current_price(quotations.current_price());
        //r_real_time_unit->set_high_price(quotations.high_price());
        //r_real_time_unit->set_low_price(quotations.low_price());
        r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
        r_real_time_unit->set_symbol(quotations.symbol());
        time_line.set_unit(r_real_time_unit->get());
        t_count++;
        if (time_line.Size() % base_num == 0 && time_line.Size() != 0) {
            struct PacketControl packet_control;
            MAKE_HEAD(packet_control, S_QUOTATIONS_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                      session, 0);
            packet_control.body_ = time_line.get();
            send_message(socket, &packet_control);
            time_line.Reset();
        }
    }

    if (time_line.Size() > 0) {
        struct PacketControl packet_control;
        MAKE_HEAD(packet_control, S_QUOTATIONS_TIME_LINE_DATA, QUOTATIONS_TYPE, 0,
                  session, 0);
        packet_control.body_ = time_line.get();
        send_message(socket, &packet_control);
    }

}

void QuotationsManager::SendRealTime(const int socket, const int64 session,
                                     const int32 reversed,
                                     base_logic::ListValue* value) {
    bool r;
    net_reply::RealTime net_reply_real_time;
    for (base_logic::ListValue::iterator it = value->begin(); it != value->end();
            it++) {
        star_logic::Quotations quotations;
        base_logic::Value* value = (*it);

        net_request::RealTimeUnit real_time_unit;
        r = real_time_unit.set_htt_packet((base_logic::DictionaryValue*) (value));
        if(!r){
          LOG_DEBUG2("SendRealTime get fields error.[%d]", -1);
          continue;
        }

        //查找对应的行情数据
        std::string key = "star_index:" + real_time_unit.symbol();
        GetRealTime(real_time_unit.atype(), key, &quotations);
        if (quotations.current_unix_time() <= 0)
            continue;
        net_reply::RealTimeUnit* r_real_time_unit = new net_reply::RealTimeUnit;
        r_real_time_unit->set_change(quotations.change());
        r_real_time_unit->set_pchg(quotations.pchg());
        r_real_time_unit->set_opening_today_price(quotations.opening_today_price());
        r_real_time_unit->set_closed_yesterday_price(
            quotations.closed_yesterday_price());
        r_real_time_unit->set_current_price(quotations.current_price());
        r_real_time_unit->set_high_price(quotations.high_price());
        r_real_time_unit->set_low_price(quotations.low_price());
        r_real_time_unit->set_current_unix_time(quotations.current_unix_time());
        r_real_time_unit->set_symbol(quotations.symbol());
        r_real_time_unit->set_system_unix_time(time(NULL));

        net_reply_real_time.set_unit(r_real_time_unit->get());
    }

    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, R_QUOTATIONS_REAL_TIME_DATA, QUOTATIONS_TYPE, 0,
              session, 0);
    packet_control.body_ = net_reply_real_time.get();
    send_message(socket, &packet_control);
}

void QuotationsManager::GetKChartLine(const int32 chart_type,
                                      const std::string& key,
                                      std::list<star_logic::Quotations>& list,
                                      const int64 end_time) {
    bool r = false;
    base_logic::RLockGd lk(lock_);
    K_HIS_QUOTATIONS_MAP his_quotations_map;
    QUOTATIONS_LIST his_quotations_list;
    r = base::MapGet<K_ALL_HIS_QUOTATIONS_MAP, K_ALL_HIS_QUOTATIONS_MAP::iterator,
    const int32, K_HIS_QUOTATIONS_MAP>(quotations_cache_->k_history_all_map_,
                                       chart_type, his_quotations_map);

    r = base::MapGet<K_HIS_QUOTATIONS_MAP, K_HIS_QUOTATIONS_MAP::iterator,
    std::string, QUOTATIONS_LIST>(his_quotations_map, key,
                                  his_quotations_list);
    int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60)
                      - (8 * 60 * 60);  //当前为东八区

    if (!(end_time == 0))
        start_pan = end_time;

    his_quotations_list.sort(star_logic::Quotations::after);
    QUOTATIONS_LIST::iterator it = his_quotations_list.begin();
    for (; it != his_quotations_list.end(); it++) {
        star_logic::Quotations quotations = (*it);
        //LOG_MSG2("count:{%d} index:{%d} unix_time:{%lld}", quotations_list.size(),
        //  i,quotations.current_unix_time());
        if (quotations.current_unix_time() >= start_pan)
            list.push_back(quotations);
        else
            break;
    }
}

void QuotationsManager::GetSortQuoationList(const int32 atype,std::list<star_logic::Quotations>& list,
        const int32 pos, const int32 count, const int32 sort) {

    bool r = false;
    base_logic::RLockGd lk(lock_);
    LAST_QUOTATIONS_SORT_LIST quotations_list;

    r = base::MapGet<LAST_QUOTATIONS_SORT_MAP, LAST_QUOTATIONS_SORT_MAP::iterator, int32,
    LAST_QUOTATIONS_SORT_LIST>(quotations_cache_->last_quotations_list_, atype,
                               quotations_list);
    if (!r)
        return;

    int32 t_start = 0;
    int32 t_count = 0;
    if (sort == 0) //升序
        quotations_list.sort(star_logic::Quotations::asc);
    else
        quotations_list.sort(star_logic::Quotations::desc);

    while(quotations_list.size() > 0 && t_count < count) {
        star_logic::Quotations quotations = quotations_list.front();
        quotations_list.pop_front();
        t_start++;
        if (t_start < pos)
            continue;
        list.push_back(quotations);
        t_count++;
    }
}


void QuotationsManager::GetTimeLine(const int32 atype,
                                    const std::string& symbol,
                                    std::list<star_logic::Quotations>& list,
                                    const int64 end_time) {
    bool r = false;
    base_logic::RLockGd lk(lock_);
    QUOTATIONS_MAP exchange_quotations;
    QUOTATIONS_LIST quotations_list;
    r = base::MapGet<QUOTATIONS_ALL_MAP, QUOTATIONS_ALL_MAP::iterator, int32,
    QUOTATIONS_MAP>(quotations_cache_->quotations_map_, atype,
                    exchange_quotations);
    if (!r)
        return;

    r = base::MapGet<QUOTATIONS_MAP, QUOTATIONS_MAP::iterator, std::string,
    QUOTATIONS_LIST>(exchange_quotations, symbol, quotations_list);

    quotations_list.sort(star_logic::Quotations::cmp);
//遍历
    int64 start_pan = ((time(NULL) / 24 / 60 / 60) * 24 * 60 * 60)
                      - (8 * 60 * 60);  //当前为东八区
    if (!(end_time == 0))
        start_pan  = end_time;
    int i = 0;
    QUOTATIONS_LIST::iterator it = quotations_list.begin();
    for (; it != quotations_list.end(); it++) {
        i++;
        star_logic::Quotations quotations = (*it);
        //LOG_MSG2("count:{%d} index:{%d} unix_time:{%lld} start_pan:{%lld}", quotations_list.size(),
        //i,quotations.current_unix_time(),start_pan);
        if (quotations.current_unix_time() >= start_pan)
            list.push_back(quotations);
        /*else
          break;*/
    }
}

void QuotationsManager::GetRealTime(const int32 atype,
                                    const std::string& symbol,
                                    star_logic::Quotations* quotations) {
    bool r = false;
    base_logic::WLockGd lk(lock_);
    LAST_QUOTATIONS_MAP exchange_quotations;
    star_logic::Quotations qs;
    r = base::MapGet<LAST_QUOTATIONS_ALL_MAP, LAST_QUOTATIONS_ALL_MAP::iterator,
    int32, LAST_QUOTATIONS_MAP>(quotations_cache_->last_quotations_map_,
                                atype, exchange_quotations);
    if (!r)
        return;
    r = base::MapGet<LAST_QUOTATIONS_MAP, LAST_QUOTATIONS_MAP::iterator,
    std::string, star_logic::Quotations>(exchange_quotations, symbol, qs);
    if (!r)
        return;
    (*quotations) = qs;
}

void QuotationsManager::LoginQuotationsCenter(const int socket) {
    quotations_logic::net_other::Login login;
    login.set_aid(time(NULL));
    login.set_atype(4);
    login.set_passowrd("1234567x");
    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, LOGIN_QUOTATIONS, 1, 0, 0, 0);
    packet_control.body_ = login.get();
    send_message(socket, &packet_control);
}



void QuotationsManager::GetQiniuUrlList(const int socket, const int64 session, 
                                      const int32 reversed) {
  base_logic::DictionaryValue* ret = new base_logic::DictionaryValue();
  std::map<std::string, std::string>::iterator iter;
  
  iter = quotations_cache_->sys_param_map_.find("QINIU_URL_HUADONG");
  if(iter != quotations_cache_->sys_param_map_.end()){
    base_logic::StringValue* t_huadong = new base_logic::StringValue(iter->second);
    ret->Set("QINIU_URL_HUADONG", t_huadong);
  }

  iter = quotations_cache_->sys_param_map_.find("QINIU_URL_HUABEI");
  if(iter != quotations_cache_->sys_param_map_.end()){
    base_logic::StringValue* t_huabei = new base_logic::StringValue(iter->second);
    ret->Set("QINIU_URL_HUABEI", t_huabei);
  }

  iter = quotations_cache_->sys_param_map_.find("QINIU_URL_HUANAN");
  if(iter != quotations_cache_->sys_param_map_.end()){
    base_logic::StringValue* t_huanan = new base_logic::StringValue(iter->second);
    ret->Set("QINIU_URL_HUANAN", t_huanan);
  }

  struct PacketControl packet_control;
  MAKE_HEAD(packet_control, S_GET_QINIU_URL, QUOTATIONS_TYPE, 0, session, reversed);
  packet_control.body_ = ret;
  send_message(socket, &packet_control);
  
}


}
