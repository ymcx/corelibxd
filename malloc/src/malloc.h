#ifndef MALLOC_H
#define MALLOC_H

#include <stdbool.h>
#include <stddef.h>

typedef struct Header {
  struct Header *prev;
  struct Header *next;
  size_t size;
  bool available;
} Header;

void *malloc(size_t size);
void free(void *ptr);
Header *header_new(Header *prev, size_t size);
void header_init(Header *header, size_t size, bool available);
void header_connect(Header *prev, Header *header, Header *next);
void *header_get_ptr(Header *header);
Header *header_get_header(void *ptr);
Header *header_get_next_header(Header *header);
bool header_is_prev_available(Header *header);
bool header_is_next_available(Header *header);
Header *header_merge(Header *header);
Header *header_split(Header *header, size_t size);

#endif
