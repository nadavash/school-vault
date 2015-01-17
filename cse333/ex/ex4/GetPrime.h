// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 4
#include <stdint.h>

#ifndef GET_PRIME_H_
#define GET_PRIME_H_

// Returns the value of the [nth] prime number, starting from 1.
// GetPrime(0) => 1
// GetPrime(1) => 2
// GetPrime(2) => 3
// ...
uint64_t GetPrime(uint16_t nth);

#endif
