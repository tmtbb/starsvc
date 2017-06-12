//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef SWP_LOGIC_XML_PARSER_H__
#define SWP_LOGIC_XML_PARSER_H__

#include "logic/base_values.h"
#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace logic {

class XmlParser {
 public:
  XmlParser();
  virtual ~XmlParser();
 public:
  base_logic::DictionaryValue* ReadXml(const std::string& file_name);

};
}
#endif
