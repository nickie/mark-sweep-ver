#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "gc.h"
#include "list.h"

int main(int argc, char **argv){
   word i;
   srand(time(NULL));

   mem = (word *)malloc((MEMORY_SIZE) * sizeof(word));

   mem[0] = WORD_OF_DATA(MEMORY_SIZE);
   mem[1] = null;
   freeStart = 0;

   word l = list(1023);
   word fl = reverse(l);
   printf("ok\n");
   print(l);

   i = 0;
   do {
      fl = reverse(fl);
      i++;
   } while (i < 10000000);
   print(fl);
   return 0;
}



