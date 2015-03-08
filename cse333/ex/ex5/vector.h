// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 5
#ifndef EX5_VECTOR_H_
#define EX5_VECTOR_H_

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// An arbitrary pointer to represent an element in the vector.
typedef void* element_t;

// An expandable array of element_ts.
typedef struct vector_t {
  size_t length;
  element_t *arry;
} *vector_t;

// On success, return vector_t with an initial length of n.
// Initializes all of the elements to NULL.
// On failure, returns NULL.  Assumes n > 0.
vector_t VectorCreate(size_t n);

// Frees the memory allocated for the vector_t.  Assumes v != NULL.
void VectorFree(vector_t v);

// Sets the nth element of v to the value pointed to by e. Returns the
// previous nth element_t in prev.
// The caller owns the data pointed to by prev and is responsible for
// freeing it.
// Returns true iff successful.  Assumes v != NULL.
bool VectorSet(vector_t v, uint32_t index, element_t e, element_t *prev);

// Returns the element at the given index within v.  Assumes v != NULL.
// And 0 <= index < length of v.
element_t VectorGet(vector_t v, uint32_t index);

// Returns the length of the vector v.  Assumes v != NULL.
size_t VectorLength(vector_t v);

#endif  // EX5_VECTOR_H_
