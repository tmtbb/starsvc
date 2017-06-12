// Copyright (c) 2016 The tourism Authors. All rights reserved.
// util.cc
// Created on: 2016年8月15日.
// Author: Paco.
#include "util/util.h"
#include "thread/base_thread_lock.h"
#include "thread/base_thread_handler.h"
#include "logic/logic_comm.h"
#include "basic/basictypes.h"
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <pthread.h>
#include <cmath>

//#include "gtpush/IGtPush.h"
//#include "glog/logging.h"
#include "logic/logic_comm.h"

const double PI = 3.1415926535898;
const double EARTH_R = 6371.393000;  //km
const char *APPID = "d2YVUlrbRU6yF0PFQJfPkA";
const char *APPKEY = "yEIPB4YFxw64Ag9yJpaXT9";
const char *MASTERSECRET = "bMsRgf7RrA6jBG4sNbv0F6";
//static pthread_mutex_t sock_send_mutex_lock = PTHREAD_MUTEX_INITIALIZER;
struct threadrw_t* sock_send_mutex_lock = NULL;

namespace util {
std::string GetStrftime(char* format, time_t t) {
  char buf[100];
  memset(buf, 0, sizeof(t));
  strftime(buf, sizeof(buf), format, localtime(&t));
  return buf;
}

int SendFull(int socket, const char *buffer, size_t nbytes) {
  if (NULL == sock_send_mutex_lock)
    InitThreadrw(&sock_send_mutex_lock);
  base_logic::WLockGd lk(sock_send_mutex_lock);
  //net::PacketProsess::HexEncode(buffer, nbytes);
  ssize_t amt = 0;
  ssize_t total = 0;
  const char *buf = buffer;
  do {
    amt = nbytes;
    amt = send(socket, buf, amt, 0);
    if (-1 == amt) {
      if (11 == errno)
        continue;
      else {
        break;
      }
    }
    buf = buf + amt;
    nbytes -= amt;
    total += amt;
  } while (nbytes > 0);
  return static_cast<int32>(amt == -1 ? amt : total);
  /*LOG(ERROR) << "SendFull :: lock";
   pthread_mutex_lock(&sock_send_mutex_lock);
   LOG(ERROR) << "SendFull :: locked";
   ssize_t amt = 0;
   ssize_t total = 0;
   const char *buf = buffer;
   do {
   amt = nbytes;
   amt = send(socket, buf, amt, 0);
   buf = buf + amt;
   nbytes -= amt;
   total += amt;
   } while (amt != -1 && nbytes > 0);
   pthread_mutex_unlock(&sock_send_mutex_lock);
   LOG(ERROR) << "SendFull :: unlock";
   return amt == -1 ? amt : total;*/
}

double Angle2Radian(double angle) {
  return angle * PI / 180.0;
}

double CalcDistance(double lon1, double lat1, double lon2, double lat2) {

  double lat_a = Angle2Radian(lat1);
  double lon_a = Angle2Radian(lon1);
  double lat_b = Angle2Radian(lat2);
  double lon_b = Angle2Radian(lon2);
  return acos(
      sin(lat_a) * sin(lat_b) + cos(lat_a) * cos(lat_b) * cos(lon_a - lon_b))
      * EARTH_R;
}

void BonderOfCoordinate(double lon, double lat, double dis, double* out) {
  lat = Angle2Radian(lat);
  lon = Angle2Radian(lon);  //先换算成弧度
  double rad_dist = dis / EARTH_R;  //计算X公里在地球圆周上的弧度
  double lat_min = lat - rad_dist;
  double lat_max = lat + rad_dist;   //计算纬度范围

  double lon_min, lon_max;
  //因为纬度在-90度到90度之间，如果超过这个范围，按情况进行赋值
  if (lat_min > -PI / 2 && lat_max < PI / 2) {
    //开始计算经度范围
    double lon_t = asin(sin(rad_dist) / cos(lat));
    lon_min = lon - lon_t;
    //同理，经度的范围在-180度到180度之间
    if (lon_min < -PI)
      lon_min += 2 * PI;
    lon_max = lon + lon_t;
    if (lon_max > PI)
      lon_max -= 2 * PI;
  } else {
    lat_min = lat_min > -PI / 2 ? lat_min : -PI / 2;  // max (lat_min , -PI/2)
    lat_max = lat_max < PI / 2 ? lat_max : PI / 2;  //min (lat_max, PI/2)
    lon_min = -PI;
    lon_max = PI;
  }
  //最后置换成角度进行输出
  out[0] = lon_min * 180 / PI;
  ;
  out[1] = lon_max * 180 / PI;
  out[2] = lat_min * 180 / PI;
  out[3] = lat_max * 180 / PI;
}

int Random(int min, int max) {
  return min + rand() % (max - min + 1);
}

int PushApnChatMsg(char* dt, int unreadcount, char* title, char* body,
                   char* category) {

  //准备数据
  /*
  Message msg = { 0 };
  msg.isOffline = 1;  //是否离线下发
  msg.offlineExpireTime = 1000 * 3600 * 2;  //离线下发有效期 毫秒
  SingleMessage singleMsg = { 0 };
  singleMsg.msg = msg;

//  //IOS自定义消息
//  Entry cmsg = { 0 };
//  strcpy(cmsg.key, "cmsg");
//  strcpy(cmsg.value, "cmsg");

  //title-loc-args赋值
  ListItem titlelocargs[2] = { "titlelocargs1", "titlelocargs2" };

  //locargs赋值
  ListItem locargs[2] = { "locargs1", "locargs2" };

  //APN模板，只包含了父类模板
  APNTemplate templ = { 0 };
  templ.t.appId = (char*) APPID;
  templ.t.appKey = (char*) APPKEY;

  templ.t.pushInfo.badge = unreadcount;
  //templ.t.pushInfo.sound="test1.wav";
  //templ.t.pushInfo.contentAvailable=1;
  templ.t.pushInfo.category = category;
  //templ.t.pushInfo.cmsg.size=1;
  //templ.t.pushInfo.cmsg.map = &cmsg;

  templ.t.pushInfo.body = body;
  templ.t.pushInfo.actionLocKey = "";

  templ.t.pushInfo.locKey = "";  //nick+content
  //templ.t.pushInfo.locArgs.size=2;
  //templ.t.pushInfo.locArgs.item=locargs;

  templ.t.pushInfo.launchImage = "";
  //IOS8.2以上版本支持
  templ.t.pushInfo.title = title;
  templ.t.pushInfo.titleLocKey = "V领队";

  //templ.t.pushInfo.titleLocArgs.size=2;
  //templ.t.pushInfo.titleLocArgs.item=titlelocargs;

  IPushResult result = { 0 };

  result = pushAPNMessageToSingle((char*) APPKEY, &templ, APPID, dt);
  //LOG_MSG("print result:-------------");
  for (int i = 0; i < result.size; i++) {
    //LOG_MSG(result.entry[i].key << ": " << result.entry[i].value;
  }
      //   LOG_MSG("print end:----------------");
  //打印结果
//  printResult(result);*/
  return 0;
}

bool GetIPAddress(const int socket, std::string* ip, int* port) {
  struct sockaddr_in sa;
  socklen_t len;
  len = sizeof(sa);
  if (!getpeername(socket, (struct sockaddr *) &sa, &len)) {
    if (ip)
      *ip = inet_ntoa(sa.sin_addr);
    if (port)
      *port = ntohs(sa.sin_port);
    return true;
  }
  return false;
}

std::string RandomString(int length) {
  std::string ran;
  while (length > 0) {
    int n = Random(65, 122);
    if (n > 90 && n < 97)
      continue;
    char p = (char) n;
    ran.push_back(p);
    length--;
  }
  return ran;
}

std::string GetWxpayXmlValue(std::string value) {
  int npos1 = value.find("<![CDATA[");
  int npos2 = value.find("]]>");
  return value.substr(npos1 + 9, npos2 - npos1 - 9);
}

}  // namespace util

