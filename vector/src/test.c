#include "test.h"
#include "vector.h"
#include <assert.h>
#include <stdio.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test_create(void) {
  Vector *v = vector_create(sizeof(int));

  assert(v != NULL);
  assert(vector_size(v) == 0);
  assert(vector_empty(v) == 1);

  vector_destroy(v);
}

void test_push_back(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 100; i++) {
    assert(vector_push_back(v, &i) == 0);
  }

  assert(vector_size(v) == 100);

  for (int i = 0; i < 100; i++) {
    assert(*(int *)vector_get(v, i) == i);
  }

  vector_destroy(v);
}

void test_capacity_growth(void) {
  Vector *v = vector_create(sizeof(int));

  size_t initial = vector_capacity(v);

  for (int i = 0; i < 1000; i++) {
    vector_push_back(v, &i);
  }

  assert(vector_capacity(v) > initial);
  assert(vector_capacity(v) >= vector_size(v));

  vector_destroy(v);
}

void test_pop_back(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 10; i++) {
    vector_push_back(v, &i);
  }

  assert(vector_pop_back(v) == 0);

  assert(vector_size(v) == 9);
  assert(*(int *)vector_get(v, 8) == 8);

  vector_destroy(v);
}

void test_pop_empty(void) {
  Vector *v = vector_create(sizeof(int));

  assert(vector_pop_back(v) != 0);

  vector_destroy(v);
}

void test_get(void) {
  Vector *v = vector_create(sizeof(int));

  int value = 123;

  vector_push_back(v, &value);

  assert(*(int *)vector_get(v, 0) == 123);

  vector_destroy(v);
}

void test_get_oob(void) {
  Vector *v = vector_create(sizeof(int));

  assert(vector_get(v, 0) == NULL);
  assert(vector_get(v, 99) == NULL);

  vector_destroy(v);
}

void test_set(void) {
  Vector *v = vector_create(sizeof(int));

  int a = 1;
  int b = 99;

  vector_push_back(v, &a);

  assert(vector_set(v, 0, &b) == 0);

  assert(*(int *)vector_get(v, 0) == 99);

  vector_destroy(v);
}

void test_set_oob(void) {
  Vector *v = vector_create(sizeof(int));

  int x = 42;

  assert(vector_set(v, 0, &x) != 0);

  vector_destroy(v);
}

void test_clear(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 20; i++) {
    vector_push_back(v, &i);
  }

  vector_clear(v);

  assert(vector_size(v) == 0);
  assert(vector_empty(v));

  vector_destroy(v);
}

void test_reserve(void) {
  Vector *v = vector_create(sizeof(int));

  assert(vector_reserve(v, 256) == 0);

  assert(vector_capacity(v) >= 256);

  vector_destroy(v);
}

void test_resize_grow(void) {
  Vector *v = vector_create(sizeof(int));

  assert(vector_resize(v, 50) == 0);

  assert(vector_capacity(v) == 50);

  vector_destroy(v);
}

void test_resize_shrink(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 3; i++) {
    vector_push_back(v, &i);
  }

  vector_resize(v, 10);

  assert(vector_capacity(v) == 10);

  for (int i = 0; i < 3; i++) {
    assert(*(int *)vector_get(v, i) == i);
  }

  vector_destroy(v);
}

void test_insert_front(void) {
  Vector *v = vector_create(sizeof(int));

  int a = 1;
  int b = 2;

  vector_push_back(v, &a);
  vector_insert(v, 0, &b);

  assert(*(int *)vector_get(v, 0) == 2);
  assert(*(int *)vector_get(v, 1) == 1);

  vector_destroy(v);
}

void test_insert_middle(void) {
  Vector *v = vector_create(sizeof(int));

  int values[] = {1, 2, 4};

  for (int i = 0; i < 3; i++) {
    vector_push_back(v, &values[i]);
  }

  int x = 3;

  vector_insert(v, 2, &x);

  assert(*(int *)vector_get(v, 0) == 1);
  assert(*(int *)vector_get(v, 1) == 2);
  assert(*(int *)vector_get(v, 2) == 3);
  assert(*(int *)vector_get(v, 3) == 4);

  vector_destroy(v);
}

void test_insert_end(void) {
  Vector *v = vector_create(sizeof(int));

  int x = 123;

  vector_insert(v, 0, &x);

  assert(vector_size(v) == 1);
  assert(*(int *)vector_get(v, 0) == 123);

  vector_destroy(v);
}

void test_erase_front(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    vector_push_back(v, &i);
  }

  vector_erase(v, 0);

  assert(*(int *)vector_get(v, 0) == 1);
  assert(vector_size(v) == 4);

  vector_destroy(v);
}

void test_erase_middle(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    vector_push_back(v, &i);
  }

  vector_erase(v, 2);

  assert(*(int *)vector_get(v, 0) == 0);
  assert(*(int *)vector_get(v, 1) == 1);
  assert(*(int *)vector_get(v, 2) == 3);
  assert(*(int *)vector_get(v, 3) == 4);

  vector_destroy(v);
}

void test_erase_last(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 5; i++) {
    vector_push_back(v, &i);
  }

  vector_erase(v, 4);

  assert(vector_size(v) == 4);

  vector_destroy(v);
}

void test_struct_storage(void) {
  typedef struct {
    int id;
    float score;
  } Item;

  Vector *v = vector_create(sizeof(Item));

  Item a = {1, 2.5f};
  Item b = {2, 5.0f};

  vector_push_back(v, &a);
  vector_push_back(v, &b);

  Item *p = vector_get(v, 1);

  assert(p->id == 2);
  assert(p->score == 5.0f);

  vector_destroy(v);
}

void test_reallocation_integrity(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 5000; i++) {
    vector_push_back(v, &i);
  }

  for (int i = 0; i < 5000; i++) {
    assert(*(int *)vector_get(v, i) == i);
  }

  vector_destroy(v);
}

void test_stress(void) {
  Vector *v = vector_create(sizeof(int));

  for (int i = 0; i < 100000; i++) {
    vector_push_back(v, &i);
  }

  assert(vector_size(v) == 100000);

  for (int i = 0; i < 100000; i++) {
    assert(*(int *)vector_get(v, i) == i);
  }

  vector_destroy(v);
}

void test(void) {
  RUN(test_create);
  RUN(test_push_back);
  RUN(test_capacity_growth);
  RUN(test_pop_back);
  RUN(test_pop_empty);
  RUN(test_get);
  RUN(test_get_oob);
  RUN(test_set);
  RUN(test_set_oob);
  RUN(test_clear);
  RUN(test_reserve);
  RUN(test_resize_grow);
  RUN(test_resize_shrink);
  RUN(test_insert_front);
  RUN(test_insert_middle);
  RUN(test_insert_end);
  RUN(test_erase_front);
  RUN(test_erase_middle);
  RUN(test_erase_last);
  RUN(test_struct_storage);
  RUN(test_reallocation_integrity);
  RUN(test_stress);

  printf("All tests passed\n");
}
