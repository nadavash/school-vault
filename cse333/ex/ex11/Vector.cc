// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 11
#include "Vector.h"

Vector::Vector() : data_(data_ = new double[3]) {
  Set(0, 0, 0);
}

Vector::Vector(const Vector& other) : data_(data_ = new double[3]) {
  Set(other.x(), other.y(), other.z());
}

Vector::Vector(double x, double y, double z) : data_(data_ = new double[3]) {
  Set(x, y, z);
}

Vector::~Vector() {
  delete[] data_;
  data_ = NULL;
}

void Vector::Set(double x, double y, double z) {
  data_[0] = x;
  data_[1] = y;
  data_[2] = z;
}

double Vector::x() const {
  return data_[0];
}

double Vector::y() const {
  return data_[1];
}

double Vector::z() const {
  return data_[2];
}

Vector& Vector::operator=(const Vector& other) {
  if (this != &other) {
    Set(other.x(), other.y(), other.z());
  }

  return *this;
}

Vector operator +(const Vector& a, const Vector& b) {
  return Vector(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}

Vector operator -(const Vector& a, const Vector& b) {
  return Vector(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

double operator *(const Vector& a, const Vector& b) {
  return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}

Vector operator *(const Vector& a, double k) {
  return Vector(a.x() * k, a.y() * k, a.z() * k);
}

std::ostream& operator <<(std::ostream& st, const Vector& v) {
  st << "(" << v.x() << "," << v.y() << "," << v.z() << ")";
  return st;
}
