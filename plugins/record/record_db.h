//  Copyright (c) 2017-2018 The STAR Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry

#ifndef STAR_RECORD_DB_H_
#define STAR_RECORD_DB_H_

#include <string>
#include <list>
#include "record/record_info.h"
#include "storage/data_engine.h"
#include "logic/star_infos.h"
#include "config/config.h"
#include "basic/basictypes.h"
#include "logic/base_values.h"
#include "basic/scoped_ptr.h"

namespace record_logic {

class RecordDB {
public:
    RecordDB(config::FileConfig* config);
    ~RecordDB();


private:
    base_logic::DataEngine* mysql_engine_;
};

}  // namespace record_logic

#endif /* STAR_RECORD_DB_H_ */
