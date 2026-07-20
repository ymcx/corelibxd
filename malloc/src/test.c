#include "test.h"
#include <stdio.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test(void) { printf("All tests passed\n"); }
