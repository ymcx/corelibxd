#include "malloc.h"
#include <unistd.h>

// TODO:
// Memory alignment
// Unit tests

Header *first = NULL;
Header *last = NULL;

void *malloc(size_t size) {
  if (size == 0) {
    return NULL;
  }

  // The heap is empty
  if (first == NULL) {
    first = header_new(NULL, size);
    last = first;
    return header_get_ptr(last);
  }

  // Travel forward until we've reached the end
  // or an available slot large enough has been found
  Header *header = first;
  while (header != NULL) {
    if (header->available && header->size >= size) {
      // The header has a size large enough for splitting
      if (header->size > sizeof(Header) + size) {
        header = header_split(header, size);
      }

      header->available = false;
      return header_get_ptr(header);
    }

    header = header->next;
  }

  // The only available space is at the end
  last = header_new(last, size);
  return header_get_ptr(last);
}

void free(void *ptr) {
  if (ptr == NULL) {
    return;
  }

  // Merge adjacent free slots and mark the merged header available
  Header *header = header_get_header(ptr);
  header = header_merge(header);
  header->available = true;

  // No pruning of the heap required
  if (header != last) {
    return;
  }

  last = header->prev;
  if (last == NULL) {
    // The first available space is at the very beginning
    first = NULL;
  } else {
    // The first available space is next to an unavailable slot
    last->next = NULL;
  }

  brk(header);
}

Header *header_new(Header *prev, size_t size) {
  void *ptr = sbrk(sizeof(Header) + size);
  if (ptr == (void *)-1) {
    return NULL;
  }

  Header *header = ptr;
  header_init(header, size, false);
  header_connect(prev, header, NULL);

  return header;
}

void header_init(Header *header, size_t size, bool available) {
  header->size = size;
  header->available = available;
}

void header_connect(Header *prev, Header *header, Header *next) {
  if (prev != NULL) {
    prev->next = header;
  }

  if (header != NULL) {
    header->prev = prev;
    header->next = next;
  }

  if (next != NULL) {
    next->prev = header;
  }
}

void *header_get_ptr(Header *header) { return header + 1; }

Header *header_get_header(void *ptr) { return (Header *)ptr - 1; }

Header *header_get_next_header(Header *header) {
  return (Header *)((char *)header_get_ptr(header) + header->size);
}

bool header_is_prev_available(Header *header) {
  return header->prev != NULL && header->prev->available;
}

bool header_is_next_available(Header *header) {
  return header->next != NULL && header->next->available;
}

Header *header_merge(Header *header) {
  Header *first_available =
      header_is_prev_available(header) ? header->prev : header;
  Header *last_available =
      header_is_next_available(header) ? header->next : header;

  // No merging can be done
  if (first_available == header && last_available == header) {
    return header;
  }

  Header *prev = first_available->prev;
  Header *next = last_available->next;
  size_t size = (char *)header_get_next_header(last_available) -
                (char *)header_get_ptr(first_available);

  header_init(first_available, size, true);
  header_connect(prev, first_available, next);

  if (last == last_available) {
    last = first_available;
  }

  return first_available;
}

Header *header_split(Header *header, size_t size) {
  header->size -= size + sizeof(Header);

  Header *new_header = header_get_next_header(header);
  header_init(new_header, size, false);
  header_connect(header, new_header, header->next);

  if (last == header) {
    last = new_header;
  }

  return new_header;
}
