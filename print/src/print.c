#include "print.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char *utos(const unsigned int i, const int base) {
  unsigned int u = i;
  size_t index = u != 0 ? log(u) / log(base) + 1 : 1;
  char *s = malloc(index + 1);
  s[index] = '\0';

  while (index > 0) {
    const unsigned int r = u % base;
    if (r < 10) {
      s[--index] = '0' + r;
    } else {
      s[--index] = 'a' + (r - 10);
    }
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
    const unsigned int r = u % base;
    if (r < 10) {
      s[--index] = '0' + r;
    } else {
      s[--index] = 'a' + (r - 10);
    }
    u /= base;
  }

  if (sign) {
    s[0] = '-';
  }

  return s;
}

char *dtos(const double i, const int base) {
  const int precision = 4;

  const bool sign = i < 0.0;
  unsigned int u = fabs(i);

  size_t index = u != 0 ? log(u) / log(base) + 1 + sign : 1 + sign;
  size_t index_decimals = index + precision + 1;
  char *s = malloc(index_decimals + 1);
  s[index] = '.';
  s[index_decimals] = '\0';

  unsigned int u_decimals = (fabs(i) - u) * pow(base, precision) + 0.5;
  while (index_decimals > index + 1) {
    const unsigned int r = u_decimals % base;
    if (r < 10) {
      s[--index_decimals] = '0' + r;
    } else {
      s[--index_decimals] = 'a' + (r - 10);
    }
    u_decimals /= base;
  }

  while (index > sign) {
    const unsigned int r = u % base;
    if (r < 10) {
      s[--index] = '0' + r;
    } else {
      s[--index] = 'a' + (r - 10);
    }
    u /= base;
  }

  if (sign) {
    s[0] = '-';
  }

  return s;
}

char *ctos(const char i) {
  char *s = malloc(2);
  s[0] = i;
  s[1] = '\0';

  return s;
}

char *stos(const char *i) {
  const size_t index = strlen(i);
  char *s = malloc(index + 1);
  memcpy(s, i, index);
  s[index] = '\0';

  return s;
}

char *get_printable_string(const char *format, va_list args) {
  char *string = malloc(1);
  string[0] = '\0';

  const size_t format_length = strlen(format);
  for (size_t i = 0; i < format_length; ++i) {
    if (format[i] == '%' && i + 1 < format_length) {
      const char c = format[i + 1];
      char *arg;

      switch (c) {
      case 'd':
      case 'i':
        arg = itos(va_arg(args, int), 10);
        break;
      case 'u':
        arg = utos(va_arg(args, unsigned int), 10);
        break;
      case 'o':
        arg = utos(va_arg(args, int), 8);
        break;
      case 'x':
      case 'X':
        arg = utos(va_arg(args, int), 16);
        break;
      case 'f':
      case 'F':
        arg = dtos(va_arg(args, double), 10);
        break;
      case 'c':
        arg = ctos(va_arg(args, int));
        break;
      case 's':
        arg = stos(va_arg(args, char *));
        break;
      default:
        arg = ctos(c);
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

char *get_printable_string_args(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *string = get_printable_string(format, args);

  va_end(args);

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
