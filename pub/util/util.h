// Copyright (c) 2016 The tourism Authors. All rights reserved.
// util.h
// Created on: 2016年8月15日.
// Author: Paco.
#ifndef PUB_UTIL_UTIL_H_
#define PUB_UTIL_UTIL_H_

#include <stddef.h>
#include <string>
#include <sys/types.h>

namespace util {

std::string GetStrftime(char* format, time_t t);

int SendFull(int socket, const char *buffer, size_t nbytes);

double Angle2Radian(double angle);

//根据经纬度获取具体 km
double CalcDistance(double lon1, double lat1, double lon2, double lat2);

//根据坐标以及距离获取经纬度范围 km
void BonderOfCoordinate(double lon, double lat, double dis, double* out);

int Random(int min, int max);

//dt device token 用户苹果设备标识
//unreadcount 未读消息数量
//title 通知栏显示内容第一排
//body 通知栏显示内容第二1排
//category 具体消息实体 json格式
int PushApnChatMsg(char* dt, int unreadcount, char* title, char* body,
                   char* category);

bool GetIPAddress(const int socket, std::string* ip, int* port);

//获取随机字符串
//length 要获取字符串的长度 不含 '/0'
std::string RandomString(int length);

std::string GetWxpayXmlValue(std::string value);

}  // namespace util

#endif  // PUB_UTIL_UTIL_H_
