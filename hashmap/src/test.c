#include "test.h"
#include "hashmap.h"
#include <assert.h>
#include <stdio.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test_create_empty_map(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  assert(map != NULL);
  assert(hashmap_size(map) == 0);

  hashmap_destroy(map);
}

void test_insert_and_get(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  int value = 42;

  assert(hashmap_put(map, "answer", &value) == 0);

  int *result = hashmap_get_entry(map, "answer")->val;

  assert(result != NULL);
  assert(*result == 42);
  assert(hashmap_size(map) == 1);

  hashmap_destroy(map);
}

void test_get_missing_key(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  assert(hashmap_get_entry(map, "missing") == NULL);

  hashmap_destroy(map);
}

void test_overwrite_existing_key(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  int v1 = 10;
  int v2 = 20;

  assert(hashmap_put(map, "key", &v1) == 0);
  assert(hashmap_put(map, "key", &v2) == 0);

  int *result = hashmap_get_entry(map, "key")->val;

  assert(result != NULL);
  assert(*result == 20);
  assert(hashmap_size(map) == 1);

  hashmap_destroy(map);
}

void test_remove_key(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  int value = 123;

  hashmap_put(map, "test", &value);

  assert(hashmap_contains(map, "test"));
  assert(hashmap_remove(map, "test") == 0);

  assert(!hashmap_contains(map, "test"));
  assert(hashmap_get_entry(map, "test") == NULL);
  assert(hashmap_size(map) == 0);

  hashmap_destroy(map);
}

void test_remove_missing_key(void) {
  HashMap *map = hashmap_create(16, sizeof(int));

  assert(hashmap_remove(map, "does_not_exist") != 0);

  hashmap_destroy(map);
}

void test_many_insertions(void) {
  HashMap *map = hashmap_create(300, sizeof(int));

  for (int i = 0; i < 100; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%d", i);

    assert(hashmap_put(map, key, &i) == 0);
  }

  printf("%ld\n",hashmap_size(map));
  assert(hashmap_size(map) == 100);

  for (int i = 0; i < 100; i++) {
    char key[32];
    snprintf(key, sizeof(key), "key_%d", i);

    int *result = hashmap_get_entry(map, key)->val;

    assert(result != NULL);
    assert(*result == i);
  }

  hashmap_destroy(map);
}

void test_collision_handling(void) {
  HashMap *map = hashmap_create(2,sizeof(int));

  int a = 1;
  int b = 2;
  int c = 3;

  hashmap_put(map, "aa", &a);
  hashmap_put(map, "bb", &b);
  hashmap_put(map, "cc", &c);

  assert(*(int *)hashmap_get_entry(map, "aa")->val == 1);
  assert(*(int *)hashmap_get_entry(map, "bb")->val == 2);
  assert(*(int *)hashmap_get_entry(map, "cc")->val == 3);

  hashmap_destroy(map);
}

void test_contains(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  int value = 7;

  assert(!hashmap_contains(map, "item"));

  hashmap_put(map, "item", &value);

  assert(hashmap_contains(map, "item"));

  hashmap_destroy(map);
}

void test_empty_string_key(void) {
  HashMap *map = hashmap_create(16,sizeof(int));

  int value = 55;

  hashmap_put(map, "", &value);

  int *result = hashmap_get_entry(map, "")->val;

  assert(result != NULL);
  assert(*result == 55);

  hashmap_destroy(map);
}

void test(void) {
  RUN(test_create_empty_map);
  RUN(test_insert_and_get);
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
