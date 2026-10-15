#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "map.h"

void heap_dump(void *ptr){
  memhead *heap = ptr;
  uint8_t *temp;

  printf("\n- - - HEAP DUMP - - -\n\n");
  while (1){
    printf("size: %"PRIu64"\n", heap->size);
    printf("magic: %"PRIu8"\n", heap->magic);
    printf("version: %"PRIu8"\n", heap->version);
    printf("next: %"PRIu64"\n", heap->next);
    printf("back: %"PRIu64"\n", heap->back);
    if (!heap->next){
      printf("\n- - - - - END - - - - -\n\n");
      break;
    }
    temp = (uint8_t *)heap + heap->next;
    heap = (memhead *)temp;
    printf("\n- - - - - - - - - - -\n\n");
  }
}

void heap_check(void *ptr){
  memhead *heap = ptr;
  uint8_t *temp;
  uint64_t mem_cnt = 0;
  uint64_t mult_cnt = 0;
  uint64_t mag_cnt = 0;
  uint64_t b_check = 0;
  uint64_t b_cnt = 0;
  uint8_t f_check = 0;
  uint64_t f_cnt = 0;
  uint8_t err = 0;

  printf("- - - HEAP CHECK - - -\n\n");
  while(1){
    mem_cnt += heap->size;
    if ((heap->size % 16) != 0){
      printf("size is not a multiple of 16: %"PRIu64"\n", heap->size);
      mult_cnt += 1;
    }
    if ((heap->next % 16) != 0){
      printf("next is not a multiple of 16: %"PRIu64"\n", heap->next);
      mult_cnt += 1;
    }
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
    b_check = heap->size;
    if (!heap->next)
      break;
    temp = (uint8_t *)heap + heap->next;
    heap = (memhead *)temp;
    if (!heap->magic){
      printf("Incorrect next value... Terminating check\n");
      break;
    }
  }
  if (mem_cnt != SPACE){
    printf("Heap size doesn't match mmapped size: %"PRIu64"\n", mem_cnt);
    err = 1;
  }
  if (mult_cnt){
    printf("Total amount of non 16 multiples: %"PRIu64"\n", mult_cnt);
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
    printf("No memory management error was found\n");
  printf("\n- - - - - END - - - - -\n\n");
  return;
}
