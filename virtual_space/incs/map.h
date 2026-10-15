#pragma once

#include <stdint.h>

#define SPACE 1024 * 1024 * 256
#define FREE 33
#define USED 11
#define VERSION 00

typedef struct memhead {
  uint64_t size;
  uint8_t magic;
  uint8_t version;
  int64_t *next;
  int64_t *back;
} memhead;
