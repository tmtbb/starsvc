//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#ifndef QUOTATIONS_REDIS_H__
#define QUOTATIONS_REDIS_H__

#include "config/config.h"
#include "storage/data_engine.h"
#include "logic/swp_infos.h"

namespace quotations_logic {

class QuotationsRedis {
 public:
  QuotationsRedis(config::FileConfig *config);
  virtual ~QuotationsRedis();
 public:
  bool ReadHisTimeGoodsData(const std::string& name,
                            std::list<swp_logic::Quotations>& quotations_list);
 private:
  base_logic::DataEngine* redis_engine_;
};
}
#endif
