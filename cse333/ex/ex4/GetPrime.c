// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 4
#include <stdbool.h>
#include "GetPrime.h"

// Helper function to check if a number is prime.
_Bool IsPrime(uint64_t num) {
  // Immediately return false if the number is even and above 2.
  if (num > 2 && (num & 1) != 1) {
    return false;
  }

  uint64_t mid = num / 2;
  for (int i = 3; i < mid; ++i) {
    if (num % i == 0) {
      return false;
    }
  }

  return true;
}

uint64_t GetPrime(uint16_t nth) {
  if (nth < 0) {
    return 0;
  }

  int current = 1;
  while (nth > 0) {
    ++current;

    if (IsPrime(current)) {
      --nth;
    }
  }

  return current;
}
