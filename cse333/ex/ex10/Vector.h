// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 10
#ifndef CSE333_EX_EX10_VECTOR_H_
#define CSE333_EX_EX10_VECTOR_H_

#include <iostream>

class Vector {
 public:
  // Creates a new 0-initialized vector.
  Vector();
  // Creates a new vector with the given x, y, and z values.
  Vector(double x, double y, double z);
  // Sets the vectors x, y, and z components to the given values.
  void Set(double x, double y, double z);

  double x, y, z;
};

// The addition operator adds the x, y, and z components of the given vectors
// and returns a new vector with the added components.
Vector operator +(const Vector& a, const Vector& b);
// The subtraction operator subtracts the x, y, and z components of vector a
// by the components of [b] and returns a new vector with the results.
Vector operator -(const Vector& a, const Vector& b);
// Multiplies each components of vector a by k and returns a new vector with the
// results.
Vector operator *(const Vector& a, double k);
// Prints the vector v as "(x,y,z)" to the given stream.
std::ostream& operator <<(std::ostream& strm, const Vector& v);

#endif  // CSE333_EX_EX10_VECTOR_H_
