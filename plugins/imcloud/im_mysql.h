
#ifndef PLUGINS_IMCLOUD_IM_MYSQL_H_
#define PLUGINS_IMCLOUD_IM_MYSQL_H_

#include "config/config.h"
#include "basic/basictypes.h"

#include "storage/data_engine.h"

#include "net/typedef.h"

namespace im_mysql {

class Im_Mysql {
 public:
  Im_Mysql(config::FileConfig* config);
  ~Im_Mysql();
  //写入用户信息到数据库
  int32 SetUserInfo(int64 userid, int64 phonenum, std::string name,std::string accid,std::string token,
                        DicValue* dic);

  //获取用户信息
  int32 GetStaticInfo(int64 phonenum, DicValue* dic);

  static void CallStaticSelect(void* param, base_logic::Value* value);

  bool gettalkingtimes(std::string& phone,std::string& starcode,int64 &times,
  							  std::string& accid,std::string& faccid);
  
  bool ReduceTalkingtimes(std::string& accid,std::string& faccid);

  bool delorderrecord(std::string& accid,std::string& faccid);
  static void Callreducetalkingtimes(void* param, base_logic::Value* value);

  static void Callgettalkingtimes(void* param, base_logic::Value* value);
  
  private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace 

#endif
