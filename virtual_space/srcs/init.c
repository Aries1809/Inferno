#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "map.h"

void *init_memhead(void *ptr){
  memhead *head = ptr;
  if (head->magic != 0 && head->version == VERSION)
    return ptr;
  head->size = SPACE;
  head->magic = FREE;
  head->version = VERSION;
  head->next = 0;
  head->back = 0;

  return(ptr);
}

void *memory_init(void){
  int32_t fd = open("/home/kseltenr/nerdstuff/projects/c/Inferno/memory",
      O_CREAT | O_RDWR, 00666);
  if (fd == -1){
    perror("open");
    return NULL;
  }
  if (ftruncate(fd, SPACE) == -1){
    perror("ftruncate");
    close(fd);
    return NULL;
  }
  void *ret = mmap(NULL, SPACE,
      PROT_READ | PROT_WRITE,
      MAP_SHARED, fd, 0);
  if (ret == MAP_FAILED){
    perror("mmap");
    close(fd);
    return NULL;
  }
  close(fd);
  ret = init_memhead(ret);
  return(ret);
}

int main(void){
  char *ptr = memory_init();
  if (!ptr){
    return 1;
  }
  heap_dump(ptr);
  printf("\n");
  heap_check(ptr);
  return 0;
}
