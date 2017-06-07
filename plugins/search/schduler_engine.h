//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STAR_SCHDULER_ENGINE__H_
#define STAR_SCHDULER_ENGINE__H_

#include <list>
#include "search/search_db.h"
#include "logic/star_infos.h"
#include "thread/base_thread_handler.h"
#include "thread/base_thread_lock.h"
namespace search_logic{

typedef std::list<star_logic::StarInfo> SEARCH_LIST; 

class SearchCache {
 public:
  SEARCH_LIST  search_list_;
};


class SearchManager {
 public:
    SearchManager();
    virtual ~SearchManager();
    void InitDB(search_logic::SearchDB* search_db);
    void InitSearchData();
    void SendSearchStarInfo(int socket, int64 session, int32 reserved,const std::string& message);

private:
    void MatchStarInfo(const std::string& message,std::list<star_logic::StarInfo>& list);
 private:
    void Init();
 private:
    SearchCache*               search_cache_;
    search_logic::SearchDB*    search_db_;
    struct threadrw_t*         lock_;
};


class SearchEngine {
 private:
  static SearchEngine* schduler_engine_;
  static SearchManager* schduler_mgr_;

  SearchEngine() {
  }
  virtual ~SearchEngine() {
  }
 public:
  static SearchManager* GetSchdulerManager() {
    if (schduler_mgr_ == NULL)
      schduler_mgr_ = new SearchManager();
    return schduler_mgr_;
  }

  static SearchEngine* GetSearchEngine() {
    if (schduler_engine_ == NULL)
      schduler_engine_ = new SearchEngine();
    return schduler_engine_;
  }

  static void FreeSchdulerManager() {
    if (schduler_mgr_) {
      delete schduler_mgr_;
      schduler_mgr_ = NULL;
    }
  }

  static void FreeSearchEngine() {
    if (schduler_engine_) {
      delete schduler_engine_;
      schduler_engine_ = NULL;
    }
  }
};
}

#endif
