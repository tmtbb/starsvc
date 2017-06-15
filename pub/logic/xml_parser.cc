//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#include "logic/xml_parser.h"
#include "file/file_util.h"
#include <algorithm>

namespace logic {

XmlParser::XmlParser() {
}

XmlParser::~XmlParser() {

}

base_logic::DictionaryValue* XmlParser::ReadXml(const std::string& file_name) {
  std::string error_str;
  int32 error_code;
  std::string content;
  //"<?xml version=\"1.0\" encoding=\"utf-8\" ?>"
  std::string head_start_str = "<?xml version=";
  std::string head_end_str = "?>";
  file::FilePath sfile_name(file_name);
  bool r = file::ReadFileToString(sfile_name, &content);
  if (!r)
    return NULL;
  base_logic::DictionaryValue *value = NULL;

  content.erase(remove(content.begin(), content.end(), '\n'), content.end());
  content.erase(remove(content.begin(), content.end(), '\r'), content.end());

  size_t start_pos = content.find(head_start_str);
  if (start_pos != std::string::npos) {  //存在XML头
    content = content.substr(start_pos, content.length());
    size_t end_pos = content.find(head_end_str);
    if (end_pos == std::string::npos)
      return NULL;
    content = content.substr(end_pos + head_end_str.length(), content.length());
  }

  base_logic::ValueSerializer* deserializer =
      base_logic::ValueSerializer::Create(base_logic::IMPL_XML, &content);
  value = (base_logic::DictionaryValue*) deserializer->Deserialize(&error_code,
                                                                   &error_str);

  return value;
}

}
