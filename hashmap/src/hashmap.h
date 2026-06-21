#ifndef HASHMAP_H
#define HASHMAP_H

#include <stddef.h>

#define TOMBSTONE (Entry *)1

typedef struct {
  char *key;
  void *val;
} Entry;

typedef struct {
  Entry **entries;
  size_t capacity;
  size_t size;
  size_t val_size;
} HashMap;

Entry *hashmap_get_entry(const HashMap *hashmap, const char *key);
HashMap *hashmap_create(const size_t capacity, const size_t val_size);
int hashmap_contains(const HashMap *hashmap, const char *key);
int hashmap_put(HashMap *hashmap, const char *key, const void *val);
int hashmap_remove(HashMap *hashmap, const char *key);
int hashmap_resize(HashMap *hashmap, const size_t capacity_new);
size_t hashmap_get_hash(const char *key, const size_t m);
size_t hashmap_get_index(const HashMap *hashmap, const char *key);
size_t hashmap_size(const HashMap *hashmap);
void hashmap_destroy(HashMap *hashmap);

#endif
