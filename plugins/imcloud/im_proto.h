#ifndef PLUGINS_IMCLOUD_IM_PROTO_H_
#define PLUGINS_IMCLOUD_IM_PROTO_H_

#include "net/proto_buf.h"
namespace im_logic {

namespace net_request {

class tokencode {
 public:
  tokencode()
      : im_name_(NULL),
        im_accid_(NULL) {
  }

  ~tokencode() {
    if (im_name_) {
      delete im_name_;
      im_name_ = NULL;
    }
    if (im_accid_) {
      delete im_accid_;
      im_accid_ = NULL;
    }
  }

  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_name(std::string& name) {
    im_name_ = new base_logic::StringValue(name);
  }
  void set_accid(std::string& accid) {
    im_accid_ = new base_logic::StringValue(accid);
  }

  
  std::string name() {
  std::string name;
	if (im_name_)
	  im_name_->GetAsString(&name);
	return name;
	}
  std::string accid() {
    std::string accid;
    if (im_accid_)
      im_accid_->GetAsString(&accid);
    return accid;
  }

 private:
  base_logic::StringValue* im_name_;
  base_logic::StringValue* im_accid_;
};


}
namespace net_reply {

class tokenreply {
 public:
  tokenreply()
      : im_token_(NULL),
        im_result_(NULL) {
  }

  ~tokenreply() {
    if (im_token_) {
      delete im_token_;
      im_token_ = NULL;
    }
    if (im_result_) {
      delete im_result_;
      im_result_ = NULL;
    }
  }
  base_logic::DictionaryValue* get() {
    value_ = new base_logic::DictionaryValue();
    if (im_token_ != NULL)
      value_->Set(L"token_value", im_token_);
    if (im_result_ != NULL)
      value_->Set(L"result_value", im_result_);
    return value_;
  }
  
  int32 set_http_packet(base_logic::DictionaryValue* value);

  void set_token(std::string& token) {
    im_token_ = new base_logic::StringValue(token);
  }
  void set_result(std::string& result) {
    im_result_ = new base_logic::StringValue(result);
  }

  
  std::string tokenvalue() {
  std::string tokenvalue;
	if (im_token_)
	  im_token_->GetAsString(&tokenvalue);
	return tokenvalue;
	}
  std::string resultvalue() {
    std::string resultvalue;
    if (im_result_)
      im_result_->GetAsString(&resultvalue);
    return resultvalue;
  }

 private:
  base_logic::StringValue* im_token_;
  base_logic::StringValue* im_result_;
  base_logic::DictionaryValue* value_;
};


} 

}

#endif
