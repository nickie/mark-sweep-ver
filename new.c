#include "stdlib.h"
#include "stdio.h"
#include "alloc.h"
#include "gc.h"
#include "new.h"

word new (word size)
{
  //printf("before allocate: "); print_free();
  word p = allocate(size);
  //printf("after allocate: "); print_free();
  if (p == null) {
    gc();
    //printf("after gc: "); print_free();
    p = allocate(size);
    //printf("after second allocate: "); print_free();
    if (p == null) {
      fprintf(stderr, "Out of memory...\n");
      exit(-1);
    }
  }
  return p;
}
