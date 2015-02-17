/*
 * Copyright 2012, 2015 Nadav Ashkenazi, 1230523 - nadava@uw
 *
 *  This file is part of the UW CSE 333 course project exercises.
 *  (333exer).
 *
 *  333exer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  333exer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with 333proj.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <vector>
#include <memory>

int main() {
  // Allocate an integer on the heap, initialize to value 5.
  std::unique_ptr<int> x(new int(5));
  std::cout << "*x is: " << *x << std::endl;

  // Allocate a vector of integers on the heap, add some values to
  // that vector, sort the vector, print the values.
  std::unique_ptr<std::vector<int>> v(new std::vector<int>);
  v->push_back(5);
  v->push_back(9);
  v->push_back(7);
  std::sort(v->begin(), v->end());
  std::cout << "sorted v: ";
  for (auto el : *v) {
    std::cout << el << " ";
  }
  std::cout << std::endl;

  // Allocate a vector of (integer pointers) on the stack, add some
  // values to the vector from the heap, print the values.
  std::vector<std::unique_ptr<int>> v2;
  std::unique_ptr<int> a(new int(5));
  v2.push_back(std::move(a));
  std::unique_ptr<int> b(new int(9));
  v2.push_back(std::move(b));
  std::unique_ptr<int> c(new int(7));
  v2.push_back(std::move(c));
  std::cout << "unsorted v2: ";
  for (auto& el : v2) {
    std::cout << *el << " ";
  }
  std::cout << std::endl;

  return EXIT_SUCCESS;
}