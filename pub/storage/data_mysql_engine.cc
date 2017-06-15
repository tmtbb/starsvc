#include "data_mysql_engine.h"

namespace base_logic {

void DataMYSQLEngine::InitParam(std::list<base::ConnAddr>& addrlist) {
  db_pool_.Init(addrlist);
  addrlist_ = addrlist;
}

void DataMYSQLEngine::Release() {
  db_pool_.Dest();
}

bool DataMYSQLEngine::WriteData(const int32 type, base_logic::Value* value) {
  bool r = false;
  std::string sql;
  base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*) (value);
  r = dict->GetString(L"sql", &sql);
  if (!r) {
    LOG_ERROR("WriteData sql error");
    return r;
  }
  base_storage::DBStorageEngine* engine = db_pool_.DBConnectionPop();
  if (engine == NULL) {
    LOG_ERROR("GetConnection Error");
    return false;
  }
  engine->FreeRes();
  r = engine->SQLExec(sql.c_str());
  if (!r) {
    LOG_ERROR("exec sql error");
    return false;
  }

  // engine->Release();
  db_pool_.DBConnectionPush(engine);
  return true;
}

bool DataMYSQLEngine::ReadData(const int32 type, base_logic::Value* value,
                               void (*storage_get)(void*, base_logic::Value*)) {
  bool r = false;
  base_storage::DBStorageEngine* engine = db_pool_.DBConnectionPop();
  do {
    std::string sql;
    base_logic::DictionaryValue* dict = (base_logic::DictionaryValue*) (value);
    r = dict->GetString(L"sql", &sql);
    if (!r) {
      LOG_ERROR("ReadData sql error");
      r = false;
      break;
    }
    if (engine == NULL) {
      LOG_ERROR("GetConnection Error");
      r = false;
      break;
    }
    engine->FreeRes();
    r = engine->SQLExec(sql.c_str());
    if (!r) {
      LOG_ERROR("exec sql error");
      r = false;
      break;
    }

    if (storage_get == NULL) {
      r = false;
      break;
    } else {
      storage_get(reinterpret_cast<void*>(engine), value);
    }
  } while (0);
  db_pool_.DBConnectionPush(engine);
  return r;
}

}
