// Copyright (c) 2016 The tourism Authors. All rights reserved.
// ref_base.h
// Created on: 2016年8月19日.
// Author: Paco.
#ifndef PUB_COMM_REF_BASE_H_
#define PUB_COMM_REF_BASE_H_

#include <stdlib.h>

class RefBase {
 public:
  RefBase():ref_count_(1) {}
  virtual ~RefBase() {};

  void AddRef() {
    __sync_fetch_and_add(&ref_count_,1);
  }
  void DelRef() {
    __sync_fetch_and_sub(&ref_count_, 1);
    if (ref_count_ == 0)
      delete this;
  }
 private:
  int ref_count_;
};


#endif  // PUB_COMM_REF_BASE_H_
