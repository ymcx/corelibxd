#include "test.h"
#include "src/print.h"
#include <limits.h>
#include <stdio.h>

#define RUN(function)                                                          \
  {                                                                            \
    printf("%s\n", #function);                                                 \
    function();                                                                \
  }

void test(void) {
  printf("|%s|\n", "lol");
  print("|%s|\n", 1, "lol");

  printf("|%d|\n", 20);
  print("|%d|\n", 1, 20);

  printf("|%u|\n", 1);
  print("|%u|\n", 1, 1);

  printf("|%o|\n", 69);
  print("|%o|\n", 1, 69);

  printf("|%x|\n", 69);
  print("|%x|\n", 1, 69);

  printf("|%f|\n", 69.420);
  print("|%f|\n", 1, 69.420);
}
