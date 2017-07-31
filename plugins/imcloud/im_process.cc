#include "im_process.h"
#include <string.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <sstream>
#include <iostream>
#include "json/json.h"
/*
AppKey	开发者平台分配的appkey
Nonce	随机数（最大长度128个字符）
CurTime	当前UTC时间戳，从1970年1月1日0点0 分0 秒开始到现在的秒数(String)
CheckSum	SHA1(AppSecret + Nonce + CurTime),三个参数拼接的字符串，进行SHA1哈希计算，转化成16进制字符(String，小写)
*/
namespace im_process {

ImProcess *ImProcess::instance_ = NULL;


 ImProcess::ImProcess(){
 	if(!init())
		assert(0);
 }
 ImProcess::~ImProcess(){
	if(sqlengine != NULL){
		delete sqlengine;
		sqlengine = NULL;
	}
 }
bool ImProcess::init(){
	bool r = false;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	std::string path = "./plugins/imcloud/imcoud_config.xml";
	if (config == NULL) {
		LOG_ERROR("config init error");
		return false;
	}
	r = config->LoadConfig(path);
	if (!r) {
		LOG_ERROR("config load error");
		return false;
	}
	sqlengine = new im_mysql::Im_Mysql(config);
	return true;
}
ImProcess *ImProcess::GetInstance() {
  if (instance_ == NULL)
    instance_ = new ImProcess();
  return instance_;
}

void ImProcess::FreeInstance() {
  delete instance_;
  instance_ = NULL;
}

std::string sumHash1(std::string strin)  
{  
    SHA_CTX c;  
    unsigned char md[SHA_DIGEST_LENGTH];  
    SHA1((unsigned char *)strin.c_str(), strin.length(), md); 

    std::string strout = "";
    char cc[2];
    for(int i=0;i<SHA_DIGEST_LENGTH;i++){
        sprintf(cc,"%02x",md[i]);
        strout+=cc;
    } 
    return strout;
}
 std::string ImProcess::gettoken(std::string name,std::string accid){
        std::string url = "https://api.netease.im/nimserver/user/create.action";

        http::HttpMethodPost hmp(url);
        std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
        hmp.SetHeaders(content);
        std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
        hmp.SetHeaders(appkey);

        std::string nonce = util::RandomString(32);
        std::string noncevalue = "Nonce: "+nonce;
        hmp.SetHeaders(noncevalue);

        
        std::stringstream ss;
        std::string curtime;
        ss<<time(NULL);
        ss>>curtime;
        std::string curtimevalue = "CurTime: "+curtime;
        hmp.SetHeaders(curtimevalue);
        
        std::string sumstring = "59a801fe9811"+nonce+curtime;
        std::string checksum = "CheckSum: "+ sumHash1(sumstring);
        hmp.SetHeaders(checksum);
        
        LOG_MSG2("%s",content.c_str());
        LOG_MSG2("%s",appkey.c_str());
        LOG_MSG2("%s",nonce.c_str());
        LOG_MSG2("%s",curtime.c_str());
        LOG_MSG2("checksum = %s",checksum.c_str());

        //std::string post_data = "accid=13569365932&name=qwert";
        std::string post_data = "accid="+accid+"&name="+name;
        hmp.Post(post_data.c_str());

        std::string result;
        hmp.GetContent(result);
        LOG_MSG2("http get result:%s", result.c_str());
        
        Json::Reader reader;
        Json::Value value;
        int desc;
        std::string token;
        if (reader.parse(result, value))
        {
            desc = value["code"].asInt();
            //token =  value["info"]["token"].asString();
        }
        if(desc == 200)
            token =  value["info"]["token"].asString();
        
        return token;
 }
 std::string ImProcess::refreshtoken(std::string accid){
        std::string url = "https://api.netease.im/nimserver/user/refreshToken.action";

        http::HttpMethodPost hmp(url);
        std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
        hmp.SetHeaders(content);
        std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
        hmp.SetHeaders(appkey);

        std::string nonce = util::RandomString(32);
        std::string noncevalue = "Nonce: "+nonce;
        hmp.SetHeaders(noncevalue);

        
        std::stringstream ss;
        std::string curtime;
        ss<<time(NULL);
        ss>>curtime;
        std::string curtimevalue = "CurTime: "+curtime;
        hmp.SetHeaders(curtimevalue);
        
        std::string sumstring = "59a801fe9811"+nonce+curtime;
        std::string checksum = "CheckSum: "+ sumHash1(sumstring);
        hmp.SetHeaders(checksum);
        
        LOG_MSG2("%s",content.c_str());
        LOG_MSG2("%s",appkey.c_str());
        LOG_MSG2("%s",nonce.c_str());
        LOG_MSG2("%s",curtime.c_str());
        LOG_MSG2("checksum = %s",checksum.c_str());

        std::string post_data = "accid="+accid;
        hmp.Post(post_data.c_str());

        std::string result;
        hmp.GetContent(result);
        LOG_MSG2("http refreshtoken result:%s", result.c_str());
        
        Json::Reader reader;
        Json::Value value;
        int desc;
        std::string token;
        if (reader.parse(result, value))
        {
            desc = value["code"].asInt();  
        }
        if(desc==200)
            token =  value["info"]["token"].asString();
        
        return token;
 }
 bool ImProcess::addfriend(const std::string& accid,const std::string& faccid,const std::string& msg,
  							const int64& type){
  	std::string url = "https://api.netease.im/nimserver/friend/add.action";
    std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
	http::HttpMethodPost hmp(url);
    hmp.SetHeaders(content);
    std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
    hmp.SetHeaders(appkey);
    std::string nonce = util::RandomString(32);
    std::string noncevalue = "Nonce: "+nonce;
    hmp.SetHeaders(noncevalue);

    std::stringstream ss;
    std::string curtime;
    ss<<time(NULL);
    ss>>curtime;
    std::string curtimevalue = "CurTime: "+curtime;
    hmp.SetHeaders(curtimevalue);
    std::string sumstring = "59a801fe9811"+nonce+curtime;
    std::string checksum = "CheckSum: "+ sumHash1(sumstring);
    hmp.SetHeaders(checksum);
    

    std::string post_data = "accid="+accid + "&faccid=" + faccid + "&type=" 
							+ base::BasicUtil::StringUtil::Int64ToString(type) + "&msg="+msg;
    hmp.Post(post_data.c_str());

    std::string result;
    hmp.GetContent(result);
    LOG_MSG2("http addfriend result:%s", result.c_str());
    
    Json::Reader reader;
    Json::Value value;
    int desc;
    std::string token;
    if (reader.parse(result, value))
    {
        desc = value["code"].asInt();  
    }
	else
		return false;
    if(desc==200)
        return true;
    
	return false;
 }

  bool ImProcess::delfriend(const std::string& accid,const std::string& faccid){
	std::string url = "https://api.netease.im/nimserver/friend/add.action";
    std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
	http::HttpMethodPost hmp(url);
    hmp.SetHeaders(content);
    std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
    hmp.SetHeaders(appkey);
    std::string nonce = util::RandomString(32);
    std::string noncevalue = "Nonce: "+nonce;
    hmp.SetHeaders(noncevalue);

    std::stringstream ss;
    std::string curtime;
    ss<<time(NULL);
    ss>>curtime;
    std::string curtimevalue = "CurTime: "+curtime;
    hmp.SetHeaders(curtimevalue);
    std::string sumstring = "59a801fe9811"+nonce+curtime;
    std::string checksum = "CheckSum: "+ sumHash1(sumstring);
    hmp.SetHeaders(checksum);
    

    std::string post_data = "accid="+accid + "&faccid=" + faccid;
    hmp.Post(post_data.c_str());

    std::string result;
    hmp.GetContent(result);
    LOG_MSG2("http delfriend result:%s", result.c_str());
    
    Json::Reader reader;
    Json::Value value;
    int desc;
    std::string token;
    if (reader.parse(result, value))
    {
        desc = value["code"].asInt();  
    }
	else
		return false;
    if(desc==200)
        return true;
    
	return false;
  }
bool ImProcess::getfriendlist(const std::string& accid,const std::string& createtime,base_logic::DictionaryValue& ret){
  	std::string url = "https://api.netease.im/nimserver/friend/get.action";
    std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
	http::HttpMethodPost hmp(url);
    hmp.SetHeaders(content);
    std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
    hmp.SetHeaders(appkey);
    std::string nonce = util::RandomString(32);
    std::string noncevalue = "Nonce: "+nonce;
    hmp.SetHeaders(noncevalue);

    std::stringstream ss;
    std::string curtime;
    ss<<time(NULL);
    ss>>curtime;
    std::string curtimevalue = "CurTime: "+curtime;
    hmp.SetHeaders(curtimevalue);
    std::string sumstring = "59a801fe9811"+nonce+curtime;
    std::string checksum = "CheckSum: "+ sumHash1(sumstring);
    hmp.SetHeaders(checksum);
    

    std::string post_data = "accid="+accid + "&createtime=" + createtime;
    hmp.Post(post_data.c_str());

    std::string result;
    hmp.GetContent(result);
    LOG_MSG2("http getfriendlist result:%s", result.c_str());

    Json::Reader reader;
    Json::Value value;
    int desc;
    std::string token;
    if (reader.parse(result, value))
    {
        desc = value["code"].asInt();  
    }
	else
		return false;
    if(desc==200){
		base_logic::ListValue *listvalue = new base_logic::ListValue();
		//ret.Set("list",(base_logic::Value*)listvalue);
		
		base_logic::FundamentalValue* size = new base_logic::FundamentalValue(value["size"].asInt());
		ret.Set(L"size",size);
		
		
		Json::Value arrayObj = value["friends"];
		for (int j = 0; j < value["size"].asInt(); j++)
		{
			base_logic::DictionaryValue* friends = new base_logic::DictionaryValue();
			if (arrayObj[j].isMember("faccid")){
				std::string m_faccid = arrayObj[j]["faccid"].asString(); 
				std::string  head;
				std::string  name;
				std::string  type;
				friends->SetString(L"faccid",m_faccid);
				if(sqlengine->getuserinfofromaccid(m_faccid,head,name,type)){
					friends->SetString(L"head",head);
					friends->SetString(L"name",name);
					friends->SetString(L"type",type);
				}
			}	
			listvalue->Append((base_logic::Value*)friends);
		}
		ret.Set("list",(base_logic::Value*)listvalue);
		return true;
	}
	return false;
 }
bool ImProcess::editfriendinfo(const std::string& accid,const std::string& faccid,
  							const std::string& alias,const std::string& ex){

	std::string url = "https://api.netease.im/nimserver/friend/update.action";
    std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
	http::HttpMethodPost hmp(url);
    hmp.SetHeaders(content);
    std::string appkey = "AppKey: 9c3a406f233dea0d355c6458fb0171b8";
    hmp.SetHeaders(appkey);
    std::string nonce = util::RandomString(32);
    std::string noncevalue = "Nonce: "+nonce;
    hmp.SetHeaders(noncevalue);

    std::stringstream ss;
    std::string curtime;
    ss<<time(NULL);
    ss>>curtime;
    std::string curtimevalue = "CurTime: "+curtime;
    hmp.SetHeaders(curtimevalue);
    std::string sumstring = "59a801fe9811"+nonce+curtime;
    std::string checksum = "CheckSum: "+ sumHash1(sumstring);
    hmp.SetHeaders(checksum);
    

    std::string post_data = "accid="+accid + "&faccid=" + faccid
							+ "&alias="+ alias + "&ex=" + ex;
    hmp.Post(post_data.c_str());

    std::string result;
    hmp.GetContent(result);
    LOG_MSG2("http editfriendinfo result:%s", result.c_str());
    
    Json::Reader reader;
    Json::Value value;
    int desc;
    std::string token;
    if (reader.parse(result, value))
    {
        desc = value["code"].asInt();  
    }
	else
		return false;
    if(desc==200)
        return true;
    
	return false;
}

} 