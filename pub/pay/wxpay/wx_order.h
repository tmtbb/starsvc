// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// WXOrder.h
// Created on: 2016年11月1日.
// Author: Paco.
#ifndef PUB_PAY_WXPAY_WX_ORDER_H_
#define PUB_PAY_WXPAY_WX_ORDER_H_

#include <string>

#include "net/typedef.h"

const std::string APPID = "wx9dc39aec13ee3158";
const std::string MCH_ID = "1404391902";
//const std::string NOTIFY_URL =
//   "https://pay.v.miglab.com/cgi-bin/paycall/wxpaycall/wxpaycall.fcgi";
const std::string NOTIFY_URL = "http://61.147.114.87/cgi-bin/tswp/pay/v1/wx_callback.fcgi";
	/*"https://61.147.114.78/cgi-bin/paycall/wxpaycall/wxpaycall.fcgi";*/
const std::string KEY = "241b71f361d6663c12a12e0f43208ae9";
const std::string TRADE_TYPE = "APP";
const std::string WX_URL = "https://api.mch.weixin.qq.com/pay/unifiedorder";
const std::string WX_PACKAGE = "Sign=WXPay";
/*
 *命名规则为微信文档命名方式
 */
class WXOrder {
 public:
  WXOrder();
  virtual ~WXOrder();

  inline void set_body(std::string b) {
    body = b;
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
  }
  void PreSign();
  void PreSerialize(DicValue* dic);
  std::string PlaceOrder();

  inline std::string get_appid() { return appid; }
  inline std::string get_partnerid() { return mch_id; }
  inline std::string get_prepayid() { return prepayid; }
  inline std::string get_package() { return package; }
  inline std::string get_nonce_str() { return nonce_str; }
  inline std::string get_timestamp() { return timestamp; }
  inline std::string get_prepaysign() { return prepaysign; }
 private:
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
  std::string PostFiled();
  void InitWxVerify();
//  微信下单签名
//  签名算法
//  签名生成的通用步骤如下：
//  第一步，设所有发送或者接收到的数据为集合M，将集合M内非空参数值的参数按照参数名ASCII码从小到大排序（字典序），使用URL键值对的格式（即key1=value1&key2=value2…）拼接成字符串stringA。
//  特别注意以下重要规则：
//  ◆ 参数名ASCII码从小到大排序（字典序）；
//  ◆ 如果参数的值为空不参与签名；
//  ◆ 参数名区分大小写；
//  ◆ 验证调用返回或微信主动通知签名时，传送的sign参数不参与签名，将生成的签名与该sign值作校验。
//  ◆ 微信接口可能增加字段，验证签名时必须支持增加的扩展字段
//  第二步，在stringA最后拼接上key得到stringSignTemp字符串，并对stringSignTemp进行MD5运算，再将得到的字符串所有字符转换为大写，得到sign值signValue。
//  key设置路径：微信商户平台(pay.weixin.qq.com)-->账户设置-->API安全-->密钥设置
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
  void PlaceOrderSign();
 private:
  //微信开放平台审核通过的应用APPID
  std::string appid;
  //APP——需传入应用市场上的APP名字-实际商品名称，天天爱消除-游戏充值
  std::string body;
  //微信支付分配的商户号
  std::string mch_id;
  //随机字符串，不长于32位
  std::string nonce_str;
  //接收微信支付异步通知回调地址，通知url必须为直接可访问的url，不能携带参数。
  std::string notify_url;
  //商户系统内部的订单号,32个字符内、可包含字母
  std::string out_trade_no;
  //用户端实际ip
  std::string spbill_create_ip;
  //订单总金额，单位为分
  int total_fee;
  //支付类型 固定 为 APP
  std::string trade_type;
  //商户key 参与sign签名-不做其他用途
  std::string key;
  //签名，详见函数说明
  std::string sign;


  //下单成功

  //微信生成的预支付回话标识，用于后续接口调用中使用，该值有效期为2小时
  std::string prepayid;
  //用于客户端调支付
  std::string prepaysign;
  //暂填写固定值Sign=WXPay
  std::string package;
  //时间戳
  std::string timestamp;
};

#endif  // PUB_PAY_WXPAY_WX_ORDER_H_
