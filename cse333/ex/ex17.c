/*
 * Copyright Nadav Ashkenazi, nadava@uw.edu - 1230523
 *
 *  This file is part of the UW CSE 333 course exercises (333exer).
 *
 *  333exer is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  333exer is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with 333exer.  If not, see <http://www.gnu.org/licenses/>.
 */

// CSE 333 Exercise 17 starter code.

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <pthread.h>

#define ARRAY_SIZE 200000

typedef struct sum_params {
  int32_t *a;
  uint32_t lo;
  uint32_t hi;
}SumParams;

// return the sum a[lo]+a[lo+1]+ ... + a[hi]
// pre: 0 <= lo && hi < length of a. (0 is returned if lo > hi)
int64_t ArraySum(int32_t a[], uint32_t lo, uint32_t hi) {
  int64_t ans = 0;
  for (int k = lo; k <= hi; k++) {
    ans += a[k];
  }
  return ans;
}

static void *HandleSum(void *arg) {
  SumParams *params = (SumParams*)arg;
  int64_t sum = ArraySum(params->a, params->lo, params->hi);
  int64_t* sumPtr = malloc(sizeof(sum));
  *sumPtr = sum;
  free(arg);
  return sumPtr;
}

static int64_t ArraySumThreaded(int32_t a[], int count, int32_t numThreads) {
  return 0;
}

// Store integers 1 to ARRAY_SIZE in an array then calculate
// and print their sum
int main() {
  int32_t *nums = (int32_t *)malloc(ARRAY_SIZE*sizeof(int32_t));
  if (nums == NULL) {
    printf("unable to allocate array\n");
    exit(EXIT_FAILURE);
  }

  for (int k = 0; k < ARRAY_SIZE; k++) {
    nums[k] = k+1;
  }

  int64_t sum = ArraySumThreaded(nums, ARRAY_SIZE, 4);

  printf("array sum = %" PRId64 "\n", sum);

  free(nums);
  return EXIT_SUCCESS;
}
