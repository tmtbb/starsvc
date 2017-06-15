//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "record/record_db.h"
#include "basic/basic_util.h"

namespace record_logic {

RecordDB::RecordDB(config::FileConfig* config) {
    mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
    mysql_engine_->InitParam(config->mysql_db_list_);
}

RecordDB::~RecordDB() {
    if (mysql_engine_) {
        delete mysql_engine_;
        mysql_engine_ = NULL;
    }
}


}  // namespace record_logic

