// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 2 - DumpHex
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

void DumpHex(void *pData, int byteLen);

void DumpHex(void *pData, int byteLen) {
  printf("The %d bytes starting at %p are: ", byteLen, pData);

  for (int i = 0; i < byteLen; ++i) {
    uint8_t byte = *((uint8_t*)pData + i);
    printf("%02" PRIx8 " ", byte);
  }
  printf("\n");
}

int main(int argc, char **argv) {
  char     charVal = '0';
  int32_t  intVal = 1;
  float    floatVal = 1.0;
  double   doubleVal = 1.0;

  typedef struct {
    char     charVal;
    int32_t  intVal;
    float    floatVal;
    double   doubleVal;
  } __attribute__((__packed__)) Ex2Struct;
  Ex2Struct structVal = { '0', 1, 1.0, 1.0 };

  DumpHex(&charVal, sizeof(char));
  DumpHex(&intVal, sizeof(int32_t));
  DumpHex(&floatVal, sizeof(float));
  DumpHex(&doubleVal, sizeof(double));
  DumpHex(&structVal, sizeof(structVal));

  return EXIT_SUCCESS;
}
