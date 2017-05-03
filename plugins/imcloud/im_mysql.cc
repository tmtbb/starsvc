
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
int32 Im_Mysql::GetStaticInfo(std::string phone, std::string& client_ip,
                                  DicValue* dic) {
  int32 err = 0;
  bool r = false;
  do {
    std::stringstream ss;
    ss << "call proc_UserLoginSelect('" << phone << "','" << client_ip << "')";
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
  return err;
}
void Im_Mysql::CallStaticSelect(void* param, base_logic::Value* value) {
  base_storage::DBStorageEngine* engine =
      (base_storage::DBStorageEngine*) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  DicValue* userinfo = new DicValue();
  DicValue* dict = reinterpret_cast<DicValue*>(value);
  if (num > 0) {
    while (rows = (*(MYSQL_ROW*) (engine->FetchRows()->proc))) {
      if (rows[0] != NULL)
        userinfo->SetBigInteger(L"id", atoll(rows[0]));
      if (rows[1] != NULL)
        userinfo->SetString(L"screenName", rows[1]);
      if (rows[2] != NULL)
        userinfo->SetCharInteger(L"gender", atoi(rows[2]));
      dict->Set(L"userinfo", userinfo);
    }
  } else {
    LOG_ERROR ("CallUserLoginSelect count < 0");
  }
  dict->Remove(L"sql", &value);
}

}

