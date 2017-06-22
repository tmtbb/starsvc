#include <stdio.h>
#include <stdlib.h>

#include <string>
#include <iostream>
#include <sstream>

// #include "client/linux/handler/exception_handler.h"

#if defined (FCGI_STD)
#include <fcgi_stdio.h>
#elif defined(FCGI_PLUS)
#include <fcgio.h>
#include <fcgi_config.h>
#endif

#include "log/mig_log.h"
#include "../pub/net/comm_head.h"
#include "../plugins/pay/operator_code.h"
#include "fcgimodule/fcgimodule.h"
// 设置请求类型
//#define API_TYPE            george_logic::VIP_TYPE
//#define LOG_TYPE            log_trace::TRACE_API_LOG

int main(int agrc, char* argv[]) {

/*
    while(FCGI_Accept()==0){
        char *request_method = getenv("REQUEST_METHOD");
        char *contentLength = getenv("CONTENT_LENGTH");
        printf("Content-type: text/html\r\n"
                "\r\n"
                "%s %d %s %s\n",BIND_HOST,BIND_PORT, request_method);

        if (strcmp(request_method, "POST") == 0) {
            printf("test\n");
        }
    }
    */
  fcgi_module::FcgiModule fcgi_client(false, 0);
  fcgi_client.Init(BIND_HOST, BIND_PORT, PAY_TYPE,
                   R_ALIPAY_SVC,
                   1);
  fcgi_client.Run();
  fcgi_client.Close();
  
  return 0;
}
