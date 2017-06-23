//  Copyright (c) 2017-2018 The SWP Authors. All rights reserved.
//  Created on: 2017年1月12日 Author: kerry

#ifndef PAY_ALIPAY__ORDER_H_
#define PAY_ALIPAY__ORDER_H_

#include <string>
#include "logic/base_values.h"
#include "http/http_method.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <map>

/*
*/

typedef std::map<std::string, std::string> StringMap;
/*
 */
namespace pay_logic {

class ApliPayOrder {
 public:
  ApliPayOrder();
  virtual ~ApliPayOrder();

  inline void set_out_trade_no(std::string oid) {
    out_trade_no = oid;
  }

  std::string PlaceOrder();
//---------------------------------------------------------------------------------
  inline std::string get_timestamp() {
    return timestamp;
  }
/*
*/
  std::string PlaceOrderSign(std::string &content);
  std::string analyzeResponse(const std::string &responseStr); 
  bool DealResponse(const std::string &responseStr, std::string &trade_no,
    std::string &out_trade_no, double &price, int &status); 
 private:
  //std::string PostFiled();

  std::string rsaSign(const std::string &content, const std::string &key); 
  bool rsaVerify(const std::string &content, const std::string &sign, const std::string &key);
 private:
  std::string key;
  //下单成功
  //std::string prepayid;
  //时间戳
  std::string timestamp;
//充值----------------------------------------------------------------------------------------------------------------
  //
  std::string out_trade_no;
  std::string subject;		//
  int total_amount;	//订单总金额
  
  //接收支付异步通知回调地址，通知url必须为直接可访问的url，不能携带参数。
  std::string notify_url;

public:
};

}

#endif  // PUB_PAY_WXPAY_WX_ORDER_H_
