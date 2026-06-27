#include "print.h"
#include <math.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void utos(unsigned int i, char *s) {
  size_t index = (int)log10(i);
  s[index + 1] = '\0';
  while (0 < i) {
    s[index] = i % 10 + '0';
    i /= 10;
    index -= 1;
  }
}

void itos(int i, int base, char *s) {
  size_t index = (int)(log(i) / log(base));
  s[index + 1] = '\0';
  while (0 < i) {
    s[index] = i % base + '0';
    i /= base;
    index -= 1;
  }
}

void dtos(double i, char *s) {
  itos((int)i, 10, s);

  int n = 6;
  i = (i - (int)i) * pow(10, n);

  s[strlen(s)] = '.';

  itos(i, 10, s + strlen(s));

  s[strlen(s)] = '\0';
}

void print(const char *format, const int n, ...) {
  va_list args;
  va_start(args, n);

  const size_t formatLength = strlen(format);
  size_t i = 0;
  char *input;

  while (i < formatLength) {
    if (format[0] == '%') {
      switch (format[1]) {
      case 'd':
      case 'i':
        itos(va_arg(args, int), 10, input);
        break;
      case 'u':
        utos(va_arg(args, unsigned int), input);
        break;
      case 'o':
        itos(va_arg(args, int), 8, input);
        break;
      case 'x':
      case 'X':
        itos(va_arg(args, int), 16, input);
        break;
      case 'f':
      case 'F':
        dtos(va_arg(args, double), input);
        break;
      // case 'e':
      // case 'E':
      //   printf("%e", a);
      //   break;
      // case 'g':
      // case 'G':
      //   printf("%g", a);
      //   break;
      // case 'a':
      // case 'A':
      //   printf("%a", a);
      //   break;
      // case 'c':
      //   printf("%c", a);
      //   break;
      case 's':
        input = va_arg(args, char *);
        break;
        // case 'p':
        //   printf("%p", a);
        //   break;
        // case 'n':
        //   printf("%n", a);
        //   break;
        // case '%':
        //   printf("%%", a);
        //   break;
      }

      fflush(stdout);
      write(1, input, strlen(input));

      i += 2;
      format += 2;
    } else {
      fflush(stdout);
      write(1, format, sizeof(char));

      i += 1;
      format += 1;
    }
  }

  va_end(args);
}
