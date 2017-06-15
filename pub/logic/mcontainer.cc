//  Copyright (c) 2015-2015 The KID Authors. All rights reserved.
//  Created on: 2016.8.16 Author: kerry

#include "mcontainer.h"

namespace base_logic {

template <typename KeyType,typename ValType>
AutoMMap<KeyType,ValType>::AutoMMap()
:pos_(0)
,count_(0){

}

template <typename KeyType,typename ValType>
AutoMMap<KeyType,ValType>::~AutoMMap(){

}

template <typename KeyType,typename ValType>
void AutoMMap<KeyType,ValType>::Set(KeyType key,ValType value) {
  map_[key] = value;
}

template<typename ValType>
AutoMList<ValType>::AutoMList()
:pos_(0)
,count_(0){
}

template<typename ValType>
AutoMList<ValType>::~AutoMList(){

}

template<typename ValType>
void AutoMList<ValType>::Set(ValType value) {
  list_.push_back(value);
}

template<typename ValType>
void AutoMList<ValType>::PushBack(ValType& value){
  list_.push_back(value);
}

template<typename ValType>
AutoMVector<ValType>::AutoMVector()
:pos_(0)
,count_(0){
}

template<typename ValType>
AutoMVector<ValType>::~AutoMVector(){

}

template<typename ValType>
void AutoMVector<ValType>::PushBack(ValType &value) {
  vec_.push_back(value);
}


}

