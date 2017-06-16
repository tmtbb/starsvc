//  Copyright (c) 2016-2017 The SWP Authors. All rights reserved.
//  Created on: 2016年12月30日 Author: kerry

#ifndef BASE_MCONTAINER_H_
#define BASE_MCONTAINER_H_

#include <map>
#include <list>
#include <vector>
#include "basic/basictypes.h"

namespace base_logic {

template<typename KeyType, typename ValType>
class AutoMMap {
 public:
  AutoMMap();
  virtual ~AutoMMap();
  void Set(KeyType key, ValType value);
 private:
  std::map<KeyType, ValType> map_;
  int32 pos_;
  int32 count_;
};

template<typename ValType>
class AutoMList {
 public:
  AutoMList();
  virtual ~AutoMList();
  void Set(ValType value);
  void PushBack(ValType& value);
 private:
  std::list<ValType> list_;
  int32 pos_;
  int32 count_;

};

template<typename ValType>
class AutoMVector {
 public:
  AutoMVector();
  virtual ~AutoMVector();
 public:
  void PushBack(ValType& value);
 private:
  std::vector<ValType> vec_;
  int32 pos_;
  int32 count_;
};

}
#endif
