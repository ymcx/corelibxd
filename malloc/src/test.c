#include "test.h"
#include "malloc.h"
#include <assert.h>
#include <stdio.h>
#include <unistd.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test_malloc(void) {
  void *address_before_allocate = sbrk(0);

  void *ptr = malloc(16);
  assert(ptr != NULL);

  void *address_after_allocate = sbrk(0);

  free(ptr);

  void *address_after_free = sbrk(0);

  assert(address_before_allocate == address_after_free);
  assert(address_before_allocate != address_after_allocate);
}

void test_malloc_empty(void) {
  void *address_before_allocate = sbrk(0);

  void *ptr = malloc(0);
  assert(ptr == NULL);

  void *address_after_allocate = sbrk(0);
  assert(address_before_allocate == address_after_allocate);
}

void test_malloc_memory_alignment(void) {
  const size_t size = 10;
  const size_t alignment = 8;
  const size_t size_aligned = size + alignment - size % alignment;

  void *ptr = malloc(size);
  assert(ptr != NULL);

  Header *header = header_get_header(ptr);
  assert(header != NULL);

  assert(header->size == size_aligned);

  free(ptr);
}

void test_malloc_int(void) {
  int *ptr = malloc(10 * sizeof(int));

  for (int i = 0; i < 10; ++i) {
    ptr[i] = i;
  }

  bool all_match = true;
  for (int i = 0; i < 10; ++i) {
    all_match = all_match && ptr[i] == i;
  }

  free(ptr);

  assert(all_match);
}

void test_header_init(void) {
  Header *header = malloc(sizeof(Header));
  assert(header != NULL);

  header_init(header, 8, true);

  assert(header->size == 8);
  assert(header->available == true);

  free(header);
}

void test_header_connect(void) {
  Header *header_1 = malloc(sizeof(Header));
  assert(header_1 != NULL);

  Header *header_2 = malloc(sizeof(Header));
  assert(header_2 != NULL);

  Header *header_3 = malloc(sizeof(Header));
  assert(header_3 != NULL);

  header_connect(header_1, header_2, header_3);

  assert(header_1->next == header_2);
  assert(header_2->next == header_3);

  assert(header_3->prev == header_2);
  assert(header_2->prev == header_1);
}

void test_header_connect_left(void) {
  Header *header_1 = malloc(sizeof(Header));
  assert(header_1 != NULL);

  Header *header_2 = malloc(sizeof(Header));
  assert(header_2 != NULL);

  Header *header_3 = NULL;
  assert(header_3 == NULL);

  header_connect(header_1, header_2, header_3);

  assert(header_1->prev == NULL);
  assert(header_1->next == header_2);

  assert(header_2->prev == header_1);
  assert(header_2->next == NULL);
}

void test_header_connect_right(void) {
  Header *header_1 = NULL;
  assert(header_1 == NULL);

  Header *header_2 = malloc(sizeof(Header));
  assert(header_2 != NULL);

  Header *header_3 = malloc(sizeof(Header));
  assert(header_3 != NULL);

  header_connect(header_1, header_2, header_3);

  assert(header_2->prev == NULL);
  assert(header_2->next == header_3);

  assert(header_3->prev == header_2);
  assert(header_3->next == NULL);
}

void test_header_connect_center(void) {
  Header *header_1 = malloc(sizeof(Header));
  assert(header_1 != NULL);

  Header *header_2 = NULL;
  assert(header_2 == NULL);

  Header *header_3 = malloc(sizeof(Header));
  assert(header_3 != NULL);

  header_connect(header_1, header_2, header_3);

  assert(header_1->prev == NULL);
  assert(header_1->next == NULL);

  assert(header_3->prev == NULL);
  assert(header_3->next == NULL);
}

void test_header_get_header(void) {
  void *ptr = malloc(8);
  assert(ptr != NULL);

  Header *header = header_get_header(ptr);
  assert(header != NULL);

  assert(ptr == header_get_ptr(header));
  assert(header == header_get_header(ptr));

  free(ptr);
}

void test_header_next_header(void) {
  void *ptr_1 = malloc(8);
  assert(ptr_1 != NULL);

  void *ptr_2 = malloc(8);
  assert(ptr_2 != NULL);

  void *ptr_3 = malloc(8);
  assert(ptr_3 != NULL);

  assert(header_get_header(ptr_1)->next == header_get_header(ptr_2));

  assert(header_get_header(ptr_2)->prev == header_get_header(ptr_1));
  assert(header_get_header(ptr_2)->next == header_get_header(ptr_3));

  assert(header_get_header(ptr_3)->prev == header_get_header(ptr_2));

  free(ptr_1);
  free(ptr_2);
  free(ptr_3);
}

void test_header_is_next_available(void) {
  void *ptr_1 = malloc(8);
  assert(ptr_1 != NULL);

  Header *header_1 = header_get_header(ptr_1);
  assert(header_1 != NULL);

  void *ptr_2 = malloc(8);
  assert(ptr_2 != NULL);

  Header *header_2 = header_get_header(ptr_2);
  assert(header_2 != NULL);

  void *ptr_3 = malloc(8);
  assert(ptr_3 != NULL);

  Header *header_3 = header_get_header(ptr_3);
  assert(header_3 != NULL);

  header_1->available = true;
  header_2->available = true;
  header_3->available = true;

  assert(header_is_next_available(header_1));
  assert(header_is_next_available(header_2));
  assert(!header_is_next_available(header_3));

  assert(header_is_prev_available(header_3));
  assert(header_is_prev_available(header_2));
  assert(!header_is_prev_available(header_1));

  free(ptr_1);
  free(ptr_2);
  free(ptr_3);
}

void test(void) {
  RUN(test_malloc);
  RUN(test_malloc_empty);
  RUN(test_malloc_memory_alignment);
  RUN(test_malloc_int);
  RUN(test_header_init);
  RUN(test_header_connect);
  RUN(test_header_connect_left);
  RUN(test_header_connect_right);
  RUN(test_header_connect_center);
  RUN(test_header_get_header);
  RUN(test_header_next_header);
  RUN(test_header_is_next_available);

  printf("All tests passed\n");
}
