// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 1b - Pass by value/reference
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#define __STDC_FORMAT_MACROS
#include <inttypes.h>

#define ARR_SIZE 11

// Copies all of the elements from [src] up to [len] and places them in sorted
// order within [dst].
void CopyAndSort(const uint64_t* src, uint64_t* dst, int len);
// Inserts a single value [val] into the specified array while preserving a
// sorted order for [arr]. [lastIndex] specifies the last valid index within
// the sorted array.
void InsertSorted(uint64_t val, uint64_t* arr, int lastIndex);
// Prints all of the elements from the given array [arr], with a length [len].
void PrintArray(const uint64_t* arr, int len);

void CopyAndSort(const uint64_t* src, uint64_t* dst, int len) {
  for (int i = 0; i < len; ++i) {
    InsertSorted(src[i], dst, i);
  }
}

void InsertSorted(uint64_t val, uint64_t* arr, int lastIndex) {
  int currIndex = lastIndex;
  while (currIndex > 0 && arr[currIndex - 1] > val) {
    arr[currIndex] = arr[currIndex - 1];
    --currIndex;
  }
  arr[currIndex] = val;
}

void PrintArray(const uint64_t* arr, int len) {
  for (int i = 0; i < len; ++i) {
    printf("%" PRIu64 " ", arr[i]);
  }
  printf("\n");
}

int main() {
  uint64_t srcArr[] = {3, 2, 5, 7, 10, 4, 1, 7, 9, 8, 6};
  uint64_t dstArr[ARR_SIZE];

  CopyAndSort(srcArr, dstArr, ARR_SIZE);
  PrintArray(dstArr, ARR_SIZE);

  return EXIT_SUCCESS;
}
