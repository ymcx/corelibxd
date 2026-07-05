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

void scientific(double i, const int base, char *s) {
  int length = (int)log10((int)i);

  i /= pow(base, length);
  dtos(i, base, s);
  size_t newle = strlen(s);
  s[newle + 0] = 'e';
  s[newle + 1] = '+';
  s[newle + 2] = '0';
  s[newle + 3] = length + '0';
  s[newle + 4] = '\0';
}

char *get_printable_string(const char *format, ...) {
  char *arg;
  va_list args;
  va_start(args, format);

  char *string_new;
  char *string = malloc(1);

  double argggi;
  size_t a;
  size_t b;

  for (size_t i = 0; i < strlen(format); ++i) {
    if (format[i] == '%') {
      switch (format[i + 1]) {
      case 'd':
      case 'i':
        itos(va_arg(args, int), 10, arg);
        break;
      case 'u':
        utos(va_arg(args, unsigned int), 10, arg);
        break;
      case 'o':
        itos(va_arg(args, int), 8, arg);
        break;
      case 'x':
      case 'X':
        itos(va_arg(args, int), 16, arg);
        break;
      case 'f':
      case 'F':
        dtos(va_arg(args, double), 10, arg);
        break;
      case 'e':
      case 'E':
        scientific(va_arg(args, double), 10, arg);
        break;
      case 'g':
      case 'G':
        argggi = va_arg(args, double);
        dtos(argggi, 10, arg);
        a = strlen(arg);
        scientific(argggi, 10, arg);
        b = strlen(arg);
        if (a < b) {
          dtos(argggi, 10, arg);
        } else {
          scientific(argggi, 10, arg);
        }
        break;
      case 'a':
      case 'A':
        break;
      case 'c':
        arg[0] = va_arg(args, int);
        arg[1] = '\0';
        break;
      case 's':
        arg = va_arg(args, char *);
        break;
      case 'p':
        break;
      case 'n':
        break;
      case '%':
        arg = "%";
        break;
      }

      const size_t string_length = strlen(string);
      const size_t arg_length = strlen(arg);
      string_new = malloc(string_length + arg_length + 1);

      memcpy(string_new, string, string_length);
      memcpy(string_new + string_length, arg, arg_length);
      string_new[string_length + arg_length] = '\0';

      ++i;
    } else {
      const size_t string_length = strlen(string);
      string_new = malloc(string_length + 1 + 1);

      memcpy(string_new, string, string_length);
      string_new[string_length] = format[i];
      string_new[string_length + 1] = '\0';
    }

    free(string);
    string = string_new;
  }

  va_end(args);

  return string;
}

void print(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *input = get_printable_string(format, args);

  va_end(args);
  fflush(stdout);
  write(1, input, strlen(input));
}
