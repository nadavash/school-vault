// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 11
#ifndef CSE333_EX_EX10_VECTOR_H_
#define CSE333_EX_EX10_VECTOR_H_

#include <iostream>

class Vector {
 public:
  // Creates a new 0-initialized vector.
  Vector();
  // Copy constructor performs a deep copy of other.
  Vector(const Vector& other);
  // Creates a new vector with the given x, y, and z values.
  Vector(double x, double y, double z);
  virtual ~Vector();
  // Sets the vectors x, y, and z components to the given values.
  void Set(double x, double y, double z);
  // Assignment operator performs a deep copy of the internal data.
  Vector& operator=(const Vector& other);

  // Getters for the x, y, and z fields of the vector.
  double x() const;
  double y() const;
  double z() const;

 private:
  double* data_;
};

// The addition operator adds the x, y, and z components of the given vectors
// and returns a new vector with the added components.
Vector operator+(const Vector& a, const Vector& b);
// The subtraction operator subtracts the x, y, and z components of vector a
// by the components of [b] and returns a new vector with the results.
Vector operator-(const Vector& a, const Vector& b);
// Returns the dot product of a dot b.
double operator*(const Vector& a, const Vector& b);
// Multiplies each components of vector a by k and returns a new vector with the
// results.
Vector operator*(const Vector& a, double k);
// Prints the vector v as "(x,y,z)" to the given stream.
std::ostream& operator<<(std::ostream& strm, const Vector& v);

#endif  // CSE333_EX_EX10_VECTOR_H_
