#include <inttypes.h>
#include <strings.h>
#include <stdio.h>

#include "map.h"

void unity(memhead *home){
  void *init_b = (void *)home - home->back;
  void *init_n = (void *)home + home->next;
  memhead *left = init_b;
  memhead *right = init_n;
  uint8_t check = 0;
  uint8_t l_check = 0;
  uint8_t r_check = 0;
  uint64_t size = home->size;

  if (home->back == 0){
    l_check = 1;
  }
  if (home->next == 0){
    r_check = 1;
  }
  while (left->magic == FREE && !l_check){
    check = 1;
    if (!left->back){
      l_check = 1;
    }
    size += left->size;
    left = (void *)left - left->back;
  }
  while (right->magic == FREE && !r_check){
    size += right->size;
    if (!right->next){
      r_check = 1;
    }
    right = (void *)right + right->back;
  }
  if (check){
    left->size = size;
    bzero((void *)left + sizeof(memhead), size - sizeof(memhead));
    if (right->next)
      left->next = left->size;
    else
     left->next = 0;
  }
}

int64_t purge(void *ptr){
  if (!ptr){
    fprintf(stderr, "Error: Invalid pointer\n");
    return (-1);
  }
  memhead *start = ptr - sizeof(memhead);

  bzero(ptr, (start->size - sizeof(memhead)));
  start->magic = FREE;
  unity(start);
  return(start->size - sizeof(memhead));
}
