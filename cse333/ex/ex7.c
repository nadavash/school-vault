// Copyright 2015, Nadav Ashkenazi
// nadava@uw.edu - 1230523
// CSE 333 15wi
// Exercise 7
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <unistd.h>

// The filetype to filter files on.
#define FILETYPE ".txt"

// Checks if the given [filename] ends with the .txt extension. Returns true if
// file ends with .txt, and false othewise.
bool EndsWithTxt(char *filename);

// Loads the file given by [filename] and prints it to the stdout if successful.
void PrintFile(char *filename);

bool EndsWithTxt(char *filename) {
  size_t length = strlen(filename);
  if (filename == NULL || length < 4) {
    return false;
  }

  if (filename[length - 4] == FILETYPE[0] &&
      filename[length - 3] == FILETYPE[1] &&
      filename[length - 2] == FILETYPE[2] &&
      filename[length - 1] == FILETYPE[3]) {
    return true;
  }

  return false;
}

void PrintFile(char *filename) {
  FILE *file = fopen(filename, "rb");
  if (!file) {
    fprintf(stderr, "Failed to open file '%s'.\n", filename);
    return;
  }

  // Get the size of the file.
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);

  // Read the entire file and print it to stdout.
  char *buff = malloc(size * sizeof(*buff));
  if (!fread(buff, sizeof(*buff), size, file)) {
    fprintf(stderr, "Failed to read file '%s'.\n", filename);
    return;
  }

  fwrite(buff, sizeof(*buff), size, stdout);

  free(buff);
  fclose(file);
}

int main(int argc, char **args) {
  if (argc != 2) {
    fprintf(stderr, "Usage: ./ex7 [directory name]\n");
    return EXIT_FAILURE;
  }

  // Open the directory
  DIR *dir = opendir(args[1]);
  if (!dir) {
    fprintf(stderr, "Could not open the specified directory '%s'.\n", args[1]);
    return EXIT_FAILURE;
  }
  // Change our working directory to the specified directory to load files
  // correctly.
  chdir(args[1]);

  struct dirent *dent;
  while ((dent = readdir(dir)) != NULL) {
    if (EndsWithTxt(dent->d_name)) {
      PrintFile(dent->d_name);
    }
  }
  closedir(dir);

  return EXIT_SUCCESS;
}
