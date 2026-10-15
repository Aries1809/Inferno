#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "map.h"

void heap_dump(void *ptr){
  memhead *heap = ptr;

  while (1){
    printf("size: %"PRIu64"\n", heap->size);
    printf("magic: %"PRIu8"\n", heap->magic);
    printf("version: %"PRIu8"\n", heap->version);
    printf("next: %"PRIu64"\n", heap->next);
    printf("back: %"PRIu64"\n", heap->back);
    if (!heap->next)
      break;
    heap += heap->next;
    printf("\n");
  }
}

void heap_check(void *ptr){
  memhead *heap = ptr;
  uint64_t mem_cnt = 0;
  uint64_t mag_cnt = 0;
  uint64_t b_check = 0;
  uint64_t b_cnt = 0;
  uint8_t f_check = 0;
  uint64_t f_cnt = 0;
  uint8_t err = 0;

  while(1){
    mem_cnt += heap->size;
    if (heap->magic != USED && heap->magic != FREE){
      printf("Invalid magic number: %"PRIu8"\n", heap->magic);
      mag_cnt += 1;
    }
    if (heap->back != b_check){
      printf("Incorrect back value: %"PRIu64"\n", heap->back);
      b_cnt += 1;
    }
    if (heap->magic == FREE){
      if (f_check == 1){
        printf("Free blocks next to each other\n");
        f_cnt += 1;
      }
      else{
        f_check = 1;
      }
    }
    else{
      f_check = 0;
    }
    if (!heap->next)
      break;
    heap += heap->next;
  }
  if (mem_cnt != SPACE){
    printf("Heap size doesn't match mmapped size: %"PRIu64"\n", mem_cnt);
    err = 1;
  }
  if (mag_cnt){
    printf("Total amount of invalid magic numbers: %"PRIu64"\n", mag_cnt);
    err = 1;
  }
  if (b_cnt){
    printf("Total amount of incorrect back values: %"PRIu64"\n", b_cnt);
    err = 1;
  }
  if (f_cnt){
    printf("Total amount of free blocks next to each other: %"PRIu64"\n", f_cnt);
    err = 1;
  }
  if (!err)
    printf("---No memory management error was found---\n");
  return;
}
