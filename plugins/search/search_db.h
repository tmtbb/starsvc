//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STAR_SEARCH_DB_H_
#define STAR_SEARCH_DB_H_

#include <string>
#include <list>
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace search_logic {

class SearchDB {
public:
    SearchDB(config::FileConfig* config);
    ~SearchDB();
    bool OnGetStarSearchDetail(std::list<star_logic::StarInfo>& list);
public:
    static void CallGetSearchDetail(void* param, base_logic::Value* value);

private:
    base_logic::DataEngine* mysql_engine_;
};

}  // namespace search_logic

#endif /* STAR_SEARCH_DB_H_ */
