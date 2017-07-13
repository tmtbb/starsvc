//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.8.18 Author: kerry

#ifndef STAR_FLASH_KAFKA_H__
#define STAR_FLASH_KAFKA_H__

#include <list>
#include "logic/star_infos.h"
#include "queue/kafka_producer.h"

namespace flash_logic {

class FlashKafka {

public:
	FlashKafka(config::FileConfig* config);
  FlashKafka(base::ConnAddr& addr);
  virtual ~FlashKafka();

public:
  bool SetFlashOrder(star_logic::TradesOrder& order);
private:
  kafka_producer     kafka_producer_;
};
}
#endif
