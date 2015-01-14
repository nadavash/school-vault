// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 3
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <memory.h>

// Defines a point in 3d space with x, y, and z coordinates.
typedef struct {
	uint16_t x, y, z;
} Point3d;

Point3d* AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z);
void PrintPoint3d(Point3d* point);

Point3d* AllocatePoint3d(uint16_t x, uint16_t y, uint16_t z) {
  Point3d* point = (Point3d*)malloc(sizeof(Point3d));
  if (!point) {
    return NULL;
  }

  point->x = x;
  point->y = y;
  point->z = z;

  return point;
}

void PrintPoint3d(Point3d* point) {
  printf("(%" PRIu16 ", %" PRIu16 ", %" PRIu16 ")\n",
         point->x, point->y, point->z);
}

int main(void) {
  Point3d* point1 = AllocatePoint3d(1, 2, 3);
  Point3d* point2 = AllocatePoint3d(0, 0, 0);
  Point3d* point3 = AllocatePoint3d(3, 2, 1);

  if (point1) {
    printf("Expected: (1, 2, 3)\nActual:   ");
    PrintPoint3d(point1);
    free(point1);
  }

  if (point2) {
    printf("Expected: (0, 0, 0)\nActual:   ");
    PrintPoint3d(point2);
    free(point2);
  }

  if (point3) {
    printf("Expected: (3, 2, 1)\nActual:   ");
    PrintPoint3d(point3);
    free(point3);
  }

  return EXIT_SUCCESS;
}