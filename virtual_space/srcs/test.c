#include <inttypes.h>
#include <stdio.h>

#include "map.h"

int main(void){
  uint8_t *ptr = MEMORY;
  if (!ptr){
    return 1;
  }
  void *min_ret1 = minos(100);
  void *min_ret2 = minos(144);
  void *min_ret3 = minos(10000);
  void *min_ret4 = minos(0);
  printf("\n");
  heap_dump(ptr);
  heap_check(ptr);
  purge(min_ret1);
  purge(min_ret2);
  purge(min_ret3);
  purge(min_ret4);
  printf("\n");
  heap_dump(ptr);
  heap_check(ptr);
  return 0;
}
