// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 9
#include "IntPair.h"

IntPair::IntPair(int first, int second) {
  Set(first, second);
}

void IntPair::Get(int *first, int *second) {
  *first = this->first_;
  *second = this->second_;
}

void IntPair::Set(int first, int second) {
  this->first_ = first;
  this->second_ = second;
}
