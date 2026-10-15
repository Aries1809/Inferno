#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

#include "map.h"

void *init_memhead(void *ptr){
  memhead *head = ptr;
  if (head->magic != 0 && head->version == VERSION){
    printf("\nMemory file found: Skipping init\n");
    return ptr;
  }
  printf("\nMemory file not found: Creating memory...\n");
  head->size = SPACE;
  head->magic = FREE;
  head->version = VERSION;
  head->next = 0;
  head->back = 0;

  return(ptr);
}

void *memory_init(void){
  char *path = malloc(1024);
  getcwd(path, 1024);
  strlcat(path, "/memory", 1024);
  int32_t fd = open(path,
      O_CREAT | O_RDWR, 00666);
  free(path);
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
  uint8_t *ptr = MEMORY;
  if (!ptr){
    return 1;
  }
  printf("\nmemory address: %p\n\n", ptr);
  minos(100);
  minos(144);
  minos(10000);
  minos(0);
  heap_dump(ptr);
  printf("\n");
  heap_check(ptr);
  return 0;
}
