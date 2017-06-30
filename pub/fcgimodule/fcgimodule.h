//  Copyright (c) 2015-2015 The restful Authors. All rights reserved.
//  Created on: 2015/11/24 Author: tianyiheng
#ifndef __FCGI_MODULE_H__
#define __FCGI_MODULE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

// fcgi include
#if defined (FCGI_STD)
#include <fcgi_stdio.h>
#elif defined(FCGI_PLUS)
#include <fcgio.h>
#include <fcgi_config.h>
#endif

// library public
#include "config/config.h"
#include "comm/core_interface.h"
#include "log/mig_log.h"

// log trace
//#include "logtrace/log_trace.h"

// fcgi project module
#define RESTFUL_CORE_SOCK_FILE "/var/www/tmp/jindowincorefile"
#define BIND_HOST  "127.0.0.1"
#define BIND_PORT  16006
namespace fcgi_module {

enum HttpRequestType{
  HTTP_GET = 1,
  HTTP_POST = 2,
  HTTP_PUT = 3,
  HTTP_DELETE = 4  
};

class FcgiModule{
  public:
    FcgiModule(bool is_filter = false, int filter_type = -1);
    ~FcgiModule();
    bool Init(const char *addr, unsigned short port, int api_type, int operate_code, int log_type = 1);

    bool Init(std::string& core_sock_file, int api_type,
        int operate_code, int log_type = 1/*log_trace::TRACE_API_LOG*/);

  void Run();
    void Close();

    bool TestStrPacket();

  protected:
    void StdRun();
    void PlusRun();
    // std
    bool GetRequestMethod(const char* query);
    bool PostRequestMethod(const std::string & content);
    bool PutRequestMethod(const std::string & content);
    bool DeleteRequestMethod(const std::string &content);

    // plus
    bool GetRequestMethod(FCGX_Request * request);
    bool PostRequestMethod(FCGX_Request * request);
    bool PutRequestMethod(FCGX_Request * request);
    bool DeleteRequestMethod(FCGX_Request * request);

  protected:
    FCGX_Request request_;
    int api_type_;
    //log_trace::LogTrace api_logger_;
    int log_type_;
    int operate_code_;
    int filter_type_; //过滤类型
    bool is_filter_;
  private:
    bool XMLFilter(const std::string& content,std::string& req_msg);
    bool PostFilter(const std::string& content,std::string& req_msg);
};

}; // namespace fcgi_module end  

#endif

