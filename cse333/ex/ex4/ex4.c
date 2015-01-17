// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 4
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "GetPrime.h"

int main() {
  // Test out GetPrime function.
  if (GetPrime(0) != 1) {
    fprintf(stderr, "GetPrime(0) == 1 check failed.\n");
  }

  if (GetPrime(1) != 2) {
    fprintf(stderr, "GetPrime(1) == 2 check failed.\n");
  }

  if (GetPrime(2) != 3) {
    fprintf(stderr, "GetPrime(2) == 3 check failed.\n");
  }

  if (GetPrime(3) != 5) {
    fprintf(stderr, "GetPrime(2) == 3 check failed.\n");
  }

  if (GetPrime(4) != 7) {
    fprintf(stderr, "GetPrime(2) == 3 check failed.\n");
  }

  return EXIT_SUCCESS;
}
