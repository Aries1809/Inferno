#include <stdint.h>
#include <stddef.h>
#include <stdio.h>
#include <inttypes.h>

#include "map.h"

void *minos(uint32_t size){
  memhead *ptr = MEMORY;
  memhead *temp;
  uint8_t *conv;
  uint64_t alloc;

  if (size == 0){
    fprintf(stderr, "Error: Size must be greater than 0\n");
    return NULL;
  }
  if (((size + sizeof(memhead)) % 16) == 0)
    alloc = size + sizeof(memhead);
  else
    alloc = (((size + sizeof(memhead)) / 16) * 16) + 16;
  while (ptr->magic != FREE || ptr->size < alloc){
    if (ptr->version != VERSION){
      fprintf(stderr, "Error: Outdated header\n");
      return NULL;
    }

    if (ptr->next != 0){
      conv = (uint8_t *)ptr + ptr->next;
      ptr = (memhead *)conv;
    }
    else{
      fprintf(stderr, "Error: No suitable memory found\n");
      return NULL;
    }
  }

  uint64_t ptr_size = ptr->size;
  uint64_t next = ptr->next;

  ptr->magic = USED;
  ptr->size = alloc;
  ptr->next = alloc;

  conv = (uint8_t *)ptr + ptr->next;
  temp = (memhead *)conv;

  temp->magic = FREE;
  temp->back = alloc;
  temp->version = VERSION;
  if (next == 0)
    temp->next = next;
  else
    temp->next = next - alloc;
  temp->size = ptr_size - ptr->size;

  conv = (uint8_t *)ptr + sizeof(memhead);
  return (conv);
}
