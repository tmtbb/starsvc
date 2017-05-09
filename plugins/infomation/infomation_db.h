
#ifndef PLUGINS_INFOMATION_MYSQL_H_
#define PLUGINS_INFOMATION_MYSQL_H_

#include "config/config.h"
#include "basic/basictypes.h"

#include "storage/data_engine.h"

#include "net/typedef.h"

namespace infomation_mysql {

class Infomation_Mysql {
 public:
  Infomation_Mysql(config::FileConfig* config);
  ~Infomation_Mysql();

  bool addstarinfo(const std::string& code,
  						const std::string& phone,
  						const std::string& name,
  						const int64 gender,
  						const std::string& brief_url,
  						const double price,
  						const std::string& accid);
  bool getstarinfo(const std::string& code,const std::string& phone,DicValue &ret,int64 all);

  static void Callpublicback(void* param, base_logic::Value* value);
  
  static void Callgetinfo(void* param, base_logic::Value* value);
  
  private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace 

#endif
