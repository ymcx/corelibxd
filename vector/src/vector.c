#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_CAPACITY 128

Vector *vector_create(const size_t element_size) {
  Vector *vector = malloc(sizeof(Vector));

  vector->capacity = DEFAULT_CAPACITY;
  vector->element_size = element_size;
  vector->data = malloc(vector->capacity * vector->element_size);
  vector->size = 0;

  return vector;
}

int vector_empty(const Vector *vector) { return vector->size == 0; }

int vector_erase(Vector *vector, const size_t index) {
  if (vector->size <= index) {
    return -1;
  }

  void *curr;
  void *next = (char *)vector->data + vector->element_size * index;

  for (size_t i = index; i < vector->size - 1; ++i) {
    curr = next;
    next = (char *)next + vector->element_size;

    memcpy(curr, next, vector->element_size);
  }

  vector->size -= 1;

  return 0;
}

int vector_insert(Vector *vector, const size_t index, const void *element) {
  if (vector->size < index) {
    return -1;
  }

  if (vector->capacity < (vector->size + 1) * 2) {
    vector_resize(vector, vector->capacity * 2);
  }

  void *curr;
  void *prev = (char *)vector->data + vector->element_size * vector->size;

  for (size_t i = index; i < vector->size; ++i) {
    curr = prev;
    prev = (char *)prev - vector->element_size;

    memcpy(curr, prev, vector->element_size);
  }

  memcpy(prev, element, vector->element_size);

  vector->size += 1;

  return 0;
}

int vector_pop_back(Vector *vector) {
  if (vector->size == 0) {
    return -1;
  }

  vector->size -= 1;

  return 0;
}

int vector_push_back(Vector *vector, const void *element) {
  return vector_insert(vector, vector->size, element);
}

int vector_reserve(Vector *vector, const size_t capacity) {
  if (vector->size != 0) {
    return -1;
  }

  vector->capacity = capacity;

  free(vector->data);
  vector->data = malloc(vector->capacity * vector->element_size);

  return 0;
}

int vector_resize(Vector *vector, const size_t new_size) {
  if (new_size < vector->size) {
    return -1;
  }

  vector->capacity = new_size;

  void *data = malloc(vector->capacity * vector->element_size);
  void *src = vector->data;
  void *dst = data;

  for (size_t i = 1; i < vector->size; ++i) {
    memcpy(dst, src, vector->element_size);

    dst = (char *)dst + vector->element_size;
    src = (char *)src + vector->element_size;
  }

  memcpy(dst, src, vector->element_size);

  free(vector->data);
  vector->data = data;

  return 0;
}

int vector_set(const Vector *vector, const size_t index, const void *element) {
  if (vector->size <= index) {
    return -1;
  }

  void *item = (char *)vector->data + vector->element_size * index;
  memcpy(item, element, vector->element_size);

  return 0;
}

size_t vector_capacity(const Vector *vector) { return vector->capacity; }

size_t vector_size(const Vector *vector) { return vector->size; }

void *vector_get(const Vector *vector, const size_t index) {
  if (vector->size <= index) {
    return NULL;
  }

  void *item = (char *)vector->data + vector->element_size * index;

  return item;
}

void vector_clear(Vector *vector) { vector->size = 0; }

void vector_destroy(Vector *vector) {
  free(vector->data);
  free(vector);
}
