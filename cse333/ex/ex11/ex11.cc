// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 11
#include <iostream>
#include <cstdlib>
#include "Vector.h"

int main() {
  Vector v1(1.5, 2.5, 3.5);
  Vector v2(1, 2, 3);
  Vector v3(v1);
  Vector v4 = v2;
  Vector v5;
  v5 = v4;

  std::cout << "Vector v1 = " << v1 << std::endl;
  std::cout << "Vector v2 = " << v2 << std::endl;
  std::cout << "Vector v3(v1); v3 = " << v3 << std::endl;
  std::cout << "Vector v4 = v2; v4 = " << v4 << std::endl;
  std::cout << "Vector v5;" << std::endl;
  std::cout << "v5 = v4; v5 = " << v5 << std::endl << std::endl;

  std::cout << "v1 + v2 = " << v1 + v2 << std::endl;
  std::cout << "v1 - v2 = " << v1 - v2 << std::endl;
  std::cout << "v2 - v1 = " << v2 - v1 << std::endl << std::endl;

  std::cout << "v1 * v2 = " << v1 * v2 << std::endl;
  std::cout << "v1 * 2 = " << v1 * 2 << std::endl;
  std::cout << "v2 * -3 = " << v2 * -3 << std::endl;

  return EXIT_SUCCESS;
}