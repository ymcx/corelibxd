#include "vector.h"
#include <stdlib.h>
#include <string.h>

Vector *vector_create(const size_t capacity, const size_t entry_size) {
  Vector *vector = malloc(sizeof(Vector));
  if (!vector) {
    return NULL;
  }

  vector->capacity = capacity;
  vector->entry_size = entry_size;
  vector->size = 0;
  vector->entries = malloc(entry_size * capacity);
  if (!vector->entries) {
    free(vector);
    return NULL;
  }

  return vector;
}

int vector_empty(const Vector *vector) { return vector->size == 0; }

int vector_erase(Vector *vector, const size_t index) {
  if (vector->size <= index) {
    return -1;
  }

  void *curr;
  void *next = (char *)vector->entries + vector->entry_size * index;

  for (size_t i = index; i < vector->size - 1; ++i) {
    curr = next;
    next = (char *)next + vector->entry_size;

    memcpy(curr, next, vector->entry_size);
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
  void *prev = (char *)vector->entries + vector->entry_size * vector->size;

  for (size_t i = index; i < vector->size; ++i) {
    curr = prev;
    prev = (char *)prev - vector->entry_size;

    memcpy(curr, prev, vector->entry_size);
  }

  memcpy(prev, element, vector->entry_size);

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

  free(vector->entries);
  vector->entries = malloc(vector->capacity * vector->entry_size);

  return 0;
}

int vector_resize(Vector *vector, const size_t new_size) {
  if (new_size < vector->size) {
    return -1;
  }

  vector->capacity = new_size;

  void *data = malloc(vector->capacity * vector->entry_size);
  void *src = vector->entries;
  void *dst = data;

  for (size_t i = 1; i < vector->size; ++i) {
    memcpy(dst, src, vector->entry_size);

    dst = (char *)dst + vector->entry_size;
    src = (char *)src + vector->entry_size;
  }

  memcpy(dst, src, vector->entry_size);

  free(vector->entries);
  vector->entries = data;

  return 0;
}

int vector_set(const Vector *vector, const size_t index, const void *element) {
  if (vector->size <= index) {
    return -1;
  }

  void *item = (char *)vector->entries + vector->entry_size * index;
  memcpy(item, element, vector->entry_size);

  return 0;
}

size_t vector_capacity(const Vector *vector) { return vector->capacity; }

size_t vector_size(const Vector *vector) { return vector->size; }

void *vector_get(const Vector *vector, const size_t index) {
  if (vector->size <= index) {
    return NULL;
  }

  void *item = (char *)vector->entries + vector->entry_size * index;

  return item;
}

void vector_clear(Vector *vector) { vector->size = 0; }

void vector_destroy(Vector *vector) {
  free(vector->entries);
  free(vector);
}
