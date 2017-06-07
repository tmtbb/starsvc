//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#include "search/schduler_engine.h"
#include "search/search_proto_buf.h"
#include "search/operator_code.h"
#include "search/errno.h"
#include "net/comm_head.h"
#include "net/packet_processing.h"
#include "logic/logic_unit.h"
#include "basic/template.h"
#include <regex.h>

namespace search_logic {
SearchManager* SearchEngine::schduler_mgr_ = NULL;
SearchEngine* SearchEngine::schduler_engine_ = NULL;

SearchManager::SearchManager() {
    search_cache_ = new SearchCache();
    Init();
}

SearchManager::~SearchManager() {
    DeinitThreadrw(lock_);
    search_cache_->search_list_.clear();
    if (search_cache_) {
        delete search_cache_;
        search_cache_ = NULL;
    }

}

void SearchManager::InitDB(search_logic::SearchDB* search_db) {
    search_db_ = search_db;
}

void SearchManager::Init() {
    InitThreadrw(&lock_);
}

void SearchManager::InitSearchData() {
    search_db_->OnGetStarSearchDetail(search_cache_->search_list_);
    LOG_MSG2("search_list %d",search_cache_->search_list_.size());
}


void SearchManager::SendSearchStarInfo(int socket, int64 session, int32 reserved,
                                       const std::string& message) {
    //进行匹配
    std::list<star_logic::StarInfo> list;
    {
        base_logic::RLockGd lk(lock_);
        MatchStarInfo(message, list);
    }
    if (list.size() <= 0) {
        send_error(socket, ERROR_TYPE, NO_HAVE_SEARCH_DATA, session);
        return;
    }

    int32 t_count = 0;
    int32 max_count = 10;
    //发送给客户端
    net_reply::SearchAllStar net_search_all_star;
    while(list.size() > 0 && t_count < max_count) {
        net_reply::StarUnit* net_star_unit =
            new net_reply::StarUnit;
        star_logic::StarInfo star = list.front();
        list.pop_front();
        net_star_unit->set_symbol(star.symbol());
        net_star_unit->set_gender(star.gender());
        net_star_unit->set_name(star.name());
        net_star_unit->set_pic(star.pic());
        net_star_unit->set_wid(star.weibo_index_id());
        net_search_all_star.set_unit(net_star_unit->get());
        t_count++;
    }

    struct PacketControl packet_control;
    MAKE_HEAD(packet_control, S_SEARCH_STAR, SEARCH_TYPE, 0, session, 0);
    packet_control.body_ = net_search_all_star.get();
    send_message(socket, &packet_control);
}


void SearchManager::MatchStarInfo(const std::string& message,
                                  std::list<star_logic::StarInfo>& list) {
    std::string regex_str;
    regex_t o_regex;
    regex_str = message + ".*";
    if (regcomp(&o_regex, regex_str.c_str(), 0) != 0) {
        LOG_ERROR2("message %s regcomp error", message.c_str());
        return;
    }

    SEARCH_LIST::iterator it = search_cache_->search_list_.begin();
    for(; it != search_cache_->search_list_.end(); it++) {
        star_logic::StarInfo star = (*it);
        if(regexec(&o_regex,star.name().c_str(),0,NULL,0) == 0 ||
                regexec(&o_regex,star.introduction().c_str(),0,NULL,0) == 0 ||
                regexec(&o_regex,star.symbol().c_str(),0,NULL,0) == 0) {
            list.push_back(star);
            //LOG_MSG2("name %s",star.name().c_str());
        }
    }
    regfree(&o_regex);
}

}
