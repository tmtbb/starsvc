//  Copyright (c) 2015-2015 The quotations Authors. All rights reserved.
//  Created on: 2017年1月3日 Author: kerry

#ifndef QUOTATIONS_PUB_LOGIC_NET_HTTP_API_H_
#define QUOTATIONS_PUB_LOGIC_NET_HTTP_API_H_

namespace quotations_logic {

class HTTPAPI {
 public:
  static bool RequestGetMethod(const std::string& url,
                               base_logic::DictionaryValue* info,
                               std::string& result);

  static bool RequestPostMethod(const std::string& url,
                                base_logic::DictionaryValue* info,
                                std::string& result);

};
}

#endif /* QUOTATIONS_GOODS_FX_PULL_ENGINE_H_ */
