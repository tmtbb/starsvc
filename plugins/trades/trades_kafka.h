//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.8.18 Author: kerry

#ifndef STAR_TRADES_KAFKA_H__
#define STAR_TRADES_KAFKA_H__

#include <list>
#include "logic/star_infos.h"
#include "queue/kafka_producer.h"

namespace trades_logic {

class TradesKafka {

 public:
  TradesKafka(config::FileConfig* config, int32 id);
  TradesKafka(base::ConnAddr& addr);
  virtual ~TradesKafka();

 public:
  bool SetTradesPosition(star_logic::TradesPosition& position);
  bool SetTradesOrder(star_logic::TradesOrder& order);
  bool SetPushMessage(star_logic::PushMessage& message);
 private:
  kafka_producer     kafka_producer_;
};
}
#endif
