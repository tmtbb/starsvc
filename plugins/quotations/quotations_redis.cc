//  Copyright (c) 2016-2017 The quotations Authors. All rights reserved.
//  Created on: 2017年1月7日 Author: kerry

#include "quotations/quotations_redis.h"
#include "storage/data_engine.h"
#include "basic/basic_util.h"
#include "logic/logic_comm.h"

namespace quotations_logic {

QuotationsRedis::QuotationsRedis(config::FileConfig* config) {
  redis_engine_ = base_logic::DataEngine::Create(REIDS_TYPE);
  redis_engine_->InitParam(config->redis_list_);
}

QuotationsRedis::~QuotationsRedis() {
  if (redis_engine_) {
    delete redis_engine_;
    redis_engine_ = NULL;
  }
}

bool QuotationsRedis::ReadHisTimeGoodsData(
    const std::string& name,
    std::list<swp_logic::Quotations>& quotations_list) {
  //[hash name] rule : platform:exchangename:type: FX:PMEC:AG
  // [key name] rule : unix - minute
  base_logic::DictionaryValue value;
  value.SetString(L"name", name);

  bool r = redis_engine_->ReadData(LIST_VALUE, &value,
                                   NULL);
  if (!r)
    return false;
  base_logic::ListValue* list = NULL;

  r = value.GetList(L"value",&list);
  if (!r)
    return false;
  for (base_logic::ListValue::iterator it = list->begin(); it != list->end(); ++it) {
    swp_logic::Quotations quotations;
    base_logic::Value* string_value = (*it);
    std::string str;
    r = string_value->GetAsString(&str);
    if (!r)
      continue;
    quotations.ValueDeserialize(str);
    quotations_list.push_back(quotations);
  }
  return r;
}

}
