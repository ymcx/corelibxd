#ifndef VECTOR_H
#define VECTOR_H

#include <stddef.h>

typedef struct {
  size_t capacity;
  size_t entry_size;
  size_t size;
  void *entries;
} Vector;

Vector *vector_create(const size_t capacity, const size_t element_size);
int vector_empty(const Vector *vector);
int vector_erase(Vector *vector, const size_t index);
int vector_insert(Vector *vector, const size_t index, const void *element);
int vector_pop_back(Vector *vector);
int vector_push_back(Vector *vector, const void *element);
int vector_reserve(Vector *vector, const size_t capacity);
int vector_resize(Vector *vector, const size_t new_size);
int vector_set(const Vector *vector, const size_t index, const void *element);
size_t vector_capacity(const Vector *vector);
size_t vector_size(const Vector *vector);
void *vector_get(const Vector *vector, const size_t index);
void vector_clear(Vector *vector);
void vector_destroy(Vector *vector);

#endif
