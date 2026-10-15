#pragma once

#include <stdint.h>

#define FREE 3333
#define USED 1111

typedef struct memhead {
  uint64_t size;
  uint8_t magic;
  uint8_t version;
  void *next;
  void *back;
} memhead;
