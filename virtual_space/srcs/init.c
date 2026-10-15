#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdint.h>

void  *memory_init(void){
  int32_t size = 1024 * 1024 * 256;
  int32_t fd = open("/home/kseltenr/nerdstuff/projects/c/Inferno/memory",
      O_CREAT | O_RDWR, 00666);
  if (fd == -1){
    perror("open");
    return NULL;
  }
  if (ftruncate(fd, size) == -1){
    perror("ftruncate");
    close(fd);
    return NULL;
  }
  void *ret = mmap(NULL, size,
      PROT_READ | PROT_WRITE,
      MAP_SHARED, fd, 0);
  if (ret == MAP_FAILED){
    perror("mmap");
    close(fd);
    return NULL;
  }
  close(fd);
  return(ret);
}

int main(void){
  char *ptr = memory_init();
  if (!ptr){
    return 1;
  }
  for(int i = 0; i < 100; i++){
    printf("Before: %c      ", ptr[i]);
    ptr[i] = 'w';
    printf("After: %c\n", ptr[i]);
  }
  msync(ptr, 1024 * 1024 * 256, MS_SYNC);
  return 0;
}
