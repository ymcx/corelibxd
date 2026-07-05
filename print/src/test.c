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
  char *s;

  s = itos(0, 10);
  assert(!strcmp(s, "0"));
  free(s);

  s = itos(5, 10);
  assert(!strcmp(s, "5"));
  free(s);

  s = itos(-5, 10);
  assert(!strcmp(s, "-5"));
  free(s);

  s = itos(INT_MAX, 10);
  assert(!strcmp(s, "2147483647"));
  free(s);

  s = itos(INT_MIN, 10);
  assert(!strcmp(s, "-2147483648"));
  free(s);
}

void test_utos(void) {
  char *s;

  s = utos(0, 10);
  assert(!strcmp(s, "0"));
  free(s);

  s = utos(5, 10);
  assert(!strcmp(s, "5"));
  free(s);

  s = utos(UINT_MAX, 10);
  assert(!strcmp(s, "4294967295"));
  free(s);
}

void test_dtos(void) {
  char *s;

  s = dtos(0.0000, 10);
  assert(!strcmp(s, "0.0000"));
  free(s);

  s = dtos(0.1000, 10);
  assert(!strcmp(s, "0.1000"));
  free(s);

  s = dtos(-0.1000, 10);
  assert(!strcmp(s, "-0.1000"));
  free(s);

  s = dtos(9.5000, 10);
  assert(!strcmp(s, "9.5000"));
  free(s);

  s = dtos(-9.5000, 10);
  assert(!strcmp(s, "-9.5000"));
  free(s);

  s = dtos(10.5000, 10);
  assert(!strcmp(s, "10.5000"));
  free(s);

  s = dtos(-10.5000, 10);
  assert(!strcmp(s, "-10.5000"));
  free(s);

  s = dtos(123.4567, 10);
  assert(!strcmp(s, "123.4567"));
  free(s);

  s = dtos(-123.4567, 10);
  assert(!strcmp(s, "-123.4567"));
  free(s);
}

void test_ctos(void) {
  char *s;

  s = ctos('c');
  assert(!strcmp(s, "c"));
  free(s);

  s = ctos('a');
  assert(!strcmp(s, "a"));
  free(s);
}

void test_stos(void) {
  char *s;

  s = stos("test");
  assert(!strcmp(s, "test"));
  free(s);

  s = stos("tst");
  assert(!strcmp(s, "tst"));
  free(s);
}

void test_utos_octal(void) {
  char *s;

  s = utos(0, 8);
  assert(!strcmp(s, "0"));
  free(s);

  s = utos(5, 8);
  assert(!strcmp(s, "5"));
  free(s);

  s = utos(10, 8);
  assert(!strcmp(s, "12"));
  free(s);

  s = utos(UINT_MAX, 8);
  assert(!strcmp(s, "37777777777"));
  free(s);
}

void test_itos_hexidecimal(void) {
  char *s;

  s = utos(0, 16);
  assert(!strcmp(s, "0"));
  free(s);

  s = utos(5, 16);
  assert(!strcmp(s, "5"));
  free(s);

  s = utos(20, 16);
  assert(!strcmp(s, "14"));
  free(s);

  s = utos(30, 16);
  assert(!strcmp(s, "1e"));
  free(s);

  s = utos(500, 16);
  assert(!strcmp(s, "1f4"));
  free(s);

  s = utos(UINT_MAX, 16);
  assert(!strcmp(s, "ffffffff"));
  free(s);
}

void test_get_printable_string(void) {}

void test(void) {
  RUN(test_itos);
  RUN(test_utos);
  RUN(test_dtos);
  RUN(test_ctos);
  RUN(test_stos);
  RUN(test_utos_octal);
  RUN(test_itos_hexidecimal);
  RUN(test_get_printable_string);

  printf("All tests passed\n");
}
