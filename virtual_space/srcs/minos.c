#include <stdint.h>
#include <stddef.h>

#include "map.h"

void *minos(uint32_t size){
  memhead *ptr = memory_init();
  memhead *temp;
  uint8_t *conv;
  uint32_t alloc;

  if (!size)
    return NULL;
  if (((size + sizeof(memhead)) % 16) == 0)
    alloc = size + sizeof(memhead);
  else
    alloc = (((size + sizeof(memhead)) / 16) * 16) + 16;
  while (ptr->magic != FREE || ptr->size < alloc){
    if (ptr->version != VERSION)
      return NULL;

    if (ptr->next != 0){
      conv = (uint8_t *)ptr + ptr->next;
      ptr = (memhead *)conv;
    }
    else
      return NULL;
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
