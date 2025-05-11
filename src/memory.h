#ifndef clox_memory_h
#define clox_memory_h

#include "common.h"
#include "chunk.h"
#include <stdlib.h>

// TODO: GROW_CAPACITY macro fails to properly expand when using below const
#define MIN_ARR_THRESHOLD = 8;

#define GROW_CAPACITY(capacity)                                                \
  ((capacity) < (8) ? (8) : (capacity) * 2)

#define GROW_ARRAY(type, pointer, old_count, new_count)                        \
  (type *)reallocate(pointer, sizeof(old_count) * (old_count),                   \
                     sizeof(new_count) * (new_count))

#define FREE_ARRAY(type, pointer, old_size) \
  reallocate(pointer, sizeof(type) * (old_size), 0)
  

void *reallocate(void *pointer, size_t old_size, size_t new_size);

#endif
