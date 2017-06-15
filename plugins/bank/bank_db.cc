//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年2月12日 Author: kerry
#include <mysql.h>
#include "logic/logic_unit.h"
#include "bank/bank_db.h"
#include "basic/basic_util.h"

namespace bank_logic {

BankDB::BankDB(config::FileConfig* config) {
  mysql_engine_ = base_logic::DataEngine::Create(MYSQL_TYPE);
  mysql_engine_->InitParam(config->mysql_db_list_);
}

BankDB::~BankDB() {
  if (mysql_engine_) {
    delete mysql_engine_;
    mysql_engine_ = NULL;
  }
}

bool BankDB::OnWithDrawCash(const int64 uid, const int64 wid, const int64  card_id, const double money,
                            const double charge, int32& result, double& balance) {
                              
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();
  base_logic::DictionaryValue* resultvalue;
  std::string sql;
  //call actuals.proc_Withdraw(9,123213213,'6214855715672891',100,0.1);
  sql = "call proc_Withdraw("
      + base::BasicUtil::StringUtil::Int64ToString(uid) 
      + ","
      + base::BasicUtil::StringUtil::Int64ToString(wid)
      + ","
      + base::BasicUtil::StringUtil::Int64ToString(card_id)
      + ","
      + base::BasicUtil::StringUtil::DoubleToString(money)
      + ","
      + base::BasicUtil::StringUtil::DoubleToString(charge)
      + ");";

  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallWithDrawCash);
  if (!r)
    return false; 
  r = dict->GetDictionary(L"resultvalue", &resultvalue);
  if(r) {
    r = resultvalue->GetInteger(L"result", &result);
    r = resultvalue->GetReal(L"balance", &balance);
  }
  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}
bool BankDB::OnGetBankCard(const int64 uid,
                            std::list<bank_logic::BankCard>& list) {

  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetBindBankName("
      + base::BasicUtil::StringUtil::Int64ToString(uid) + ");";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetBankCard);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    bank_logic::BankCard bank_card;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    bank_card.ValueSerialization(dict_result_value);
    list.push_back(bank_card);
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool BankDB::OnGetBankCard(const int64 uid,
                           std::map<int32, bank_logic::BankCard>& bank_map) {
  bool r = false;
  base_logic::DictionaryValue* dict = new base_logic::DictionaryValue();

  std::string sql;
  sql = "call proc_GetBindBankName("
      + base::BasicUtil::StringUtil::Int64ToString(uid) + ");";
  base_logic::ListValue *listvalue;
  dict->SetString(L"sql", sql);
  r = mysql_engine_->ReadData(0, (base_logic::Value *) (dict), CallGetBankCard);
  if (!r)
    return false;
  dict->GetList(L"resultvalue", &listvalue);
  while (listvalue->GetSize()) {
    bank_logic::BankCard bank_card;
    base_logic::Value *result_value;
    listvalue->Remove(0, &result_value);
    base_logic::DictionaryValue *dict_result_value =
        (base_logic::DictionaryValue *) (result_value);
    bank_card.ValueSerialization(dict_result_value);
    //list->push_back(bank_card);
    bank_map[bank_card.bank_id()] = bank_card;
    delete dict_result_value;
    dict_result_value = NULL;
  }

  if (dict) {
    delete dict;
    dict = NULL;
  }
  return true;
}

bool BankDB::OnBindBankCard(const int64 uid, const std::string& card_no, const std::string& branch_bank, 
                            const std::string& name, bank_logic::BankCard& bank_card) {
   bool r = false;
   std::string sql;
   base_logic::DictionaryValue* dict = new base_logic::DictionaryValue; 
   base_logic::DictionaryValue* resultvalue = NULL;
   //call actuals.proc_BindBankCard(32,'6214855715672891','凤起支行','kerrt')
   sql = "call proc_BindBankCard(" + base::BasicUtil::StringUtil::Int64ToString(uid)
       + ",'"
       + card_no
       + "','"
       + branch_bank
       + "','"
       + name
       + "');";
   dict->SetString(L"sql", sql);
   r = mysql_engine_->ReadData(0, (base_logic::Value*)(dict), CallBindBankCard);
   if(!r){
     if (dict) {delete dict; dict = NULL;}
     return false;
   }
   
   r = dict->GetDictionary(L"resultvalue", &resultvalue);
   if (r)
     bank_card.ValueSerialization(resultvalue);
   if (dict) {delete dict; dict = NULL;}
   return true;
}

bool BankDB::OnUnbindBankCard(const int64 uid , const int32 bankcard_id) {
   bool r = false;
   std::string sql;
   //call actuals.proc_UnbindBankCard(32,57)
   base_logic::DictionaryValue* dict = new base_logic::DictionaryValue;
   sql = "call proc_UnbindBankCard(" + base::BasicUtil::StringUtil::Int64ToString(uid)
         + "," + base::BasicUtil::StringUtil::Int64ToString(bankcard_id) + ");";
   dict->SetString(L"sql", sql);
   r = mysql_engine_->WriteData(0, (base_logic::Value*)(dict));
   if (dict) {delete dict; dict = NULL;}
   if (!r)
     return false;

   return true; 
}

bool BankDB::OnGetBankCardInfo(const std::string& card_no, int32& bank_id, std::string& card_name,
                              std::string& bank_name) {
   bool r = false;
   //call actuals.proc_GetBankCardInfo('6214855711652749')
   std::string sql;
   base_logic::DictionaryValue* dict = new base_logic::DictionaryValue;
   base_logic::DictionaryValue* resultvalue = NULL;
   sql = "call proc_GetBankCardInfo('" + card_no + "');";
   dict->SetString(L"sql", sql);
   r = mysql_engine_->ReadData(0, (base_logic::Value*)(dict),CallBankCardInfo);
   if (!r){
     delete dict;
     dict = NULL;
     return false;
   }

  r = dict->GetDictionary(L"resultvalue", &resultvalue);
  if(r) {
    r = resultvalue->GetInteger(L"bank_id", &bank_id);
    r = resultvalue->GetString(L"card_name", &card_name);
    r = resultvalue->GetString(L"bank_name", &bank_name);
  }

  if (dict) {delete dict; dict = NULL;}
  return true;
}   

void BankDB::CallBindBankCard(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
   base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue;
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {              
      if (rows[0] != NULL)
        info_value->SetBigInteger(L"id",atoll(rows[0]));

      if (rows[1] != NULL)
          info_value->SetBigInteger(L"uid", atoll(rows[1]));

      if (rows[2] != NULL)
          info_value->SetString(L"card_no", rows[2]);

      if (rows[3] != NULL)
        info_value->SetString(L"bank_username", rows[3]);

      if (rows[4] != NULL)
        info_value->SetInteger(L"bank_id", atoi(rows[4]));

      if (rows[5] != NULL)
        info_value->SetString(L"branch_bank", rows[5]);

      if (rows[6] != NULL)
        info_value->SetString(L"card_name", rows[6]);

      if (rows[7] != NULL)
        info_value->SetString(L"bank_name", rows[7]);

      if (rows[8] != NULL)
        info_value->SetInteger(L"is_default", atoi(rows[8]));
   }
 }

  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}


void BankDB::CallWithDrawCash(void* param, base_logic::Value* value) { 
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
   base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue;
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

      if (rows[0] != NULL)
        info_value->SetInteger(L"result",atoi(rows[0]));

      if (rows[1] != NULL)
          info_value->SetReal(L"balance", atof(rows[1]));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void BankDB::CallBankCardInfo(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
   base_logic::DictionaryValue* info_value = new base_logic::DictionaryValue;
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {

      if (rows[0] != NULL)
        info_value->SetInteger(L"bank_id",atoi(rows[0]));

      if (rows[1] != NULL)
          info_value->SetString(L"card_name", rows[1]);

      if (rows[2] != NULL)
          info_value->SetString(L"bank_name", rows[2]);}
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (info_value));
}

void BankDB::CallGetBankCard(void* param, base_logic::Value* value) {
  base_logic::DictionaryValue *dict = (base_logic::DictionaryValue *) (value);
  base_logic::ListValue *list = new base_logic::ListValue();
  base_storage::DBStorageEngine *engine =
      (base_storage::DBStorageEngine *) (param);
  MYSQL_ROW rows;
  int32 num = engine->RecordCount();
  if (num > 0) {
    while (rows = (*(MYSQL_ROW *) (engine->FetchRows())->proc)) {
      base_logic::DictionaryValue *info_value =
          new base_logic::DictionaryValue();

      if (rows[0] != NULL)
        info_value->SetBigInteger(L"id",atoll(rows[0]));

      if (rows[1] != NULL)
          info_value->SetBigInteger(L"uid", atoll(rows[1]));

      if (rows[2] != NULL)
          info_value->SetString(L"card_no", rows[2]);

      if (rows[3] != NULL)
        info_value->SetString(L"bank_username", rows[3]);

      if (rows[4] != NULL)
        info_value->SetInteger(L"bank_id", atoi(rows[4]));

      if (rows[5] != NULL)
        info_value->SetString(L"branch_bank", rows[5]);

      if (rows[6] != NULL)
        info_value->SetString(L"card_name", rows[6]);

      if (rows[7] != NULL)
        info_value->SetString(L"bank_name", rows[7]);

      if (rows[8] != NULL)
        info_value->SetInteger(L"is_default", atoi(rows[8]));

      list->Append((base_logic::Value *) (info_value));
    }
  }
  dict->Set(L"resultvalue", (base_logic::Value *) (list));
}

}  // namespace history_logic

