
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
  						const std::string& accid,
  						const std::string& picurl);
  //获取粉丝评论
  bool getfanscomments(const std::string& starcode,DicValue &ret,int64& startnum,int64& endnum);
  static void Callgetfanscomments(void* param, base_logic::Value* value);
  //获取全量明星信息
  bool getstarinfo(const std::string& code,const std::string& phone,DicValue &ret,int64 all);

  //获取banner信息
  bool getbannerinfo(const std::string& code,DicValue &ret,int64 all);
  
  //获取预约明星列表
  bool getorderstarinfo(const std::string& code,const std::string& phone,DicValue &ret);

  //获取咨询列表
  bool getstarnews(const std::string& code,const std::string& name,DicValue &ret,
  					int64& startnum,int64& endnum,int64& all);
  
  //获取明星服务列表
  bool getstarservicelist(const std::string& code, DicValue &ret);
  //用户订购明星服务
  bool userorderstarservice(const int64 uid, const std::string& starcode,const int64 mid,
  	        const std::string& cityname,const std::string& appointtime,const int64 meettype,
  	        const std::string& comment);
  					
  //获取用户订购明星数
  bool getuserstaramount(const int64 uid, int64& num);
  
  //获取用户明星时间
  bool getuserstartime(const int64 uid, const std::string code, int64& time);
  
  //获取明星时间
  bool getstartime(const std::string code, int64& time);
  
  static void Callpublicback(void* param, base_logic::Value* value);
  
  static void Callgetinfo(void* param, base_logic::Value* value);

  static void Callgetbannerinfo(void* param, base_logic::Value* value);

  static void Callgetorderstarinfo(void* param, base_logic::Value* value);

  static void Callgetstarnewsinfo(void* param, base_logic::Value* value);
  	
  static void Callgetstarservicelist(void* param, base_logic::Value* value);
  static void Calluserorderstarservice(void* param, base_logic::Value* value);
  static void Callgetuserstaramount(void* param, base_logic::Value* value);
  static void Callgetuserstartime(void* param, base_logic::Value* value);
  static void Callgetstartime(void* param, base_logic::Value* value);
  private:
  base_logic::DataEngine* mysql_engine_;
};

}  // namespace 

#endif
