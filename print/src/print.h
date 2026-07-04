#ifndef PRINT_H
#define PRINT_H

void utos(unsigned int u, const int base, char *s);
void itos(const int i, const int base, char *s);
void dtos(const double d, const int base, char *s);
void scientific(double i, char *s);
void print(const char *format, ...);

#endif
