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
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  assert(vector_size(vector) == 0);
  assert(vector_empty(vector) == 1);
  assert(vector_capacity(vector) == 128);

  vector_destroy(vector);
}

void test_push_back(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 32; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  assert(vector_size(vector) == 32);

  for (int i = 0; i < 32; ++i) {
    assert(*(int *)vector_get(vector, i) == i);
  }

  vector_destroy(vector);
}

void test_capacity_growth(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const size_t capacity_old = vector_capacity(vector);
  assert(capacity_old == 128);

  for (int i = 0; i < 129; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  const size_t capacity_new = vector_capacity(vector);
  assert(capacity_old < capacity_new);
  assert(vector_size(vector) <= capacity_new);

  vector_destroy(vector);
}

void test_pop_back(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }
  assert(vector_size(vector) == 4);

  assert(vector_pop_back(vector) == 0);
  assert(vector_size(vector) == 3);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 1);
  assert(*(int *)vector_get(vector, 2) == 2);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_pop_back(vector) == 0);
  assert(vector_size(vector) == 2);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 1);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  vector_destroy(vector);
}

void test_pop_front(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }
  assert(vector_size(vector) == 4);

  assert(vector_pop_front(vector) == 0);
  assert(vector_size(vector) == 3);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == 1);
  assert(*(int *)vector_get(vector, 1) == 2);
  assert(*(int *)vector_get(vector, 2) == 3);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_pop_front(vector) == 0);
  assert(vector_size(vector) == 2);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == 2);
  assert(*(int *)vector_get(vector, 1) == 3);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  vector_destroy(vector);
}

void test_pop_empty(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  assert(vector_empty(vector));
  assert(vector_pop_back(vector) != 0);
  assert(vector_get(vector, 0) == NULL);

  vector_destroy(vector);
}

void test_get(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  assert(vector_get(vector, 0) == NULL);
  assert(vector_back(vector) == NULL);
  assert(vector_front(vector) == NULL);

  const int a = 123;
  assert(vector_push_back(vector, &a) == 0);

  assert(*(int *)vector_get(vector, 0) == a);
  assert(*(int *)vector_back(vector) == a);
  assert(*(int *)vector_front(vector) == a);

  const int b = 456;
  assert(vector_push_back(vector, &b) == 0);

  assert(*(int *)vector_get(vector, 0) == a);
  assert(*(int *)vector_get(vector, 1) == b);
  assert(*(int *)vector_back(vector) == b);
  assert(*(int *)vector_front(vector) == a);

  vector_destroy(vector);
}

void test_get_oob(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  assert(vector_get(vector, -1) == NULL);
  assert(vector_get(vector, 0) == NULL);
  assert(vector_get(vector, 1) == NULL);

  const int value = 123;
  assert(vector_push_back(vector, &value) == 0);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == value);
  assert(vector_get(vector, 1) == NULL);

  vector_destroy(vector);
}

void test_set(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const int a = 1;
  const int b = 99;

  assert(vector_push_back(vector, &a) == 0);
  assert(*(int *)vector_get(vector, 0) == a);

  assert(vector_set(vector, 0, &b) == 0);
  assert(*(int *)vector_get(vector, 0) == b);

  vector_destroy(vector);
}

void test_set_oob(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const int a = 42;
  assert(vector_set(vector, -1, &a) != 0);
  assert(vector_set(vector, 0, &a) != 0);
  assert(vector_set(vector, 1, &a) != 0);
  assert(vector_insert(vector, -1, &a) != 0);
  assert(vector_insert(vector, 1, &a) != 0);
  assert(vector_size(vector) == 0);

  assert(vector_insert(vector, 0, &a) == 0);
  assert(vector_size(vector) == 1);
  assert(*(int *)vector_get(vector, 0) == a);
  assert(vector_get(vector, 1) == NULL);

  const int b = 42;
  assert(vector_set(vector, 0, &b) == 0);
  assert(vector_size(vector) == 1);
  assert(*(int *)vector_get(vector, 0) == b);
  assert(vector_get(vector, 1) == NULL);

  vector_destroy(vector);
}

void test_clear(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  vector_clear(vector);

  assert(vector_size(vector) == 0);
  assert(vector_back(vector) == NULL);
  assert(vector_front(vector) == NULL);
  assert(vector_get(vector, -1) == NULL);
  assert(vector_get(vector, 0) == NULL);
  assert(vector_get(vector, 1) == NULL);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);
  assert(vector_get(vector, 4) == NULL);
  assert(vector_empty(vector));

  vector_destroy(vector);
}

void test_resize_grow(void) {
  Vector *vector = vector_create(32, sizeof(int));
  assert(vector != NULL);

  assert(vector_size(vector) == 0);
  assert(vector_capacity(vector) == 32);

  assert(vector_resize(vector, 64) == 0);

  assert(vector_size(vector) == 0);
  assert(vector_capacity(vector) == 64);

  const int value = 123;
  assert(vector_push_back(vector, &value) == 0);
  assert(vector_size(vector) == 1);
  assert(*(int *)vector_get(vector, 0) == value);

  assert(vector_resize(vector, 128) == 0);

  assert(vector_size(vector) == 1);
  assert(*(int *)vector_get(vector, 0) == value);

  vector_destroy(vector);
}

void test_resize_shrink(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  assert(vector_resize(vector, 16) == 0);
  assert(vector_capacity(vector) == 16);
  assert(vector_size(vector) == 4);

  for (int i = 0; i < 4; ++i) {
    assert(*(int *)vector_get(vector, i) == i);
  }

  assert(vector_resize(vector, 3) != 0);
  assert(vector_capacity(vector) == 16);
  assert(vector_size(vector) == 4);

  assert(vector_resize(vector, 4) == 0);
  assert(vector_capacity(vector) == 4);
  assert(vector_size(vector) == 4);

  vector_destroy(vector);
}

void test_insert_front(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const int a = 1;
  const int b = 2;

  assert(vector_push_back(vector, &a) == 0);
  assert(vector_insert(vector, 0, &b) == 0);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == b);
  assert(*(int *)vector_get(vector, 1) == a);
  assert(vector_get(vector, 2) == NULL);

  const int c = 3;
  const int d = 4;
  assert(vector_push_front(vector, &c) == 0);
  assert(vector_push_front(vector, &d) == 0);

  assert(vector_get(vector, -1) == NULL);
  assert(*(int *)vector_get(vector, 0) == d);
  assert(*(int *)vector_get(vector, 1) == c);
  assert(*(int *)vector_get(vector, 2) == b);
  assert(*(int *)vector_get(vector, 3) == a);
  assert(vector_get(vector, 4) == NULL);

  vector_destroy(vector);
}

void test_insert_middle(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const int values[3] = {5, 6, 7};
  for (int i = 0; i < 3; ++i) {
    assert(vector_push_back(vector, &values[i]) == 0);
  }

  const int value = 2;
  assert(vector_insert(vector, 2, &value) == 0);

  assert(*(int *)vector_front(vector) == values[0]);
  assert(*(int *)vector_get(vector, 0) == values[0]);
  assert(*(int *)vector_get(vector, 1) == values[1]);
  assert(*(int *)vector_get(vector, 2) == value);
  assert(*(int *)vector_get(vector, 3) == values[2]);
  assert(*(int *)vector_back(vector) == values[2]);

  vector_destroy(vector);
}

void test_insert_end(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  const int value = 123;
  assert(vector_insert(vector, 0, &value) == 0);
  assert(vector_insert(vector, 1, &value) == 0);

  assert(vector_size(vector) == 2);
  assert(*(int *)vector_get(vector, 0) == value);
  assert(*(int *)vector_get(vector, 1) == value);

  vector_destroy(vector);
}

void test_erase_front(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  assert(vector_erase(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 0) == 1);
  assert(vector_size(vector) == 3);

  assert(vector_erase(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 0) == 2);
  assert(vector_size(vector) == 2);

  assert(vector_erase(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 0) == 3);
  assert(vector_size(vector) == 1);

  assert(vector_erase(vector, 0) == 0);
  assert(vector_get(vector, 0) == NULL);
  assert(vector_size(vector) == 0);

  assert(vector_erase(vector, 0) != 0);
  assert(vector_get(vector, 0) == NULL);
  assert(vector_size(vector) == 0);

  vector_destroy(vector);
}

void test_erase_middle(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }
  assert(vector_size(vector) == 4);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 1);
  assert(*(int *)vector_get(vector, 2) == 2);
  assert(*(int *)vector_get(vector, 3) == 3);

  assert(vector_erase(vector, 1) == 0);
  assert(vector_size(vector) == 3);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 2);
  assert(*(int *)vector_get(vector, 2) == 3);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 1) == 0);
  assert(vector_size(vector) == 2);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 3);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 1) == 0);
  assert(vector_size(vector) == 1);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(vector_get(vector, 1) == NULL);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 1) != 0);
  assert(vector_size(vector) == 1);

  vector_destroy(vector);
}

void test_erase_last(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }
  assert(vector_size(vector) == 4);

  assert(vector_erase(vector, 3) == 0);
  assert(vector_size(vector) == 3);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 1);
  assert(*(int *)vector_get(vector, 2) == 2);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 2) == 0);
  assert(vector_size(vector) == 2);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(*(int *)vector_get(vector, 1) == 1);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 1) == 0);
  assert(vector_size(vector) == 1);

  assert(*(int *)vector_get(vector, 0) == 0);
  assert(vector_get(vector, 1) == NULL);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 0) == 0);
  assert(vector_size(vector) == 0);

  assert(vector_get(vector, 0) == NULL);
  assert(vector_get(vector, 1) == NULL);
  assert(vector_get(vector, 2) == NULL);
  assert(vector_get(vector, 3) == NULL);

  assert(vector_erase(vector, 0) != 0);
  assert(vector_size(vector) == 0);

  vector_destroy(vector);
}

void test_struct_storage(void) {
  typedef struct {
    int id;
    float score;
  } Item;

  Vector *vector = vector_create(128, sizeof(Item));
  assert(vector != NULL);

  const Item a = {1, 2.5f};
  const Item b = {2, 5.0f};

  assert(vector_push_back(vector, &a) == 0);
  assert(vector_push_back(vector, &b) == 0);

  const Item *bb = vector_get(vector, 1);
  assert(bb->id == b.id);
  assert(bb->score == b.score);

  assert(bb != &b);

  vector_destroy(vector);
}

void test_reallocation_integrity(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 8192; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  for (int i = 0; i < 8192; ++i) {
    assert(*(int *)vector_get(vector, i) == i);
  }

  vector_destroy(vector);
}

void test_stress(void) {
  Vector *vector = vector_create(128, sizeof(int));
  assert(vector != NULL);

  for (int i = 0; i < 4194304; ++i) {
    assert(vector_push_back(vector, &i) == 0);
  }

  assert(vector_size(vector) == 4194304);

  for (int i = 0; i < 4194304; ++i) {
    assert(*(int *)vector_get(vector, i) == i);
  }

  vector_destroy(vector);
}

void test(void) {
  RUN(test_create);
  RUN(test_push_back);
  RUN(test_capacity_growth);
  RUN(test_pop_back);
  RUN(test_pop_front);
  RUN(test_pop_empty);
  RUN(test_get);
  RUN(test_get_oob);
  RUN(test_set);
  RUN(test_set_oob);
  RUN(test_clear);
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
