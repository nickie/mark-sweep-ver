#include "stdlib.h"
#include "stdio.h"
#include "alloc.h"
#include "gc.h"
#include "new.h"

word new(word size){
   word p = allocate(size);
   if (p == null) {
      gc();
      p = allocate(size);
      if (p == null) {
         printf("Out of memory...\n");
         exit(-1);
      }
      return p;
   }
   return p;
}
