#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alloc.h"
#include "gc.h"
#include "new.h"
#include "list.h"

word insert(word d, word list){
   word n = new(2);
   if (n != null) {
      mem[n] = WORD_OF_DATA(4);
      mem[n + 1] = WORD_OF_DATA(0);
      mem[n + OBJ_HEADER_SIZE] = WORD_OF_DATA(d);
      if (list!= null) mem[n + OBJ_HEADER_SIZE + 1] = WORD_OF_POINTER(list);
      else mem[n + OBJ_HEADER_SIZE + 1] = null;
      return n;
   }
   exit(-1);
}

word list(word size){
   word list = insert(rnd, null);
   word i = 0;
   while(i < size) {
      word n = insert(++rnd, list);
      list = n;
      roots[0] = WORD_OF_POINTER(list);
      i++;
   }
   return list;
}

word append(word x, word y){

   if (y == null) return x;
   if (x == null) return y;

   roots[0] = WORD_OF_POINTER(x);
   roots[1] = WORD_OF_POINTER(y);

   word lag = null;
   word p = x;
   while (p != null) {
      lag = p;
      word temp = mem[p + OBJ_HEADER_SIZE + 1];
      if (temp != null) p = POINTER_OF_WORD(temp);
      else p = null;
   }
   mem[lag + OBJ_HEADER_SIZE + 1] = WORD_OF_POINTER(y);
   return x;
}

word length_aux(word list, word acc){
   word next;
   if (list == null) return acc;
   else{
      word temp = mem[list + OBJ_HEADER_SIZE + 1];
      if (temp != null) next = POINTER_OF_WORD(temp);
      else next = null;
      return length_aux(next, acc + 1);
  }
}

word length(word list){
   roots[0] = WORD_OF_POINTER(list); 
   return length_aux(list, 0);
}

word reverse_aux(word list, word acc){
   word next;
   if (list == null) return acc;
   else{
      word d = DATA_OF_WORD(mem[list + OBJ_HEADER_SIZE]);
      word temp = mem[list + OBJ_HEADER_SIZE + 1];
      if (temp != null) next = POINTER_OF_WORD(temp);
      else next = null;
      word nacc = insert(d, acc);
      roots[1] = WORD_OF_POINTER(nacc);
      return reverse_aux(next, nacc);
   }
}

word reverse(word list) {
   roots[0] = WORD_OF_POINTER(list);
   return reverse_aux(list, null);
}

word f(word x){
  if (x % 3 == 0) return 1; 
  else return 0;
}

word filter_aux(word (*f)(word), word list, word acc){
   word next, nacc;
   if (list == null) return acc;
   else{
      word d = DATA_OF_WORD(mem[list + OBJ_HEADER_SIZE]);
      word temp = mem[list + OBJ_HEADER_SIZE + 1];
      if (temp != null) next = POINTER_OF_WORD(temp);
      else next = null;
      if (f(d) == 1) nacc = insert(d, acc);
      else nacc = acc;
      roots[1] = WORD_OF_POINTER(nacc);
      return filter_aux(f, next, nacc);
   }
}

word filter(word (*f)(word), word list) {
   roots[0] = WORD_OF_POINTER(list);
   return filter_aux(f, list, null);
}

void print(word list){
   printf("[");

   word y = list;
   while(y != null){
      word d = DATA_OF_WORD(mem[y + OBJ_HEADER_SIZE]);
      printf("(%d, %d)", y, d);
      word temp = mem[y + OBJ_HEADER_SIZE + 1];
      if (temp != null) {
         y = POINTER_OF_WORD(mem[y + OBJ_HEADER_SIZE + 1]);
         printf(", ");
      }
      else y = null;
   }
   printf("]\n");
   return;
}

void print_free(void){
   word list = freeStart;
   printf("free: [");

   word y = list;
   while(y != null){
      word x = DATA_OF_WORD(mem[y]);
      printf("(%d, %d)", y, x);
      word temp = mem[y + 1];
      if (temp != null) { printf(", "); y = POINTER_OF_WORD(mem[y + 1]); }
      else y = null;
   }
   printf("]\n");
   return;
}

/*
word filter2(word list){
   word temp;

   if (list == null) return null;

   word x = DATA_OF_WORD((mem[list + OBJ_HEADER_SIZE]));
   word y = list;

   while ((x % 3 != 0) && (y != null)) {
      temp = mem[y + OBJ_HEADER_SIZE + 1];
      if (temp != null) {
         y = POINTER_OF_WORD(temp);
         x = DATA_OF_WORD((mem[y + OBJ_HEADER_SIZE]));
      }
      else y = null;
   }
   if (y == null) {
//printf("filter %d : returning %d\n", list, null);
      return null;
   }
   else {
      temp = mem[y + OBJ_HEADER_SIZE + 1];
      if (temp != null) {
         word next = POINTER_OF_WORD(temp);
         word res = filter2(next);
         if (res != null) mem[y + OBJ_HEADER_SIZE + 1] = WORD_OF_POINTER(res);
         else mem[y + OBJ_HEADER_SIZE + 1] = null;
         //printf("filter %d : returning %d\n", list, y);
         return y;
      }
      else {
         //printf("filter %d : fff returning %d\n", list, y);
         return y;
      }
   }
}
 */


