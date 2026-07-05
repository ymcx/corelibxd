#include "print.h"
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *utos(unsigned int u, const int base) {
  size_t index = u != 0 ? log(u) / log(base) + 1 : 1;
  char *s = malloc(index + 1);
  s[index] = '\0';

  while (index > 0) {
    s[--index] = u % base + '0';
    u /= base;
  }

  return s;
}

char *itos(const int i, const int base) {
  const bool sign = i < 0;
  unsigned int u = abs(i);

  size_t index = u != 0 ? log(u) / log(base) + 1 + sign : 1;
  char *s = malloc(index + 1);
  s[index] = '\0';

  while (index > sign) {
    s[--index] = u % base + '0';
    u /= base;
  }

  if (sign) {
    s[0] = '-';
  }

  return s;
}

char *dtos(const double d, const int base) {
  const int precision = 4;

  const bool sign = d < 0.0;
  unsigned int u = fabs(d);

  size_t index = u != 0 ? log(u) / log(base) + 1 + sign : 1 + sign;
  size_t index_decimals = index + precision + 1;
  char *s = malloc(index_decimals + 1);
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

  return s;
}

char *scientific(double i, const int base) {
  int length = (int)log10((int)i);

  i /= pow(base, length);
  char *s = dtos(i, base);
  size_t newle = strlen(s);
  s[newle + 0] = 'e';
  s[newle + 1] = '+';
  s[newle + 2] = '0';
  s[newle + 3] = length + '0';
  s[newle + 4] = '\0';

  return s;
}

char *get_printable_string(const char *format, va_list args) {
  char *string = malloc(1);
  string[0] = '\0';

  const size_t format_length = strlen(format);
  for (size_t i = 0; i < format_length; ++i) {
    if (format[i] == '%') {
      char *arg;

      switch (format[i + 1]) {
      case 'd':
      case 'i':
        arg = itos(va_arg(args, int), 10);
        break;
      }

      const size_t string_length = strlen(string);
      const size_t arg_length = strlen(arg);
      char *tmp = malloc(string_length + arg_length + 1);

      memcpy(tmp, string, string_length);
      free(string);
      memcpy(tmp + string_length, arg, arg_length);
      free(arg);
      tmp[string_length + arg_length] = '\0';

      string = tmp;

      ++i;
    } else {
      const size_t string_length = strlen(string);
      char *tmp = malloc(string_length + 2);

      memcpy(tmp, string, string_length);
      free(string);
      tmp[string_length] = format[i];
      tmp[string_length + 1] = '\0';

      string = tmp;
    }
  }

  return string;
}

void print(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *string = get_printable_string(format, args);

  va_end(args);

  fflush(stdout);
  write(1, string, strlen(string));

  free(string);
}
