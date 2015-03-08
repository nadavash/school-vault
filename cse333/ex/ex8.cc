// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 8
#include <cstdlib>
#include <iostream>

// Print all of the factors of the given integer to stdout.
void Factorize(int num);

void Factorize(int num) {
  int midpoint = num / 2;
  for (int i = 1; i <= midpoint; ++i) {
    if (num % i == 0) {
      std::cout << i << " ";
    }
  }
  std::cout << num << std::endl;
}

int main() {
  int number;

  std::cout << "Which positive integer would you like me to factorize? ";
  std::cin >> number;

  Factorize(number);

  return EXIT_SUCCESS;
}
