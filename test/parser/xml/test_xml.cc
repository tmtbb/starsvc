//  Copyright (c) 2015-2016 The kid Authors. All rights reserved.
//   Created on: 2016.12.23 Author: kerry

#include "gtest/gtest.h"
#include "logic/logic_comm.h"
#include "logic/base_values.h"
#include "basic/basictypes.h"
#include "log/mig_log.h"
#include "basic/radom_in.h"
#include "logic/xml_parser.h"
#include <string>

class XueQiuCookieTest: public testing::Test {
};


TEST(XueQiuCookieTest, Basic){
  std::string file_name = "/home/ky/trades/bin/plugins/pay/custom_config1.xml";
  logic::XmlParser* xml_parser = new logic::XmlParser();
  base_logic::DictionaryValue* value = xml_parser->ReadXml(file_name);
  if (xml_parser) {delete xml_parser;xml_parser = NULL;}
}
