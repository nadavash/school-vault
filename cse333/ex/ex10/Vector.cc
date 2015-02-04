// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 10
#include "Vector.h"

Vector::Vector() {
  Set(0, 0, 0);
}

Vector::Vector(double x, double y, double z) {
  Set(x, y, z);
}

void Vector::Set(double x, double y, double z) {
  this->x = x;
  this->y = y;
  this->z = z;
}

Vector operator +(const Vector& a, const Vector& b) {
  return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vector operator -(const Vector& a, const Vector& b) {
  return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector operator *(const Vector& a, double k) {
  return Vector(a.x * k, a.y * k, a.z * k);
}

std::ostream& operator <<(std::ostream& st, const Vector& v) {
  st << "(" << v.x << "," << v.y << "," << v.z << ")";
  return st;
}
