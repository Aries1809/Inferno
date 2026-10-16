#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

#include "map.h"

void heap_dump(void *ptr){
  memhead *heap = ptr;
  uint8_t *temp;
  uint64_t offset = 0;
  uint64_t id = 1;

  fprintf(stdout, "\n- - - HEAP DUMP - - -\n\n");
  while (1){
    fprintf(stdout, "      %"PRIu64".      \n\n", id);
    fprintf(stdout, "offset: %"PRIu64"\n", offset);
    fprintf(stdout, "size: %"PRIu64"\n", heap->size);
    fprintf(stdout, "magic: %"PRIu8"\n", heap->magic);
    fprintf(stdout, "version: %"PRIu8"\n", heap->version);
    fprintf(stdout, "next: %"PRIu64"\n", heap->next);
    fprintf(stdout, "back: %"PRIu64"\n", heap->back);
    offset += heap->size;
    id++;
    if (!heap->next){
      fprintf(stdout, "\n- - - - - END - - - - -\n\n");
      break;
    }
    temp = (uint8_t *)heap + heap->next;
    heap = (memhead *)temp;
    fprintf(stdout, "\n- - - - - - - - - - -\n\n");
  }
}

void heap_check(void *ptr){
  memhead *heap = ptr;
  uint8_t *temp;
  uint64_t mem_cnt = 0;
  uint64_t msmch_cnt = 0;
  uint64_t mult_cnt = 0;
  uint64_t mag_cnt = 0;
  uint64_t b_check = 0;
  uint64_t b_cnt = 0;
  uint8_t f_check = 0;
  uint64_t f_cnt = 0;
  uint8_t err = 0;

  fprintf(stdout, "- - - HEAP CHECK - - -\n\n");
  while(1){
    mem_cnt += heap->size;
    if ((heap->size % 16) != 0){
      fprintf(stderr, "size is not a multiple of 16: %"PRIu64"\n", heap->size);
      mult_cnt += 1;
    }
    if ((heap->next % 16) != 0){
      fprintf(stderr, "next is not a multiple of 16: %"PRIu64"\n", heap->next);
      mult_cnt += 1;
    }
    if ((heap->next != heap->size) && (heap->next != 0)){
      fprintf(stderr, "next doesn't match size: %"PRIu64" - %"PRIu64"\n", heap->next, heap->size);
      msmch_cnt += 1;
    }
    if (heap->magic != USED && heap->magic != FREE){
      fprintf(stderr, "Invalid magic number: %"PRIu8"\n", heap->magic);
      mag_cnt += 1;
    }
    if (heap->back != b_check){
      fprintf(stderr, "Incorrect back value: %"PRIu64"\n", heap->back);
      b_cnt += 1;
    }
    if (heap->magic == FREE){
      if (f_check == 1){
        fprintf(stderr, "Free blocks next to each other\n");
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
    if (heap){
      if (!heap->magic){
        fprintf(stderr, "Incorrect next value... Terminating check\n");
        break;
      }
    } else{
        fprintf(stderr, "Incorrect next value... Terminating check\n");
        break; 
    }
  }
  if (mem_cnt != SPACE){
    fprintf(stderr, "Heap size doesn't match mmapped size: %"PRIu64"\n", mem_cnt);
    err = 1;
  }
  if (msmch_cnt){
    fprintf(stderr, "Total amount of next-size missmatches: %"PRIu64"\n", msmch_cnt);
    err = 1;
  }
  if (mult_cnt){
    fprintf(stderr, "Total amount of non 16 multiples: %"PRIu64"\n", mult_cnt);
    err = 1;
  }
  if (mag_cnt){
    fprintf(stderr, "Total amount of invalid magic numbers: %"PRIu64"\n", mag_cnt);
    err = 1;
  }
  if (b_cnt){
    fprintf(stderr, "Total amount of incorrect back values: %"PRIu64"\n", b_cnt);
    err = 1;
  }
  if (f_cnt){
    fprintf(stderr, "Total amount of free blocks next to each other: %"PRIu64"\n", f_cnt);
    err = 1;
  }
  if (!err)
    fprintf(stdout, "No memory management error was found\n");
  fprintf(stdout, "\n- - - - - END - - - - -\n\n");
  return;
}
