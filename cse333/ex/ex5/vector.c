// Copyright 2015 Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 5
#include "vector.h"

#include <assert.h>

// Returns a copy of arry with new length newLen.  If newLen < oldLen
// then the returned array is clipped.  If newLen > oldLen, then the
// resulting array will be padded with NULL elements.
static element_t *ResizeArray(element_t *arry, size_t oldLen, size_t newLen);

vector_t VectorCreate(size_t n) {
  assert(n > 0);
  vector_t v = (vector_t)malloc(sizeof(struct vector_t));
  v->length = n;
  if (!v)
    return NULL;

  v->arry = (element_t*)malloc(n*sizeof(element_t));
  if (!v->arry)
    return NULL;

  for (int i = 0; i < n; ++i) {
    v->arry[i] = NULL;
  }

  return v;
}

void VectorFree(vector_t v) {
  assert(v != NULL);
  free(v->arry);
  free(v);
}

bool VectorSet(vector_t v, uint32_t index, element_t e, element_t *prev) {
  assert(v != NULL);

  if (index >= v->length) {
    size_t newLength = index+1;

    v->arry = ResizeArray(v->arry, v->length, newLength);
    v->length = newLength;
  } else {
    prev = v->arry[index];
  }

  v->arry[index] = e;

  return true;
}

element_t VectorGet(vector_t v, uint32_t index) {
  assert(v != NULL);
  assert(index >= 0);
  assert(index <= VectorLength(v));
  return v->arry[index];
}

size_t VectorLength(vector_t v) {
  assert(v != NULL);
  return v->length;
}

static element_t *ResizeArray(element_t *arry, size_t oldLen, size_t newLen) {
  uint32_t i;
  size_t copyLen = oldLen > newLen ? newLen : oldLen;
  element_t *newArry;

  assert(arry != NULL);

  newArry = (element_t*)malloc(newLen*sizeof(element_t));

  if (newArry == NULL)
    return NULL;  // malloc error!!!

  // Copy elements to new array
  for (i = 0; i < copyLen; ++i)
    newArry[i] = arry[i];

  // Null initialize rest of new array.
  for (i = copyLen; i < newLen; ++i)
    newArry[i] = NULL;

  free(arry);

  return newArry;
}
