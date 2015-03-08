// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 9
#include <iostream>
#include <cstdlib>
#include "IntPair.h"

// Increments both integers of the given pair by 1.
void Test(IntPair pair);

void Test(IntPair pair) {
  int first, second;
  pair.Get(&first, &second);
  pair.Set(first + 1, second + 1);
}

int main() {
  int first, second;
  IntPair pair(0, 0);

  Test(pair);
  pair.Get(&first, &second);

  if (first == 0 && second == 0) {
    std::cout << "Is pass by value";
  } else {
    std::cout << "Is pass by reference";
  }

  std::cout << std::endl;

  return EXIT_SUCCESS;
}
