#include "test.h"
#include "hashmap.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test_get_hash(void) {
  assert(hashmap_get_hash("test123", 1024) == 98);
  assert(hashmap_get_hash("test124", 1024) == 11);
  assert(hashmap_get_hash("test125", 1024) == 948);
}

void test_create_empty_map(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  assert(hashmap_size(hashmap) == 0);
  assert(hashmap_empty(hashmap));

  hashmap_destroy(hashmap);
}

void test_insert_and_get(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *key = "answer";
  const int value = 42;
  assert(hashmap_put(hashmap, key, &value) == 0);

  const int *result = hashmap_get_value(hashmap, key);
  assert(result != NULL);
  assert(*result == 42);
  assert(hashmap_size(hashmap) == 1);
  assert(!hashmap_empty(hashmap));

  hashmap_destroy(hashmap);
}

void test_resize(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const int a = 0;
  const int b = 0;
  const int c = 0;
  const int d = 0;

  assert(hashmap_put(hashmap, "a", &a) == 0);
  assert(hashmap_put(hashmap, "b", &b) == 0);
  assert(hashmap_put(hashmap, "c", &c) == 0);
  assert(hashmap_put(hashmap, "d", &d) == 0);

  assert(hashmap_capacity(hashmap) == 16);
  assert(hashmap_size(hashmap) == 4);

  assert(hashmap_resize(hashmap, 32) == 0);
  assert(hashmap_capacity(hashmap) == 32);
  assert(hashmap_size(hashmap) == 4);

  assert(hashmap_resize(hashmap, 3) != 0);
  assert(hashmap_capacity(hashmap) == 32);
  assert(hashmap_size(hashmap) == 4);

  assert(*(int *)hashmap_get_value(hashmap, "a") == a);
  assert(*(int *)hashmap_get_value(hashmap, "b") == b);
  assert(*(int *)hashmap_get_value(hashmap, "c") == c);
  assert(*(int *)hashmap_get_value(hashmap, "d") == d);

  hashmap_destroy(hashmap);
}

void test_resize_grow(void) {
  HashMap *hashmap = hashmap_create(2, sizeof(int));
  assert(hashmap != NULL);

  const int a = 0;
  const int b = 0;
  const int c = 0;
  const int d = 0;

  const size_t capacity = hashmap_capacity(hashmap);

  assert(hashmap_put(hashmap, "a", &a) == 0);
  assert(hashmap_put(hashmap, "b", &b) == 0);
  assert(hashmap_put(hashmap, "c", &c) == 0);
  assert(hashmap_put(hashmap, "d", &d) == 0);

  assert(hashmap_capacity(hashmap) > capacity);
  assert(hashmap_size(hashmap) == 4);

  hashmap_destroy(hashmap);
}

void test_get_entry(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *a = "test";
  const char *b = "string";
  const char *c = "67";
  const char *d = "123";

  const int a_value = 0;
  const int b_value = 1;
  const int c_value = 2;
  const int d_value = 3;

  assert(hashmap_put(hashmap, a, &a_value) == 0);
  assert(hashmap_put(hashmap, b, &b_value) == 0);
  assert(hashmap_put(hashmap, c, &c_value) == 0);
  assert(hashmap_put(hashmap, d, &d_value) == 0);

  const Entry *a_entry = hashmap_get_entry(hashmap, a);
  const Entry *b_entry = hashmap_get_entry(hashmap, b);
  const Entry *c_entry = hashmap_get_entry(hashmap, c);
  const Entry *d_entry = hashmap_get_entry(hashmap, d);

  assert(*(int *)a_entry->val == a_value);
  assert(*(int *)b_entry->val == b_value);
  assert(*(int *)c_entry->val == c_value);
  assert(*(int *)d_entry->val == d_value);

  assert(!strcmp(a_entry->key, a));
  assert(!strcmp(b_entry->key, b));
  assert(!strcmp(c_entry->key, c));
  assert(!strcmp(d_entry->key, d));

  assert(hashmap_remove(hashmap, a) == 0);
  assert(hashmap_get_entry(hashmap, a) == NULL);

  hashmap_destroy(hashmap);
}

void test_get_index(void) {
  HashMap *hashmap = hashmap_create(4, sizeof(int));
  assert(hashmap != NULL);

  const char *a = "test";
  const char *b = "string";
  const char *c = "67";
  const char *d = "123";
  const char *e = "hashmap";
  const char *f = "get_index";
  const char *g = "42";
  const char *h = "aatahs";

  const int value = 0;

  assert(hashmap_put(hashmap, a, &value) == 0);
  assert(hashmap_put(hashmap, b, &value) == 0);
  assert(hashmap_put(hashmap, c, &value) == 0);
  assert(hashmap_put(hashmap, d, &value) == 0);

  assert(hashmap_get_index(hashmap, a) == 4);
  assert(hashmap_get_index(hashmap, b) == 7);
  assert(hashmap_get_index(hashmap, c) == 1);
  assert(hashmap_get_index(hashmap, d) == 6);

  assert(hashmap_remove(hashmap, a) == 0);
  assert(hashmap_remove(hashmap, b) == 0);

  assert(hashmap_get_index(hashmap, a) == 4);
  assert(hashmap_get_index(hashmap, b) == 7);

  assert(hashmap_put(hashmap, e, &value) == 0);
  assert(hashmap_put(hashmap, f, &value) == 0);
  assert(hashmap_put(hashmap, g, &value) == 0);
  assert(hashmap_put(hashmap, h, &value) == 0);

  assert(hashmap_get_index(hashmap, e) == 2);
  assert(hashmap_get_index(hashmap, f) == 3);
  assert(hashmap_get_index(hashmap, g) == 14);
  assert(hashmap_get_index(hashmap, h) == 15);

  assert(hashmap_remove(hashmap, c) == 0);
  assert(hashmap_remove(hashmap, d) == 0);
  assert(hashmap_remove(hashmap, e) == 0);
  assert(hashmap_remove(hashmap, f) == 0);
  assert(hashmap_remove(hashmap, g) == 0);
  assert(hashmap_remove(hashmap, h) == 0);

  assert(hashmap_get_index(hashmap, a) == 12);
  assert(hashmap_get_index(hashmap, b) == 15);
  assert(hashmap_get_index(hashmap, c) == 9);
  assert(hashmap_get_index(hashmap, d) == 6);
  assert(hashmap_get_index(hashmap, e) == 2);
  assert(hashmap_get_index(hashmap, f) == 3);
  assert(hashmap_get_index(hashmap, g) == 14);
  assert(hashmap_get_index(hashmap, h) == 14);

  assert(hashmap_put(hashmap, h, &value) == 0);
  assert(hashmap_put(hashmap, g, &value) == 0);
  assert(hashmap_put(hashmap, f, &value) == 0);
  assert(hashmap_put(hashmap, e, &value) == 0);
  assert(hashmap_put(hashmap, d, &value) == 0);
  assert(hashmap_put(hashmap, c, &value) == 0);
  assert(hashmap_put(hashmap, b, &value) == 0);
  assert(hashmap_put(hashmap, a, &value) == 0);

  assert(hashmap_get_index(hashmap, a) == 12);
  assert(hashmap_get_index(hashmap, b) == 0);
  assert(hashmap_get_index(hashmap, c) == 9);
  assert(hashmap_get_index(hashmap, d) == 6);
  assert(hashmap_get_index(hashmap, e) == 2);
  assert(hashmap_get_index(hashmap, f) == 3);
  assert(hashmap_get_index(hashmap, g) == 15);
  assert(hashmap_get_index(hashmap, h) == 14);

  hashmap_destroy(hashmap);
}

void test_get_missing_key(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  assert(hashmap_get_value(hashmap, "missing") == NULL);

  hashmap_destroy(hashmap);
}

void test_overwrite_existing_key(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *key = "key";
  const int a = 10;
  const int b = 20;

  assert(hashmap_put(hashmap, key, &a) == 0);
  const int *aa = hashmap_get_value(hashmap, key);

  assert(aa != NULL);
  assert(*aa == a);
  assert(hashmap_size(hashmap) == 1);
  assert(!hashmap_empty(hashmap));

  assert(hashmap_put(hashmap, key, &b) == 0);
  const int *bb = hashmap_get_value(hashmap, key);

  assert(bb != NULL);
  assert(*bb == b);
  assert(hashmap_size(hashmap) == 1);
  assert(!hashmap_empty(hashmap));

  hashmap_destroy(hashmap);
}

void test_remove_key(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *key = "key";
  const int value = 123;

  assert(!hashmap_contains(hashmap, key));

  assert(hashmap_put(hashmap, key, &value) == 0);
  assert(hashmap_contains(hashmap, key));

  assert(hashmap_remove(hashmap, key) == 0);
  assert(!hashmap_contains(hashmap, key));
  assert(hashmap_get_value(hashmap, key) == NULL);
  assert(hashmap_size(hashmap) == 0);

  hashmap_destroy(hashmap);
}

void test_remove_missing_key(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  assert(hashmap_remove(hashmap, "does_not_exist") != 0);

  hashmap_destroy(hashmap);
}

void test_many_insertions(void) {
  HashMap *hashmap = hashmap_create(256, sizeof(int));
  assert(hashmap != NULL);

  for (int i = 0; i < 64; ++i) {
    char key[32];
    snprintf(key, sizeof(key), "key_%d", i);

    assert(hashmap_put(hashmap, key, &i) == 0);
    assert(hashmap_size(hashmap) == (size_t)i + 1);
  }

  for (int i = 0; i < 64; ++i) {
    char key[32];
    snprintf(key, sizeof(key), "key_%d", i);

    const int *result = hashmap_get_value(hashmap, key);
    assert(result != NULL);
    assert(*result == i);
  }

  hashmap_destroy(hashmap);
}

void test_collision_handling(void) {
  HashMap *hashmap = hashmap_create(2, sizeof(int));
  assert(hashmap != NULL);

  const int a = 1;
  const int b = 2;
  const int c = 3;

  assert(hashmap_put(hashmap, "aa", &a) == 0);
  assert(hashmap_put(hashmap, "bb", &b) == 0);
  assert(hashmap_put(hashmap, "cc", &c) == 0);

  assert(*(int *)hashmap_get_value(hashmap, "aa") == a);
  assert(*(int *)hashmap_get_value(hashmap, "bb") == b);
  assert(*(int *)hashmap_get_value(hashmap, "cc") == c);

  hashmap_destroy(hashmap);
}

void test_contains(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *key = "item";
  const int value = 7;

  assert(!hashmap_contains(hashmap, key));

  assert(hashmap_put(hashmap, key, &value) == 0);
  assert(hashmap_contains(hashmap, key));

  hashmap_destroy(hashmap);
}

void test_empty_string_key(void) {
  HashMap *hashmap = hashmap_create(16, sizeof(int));
  assert(hashmap != NULL);

  const char *key = "";
  const int value = 55;

  assert(hashmap_put(hashmap, key, &value) == 0);

  const int *result = hashmap_get_value(hashmap, key);
  assert(result != NULL);
  assert(*result == value);

  hashmap_destroy(hashmap);
}

void test(void) {
  RUN(test_get_hash);
  RUN(test_create_empty_map);
  RUN(test_insert_and_get);
  RUN(test_resize);
  RUN(test_resize_grow);
  RUN(test_get_entry);
  RUN(test_get_index);
  RUN(test_get_missing_key);
  RUN(test_overwrite_existing_key);
  RUN(test_remove_key);
  RUN(test_remove_missing_key);
  RUN(test_many_insertions);
  RUN(test_collision_handling);
  RUN(test_contains);
  RUN(test_empty_string_key);

  printf("All tests passed\n");
}
