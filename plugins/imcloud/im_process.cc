#include "im_process.h"
#include <string.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <sstream>
#include <iostream>
/*
AppKey	开发者平台分配的appkey
Nonce	随机数（最大长度128个字符）
CurTime	当前UTC时间戳，从1970年1月1日0点0 分0 秒开始到现在的秒数(String)
CheckSum	SHA1(AppSecret + Nonce + CurTime),三个参数拼接的字符串，进行SHA1哈希计算，转化成16进制字符(String，小写)
*/
namespace im_process {


 ImProcess::ImProcess(){

 }
 ImProcess::~ImProcess(){

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
 void ImProcess::gettoken(){
        std::string url = "https://api.netease.im/nimserver/user/create.action";

        http::HttpMethodPost hmp(url);
        std::string content = "Content-Type: application/x-www-form-urlencoded;charset=utf-8";
        hmp.SetHeaders(content);
        std::string appkey = "AppKey: 709f85147892793d23a2ed6bfe3de1b3";
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
        
        std::string sumstring = "f40ab1832147"+nonce+curtime;
        std::string checksum = "CheckSum: "+ sumHash1(sumstring);
        hmp.SetHeaders(checksum);
        
        LOG_MSG2("%s",content.c_str());
        LOG_MSG2("%s",appkey.c_str());
        LOG_MSG2("%s",nonce.c_str());
        LOG_MSG2("%s",curtime.c_str());
        LOG_MSG2("checksum = %s",checksum.c_str());

        std::string post_data = "accid=zhangsan&name=zhangsan";
        hmp.Post(post_data.c_str());

        std::string result;
        hmp.GetContent(result);
        LOG_MSG2("http get result:%s", result.c_str());

        return;
 }
  
} 