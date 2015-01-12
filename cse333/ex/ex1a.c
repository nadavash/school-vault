// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 1a - Pass by value/reference
#include <stdio.h>
#include <stdlib.h>

// Modifues the vale of given double.
void ModifyDouble(double d);
// Modifies the value of the first three element of given array.
void ModifyArray(char arr[]);
// Returns string with value "pass-by-value" if isValue is true. Otherwise
// returns "pass-by-reference".
char* PassType(_Bool isValue);

void ModifyDouble(double d) {
  d = 50.0;
}

void ModifyArray(char arr[]) {
  arr[0] = 'c';
  arr[1] = 'b';
  arr[2] = 'a';
}

char* PassType(_Bool isValue) {
  if (isValue)
    return "pass-by-value";
  else
    return "pass-by-reference";
}

int main() {
  double d = 0;
  char arr[] = {'a', 'b', 'c'};

  ModifyDouble(d);
  ModifyArray(arr);

  printf("double: %s\n", PassType(d == 0));
  printf("array of chars: %s\n", PassType(arr[0] == 'a'));

  return EXIT_SUCCESS;
}
