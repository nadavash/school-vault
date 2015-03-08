#include <stdlib.h>
int main(int argc, char **argv) {
  if (argc != 2) return -1;
  int idx = atoi(argv[1]);
  char *a = malloc(100);
  char *b = malloc(1000);
  a[idx] = 0; // BUG: if idx < 0 or idx >= 100
  free(a);
  free(b);
  return 0;
}
