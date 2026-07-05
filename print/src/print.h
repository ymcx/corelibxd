#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>

char *utos(const unsigned int i, const int base);
char *itos(const int i, const int base);
char *dtos(const double i, const int base);
char *ctos(const char i);
char *stos(const char *i);
char *get_printable_string(const char *format, va_list args);
char *get_printable_string_args(const char *format, ...);
void print(const char *format, ...);

#endif
