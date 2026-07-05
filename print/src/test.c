#include "test.h"
#include "src/print.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test_itos(void) {
  char *s = malloc(64 * sizeof(char));

  itos(0, 10, s);
  assert(!strcmp(s, "0"));

  itos(5, 10, s);
  assert(!strcmp(s, "5"));

  itos(-5, 10, s);
  assert(!strcmp(s, "-5"));

  itos(INT_MAX, 10, s);
  assert(!strcmp(s, "2147483647"));

  itos(INT_MIN, 10, s);
  assert(!strcmp(s, "-2147483648"));

  free(s);
}

void test_utos(void) {
  char *s = malloc(64 * sizeof(char));

  utos(0, 10, s);
  assert(!strcmp(s, "0"));

  utos(5, 10, s);
  assert(!strcmp(s, "5"));

  utos(UINT_MAX, 10, s);
  assert(!strcmp(s, "4294967295"));

  free(s);
}

void test_dtos(void) {
  char *s = malloc(64 * sizeof(char));

  dtos(0.0000, 10, s);
  assert(!strcmp(s, "0.0000"));

  dtos(0.1000, 10, s);
  assert(!strcmp(s, "0.1000"));

  dtos(-0.1000, 10, s);
  assert(!strcmp(s, "-0.1000"));

  dtos(9.5000, 10, s);
  assert(!strcmp(s, "9.5000"));

  dtos(-9.5000, 10, s);
  assert(!strcmp(s, "-9.5000"));

  dtos(10.5000, 10, s);
  assert(!strcmp(s, "10.5000"));

  dtos(-10.5000, 10, s);
  assert(!strcmp(s, "-10.5000"));

  dtos(123.4567, 10, s);
  assert(!strcmp(s, "123.4567"));

  dtos(-123.4567, 10, s);
  assert(!strcmp(s, "-123.4567"));

  free(s);
}

void test_get_printable_string(void) {}

void test(void) {
  RUN(test_itos);
  RUN(test_utos);
  RUN(test_dtos);
  RUN(test_get_printable_string);

  printf("All tests passed\n");
}
