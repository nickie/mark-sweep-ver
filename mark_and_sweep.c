#include "alloc.h"
#include "gc.h"
#include "mark_and_sweep.h"

void dfs(word x) {
   word y;

   if ((IS_POINTER(x)) && (!(IS_MARKED(mem[POINTER_OF_WORD(x)])))) {

      word prev = null;
      word curr = POINTER_OF_WORD(x);
      mem[curr] = MARK(mem[curr]); 
      cnt++;

      mem[curr + 1] = WORD_OF_DATA(0);

      while(1) {
         word i = DATA_OF_WORD(mem[curr + 1]);
       //printf("curr = %d\n", curr);
         word size = DATA_OF_WORD(mem[curr]) - OBJ_HEADER_SIZE;
         if (i < size) {
            word t = mem[curr + OBJ_HEADER_SIZE + i];
          //printf("t = %d\n", t);
            if ((IS_POINTER(t)) && (!(IS_MARKED(mem[POINTER_OF_WORD(t)])))) {
             //printf("accessing %d field, which is pointer to address : (%d)\n", i, POINTER_OF_WORD(t));
               y = POINTER_OF_WORD(t);  
               mem[curr + OBJ_HEADER_SIZE + i] = prev;
             //printf("prev=%d curr=%d\n", prev, curr);
	       prev = curr;
               curr = y;
            // printf("prev=%d curr=%d\n", prev, curr);
             //printf("%d, %x\n", curr, mem[curr]);    
               mem[curr] = MARK(mem[curr]);
             //printf("%d, %x\n", curr, mem[curr]);
               mem[curr + 1] = WORD_OF_DATA(0);
               cnt++;
            }
            else {
             //printf("accesing %d field, which is data: (%d)\n", i, DATA_OF_WORD(t));
               mem[curr + 1] = WORD_OF_DATA((i + 1));
            }
         }
         else {
            y = curr;
            curr = prev;
          //printf("prev = %d curr = %d\n", prev, curr);
            if (curr == null) return;
            i = DATA_OF_WORD(mem[curr + 1]);
            prev = mem[curr + OBJ_HEADER_SIZE + i];
          //printf("prev = %d curr = %d\n", prev, curr);
            mem[curr + OBJ_HEADER_SIZE + i] = WORD_OF_POINTER(y);
	    mem[curr + 1] = WORD_OF_DATA((i + 1));
         }
      }
   }
}


void mark() {
  cnt = 0;
  word i;
  for(i = 0; i < 2; i++) {
     dfs(roots[i]);
  }
}

void sweep(void){
   word p = 0;
   while (p < (MEMORY_SIZE) - 1){
      word size = DATA_OF_WORD(mem[p]);
      // if next node not already swept, or not last free node, sweep it
      if ((mem[p + 1] != null) && (!(IS_POINTER(mem[p + 1])))) {
         if (IS_MARKED(mem[p])) mem[p] = UNMARK(mem[p]);
         else{
            deallocate(p);
            swept++;
         }
      }
      p += size;
   }
}
