#ifndef HTTP_CLIENT_H
#define HTTP_CLIENT_H

#include "curl/curl.h"
#include <string>
#include <string.h>
#include <list>
#include <map>

using std::string;
using std::list;
using std::map;


typedef map<string, string> HeaderMap;
typedef map<string, string> ParamsMap;

/**
 *  * HttpClient
 *   * @brief The HttpClient class
 *    */
class HttpClient
{
public:
    HttpClient();
    HttpClient(const HttpClient &);
    HttpClient & operator = (const HttpClient &);

    ~HttpClient();

public:
    /**
 *  * Set global proxy
 *  * 设置全局代理
 *  * @brief setProxy
 *  * @param proxyIp
 *  * @param proxyPort
 *  * @param proxyUserName
 *  * @param proxyPassword
 **/
 /*
    static void setProxy(const string &proxyIp = string(),
                         const string &proxyPort = string(),
                         const string &proxyUserName = string(),
                         const string &proxyPassword = string());
    */

private:
/*
    static string proxyIp;
    static string proxyPort;
    static string proxyUserName;
    static string proxyPassword;
    */

private:
    string responseEntity;

public:
/**
 ** @brief sendSyncRequest
 ** @param url
 ** @param requestEntity
 ** @param headers
 ** @return
 **/
 /*
    string sendSyncRequest(const string &url,
                           const string &requestEntity,
                           const HeaderMap &headers = HeaderMap());
    */

/**
 ** @brief sendSyncRequest
 ** @param url
 ** @param paramsMap
 ** @param headers
 ** @return
 **/
/*    string sendSyncRequest(const string &url,
                           const ParamsMap &paramsMap,
                           const HeaderMap &headers = HeaderMap());

*/
    string getOrderInfo(const string &url,
                           const ParamsMap &paramsMap,
                           const HeaderMap &headers = HeaderMap());
private:
    CURL *curl;
 //  static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream);

};

#endif // HTTP_CLIENT_H

