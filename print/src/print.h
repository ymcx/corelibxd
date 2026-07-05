#ifndef PRINT_H
#define PRINT_H

#include <stdarg.h>

char *utos(unsigned int u, const int base);
char *itos(const int i, const int base);
char *dtos(const double d, const int base);
char *scientific(double i, const int base);
char *get_printable_string(const char *format, va_list args);
void print(const char *format, ...);

#endif
