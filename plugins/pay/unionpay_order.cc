// Copyright (c) 2016 The tvlsvc Authors. All rights reserved.
// UnionpayOrder.cc
// Created on: 2016年11月1日.
// Author: Paco.
#include "unionpay_order.h"

#include <iostream>
#include <fstream>
#include <iomanip>

#include "basic/md5sum.h"
#include "http/http_method.h"
#include "util/util.h"
#include "logic/logic_comm.h"
#include "logic/logic_unit.h"

#include <openssl/engine.h>
#include <openssl/pem.h>
#include <openssl/rsa.h>
#include <openssl/sha.h>
#include <openssl/err.h>
#include "basic/base64.h"

namespace pay_logic {
//const char* UnionpayOrder::biz_type_ = "000201";
//const char* UnionpayOrder::txn_sub_type_ = "01";
UnionpayOrder* UnionpayOrder::instance_ = NULL;
//const char* UnionpayOrder::mer_id_ = "777290058110048";

//const char* UnionpayOrder::mer_id_ = "808080211304858";
//const char* UnionpayOrder::mer_id_ = "104210242141007";

bool KVConfig::Load(const char* filename) {
  std::ifstream inf(filename);
  if (inf.is_open() == false)
    return false;

  std::string value;
  int line = 0;
  while (std::getline(inf, value)) {
    if (value[0] == '#')
      continue;
    if (value.length() == 0)
      continue;

    std::string::size_type idx = value.find("=");
    if (idx != std::string::npos) {
      std::string key = value.substr(0, idx);
      TrimString(key);

      std::string val = value.substr(idx + 1);
      TrimString(val);

      key_value_map_[key] = val;
    }
  }

  return true;
}

void KVConfig::TrimString(std::string& str) {
  int i = 0;

  for (; i < (int) str.length() && isspace(str[i]); i++)
    ;
  str = str.substr(i);

  for (i = (int) str.length() - 1; i >= 0 && isspace(str[i]); i--)
    ;
  str.erase(i + 1);
}

UnionpayOrder::UnionpayOrder() {
  content_data_[K_BIZ_TYPE] = UNIONPAY_BIZ_TYPE;
  content_data_[K_TXN_SUB_TYPE] = UNIONPAY_TXN_SUB_TYPE;
  content_data_[K_BACK_URL] = UNIONPAY_BACK_URL;
  content_data_[K_TXN_TYPE] = UNIONPAY_TXN_TYPE;
  content_data_[K_CHANNEL_TYPE] = UNIONPAY_CHANNEL_TYPE;
  content_data_[K_ENCODING] = UNIONPAY_ENCODING;
  content_data_[K_VERSION] = UNIONPAY_VERSION;
  content_data_[K_ACCESS_TYPE] = UNIONPAY_ACCESS_TYPE;
  content_data_[K_MER_ID] = UNIONPAY_MER_ID;
  content_data_[K_ACC_TYPE] = UNIONPAY_ACC_TYPE;
  content_data_[K_CURRENCY_CODE] = UNIONPAY_CURRENCY_CODE;
  content_data_[K_SIGN_METHOD] = UNIONPAY_SIGN_METHOD;

  total_fee_ = 0;
}

UnionpayOrder::~UnionpayOrder() {
}

UnionpayOrder* UnionpayOrder::GetInstance() {
  if (instance_ == NULL)
    instance_ = new UnionpayOrder();
  return instance_;
}

bool UnionpayOrder::Init() {
  pay_logic::KVConfig cfg;
  if (!cfg.Load(UNIONPAY_CONFIG_FILE))
    assert(0);

  std::string single_mode = cfg.GetString(SDK_SINGLE_MODE);
  if (single_mode == "true") {
    single_mode_ = true;
    LOG_MSG("unionpay single mode");
  } else if (single_mode == "false") {
    single_mode_ = false;
    LOG_MSG("unionpay multi mode");
  } else
    LOG_ERROR("unionpay mode load error");

  signcert_path_ = cfg.GetString(SDK_SIGNCERT_PATH);
  LOG_MSG2("unionpay signcert path:%s", signcert_path_.c_str());

  signcert_pwd_ = cfg.GetString(SDK_SIGNCERT_PWD);
  LOG_MSG2("unionpay signcert pwd:%s", signcert_pwd_.c_str());

  signcert_type_ = cfg.GetString(SDK_SIGNCERT_TYPE);
  LOG_MSG2("unionpay signcert type:%s", signcert_type_.c_str());

  encryptcert_path_ = cfg.GetString(SDK_ENCRYPTCERT_PATH);
  LOG_MSG2("unionpay encryptcert path:%s", encryptcert_path_.c_str());

  validatecert_dir_ = cfg.GetString(SDK_VALIDATECERT_DIR);
  LOG_MSG2("unionpay validatecert path:%s", validatecert_dir_.c_str());

  int rv;
  X509* cert_clnt;
  PKCS12* p12 = NULL;
  STACK_OF(X509) * ca_chain = NULL;
  FILE* fp;

  OpenSSL_add_all_algorithms();

  if ((fp = fopen(signcert_path_.c_str(), "r")) == NULL) {
    LOG_ERROR("open sign cert file error");
    return false;
  }
  if ((p12 = d2i_PKCS12_fp(fp, NULL)) == NULL) {
    LOG_ERROR("read sign cert from fp error");
    return false;
  }

  if (!PKCS12_parse(p12, PFX_PASSWD, &pkey_, &cert_clnt, &ca_chain)) {
    LOG_ERROR2("parse pkcs12 error:%s", ERR_error_string(ERR_get_error(), NULL));
    ERR_print_errors_fp (stderr);
    return false;
  }

  ASN1_INTEGER* serial = X509_get_serialNumber(cert_clnt);
  BIGNUM* bn = ASN1_INTEGER_to_BN(serial, NULL);
  char certid[64];
  sprintf(certid, "%lu", *bn->d);
  content_data_[K_CERTID] = certid;
  LOG_MSG2("pfx certid:%s", certid);

  //verify
  //if ((fp = fopen("plugins/user/test/acp_test_verify_sign.cer", "r")) == NULL) {
  if ((fp = fopen("plugins/user/online/acp_prod_verify_sign.cer", "r"))
      == NULL) {
    LOG_ERROR("open encrypt cert file error");
    return false;
  }

  //ca_chain = sk_X509_new_null();
  X509* cert_tmp;
  while (1) {
    if ((cert_tmp = PEM_read_X509(fp, NULL, NULL, NULL)) != NULL)
      break;
    //sk_X509_push(ca_chain, cert_tmp);
    /*if (ca_chain->num == 1) {
     cert_clnt = cert_tmp;
     LOG_DEBUG("ca chain num is 1");
     }*/
  }
  fclose(fp);
  /*if (ca_chain->num == 0) {
   LOG_DEBUG("ca chain num is 0");
   return;
   }*/

  serial = X509_get_serialNumber(cert_tmp);
  bn = ASN1_INTEGER_to_BN(serial, NULL);
  encrypt_certid_ = *bn->d;
  LOG_MSG2("verify encrypt certid:%lu", encrypt_certid_);

  verify_key_ = X509_get_pubkey(cert_tmp);
  return true;
}


std::string UnionpayOrder::URLEncode(const std::string& value) {
  std::ostringstream escaped;
  escaped.fill('0');
  escaped << std::hex;

  for (std::string::const_iterator it = value.begin(); it != value.end();
      ++it) {
    std::string::value_type c = *it;

    if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~') {
      escaped << c;
      continue;
    }

    escaped << std::uppercase;
    escaped << '%' << std::setw(2) << int((unsigned char) c);
    escaped << std::nouppercase;
  }

  return escaped.str();
}

std::string UnionpayOrder::URLDecode(std::string& value) {
  std::string decoded;

  for (std::string::iterator it = value.begin(); it != value.end(); ++it) {
    std::string::value_type c = *it;

    switch (c) {
      case '%':
        if (isxdigit(*(it + 1)) && isxdigit(*(it + 2))) {
          std::stringstream hex_str;
          hex_str << *(it + 1);
          hex_str << *(it + 2);
          int hex = strtol(hex_str.str().c_str(), NULL, 16);
          /////////////!if hex >=48 && hex <= 57 ?
          decoded += (unsigned char) hex;
          it += 2;
        } else {
          decoded += '%';
        }
      case '+':
        break;  //?
      default:
        decoded += *it;
    }
  }

  LOG_DEBUG2("url code result:%s", decoded.c_str());

  return decoded;
}

std::string UnionpayOrder::ToString(
    std::map<std::string, std::string> content_data, bool url_encode,
    bool cover) {
  std::stringstream post_data;
  for (std::map<std::string, std::string>::iterator it = content_data.begin();
      it != content_data.end(); ++it) {
    if (cover && it->first == K_SIGNATURE)
      continue;

    if (it != content_data.begin())
      post_data << "&" << it->first << "="
          << (url_encode ? URLEncode(it->second) : it->second);
    else
      post_data << it->first << "="
          << (url_encode ? URLEncode(it->second) : it->second);
  }

  return post_data.str();
}

std::string UnionpayOrder::ShaDigest(std::string& str) {
  unsigned char md[SHA_DIGEST_LENGTH];
  unsigned char md_hex[SHA_DIGEST_LENGTH * 2 + 1] = { 0 };
  SHA1((const unsigned char*) str.c_str(), str.length(), md);
  for (int i = 0; i < SHA_DIGEST_LENGTH; ++i) {
    sprintf((char*) md_hex + i * 2, "%02x", md[i]);
  }

  return std::string((const char*) md_hex);
}

std::string UnionpayOrder::PlaceOrder() {
  //std::stringstream req_url;
  time_t seconds = time(0);
  struct tm* now = localtime(&seconds);
  std::stringstream ss;
  char time_buf[128];
  snprintf(time_buf, 128, "%04d%02d%02d%02d%02d%02d", now->tm_year + 1900,
           now->tm_mon + 1, now->tm_mday, now->tm_hour, now->tm_min,
           now->tm_sec);
  LOG_MSG2("time now:%s", time_buf);
  std::string orderId = time_buf;
  std::string time_send = orderId;
  //req_url << ACP_SERVER_URL << "?merId=" << mer_id_
  //       << "&txnTime=" << time_send << "&orderId="
  //       << orderId << "&txnAmt=" << price_;

  content_data_[K_TXN_TIME] = time_send;
  content_data_[K_ORDER_ID] = orderId;
  content_data_[K_PRICE] = price_;
  LOG_DEBUG2("kkkk price_:%s", price_);
  content_data_[K_REQ_RESERVED] = req_reserved_;
  LOG_DEBUG2("kkkk req_reserved_:%s", req_reserved_);
  content_data_[K_REQ_RESERVED] = req_reserved_;
  //content_data_[K_ACC_NO] = "6214835891604306";
  content_data_[K_ORDER_DESC] = WP_UNIONPAY;

  std::string req_str = ToString(content_data_);
  std::string md_hex = ShaDigest(req_str);
  EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new(pkey_, NULL);
  if (!ctx) {
    LOG_ERROR("evp pkey cxt new null");
    return "";
  }
  if (EVP_PKEY_encrypt_init(ctx) <= 0) {
    LOG_ERROR("evp pkey encrypt init err");
    return "";
  }

  EVP_MD_CTX md_ctx;
  EVP_SignInit(&md_ctx, EVP_sha1());
  EVP_SignUpdate(&md_ctx, md_hex.c_str(), md_hex.length());
  unsigned int sign_len;
  if (!EVP_SignFinal(&md_ctx, NULL, &sign_len, pkey_)) {
    LOG_ERROR("pkey sign get len error");
    return "";
  }
  unsigned char* sign_buf = (unsigned char*) OPENSSL_malloc(sign_len);
  if (!EVP_SignFinal(&md_ctx, sign_buf, &sign_len, pkey_)) {
    LOG_ERROR("pkey sign error");
    return "";
  }
  //OPENSSL_free(sign_buf);!!!!!!!!!!!!!!
  LOG_DEBUG2("pkey sign len:%d result:s", sign_len, sign_buf);
  content_data_[K_SIGNATURE] = base64_encode(sign_buf, sign_len);
  LOG_DEBUG2("sign base64:%s", content_data_[K_SIGNATURE].c_str());

  std::string post_data = ToString(content_data_, true);

  std::map<std::string, std::string>::iterator it_sig = content_data_.find(
  K_SIGNATURE);
  content_data_.erase(it_sig);

  LOG_MSG2("unionpay post data:%s", post_data.c_str());

  //std::string url = "https://101.231.204.80:5000/gateway/api/appTransReq.do";
  std::string url = "https://gateway.95516.com/gateway/api/appTransReq.do";  //////////////////////
  http::HttpMethodPost hmp(url);
  //std::string headers = "Content-Type: text/xml";
  //hmp.SetHeaders(headers);
  hmp.Post(post_data.c_str());
  std::string result;
  hmp.GetContent(result);
  LOG_MSG2("http get result:%s", result.c_str());

  if (!VerifySignature(result))
    return "";

  return result;
}

bool UnionpayOrder::VerifySignature(std::string& result) {
  std::vector < std::string > key_value;
  logic::SomeUtils::SplitString(result, '&', key_value);
  std::map < std::string, std::string > resp_data;
  for (std::vector<std::string>::iterator it = key_value.begin();
      it != key_value.end(); ++it) {
    int32 equal_pos = it->find("=");
    if (equal_pos != std::string::npos) {
      resp_data[it->substr(0, equal_pos)] = it->substr(equal_pos + 1);
    }
  }

  std::string resp_str = ToString(resp_data, false, true);
  std::string md_hex = ShaDigest(resp_str);

  EVP_MD_CTX md_ctx;
  EVP_VerifyInit(&md_ctx, EVP_sha1());
  EVP_VerifyUpdate(&md_ctx, md_hex.c_str(), md_hex.length());
  std::string decoded_str = base64_decode(resp_data[K_SIGNATURE]);
  if (EVP_VerifyFinal(&md_ctx, (const unsigned char*) decoded_str.c_str(),
                      decoded_str.length(), verify_key_) != 1) {
    LOG_ERROR2("verify key sign error md_hex:%s len:%d\nresponse data:%s\nresponse data len:%d",
        md_hex.c_str(), md_hex.length(), resp_data[K_SIGNATURE].c_str(), resp_data[K_SIGNATURE].length());
    ERR_print_errors_fp (stderr);
    return false;
  }
  //OPENSSL_free(sign_buf);!!!!!!!!!!!!!!

  return true;
}

}

