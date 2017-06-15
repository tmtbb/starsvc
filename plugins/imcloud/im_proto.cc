

#include "imcloud/im_proto.h"

#include <typeinfo>

#include "basic/md5sum.h"

#include "comm/comm_head.h"
#include "util/util.h"
#include "logic/logic_comm.h"

namespace im_logic {

namespace net_request {


int32 tokencode::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  std::string name_value;
  std::string accid_value;
  do {
    if (value != NULL) {
      r = value->GetString(L"name_value", &name_value);
      if (r)
        set_name(name_value);
      if (!r)
        LOG_ERROR("tokencode::name_value parse error");
    } else {
      LOG_ERROR("tokencode Deserialize error");
      err = JSON_FORMAT_ERR;
      break;
    }
	if (value != NULL) {
      r = value->GetString(L"accid_value", &accid_value);
      if (r)
        set_accid(accid_value);
      if (!r)
        LOG_ERROR("tokencode::accid_value parse error");
    } else {
      LOG_ERROR("tokencode Deserialize error");
      err = JSON_FORMAT_ERR;
      break;
    }
  } while (0);
  return err;
}


}  // namespace net_request

namespace net_reply {


int32 tokenreply::set_http_packet(base_logic::DictionaryValue* value) {
  int32 err = 0;
  bool r = false;
  std::string token_value;
  std::string result_value;
  do {
    if (value != NULL) {
      r = value->GetString(L"token_value", &token_value);
      if (r)
        set_token(token_value);
      if (!r)
        LOG_ERROR("tokenreply::token_value parse error");
    } else {
      LOG_ERROR("tokenreply Deserialize error");
      err = JSON_FORMAT_ERR;
      break;
    }
	if (value != NULL) {
      r = value->GetString(L"result_value", &result_value);
      if (r)
        set_result(result_value);
      if (!r)
        LOG_ERROR("tokenreply::result_value parse error");
    } else {
      LOG_ERROR("tokenreply Deserialize error");
      err = JSON_FORMAT_ERR;
      break;
    }
  } while (0);
  return err;
}


}


}

