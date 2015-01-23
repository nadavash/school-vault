// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 6
#include <err.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** args) {
  if (argc < 2) {
    fprintf(stderr, "Usage: ./ex6 [filename]");
    return EXIT_FAILURE;
  }

  FILE* file = fopen(args[1], "rb");
  if (!file) {
    err(EXIT_FAILURE, "fopen");
    return EXIT_FAILURE;
  }

  fseek(file, -1, SEEK_END);

  char character;
  for (int64_t size = ftell(file); size >= 0; --size) {
    fread(&character, sizeof(character), 1, file);
    printf("%c", character);
    fseek(file, -2, SEEK_CUR);
  }

  fclose(file);

  return EXIT_SUCCESS;
}
