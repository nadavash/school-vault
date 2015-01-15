#include <stdio.h>
#include <stdlib.h>

typedef int T;

struct vector {
  size_t size;
  T* data;
};

struct vector* vector_alloc(size_t n, T initial_val);
void vector_free(struct vector* s);

struct vector* vector_alloc(size_t n, T initial_val) {
  struct vector* s = malloc(sizeof(struct vector));
  if (!s) {
    return NULL;
  }

  s->size = n;
  s->data = malloc(n * sizeof(T));

  if (!s->data) {
    free(s);
    return NULL;
  }

  for (int i = 0; i < n; ++i) {
    s->data[i] = initial_val;
  }

  return s;
}

void vector_free(struct vector* s) {
  free(s->data);
  free(s);
}

int main() {
  struct vector* s = vector_alloc(12, 10);

  printf("%p %d\n", &s->size, sizeof(size_t));
  for (int i = 0; i < s->size; ++i) {
    printf("%p %d\n", s->data + i, s->data[i]);
  }

  vector_free(s);
  return EXIT_SUCCESS;
}