#ifndef VECTOR_TEST_H
#define VECTOR_TEST_H

void test_create(void);
void test_push_back(void);
void test_capacity_growth(void);
void test_pop_back(void);
void test_pop_empty(void);
void test_get(void);
void test_get_oob(void);
void test_set(void);
void test_set_oob(void);
void test_clear(void);
void test_resize_grow(void);
void test_resize_shrink(void);
void test_insert_front(void);
void test_insert_middle(void);
void test_insert_end(void);
void test_erase_front(void);
void test_erase_middle(void);
void test_erase_last(void);
void test_struct_storage(void);
void test_reallocation_integrity(void);
void test_stress(void);
void test(void);

#endif
