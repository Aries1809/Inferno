#pragma once

#include <stdint.h>

#define MEMORY memory_init();
#define SPACE (uint64_t)(1024 * 1024 * 256)
#define FREE 33
#define USED 11
#define VERSION 00

typedef struct memhead {
  uint64_t size;
  uint8_t magic;
  uint8_t version;
  uint64_t next;
  uint64_t back;
} memhead;

void *memory_init(void);
void heap_dump(void *ptr);
void heap_check(void *ptr);
void *minos(uint32_t size);
