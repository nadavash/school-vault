// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 12
#include <cstdlib>
#include <iostream>
#include <vector>

// Reads a value with the given type from istream and returns that value.
template<typename T> T ReadValue(std::istream &in);

template<typename T>
T ReadValue(std::istream &in) {
  T value;
  in >> value;

  if (in.fail()) {
    fprintf(stderr, "Could not read value from stdin.\n");
    exit(EXIT_FAILURE);
  }

  return value;
}

int main() {
  std::vector<double> vec;

  std::cout << "Input 10 doubles:" << std::endl;
  for (int i = 0; i < 10; ++i) {
    vec.push_back(ReadValue<double>(std::cin));
  }

  std::sort(vec.begin(), vec.end());

  std::cout << "Your sorted doubles are:" << std::endl;
  for (auto &val : vec) {
    std::cout << val << std::endl;
  }

  return EXIT_SUCCESS;
}
