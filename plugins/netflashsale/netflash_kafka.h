//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.8.18 Author: kerry

#ifndef STAR_FLASH_KAFKA_H__
#define STAR_FLASH_KAFKA_H__

#include <list>
#include "logic/star_infos.h"
#include "queue/kafka_producer.h"

namespace netflash_logic {

class NetflashKafka {

public:
	NetflashKafka(config::FileConfig* config);
  NetflashKafka(base::ConnAddr& addr);
  virtual ~NetflashKafka();

public:
  bool SetNetflashOrder(star_logic::TradesOrder& order);
private:
  kafka_producer     kafka_producer_;
};
}
#endif
