#include "print.h"
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void utos(unsigned int u, const int base, char *s) {
  size_t index = u != 0 ? log(u) / log(base) + 1 : 1;
  s[index] = '\0';

  while (index > 0) {
    s[--index] = u % base + '0';
    u /= base;
  }
}

void itos(const int i, const int base, char *s) {
  const bool sign = i < 0;
  unsigned int u = abs(i);

  size_t index = u != 0 ? log(u) / log(base) + 1 + sign : 1;
  s[index] = '\0';

  while (index > sign) {
    s[--index] = u % base + '0';
    u /= base;
  }

  if (sign) {
    s[0] = '-';
  }
}

void dtos(const double d, const int base, char *s) {
  const int precision = 4;

  const bool sign = d < 0.0;
  unsigned int u = fabs(d);

  size_t index = u != 0 ? log(u) / log(base) + 1 + sign : 1 + sign;
  size_t index_decimals = index + precision + 1;
  s[index] = '.';
  s[index_decimals] = '\0';

  unsigned int u_decimals = (fabs(d) - u) * pow(base, precision) + 0.5;
  while (index_decimals > index + 1) {
    s[--index_decimals] = u_decimals % base + '0';
    u_decimals /= base;
  }

  while (index > sign) {
    s[--index] = u % base + '0';
    u /= base;
  }

  if (sign) {
    s[0] = '-';
  }
}

void scientific(double i, char *s) {
  int length = (int)log10((int)i);

  i /= pow(10, length);
  dtos(i, 10, s);
  size_t newle = strlen(s);
  s[newle + 0] = 'e';
  s[newle + 1] = '+';
  s[newle + 2] = '0';
  s[newle + 3] = length + '0';
  s[newle + 4] = '\0';
}

void print(const char *format, ...) {
  va_list args;
  const size_t n = 1;
  va_start(args, n);

  const size_t formatLength = strlen(format);
  size_t i = 0;
  char *input;
  double arg;
  size_t a;
  size_t b;

  while (i < formatLength) {
    if (format[0] == '%') {
      switch (format[1]) {
      case 'd':
      case 'i':
        itos(va_arg(args, int), 10, input);
        break;
      case 'u':
        utos(va_arg(args, unsigned int), 10, input);
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
        dtos(va_arg(args, double), 10, input);
        break;
      case 'e':
      case 'E':
        scientific(va_arg(args, double), input);
        break;
      case 'g':
      case 'G':
        arg = va_arg(args, double);
        dtos(arg, 10, input);
        a = strlen(input);
        scientific(arg, input);
        b = strlen(input);
        if (a < b) {
          dtos(arg, 10, input);
        } else {
          scientific(arg, input);
        }
        break;
      // case 'a':
      // case 'A':
      //   printf("%a", a);
      //   break;
      case 'c':
        input[0] = va_arg(args, int);
        input[1] = '\0';
        break;
      case 's':
        input = va_arg(args, char *);
        break;
      // case 'p':
      //   printf("%p", a);
      //   break;
      // case 'n':
      //   printf("%n", a);
      //   break;
      case '%':
        input = "%";
        break;
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
