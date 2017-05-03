
#include "imcloud/im_mysql.h"

#include <mysql/mysql.h>
#include <sstream>

#include "storage/data_engine.h"
#include "comm/comm_head.h"

#include "logic/logic_comm.h"

namespace im_mysql {

Im_Mysql::Im_Mysql(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

Im_Mysql::~Im_Mysql() {
  if (mysql_engine_) {
    delete mysql_engine_;
  }
  mysql_engine_ = NULL;
}
//写用户信息
int32 Im_Mysql::SetUserInfo(std::string phone_num, std::string& client_ip,
                        DicValue* dic){
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call test('100')";
    dic->SetString(L"sql", ss.str());
    LOG_DEBUG2("%s", ss.str().c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),NULL);
    if (!r) {
      err = SQL_EXEC_ERR;
      break;
    }
    if (dic->empty()) {
      err = PHONE_OR_PASSWD_ERR;
      break;
    }
  } while (0);

  return err;
}
//读用户信息
int32 Im_Mysql::GetStaticInfo(std::string phone, std::string& client_ip,
                                  DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call test('100')";
    dic->SetString(L"sql", ss.str());
    LOG_DEBUG2("%s", ss.str().c_str());
    r = mysql_engine_->ReadData(0, (base_logic::Value*) (dic),
                                CallStaticSelect);
    if (!r) {
      err = SQL_EXEC_ERR;
      break;
    }
    if (dic->empty()) {
      err = PHONE_OR_PASSWD_ERR;
      break;
    }
  } while (0);
  base_logic::DictionaryValue *userinfo = NULL;
  dic->GetDictionary(L"userinfo", &userinfo);
  std::string userid;
  userinfo->GetString(L"userid", &userid);
  return err;
}
void Im_Mysql::CallStaticSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();

  base_logic::DictionaryValue *userinfo = new base_logic::DictionaryValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetString(L"userid", rows[0]);
      if (rows[1] != NULL)
        userinfo->SetString(L"accid", rows[1]);
      dict->Set(L"userinfo", (base_logic::Value *) (userinfo));
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

}

