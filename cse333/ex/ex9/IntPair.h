// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 9
#ifndef CSE333_EX_EX9_INTPAIR_H_
#define CSE333_EX_EX9_INTPAIR_H_

// Represents a pair of two integers.
class IntPair {
 public:
  // Creates a new IntPair object with the given [first] and [second] value.
  IntPair(int first, int second);
  // Gets the first and second value of the IntPair.
  void Get(int *first, int *second);
  // Sets the first and second value of the IntPair to the given values.
  void Set(int first, int second);

 private:
  int first_, second_;
};

#endif  // CSE333_EX_EX9_INTPAIR_H_
