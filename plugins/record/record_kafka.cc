//  Copyright (c) 2015-2016 The KID Authors. All rights reserved.
//  Created on: 2016.2.18 Author: yangge

#include "record/record_kafka.h"
#include "basic/basic_util.h"
#include "basic/radom_in.h"
#include "basic/template.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"
#include "logic/time.h"
#include <mysql.h>
#include <set>
#include <sstream>

namespace record_logic {

RecordKafka::RecordKafka(config::FileConfig *config) {
    /* if (CONSUMER_INIT_SUCCESS
     != kafka_consumer_.Init(
     0,
     "kafka_newsparser_algo",
     "192.168.1.85:9091,192.168.1.80:9091,192.168.1.81:9091",
     NULL))*/

    /*
    base::ConnAddr addr = config->kafka_list_.front();
    if (CONSUMER_INIT_SUCCESS
            != kafka_consumer_.Init(addr))
        LOG_ERROR2("producer init failed: host:%s source %s additional %s",
                   addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
    else
        LOG_MSG2("producer init success: host:%s source %s additional %s",
                 addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
    */

    for(std::list<base::ConnAddr>::iterator it = config->kafka_list_.begin();
            it != config->kafka_list_.end(); it++) {
        base::ConnAddr addr = (*it);
        if (addr.additional() == "consumer")
            InitConsumer(addr);
        else if(addr.additional() == "producer")
            InitProducer(addr);
    }
}

RecordKafka::RecordKafka(const int32 svc_id, base::ConnAddr& addr) {
    std::string source = addr.source() + "_" + base::BasicUtil::StringUtil::Int64ToString(svc_id);
    base::ConnAddr new_addr(svc_id, addr.host(), addr.port(),
                            addr.usr(), addr.pwd(),source);

    if (CONSUMER_INIT_SUCCESS != kafka_consumer_.Init(new_addr))
        LOG_ERROR("kafka consumer kafka_task_algo init failed");
    else
        LOG_MSG("kafka consumer kafka_task_algo init success");
}


void RecordKafka::InitConsumer(base::ConnAddr& addr) {
    if (CONSUMER_INIT_SUCCESS
            != kafka_consumer_.Init(addr))
        LOG_ERROR2("consumer init failed: host:%s source %s additional %s",
                   addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
    else
        LOG_MSG2("consumer init success: host:%s source %s additional %s",
                 addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
}


void RecordKafka::InitProducer(base::ConnAddr& addr) {
    if (CONSUMER_INIT_SUCCESS
            != kafka_producer_.Init(addr))
        LOG_ERROR2("producer init failed: host:%s source %s additional %s",
                   addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
    else
        LOG_MSG2("producer init success: host:%s source %s additional %s",
                 addr.host().c_str(), addr.source().c_str(),addr.additional().c_str());
}

RecordKafka::~RecordKafka() {
    kafka_consumer_.Close();
    kafka_producer_.Close();
    LOG_MSG("~RecordKafka()");
}

bool RecordKafka::FectchBatchTempTask(std::list<base_logic::DictionaryValue*> *list) {

    std::set < std::string > data_list;
    std::string data;
    for (int i = 0; i < 1000; i++) {
        int pull_re = kafka_consumer_.PullData(data);
        if (CONSUMER_CONFIG_ERROR == pull_re) {
            LOG_MSG2("CONSUMER_CONFIG_ERROR,pull_re=%d", pull_re);
        }
        if (PULL_DATA_DATA_NULL == pull_re) {
            break;
        }
        if (PULL_DATA_TIMEOUT == pull_re) {
            LOG_MSG2("consumer get url timeout,pull_re=%d", pull_re);
            break;
        }
        data_list.insert(data);
    }

    for (std::set<std::string>::iterator it = data_list.begin();
            it != data_list.end(); it++) {
        std::string data = *it;
        LOG_DEBUG2("%s",data.c_str());
        base_logic::ValueSerializer *engine = base_logic::ValueSerializer::Create(
                0, &data);
        int error_code = 0;
        std::string error_str;
        base_logic::Value *value = engine->Deserialize(&error_code, &error_str);
        if (0 != error_code || NULL == value)
            continue;
        base_logic::DictionaryValue *task_info_dic =
            (base_logic::DictionaryValue *) value;
        list->push_back(task_info_dic);
        base_logic::ValueSerializer::DeleteSerializer(0, engine);
    }  // LOG_DEBUG2("update task info, total task num:%d", list->size());
    return true;

}

bool RecordKafka::SetBuyPosition(const std::string& index, const double price, 
        const int64 current_time) {
    int32 re = 0;
    base_logic::DictionaryValue* buy_info = new base_logic::DictionaryValue();
    buy_info->SetString(L"index", index);
    buy_info->SetReal(L"price", price);
    buy_info->SetBigInteger(L"current",current_time);
    buy_info->SetInteger(L"type",2);
    re = kafka_producer_.PushData(buy_info);
    delete buy_info;
    if (PUSH_DATA_SUCCESS == re)
        return true;
    else {
        LOG_ERROR("kafka producer send data failed");
        return false;
    }
}


bool RecordKafka::SetSellPosition(const std::string& index, const double price, 
        const int64 current_time) {
    int32 re = 0;
    base_logic::DictionaryValue* sell_info = new base_logic::DictionaryValue();
    sell_info->SetString(L"index", index);
    sell_info->SetReal(L"price", price);
    sell_info->SetBigInteger(L"current",current_time);
    sell_info->SetInteger(L"type",3);
    re = kafka_producer_.PushData(sell_info);
    delete sell_info;
    if (PUSH_DATA_SUCCESS == re)
        return true;
    else {
        LOG_ERROR("kafka producer send data failed");
        return false;
    }
}


bool RecordKafka::SetVolume(const std::string& index, const double price, 
        const int64 current_time) {
    int32 re = 0;
    base_logic::DictionaryValue* volume_info = new base_logic::DictionaryValue();
    volume_info->SetString(L"index", index);
    volume_info->SetReal(L"price", price);
    volume_info->SetBigInteger(L"current",current_time);
    volume_info->SetInteger(L"type",4);
    re = kafka_producer_.PushData(volume_info);
    delete volume_info;
    if (PUSH_DATA_SUCCESS == re)
        return true;
    else {
        LOG_ERROR("kafka producer send data failed");
        return false;
    }
}
}  // namespace record_logic
