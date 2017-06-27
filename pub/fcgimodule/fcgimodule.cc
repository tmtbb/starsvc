//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: tianyiheng

#include "fcgimodule.h"
#include "logic/logic_comm.h"
#include "net/packet_processing.h"
#include <algorithm>

namespace fcgi_module {

FcgiModule::FcgiModule(bool is_filter, int filter_type) {
  api_type_ = 0;
  log_type_ = 0;
  operate_code_ = 0;
  is_filter_ = is_filter;
  filter_type_ = filter_type;
}

FcgiModule::~FcgiModule() {

}

bool FcgiModule::Init(const char *addr, unsigned short port, int api_type,
              int operate_code, int log_type){
 net::core_connect_net(addr, port);
 #if defined (FCGI_PLUS)
   FCGX_Init();
   FCGX_InitRequest(&request_, 0, 0);
 #endif
   api_type_ = api_type;
   log_type_ = log_type;
   operate_code_ = operate_code;
   //api_logger_.Init(UNIX_LOGGER_SOCK_FILE_PATH);

   return true;
 }


bool FcgiModule::Init(std::string& core_sock_file, int api_type,
                      int operate_code, int log_type) {
  net::core_connect_ipc(core_sock_file.c_str());
#if defined (FCGI_PLUS)
  FCGX_Init();
  FCGX_InitRequest(&request_, 0, 0);
#endif
  api_type_ = api_type;
  log_type_ = log_type;
  operate_code_ = operate_code;
  //api_logger_.Init(UNIX_LOGGER_SOCK_FILE_PATH);

  return true;
}

void FcgiModule::Run() {
#if defined (FCGI_STD)
  StdRun();
#elif defined (FCGI_PLUS)
  PlusRun();
#endif
}

void FcgiModule::StdRun() {
  std::string content;
  const char* query;
#if defined (FCGI_STD)
  while (FCGI_Accept() == 0) {
    LOG_DEBUG("=============================fcgiaccept");
    char *request_method = getenv("REQUEST_METHOD");
    char *contentLength = getenv("CONTENT_LENGTH");
    if (strcmp(request_method, "POST") == 0) {
      int len = strtol(contentLength, NULL, 10);
      for (int i = 0; i < len; i++) {
        char ch;
        ch = getchar();
        content.append(1, ch);
      }
      PostRequestMethod(content);
      content.clear();
    } else if (strcmp(request_method, "GET") == 0) {
      query = getenv("QUERY_STRING");
      GetRequestMethod(query);
    } else if (strcmp(request_method, "PUT") == 0) {
      std::cin >> content;
      PutRequestMethod(content);
    } else if (strcmp(request_method, "DELETE") == 0) {
      std::cin >> content;
      DeleteRequestMethod(content);
    }
  }  // while end
#endif
}

void FcgiModule::PlusRun() {
  std::string content;
  const char* query;
#if defined (FCGI_PLUS)
  while (FCGX_Accept_r(&request_) == 0) {
    fcgi_streambuf cin_fcgi_streambuf(request_.in);
    fcgi_streambuf cout_fcgi_streambuf(request_.out);
    fcgi_streambuf cerr_fcgi_streambuf(request_.err);

#if HAVE_IOSTREAM_WITHASSIGN_STREAMBUF
    std::cin = &cin_fcgi_streambuf;
    std::cout = &cout_fcgi_streambuf;
    std::cerr = &cerr_fcgi_streambuf;
#else
    std::cin.rdbuf(&cin_fcgi_streambuf);
    std::cout.rdbuf(&cout_fcgi_streambuf);
    std::cerr.rdbuf(&cerr_fcgi_streambuf);
#endif

    char *request_method = FCGX_GetParam(
        "REQUEST_METHOD", request.envp);
    if (strcmp(request_method, "POST") == 0)
    PostRequestMethod(&request_);
    else if (strcmp(request_method, "GET") == 0)
    GetRequestMethod(&request_);
    else if (strcmp(request_method, "PUT") == 0)
    PutRequestMethod(&request_);
    else if (strcmp(request_method, "DELETE") == 0)
    DeleteRequestMethod(&request_);
  }  // while end
#endif
}

void FcgiModule::Close() {
  net::core_close();
}

// std
bool FcgiModule::GetRequestMethod(const char* query) {
  std::string content;
  std::string respone;
  std::stringstream os;
  int flag = 0;
  int code = 0;
  bool ret = false;
  char *addr = getenv("REMOTE_ADDR");
  os << std::string(query) << "&remote_addr=" << addr << "&operate_code="
      << operate_code_ << "&type=" << api_type_ << "&log_type=" << log_type_
      << "\n\t\r";
  content = os.str();
  // log trace,暂时不用 
  //api_logger_.LogMsg(content.c_str(), content.length());
  ret = net::core_get(0, content.c_str(), content.length(), respone, flag,
                      code);
  LOG_DEBUG2("Get responst:%s", respone.c_str());

  if (!respone.empty()) {
    printf("Content-type: text/html\r\n"
           "\r\n"
           "%s",
           respone.c_str());
  }
  return true;
}

bool FcgiModule::TestStrPacket() {
  int flag;
  int code;
  std::string respone;
  std::string content =
      "{\"id\":100002,\"token\":\"adc28ac69625652b46d5c00b\",\"exchangeName\":\"TJPME\",\"platformName\":\"JH\",\"symbol\":\"AG\",\"chartType\":1}";
  std::stringstream os;
  std::string addr = "115.122.238.25";
  os << content;
  //os << content << "&remote_addr=" << addr << "&type=" << api_type_
  //  << "&operate_code=" << operate_code_ << "&log_type=" << log_type_ << "\n";

  /*
   * const int16 t_operate_code,
   const int8 t_is_zip_encrypt, const int8 t_type,
   int64 t_session_id, const int32 t_reserved,
   const std::string& body_stream, void **packet_stream,
   int32 *packet_stream_length
   * */

  void *packet_stream = NULL;
  int32_t packet_stream_length = 0;
  int16 operate_code = operate_code_;
  int8 t_is_zip_encrypt = 0;
  const int8 t_type = api_type_;
  net::PacketProsess::StrPacket(operate_code_, t_is_zip_encrypt, t_type, 0, 0,
                                os.str().c_str(), &packet_stream,
                                &packet_stream_length);

  respone.append(reinterpret_cast<const char*>(packet_stream),
                 packet_stream_length);
  if (packet_stream) {
    delete packet_stream;
    packet_stream = NULL;
  }

  std::string r_respone = net::PacketProsess::StrUnpacket(
      (void*) (respone.c_str()), respone.length());

  //使用解包函数
  struct PacketHead *packet = NULL;

  if (!net::PacketProsess::UnpackStream(packet_stream, packet_stream_length,
                                        &packet)) {
    return false;
  }

  return true;
}

bool FcgiModule::PostRequestMethod(const std::string & content) {
  std::string respone;
  int flag;
  int code;
  std::stringstream os;
  bool r = false;
  // 若CONTENT_TYPE非application/x-www-form-urlencode 直接pass掉
#if defined (CHECK_HEADER)
  if (strcmp(content_type, "application/x-www-form-urlencoded") != 0)
  return false;
#endif
  os << content;


  void *packet_stream = NULL;
  int32 packet_stream_length = 0;

  void *r_packet_stream = NULL;
  int32 r_packet_stream_length = 0;
  int16 operate_code = operate_code_;
  int8 t_is_zip_encrypt = 0;
  const int8 t_type = api_type_;
  std::string ct = os.str();
  if (is_filter_){
    std::string result;
    if (XMLFilter(ct,result))
      ct = result;
  }
  //postdeal key=value&key=value
  if (filter_type_ == 0){
    std::string result;
    if (PostFilter(ct,result))
      ct = result;
  }
  net::PacketProsess::StrPacket(operate_code_, t_is_zip_encrypt, t_type, 0,
                                2001, ct.c_str(), &packet_stream,
                                &packet_stream_length);


  respone.clear();
  r = net::core_get(0, reinterpret_cast<char*>(packet_stream),
                    packet_stream_length, respone, flag, code);

  LOG_DEBUG2("respone length %d,r %d",respone.length(),r);

  std::string r_repone;
  r_repone.clear();
  if (r && !is_filter_)
    r_repone = net::PacketProsess::StrUnpacket((void*) (respone.c_str()),
                                               respone.length());
  else
    r_repone = respone;
  if (!r_repone.empty() && r) {
    printf(
        "Content-type: application/json;charset=utf-8\r\nAccess-Control-Allow-Origin: *\r\n"
           "\r\n"
           "%s",
           r_repone.c_str());
  }
  return true;
}

bool FcgiModule::PutRequestMethod(const std::string & content) {
  return true;
}

bool FcgiModule::DeleteRequestMethod(const std::string &content) {
  return true;
}

// plus
bool FcgiModule::GetRequestMethod(FCGX_Request * request) {
  return true;
}

bool FcgiModule::PostRequestMethod(FCGX_Request * request) {
  char * clenstr = FCGX_GetParam("CONTENT_LENGTH", request->envp);
  if (clenstr) {
    unsigned long clen = strtol(clenstr, &clenstr, 10);
    char* content = new char[clen];
    std::cin.read(content, clen);
    clen = std::cin.gcount();
    if (content) {
      delete[] content;
      content = NULL;
    }
  }
  return true;
}

bool FcgiModule::PutRequestMethod(FCGX_Request * request) {
  return true;
}

bool FcgiModule::DeleteRequestMethod(FCGX_Request * request) {
  return true;
}

bool FcgiModule::XMLFilter(const std::string& content,std::string& req_msg) {
  if (!content.empty()) {
    std::string s = content;
    req_msg = "{\"result\":";
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    int beg = s.find("<xml>");
    int end = s.find("</xml>") + sizeof("</xml>") + 1;
    s = s.substr(beg, end - beg);
    req_msg += "\"";
    req_msg += s;
    req_msg += "\"}";
    return true;
  }
  return false;
}

bool FcgiModule::PostFilter(const std::string& content,std::string& req_msg) {
  if (!content.empty()) {
    std::string s = content;
    req_msg = "{\"result\":";
    s.erase(remove(s.begin(), s.end(), '\n'), s.end());
    //int beg = s.find("<xml>");
    //int end = s.find("</xml>") + sizeof("</xml>") + 1;
    //s = s.substr(beg, end - beg);
    req_msg += "\"";
    req_msg += s;
    req_msg += "\"}";
    return true;
  }
  return false;
}

}  // namespace fcgi_module end
