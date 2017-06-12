//  Copyright (c) 2015-2016 The kid Authors. All rights reserved.
//   Created on: 2017.2.15 Author: kerry

#include "gtest/gtest.h"
#include "logic/logic_comm.h"
#include "basic/basictypes.h"
#include "log/mig_log.h"
#include "basic/radom_in.h"
#include "log/mig_log.h"
#include "pub/net/comm_head.h"
#include "plugins/history/operator_code.h"
#include "pub/fcgimodule/fcgimodule.h"
#include <string>

class PacketStreamTest: public testing::Test {
};

TEST(XueQiuCookieTest, Basic){
  fcgi_module::FcgiModule fcgi_client;
  std::string core_sock_file = "/var/www/tmp/swpcorefile";
  fcgi_client.Init(core_sock_file,HISTORY_TYPE,
                   R_HISTORY_TRADES,1);
  fcgi_client.TestStrPacket();
  fcgi_client.Run();
  fcgi_client.Close();

}
