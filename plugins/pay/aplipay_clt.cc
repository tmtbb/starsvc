//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#include "pay/aplipay_clt.h"
#include "pay/http_client.h"

#include <iostream>
#include <sstream>
#include "basic/md5sum.h"
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"

#ifndef XRSA_KEY_BITS
#define XRSA_KEY_BITS (1024)
#endif

/** 此处替换为开发者在支付宝开放平台申请的应用ID **/
//#std::string appId = "2017052107299862";
//std::string appId = "2017060807450365";
std::string appId = "2017073107974282";

/** 此处替换为开发者使用openssl生成的rsa私钥 **/
/*
std::string pKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXAIBAAKBgQDi4becNJoefmUGSOS2C+WUCCrcUZLK0J2y1L6HvMu+WKJ1PStb\n"
"TtD67AeskTN4g17nfKw/nsK1pKEVBv8E/jSsOyqB6zF+J6DbyCLJMK4P4qoxAROQ\n"
"B9oyvuJgEVBUNNN9u4p409MKm3l7nKvFLeDBe8yG+aAvxi1BYl79cWzz7QIDAQAB\n"
"AoGATd8nRCgR1fGP/y45wfonXD3JaEFfXtlnpx+6HaDBVZ3adN7/6KEOvXER2Tsl\n"
"LXH5uv5hqJx3PB07ZJo4IaCWtvkkhGRk+OJuxY539heTQu5LI1f8TT9J6gvsc46S\n"
"W9rI08Q/YhrT4DnCkjCC319l+6uoaxs/QHWJITVvehMiRUECQQD8HH3GF98/p3mm\n"
"Q/d3IjdOEUZiS0eJvbZ4vqGYxn55pljO88vtoPNGVGiOdl5acD5jKwD6i/o0siXA\n"
"1UDWxdT5AkEA5mGaKAsnDNoWElRmP+peiaXwtSNoHPllA3FA1noCvZqk5ostP+ZC\n"
"m0l+XT9lNO9T5IdQ64uevaSjsY3uMdq3lQJBALG1k6Ky6RcBgmqEtkcvwzQwUSCw\n"
"V7jsFVd/aIE8SaKOc0NN7o2OSm1kyl7BaTjurctRYNs7GB9VA++tYosB4GECQHWk\n"
"fY3ZNBWx//dYNeaJjcEIhcRZ0j6Jc/WwDYX4RBICOBaqF2876+NUQjzntIy1cceO\n"
"+dluMJ9yxUxTx8CZiYkCQEH8wNqGse6YEVL7gcBmsJQ0SpBAuVRewdNUs3ZTcF28\n"
"V3kJD+TzF7nSRjUmh6FVONjieeZY2d0i9KyveDggScA=\n"
"-----END RSA PRIVATE KEY-----";
*/

std::string pKey = "-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXAIBAAKBgQClaExuBAEcD7zGZ5lnvR+gdGLZsJOCAebJmqUkds/wUt6H4h92\n"
"+bemYeg4hR8c1u86SW8FLPFzgY+i9owfZfWYbeY83G7y3yJQ4BSDJ6/Qh5o32r3x\n"
"/vdsQdpFSGWwMAiqs5JZFODfBSe4DiePY687nOimEVWOZYZ947tYbx+acQIDAQAB\n"
"AoGAYwodYW5iX7QIm1/i2dvC+SdXHPN+j0SmNKa1JS1C+qoT9pbtsiZ50VUkX9tN\n"
"ss37dhIMOLQdasT0BWbMpew97e3DlpN2Wv4k+pvXK2M+aLw7QXYRq6v2qH8roPjE\n"
"dId7rY195tmT40dyaYxxAWkAdM5sdiZHp3efqXmtxFUUDRUCQQDQ6ZzScCI4q/9/\n"
"rJh9cSigjRMUrNoCVZyxWk04pdfM1R4V6m0yNSTmbX3goO6hhxvf4EEJftflSz64\n"
"lOXBOfOrAkEAyrBpevLd9JxfbGlApwKhcJeqTpiEJaogKRPSvixT0WgIbgJ+1/yO\n"
"xhc6/NyAZn+VSOoEImyoRU0btaRiE8bOUwJBAKTDSipxw5uckzI3hkCQyXDuMllB\n"
"BeOHb8A0Luj5axwiDMtOCvbpEQQm4HsTkYCVVTeY3tJFVrKBSKM804M0WGcCQQDD\n"
"e6cPrELDEzm5Dj4LlS7sr2nSyafRx6Wi2HqNbaUdNs/CgeeiGfP9OxRqIaQ4tt3c\n"
"5847AT69L+QEqNjqp/f/Aj8kaWMYfepVNfhbf+Ylh1MawQBd4LRd/jHAft05QBqO\n"
"1CBwN5YSE+S5vQ/UmH/gLvU5QKYNvW6OMo/SMKcVAwU=\n"
"-----END RSA PRIVATE KEY-----";


/** 支付宝公钥，用来验证支付宝返回请求的合法性 **/
std::string aliPubKey = "-----BEGIN PUBLIC KEY-----\n"
"MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQDDI6d306Q8fIfCOaTXyiUeJHkr\n"
"IvYISRcc73s3vF1ZT7XN8RNPwJxo8pWaJMmvyTn9N4HQ632qJBVHf8sxHi/fEsra\n"
"prwCtzvzQETrNRwVxLO5jVmRGi60j8Ue1efIlzPXV9je9mkjzOmdssymZkh2QhUr\n"
"CmZYI/FCEa3/cNMW0QIDAQAB\n"
"-----END PUBLIC KEY-----";

namespace pay_logic {
///---
const std::string default_charset      = "utf-8";
const std::string default_url          = "https://openapi.alipay.com/gateway.do";
const std::string default_sign_type    = "RSA";
const std::string default_version      = "1.0";

const std::string KEY_APP_ID           = "app_id";
const std::string KEY_METHOD           = "method";
const std::string KEY_CHARSET          = "charset";
const std::string KEY_PARTNERID        = "partnerid"; //biz content
const std::string KEY_SIGN_TYPE        = "sign_type";
const std::string KEY_SIGN             = "sign";
const std::string KEY_TIMESTAMP        = "timestamp";
const std::string KEY_VERSION          = "version";
const std::string KEY_BIZ_CONTENT      = "biz_content";
const std::string NOTIFY_URL           = "http://139.224.18.190:8080/cgi-bin/star/pay/v1/alipay_callback.fcgi";
//static std::string method = "alipay.trade.create";
static std::string method = "alipay.trade.app.pay";
ApliPayOrder::ApliPayOrder() {
}

ApliPayOrder::~ApliPayOrder() {
}
unsigned char ToHex(unsigned char x)   
{   
    return  x > 9 ? x + 55 : x + 48;   
}  
  
unsigned char FromHex(unsigned char x)   
{   
  unsigned char y;  
  if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
  else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
  else if (x >= '0' && x <= '9') y = x - '0';  
  else assert(0);  
  return y;  
}  
  
std::string UrlDecode(const std::string& str)  
{  
  std::string strTemp = "";  
  size_t length = str.length();  
  for (size_t i = 0; i < length; i++)  
  {  
    if (str[i] == '+') strTemp += ' ';  
    else if (str[i] == '%')  
    {  
        assert(i + 2 < length);  
        unsigned char high = FromHex((unsigned char)str[++i]);  
        unsigned char low = FromHex((unsigned char)str[++i]);  
        strTemp += high*16 + low;  
    }  
    else strTemp += str[i];  
  }  
  return strTemp;  
}

static std::string Upper(std::string &text) {
  for (unsigned int i = 0; i < text.length(); i ++)
  {
	if (text[i] >= 'a' && text[i] <= 'z')
		text[i] = text[i] - 32;
  }
  return text;
}
/*
*/
std::string buildContent(const StringMap &contentPairs) {

    std::string content;
    for (StringMap::const_iterator iter = contentPairs.begin();
         iter != contentPairs.end(); ++iter) {
        if (!content.empty()) {
            content.push_back('&');
        }
        content.append(iter->first);
        content.push_back('=');
        content.append(iter->second);
    }
    return content;
}

std::string ApliPayOrder::PlaceOrderSign(std::string &content) {
    StringMap requestPairs;      
    time_t t = time(0);
    char tmp[64];
    strftime(tmp, sizeof(tmp), "%Y-%m-%d %X", localtime(&t));

    //std::string content = "{\"out_trade_no\":\"20170616191212\",\"product_code\":\"QUICK_MSECURITY_PAY\",\"total_amount\":1000,\"subject\":\"a goods\"}";
    //std::string content = "{\"out_trade_no\":\"20170525191212\",\"product_code\":\"QUICK_MSECURITY_PAY\",\"total_amount\":\"0.01\",\"subject\":\"a goods\"}";
//____________________
    requestPairs.insert(StringMap::value_type(KEY_APP_ID, appId));
    requestPairs.insert(StringMap::value_type(KEY_BIZ_CONTENT, content));
    requestPairs.insert(StringMap::value_type(KEY_CHARSET, default_charset));
    //requestPairs.insert(StringMap::value_type("notify_url", "http://122.144.169.214/cgi-bin/star/router/v1/get_server.fcgi"));
    requestPairs.insert(StringMap::value_type("notify_url", NOTIFY_URL));
//    requestPairs.insert(StringMap::value_type(KEY_PARTNERID, "2088421752335044"));
    requestPairs.insert(StringMap::value_type(KEY_METHOD, method));
    requestPairs.insert(StringMap::value_type(KEY_SIGN_TYPE, default_sign_type));
    requestPairs.insert(StringMap::value_type(KEY_TIMESTAMP, tmp));
    requestPairs.insert(StringMap::value_type(KEY_VERSION,default_version));

    //LOG_ERROR2("key___ [%s]___________", pKey.c_str()); //tw test
//sign create
    std::string wholeContent = buildContent(requestPairs);
    LOG_ERROR2("sign before [%s]___________", wholeContent.c_str()); //tw test
    std::string sign = rsaSign(wholeContent, pKey);

    LOG_ERROR2("sign[%s]", sign.c_str()); //tw test
    requestPairs.insert(StringMap::value_type(KEY_SIGN, sign));	
    //wholeContent = buildContent(requestPairs);
    //LOG_ERROR2("wholeContent[%s]", wholeContent.c_str()); //tw test
    //
///*
//例子 
    HttpClient httpClient;
    return httpClient.getOrderInfo(default_url, requestPairs);

    //LOG_ERROR2("Response[%s]", responseStr.c_str()); //tw test
//    */
}
/*
*/
//_______________________________________________________
//
std::string base64Encode(const unsigned char *bytes, int len) {

    BIO *bmem = NULL;
    BIO *b64 = NULL;
    BUF_MEM *bptr = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());
    b64 = BIO_push(b64, bmem);
    BIO_write(b64, bytes, len);
    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    std::string str = std::string(bptr->data, bptr->length);
    BIO_free_all(b64);
    return str;
}


std::string ApliPayOrder::rsaSign(const std::string &content, const std::string &key) {

    std::string signed_str;
    const char *key_cstr = key.c_str();
    int key_len = strlen(key_cstr);
    //BIO *p_key_bio = BIO_new_mem_buf((void *)key_cstr, key_len);
    BIO *p_key_bio = BIO_new_mem_buf((void *)key_cstr, -1);
    RSA *p_rsa = PEM_read_bio_RSAPrivateKey(p_key_bio, NULL, NULL, NULL);

    LOG_DEBUG2("content  _____[%s]__________________________", content.c_str());
    if (p_rsa != NULL) {

        //LOG_DEBUG("_____[start]__________________________" );
        const char *cstr = content.c_str();
        unsigned char hash[SHA_DIGEST_LENGTH] = {0};
        SHA1((unsigned char *)cstr, strlen(cstr), hash);
        unsigned char sign[XRSA_KEY_BITS / 8] = {0};
        unsigned int sign_len = sizeof(sign);
        int r = RSA_sign(NID_sha1, hash, SHA_DIGEST_LENGTH, sign, &sign_len, p_rsa);

        if (0 != r && sizeof(sign) == sign_len) {
            signed_str = base64Encode(sign, sign_len);
            LOG_DEBUG("sign base64Encode_____[%s]__________________________" );
        }
    }

    RSA_free(p_rsa);
    BIO_free(p_key_bio);
    return signed_str;
}
//返回校验


//
bool base64Decode(const string &str, unsigned char *bytes, int &len) {

    const char *cstr = str.c_str();
    BIO *bmem = NULL;
    BIO *b64 = NULL;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new_mem_buf((void *)cstr, strlen(cstr));
    b64 = BIO_push(b64, bmem);
    len = BIO_read(b64, bytes, len);

    BIO_free_all(b64);
    return len > 0;
}

std::string ApliPayOrder::analyzeResponse(const string &responseStr) {

 // JsonType responseObj = JsonUtil::stringToObject(responseStr);
  //JsonMap responseMap = responseObj.toMap();
  int beg = responseStr.find("_response\"");
    int end = responseStr.rfind("\"sign\"");
  if (beg < 0 || end < 0) {
    return string();
  }
  beg = responseStr.find('{', beg);
  end = responseStr.rfind('}', end);
  string responseContent = responseStr.substr(beg, end - beg + 1);

  if (!aliPubKey.empty()) 
  {
    string responseSign = "";
    bool verifyResult = rsaVerify(responseContent, responseSign, aliPubKey);
    if (!verifyResult)
    {
      LOG_DEBUG("Verify Failed");
      return string();
    }

    LOG_DEBUG("Verify Sucess");
  }
  else
  {
    LOG_DEBUG("AlipayPublicKey is empty, Skip the Verify");
  }
  
}

bool ApliPayOrder::rsaVerify(const std::string &content, const std::string &sign, const std::string &key) {

    bool result = false;
    const char *key_cstr = key.c_str();
    int key_len = strlen(key_cstr);
    //BIO *p_key_bio = BIO_new_mem_buf((void *)key_cstr, key_len);
    BIO *p_key_bio = BIO_new_mem_buf((void *)key_cstr, -1);
    RSA *p_rsa = PEM_read_bio_RSA_PUBKEY(p_key_bio, NULL, NULL, NULL);

    if (p_rsa != NULL) {
        const char *cstr = content.c_str();
        unsigned char hash[SHA_DIGEST_LENGTH] = {0};
        SHA1((unsigned char *)cstr, strlen(cstr), hash);
        unsigned char sign_cstr[XRSA_KEY_BITS / 8] = {0};
        int len = XRSA_KEY_BITS / 8;
        base64Decode(sign, sign_cstr, len);
        unsigned int sign_len = XRSA_KEY_BITS / 8;
        int r = RSA_verify(NID_sha1, hash, SHA_DIGEST_LENGTH, (unsigned char *)sign_cstr, sign_len, p_rsa);

        if (r > 0) {
            result = true;
        }
    }

    RSA_free(p_rsa);
    BIO_free(p_key_bio);
    return result;
}
std::string getvalue(std::string &content, const std::string &key)
{
    int beg = content.find(key.c_str())+ strlen(key.c_str()); 
    int end = content.find("&", beg); 
    return content.substr(beg, end - beg);
}

bool ApliPayOrder::DealResponse(const string &responseStr, string &trade_no,string &out_trade_no, double &price, int &status) {

  string tmp = responseStr;
  //__delete sign signtype
  int beg = tmp.find("sign"); 
  int end = tmp.find("&", beg); 
  try 
  {
  string sign = tmp.substr(beg, end - beg + 1);

  string responseContent = tmp.erase(beg, end - beg + 1);

  beg = tmp.find("sign_type"); 
  end = tmp.find("&", beg); 
  string sign_type = tmp.substr(beg, end - beg + 1);
  LOG_DEBUG2("sign[%s],signtype[%s]", sign.c_str(), sign_type.c_str());

  responseContent = tmp.erase(beg, end - beg + 1);

  LOG_DEBUG2("responseStr[%s]", responseContent.c_str());
//UrlDecode
  LOG_DEBUG2("urldecode responseStr[%s]",UrlDecode(responseContent).c_str());

  responseContent = UrlDecode(responseContent);
  price = atof(getvalue(tmp, "&total_amount=").c_str());
  trade_no = getvalue(tmp, "&trade_no=");//支付宝交易号
  out_trade_no = getvalue(tmp, "&out_trade_no="); //商户订单号
  std::string trade_status = getvalue(tmp, "&trade_status="); //TRADE_SUCCESS  TRADE_CLOSED//未付款或超时关闭
  status = 0;
  if (!strcmp(trade_status.c_str(), "TRADE_SUCCESS"))
      status = 1;

  LOG_DEBUG2("trade_no[%s], trade_status[%s], out_trade_no[%s]",trade_no.c_str(), trade_status.c_str(), out_trade_no.c_str());
/*
  if (!aliPubKey.empty()) 
  {
    string responseSign = "";
    bool verifyResult = rsaVerify(responseContent, responseSign, aliPubKey);
    if (!verifyResult)
    {
      LOG_DEBUG("Verify Failed");
      return string();
    }

    LOG_DEBUG("Verify Sucess");
  }
  else
  {
    LOG_DEBUG("AlipayPublicKey is empty, Skip the Verify");
  }
*/
  }
  catch (...)
  {
    return false;
  }
  return true;
}

}//namespace http_hancel_logic 

