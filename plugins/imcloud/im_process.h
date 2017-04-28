#ifndef PLUGINS_IMCLOUD_IM_PROCESS_H_
#define PLUGINS_IMCLOUD_IM_PROCESS_H_

#include <iostream>
#include <fstream>
#include <iomanip>

#include "basic/md5sum.h"
#include "http/http_method.h"
#include "util/util.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"

#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/err.h>
#include "basic/base64.h"

namespace im_process {

class ImProcess {
 public:
  ImProcess();
  ~ImProcess();

 public:
  std::string gettoken(std::string accid,std::string name);
  
};

}  // namespace 

#endif