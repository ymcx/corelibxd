#include "hashmap.h"
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

Entry *hashmap_get_entry(const HashMap *hashmap, const char *key) {
  const size_t i = hashmap_get_index(hashmap, key);
  Entry *entry = hashmap->entries[i];

  return entry == TOMBSTONE ? NULL : entry;
}

HashMap *hashmap_create(const size_t capacity, const size_t val_size) {
  HashMap *hashmap = malloc(sizeof(HashMap));
  if (!hashmap) {
    return NULL;
  }

  const size_t entry_size = sizeof(Entry *);
  hashmap->capacity = capacity;
  hashmap->size = 0;
  hashmap->val_size = val_size;
  hashmap->entries = malloc(entry_size * capacity);
  if (!hashmap->entries) {
    free(hashmap);
    return NULL;
  }

  memset(hashmap->entries, 0, entry_size * capacity);

  return hashmap;
}

int hashmap_contains(const HashMap *hashmap, const char *key) {
  return hashmap_get_entry(hashmap, key) != NULL;
}

int hashmap_empty(const HashMap *hashmap) { return hashmap->size == 0; }

int hashmap_put(HashMap *hashmap, const char *key, const void *val) {
  if (hashmap->capacity < (hashmap->size + 1) * 2) {
    if (hashmap_resize(hashmap, hashmap->capacity * 2)) {
      return -1;
    }
  }

  Entry *entry_new = malloc(sizeof(Entry));
  if (!entry_new) {
    return -1;
  }

  const size_t key_size = strlen(key) + 1;
  entry_new->key = malloc(key_size);
  if (!entry_new->key) {
    free(entry_new);
    return -1;
  }

  entry_new->val = malloc(hashmap->val_size);
  if (!entry_new->val) {
    free(entry_new->key);
    free(entry_new);
    return -1;
  }

  memcpy(entry_new->key, key, key_size);
  memcpy(entry_new->val, val, hashmap->val_size);

  const size_t i = hashmap_get_index(hashmap, key);
  Entry *entry = hashmap->entries[i];

  if (entry == NULL || entry == TOMBSTONE) {
    hashmap->size += 1;
  } else {
    free(entry->key);
    free(entry->val);
    free(entry);
  }

  hashmap->entries[i] = entry_new;

  return 0;
}

int hashmap_remove(HashMap *hashmap, const char *key) {
  if (hashmap->size == 0) {
    return -1;
  }

  const size_t i = hashmap_get_index(hashmap, key);
  Entry *entry = hashmap->entries[i];
  if (entry == NULL || entry == TOMBSTONE) {
    return -1;
  }

  free(entry->key);
  free(entry->val);
  free(entry);

  hashmap->entries[i] = TOMBSTONE;
  hashmap->size -= 1;

  return 0;
}

int hashmap_resize(HashMap *hashmap, const size_t capacity_new) {
  if (capacity_new < hashmap->size) {
    return -1;
  }

  const size_t entry_size = sizeof(Entry *);
  Entry **entries_new = malloc(entry_size * capacity_new);
  if (!entries_new) {
    return -1;
  }

  memset(entries_new, 0, entry_size * capacity_new);

  for (size_t i = 0; i < hashmap->capacity; ++i) {
    Entry *entry = hashmap->entries[i];
    if (entry == NULL || entry == TOMBSTONE) {
      continue;
    }

    size_t i_new = hashmap_get_hash(entry->key, capacity_new);
    while (entries_new[i_new]) {
      i_new = (i_new + 1) % capacity_new;
    }

    entries_new[i_new] = entry;
  }

  free(hashmap->entries);
  hashmap->capacity = capacity_new;
  hashmap->entries = entries_new;

  return 0;
}

size_t hashmap_get_hash(const char *key, const size_t m) {
  const size_t key_size = strlen(key);
  const size_t p = 53;
  size_t hash = 0;

  for (size_t i = 0; i < key_size; ++i) {
    hash += key[i] * pow(p, i);
  }

  return hash % m;
}

size_t hashmap_get_index(const HashMap *hashmap, const char *key) {
  size_t i = hashmap_get_hash(key, hashmap->capacity);
  size_t i_tombstone = SIZE_MAX;
  Entry *entry = hashmap->entries[i];

  for (size_t j = 0; j < hashmap->capacity; ++j) {
    // The stream of possible matching entries has ended
    if (entry == NULL) {
      break;
    }

    // Keys match, current entry can be overridden
    if (entry != TOMBSTONE && !strcmp(entry->key, key)) {
      return i;
    }

    // Save the first tombstone encountered
    if (entry == TOMBSTONE && i_tombstone == SIZE_MAX) {
      i_tombstone = i;
    }

    i = (i + 1) % hashmap->capacity;
    entry = hashmap->entries[i];
  }

  // Return the first tombstone if one has been encountered
  // Otherwise return the index of the next available slot
  return i_tombstone != SIZE_MAX ? i_tombstone : i;
}

size_t hashmap_size(const HashMap *hashmap) { return hashmap->size; }

size_t hashmap_capacity(const HashMap *hashmap) { return hashmap->capacity; }

void *hashmap_get_value(const HashMap *hashmap, const char *key) {
  const Entry *entry = hashmap_get_entry(hashmap, key);

  return entry ? entry->val : NULL;
}

void hashmap_destroy(HashMap *hashmap) {
  for (size_t i = 0; i < hashmap->capacity; ++i) {
    Entry *entry = hashmap->entries[i];
    if (entry != NULL && entry != TOMBSTONE) {
      free(entry->key);
      free(entry->val);
      free(entry);
    }
  }

  free(hashmap);
}
