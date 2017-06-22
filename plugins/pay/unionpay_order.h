// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// UnionpayOrder.h
// Created on: 2016年11月1日.
// Author: Paco.
#ifndef PUB_PAY_UNIONPAY_ORDER_H_
#define PUB_PAY_UNIONPAY_ORDER_H_

#include <string>
#include <openssl/pkcs12.h>
#include <sstream>

#include "net/typedef.h"
const std::string ACP_SERVER_URL = "http://61.147.114.78:8080/ACPSample_AppServer/form05_6_2_AppConsume";
#define UNIONPAY_CONFIG_FILE "plugins/user/acp_sdk.properties"
//#define PFX_PASSWD "000000" //test
#define PFX_PASSWD "172017"

#define SDK_SINGLE_MODE "acpsdk.singleMode" 
#define SDK_SIGNCERT_PATH "acpsdk.signCert.path"
#define SDK_SIGNCERT_PWD "acpsdk.signCert.pwd"
#define SDK_SIGNCERT_TYPE "acpsdk.signCert.type"  
#define SDK_ENCRYPTCERT_PATH "acpsdk.encryptCert.path"
#define SDK_VALIDATECERT_DIR "acpsdk.validateCert.dir"

#define K_BIZ_TYPE "bizType"
#define UNIONPAY_BIZ_TYPE "000201"
#define K_TXN_SUB_TYPE "txnSubType"
#define UNIONPAY_TXN_SUB_TYPE "01"
#define K_MER_ID "merId"
#define UNIONPAY_MER_ID "104210242141007"
#define K_BACK_URL "backUrl"
#define UNIONPAY_BACK_URL "https://61.147.114.78:8071/cgi-bin/paycall/unionpaycall/unionpaycall.fcgi"
#define K_TXN_TYPE "txnType"
#define UNIONPAY_TXN_TYPE "01"
#define K_CHANNEL_TYPE "channelType"
#define UNIONPAY_CHANNEL_TYPE "08"
#define K_ENCODING "encoding"
#define UNIONPAY_ENCODING "UTF-8"
#define K_VERSION "version"
#define UNIONPAY_VERSION "5.0.0"
#define K_ACCESS_TYPE "accessType"
#define UNIONPAY_ACCESS_TYPE "0"
#define K_ACC_TYPE "accType"
#define UNIONPAY_ACC_TYPE "01"
#define K_CURRENCY_CODE "currencyCode"
#define UNIONPAY_CURRENCY_CODE "156"
#define K_SIGN_METHOD "signMethod"
#define UNIONPAY_SIGN_METHOD "01"
#define K_TXN_TIME "txnTime"
#define K_ORDER_ID "orderId"
#define K_PRICE "txnAmt"
#define K_CERTID "certId"
#define K_SIGNATURE "signature"
#define K_REQ_RESERVED "reqReserved"
#define K_ACC_NO "accNo"
#define K_ORDER_DESC "orderDesc"
#define WP_UNIONPAY "航空定盘-充值"
/*
 *命名规则为银联文档命名方式
 */

namespace pay_logic {

//键值配置文件
class KVConfig {
 public:
  bool Load(const char* filename);
  const std::string GetString(const char* key) { return key_value_map_[key]; }

 private:
  void TrimString(std::string& str);

  std::map<std::string, std::string> key_value_map_;
};


class UnionpayOrder {
 public:
  UnionpayOrder();
  virtual ~UnionpayOrder();

  void SetPrice(double price) {
    sprintf(price_, "%d", int(price * 100)); //转化为分
  }
  
  void SetReqReserved(int64 req_reserved) {
    sprintf(req_reserved_, "%lld", req_reserved);
  }
  /*  inline void set_body(std::string b) {
    body_ = b;
  }
  inline void set_out_trade_no(std::string oid) {
    out_trade_no = oid;
  }
  inline void set_total_fee(int fee) {
    total_fee = fee;
  }
  inline void set_spbill_create_ip(std::string ip) {
    spbill_create_ip = ip;
  }
  inline void set_prepayid(std::string preid) {
    prepayid = preid;
    }*/
  void PreSign();
  void PreSerialize(DicValue* dic);
  std::string PlaceOrder();
  
  std::string URLEncode(const std::string& value);
  std::string URLDecode(std::string& value);
  std::string ToString(std::map<std::string, std::string> content_data,
                              bool url_encode = false, bool cover = false);
  std::string ShaDigest(std::string& str);
  bool VerifySignature(std::string& result);
  /*inline std::string get_appid() { return appid; }
  inline std::string get_partnerid() { return mch_id; }
  inline std::string get_prepayid() { return prepayid; }
  inline std::string get_package() { return package; }
  inline std::string get_nonce_str() { return nonce_str; }
  inline std::string get_timestamp() { return timestamp; }
  inline std::string get_prepaysign() { return prepaysign; }*/
 static UnionpayOrder* GetInstance();
  bool Init(); 
 private:
  static UnionpayOrder* instance_;
  
  bool single_mode_;
  std::string signcert_path_;
  std::string signcert_pwd_;
  std::string signcert_type_;
  std::string encryptcert_path_;
  std::string validatecert_dir_;
  
  EVP_PKEY* pkey_;
  EVP_PKEY* verify_key_;
  //unsigned long certid_;
  unsigned long encrypt_certid_;
//拼接post下单参数
//"<xml>"
//    "<appid>wx9dc39aec13ee3158</appid>"
//    "<body>APP支付测试</body>"
//    "<mch_id>1404391902</mch_id>"
//    "<nonce_str>1add1a30ac87aa2db72f57a2375d8fec</nonce_str>"
//    "<prepay_id><![CDATA[wx20161031151837c7a7ea649a0616081781]]></prepay_id>"
//    "<notify_url>http://wxpay.weixin.qq.com/pub_v2/pay/notify.v2.php</notify_url>"
//    "<out_trade_no>1415659990</out_trade_no>"
//    "<spbill_create_ip>14.23.150.211</spbill_create_ip>"
//    "<total_fee>1</total_fee>"
//    "<trade_type>APP</trade_type>"
//    "<sign>2EAD4CA62CA2677D0C307CDDF6D6BC89</sign>"
//"</xml>";
//  std::string PostFiled();
  // void InitWxVerify();
//  银联下单签名
//  签名算法
//  签名生成的通用步骤如下：
//  第一步，设所有发送或者接收到的数据为集合M，将集合M内非空参数值的参数按照参数名ASCII码从小到大排序（字典序），使用URL键值对的格式（即key1=value1&key2=value2…）拼接成字符串stringA。
//  特别注意以下重要规则：
//  ◆ 参数名ASCII码从小到大排序（字典序）；
//  ◆ 如果参数的值为空不参与签名；
//  ◆ 参数名区分大小写；
//  ◆ 验证调用返回或银联主动通知签名时，传送的sign参数不参与签名，将生成的签名与该sign值作校验。
//  ◆ 银联接口可能增加字段，验证签名时必须支持增加的扩展字段
//  第二步，在stringA最后拼接上key得到stringSignTemp字符串，并对stringSignTemp进行MD5运算，再将得到的字符串所有字符转换为大写，得到sign值signValue。
//  key设置路径：银联商户平台(pay.weixin.qq.com)-->账户设置-->API安全-->密钥设置
//  举例：
//  假设传送的参数如下：
//  appid：  wxd930ea5d5a258f4f
//  mch_id： 10000100
//  device_info：  1000
//  body： test
//  nonce_str：  ibuaiVcKdpRxkhJA
//  第一步：对参数按照key=value的格式，并按照参数名ASCII字典序排序如下：
//  stringA="appid=wxd930ea5d5a258f4f&body=test&device_info=1000&mch_id=10000100&nonce_str=ibuaiVcKdpRxkhJA";
//  第二步：拼接API密钥：
//
//  stringSignTemp="stringA&key=192006250b4c09247ec02edce69f6a2d"
//  sign=MD5(stringSignTemp).toUpperCase()="9A0A8659F005D6984697E2CA0A9CF3B7"
  //void PlaceOrderSign();

 private:

  //int price_;
  char price_[32];
  char req_reserved_[64];
  
  std::map<std::string, std::string> content_data_;
  
  //产品类型 固定值000201 B2C网关支付
  const char* biz_type_;
  //交易子类 固定值01
  const char* txn_sub_type_;
  //商户号
  const char* mer_id_;
  //回调地址
  const char* back_url_;

  //商户订单号
  std::string order_id_;
  //交易金额
  int64 tnx_amt_;
  //后台通知地址
  //std::string back_url_;

  //银联开放平台审核通过的应用APPID
  std::string appid_;
  //APP——需传入应用市场上的APP名字-实际商品名称，天天爱消除-游戏充值
  std::string body_;
  //银联支付分配的商户号
  std::string mch_id_;
  //随机字符串，不长于32位
  std::string nonce_str_;
  //接收银联支付异步通知回调地址，通知url必须为直接可访问的url，不能携带参数。
  std::string notify_url_;
  //商户系统内部的订单号,32个字符内、可包含字母
  std::string out_trade_no_;
  //用户端实际ip
  std::string spbill_create_ip_;
  //订单总金额，单位为分
  int total_fee_;
  //支付类型 固定 为 APP
  std::string trade_type_;
  //商户key 参与sign签名-不做其他用途
  std::string key_;
  //签名，详见函数说明
  std::string sign_;


  //下单成功

  //银联生成的预支付回话标识，用于后续接口调用中使用，该值有效期为2小时
  std::string prepayid_;
  //用于客户端调支付
  std::string prepaysign_;
  //暂填写固定值Sign=WXPay
  std::string package_;
  //时间戳
  std::string timestamp_;
};

}

#endif  // PUB_PAY_UNIONPAY_WX_ORDER_H_
