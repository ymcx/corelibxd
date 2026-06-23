#include "vector.h"
#include <stddef.h>
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

int vector_insert(Vector *vector, const size_t index, const void *entry_new) {
  if (vector->size < index) {
    return -1;
  }

  if (vector->capacity < vector->size + 1) {
    if (vector_resize(vector, vector->capacity * 2)) {
      return -1;
    }
  }

  void *curr;
  void *prev = (char *)vector->entries + vector->entry_size * vector->size;

  for (size_t i = index; i < vector->size; ++i) {
    curr = prev;
    prev = (char *)prev - vector->entry_size;

    memcpy(curr, prev, vector->entry_size);
  }

  memcpy(prev, entry_new, vector->entry_size);

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

int vector_pop_front(Vector *vector) { return vector_erase(vector, 0); }

int vector_push_back(Vector *vector, const void *entry_new) {
  return vector_insert(vector, vector->size, entry_new);
}

int vector_push_front(Vector *vector, const void *entry_new) {
  return vector_insert(vector, 0, entry_new);
}

int vector_resize(Vector *vector, const size_t capacity_new) {
  if (capacity_new < vector->size) {
    return -1;
  }

  void *entries_new = malloc(vector->entry_size * capacity_new);
  if (!entries_new) {
    return -1;
  }

  void *entry = vector->entries;
  void *entry_new = entries_new;

  for (size_t i = 0; i < vector->size; ++i) {
    memcpy(entry_new, entry, vector->entry_size);

    entry = (char *)entry + vector->entry_size;
    entry_new = (char *)entry_new + vector->entry_size;
  }

  free(vector->entries);
  vector->entries = entries_new;
  vector->capacity = capacity_new;

  return 0;
}

int vector_set(Vector *vector, const size_t index, const void *entry_new) {
  if (vector->size <= index) {
    return -1;
  }

  void *entry = (char *)vector->entries + vector->entry_size * index;
  memcpy(entry, entry_new, vector->entry_size);

  return 0;
}

size_t vector_capacity(const Vector *vector) { return vector->capacity; }

size_t vector_size(const Vector *vector) { return vector->size; }

void *vector_back(Vector *vector) {
  return vector_get(vector, vector->size - 1);
}

void *vector_front(Vector *vector) { return vector_get(vector, 0); }

void *vector_get(Vector *vector, const size_t index) {
  if (vector->size <= index) {
    return NULL;
  }

  return (char *)vector->entries + vector->entry_size * index;
}

void vector_clear(Vector *vector) { vector->size = 0; }

void vector_destroy(Vector *vector) {
  free(vector->entries);
  free(vector);
}
