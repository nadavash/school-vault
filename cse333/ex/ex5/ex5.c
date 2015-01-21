// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 5
#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "vector.h"

// Print the elements in the vector on a line.
static void PrintIntVector(vector_t v);

#define N 10 // Test vector length.
int main(int argc, char *argv[]) {
  uint32_t i;
  vector_t v = VectorCreate(N);

  if (!v)
    return EXIT_FAILURE;

  for (i = 0; i < N; ++i) { // Place some elements in the vector.
    int *x = (int*)malloc(sizeof(int));
    element_t old;
    VectorSet(v, i, x, &old);
  }

  PrintIntVector(v);

  return EXIT_SUCCESS;
}


static void PrintIntVector(vector_t v) {
  uint32_t i;
  size_t length;

  assert(v != NULL);

  length = VectorLength(v);

  if (length > 0) {
    printf("[%d", *((int*)VectorGet(v, 0)));
    for (i = 1; i < VectorLength(v); ++i)
      printf(",%d", *((int*)VectorGet(v, i)));
    printf("]\n");
  }
}
