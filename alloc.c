#include "gc.h"
#include "alloc.h"

word allocate(word requestSize){
   word temp, nextFree;

   word size = requestSize + 2;
   word p = freeStart;
   word lag = null;

   while((p != null) && (DATA_OF_WORD(mem[p]) < size)){
      lag = p;
      temp = mem[p + 1];
      if (temp != null) p = POINTER_OF_WORD(temp);
      else p = null;
   }
   if (p == null) return null;

   temp = mem[p + 1];
   if (temp != null) nextFree = POINTER_OF_WORD(mem[p + 1]);
   else nextFree = null; 

   word unused = DATA_OF_WORD(mem[p]) - size; 
   if (unused > 2) {
      nextFree = p + size;               
      mem[nextFree] = WORD_OF_DATA(unused);
      mem[nextFree + 1] = mem[p + 1];
   }
   if (lag == null) freeStart = nextFree;
   else {
      if (nextFree != null) mem[lag + 1] = WORD_OF_POINTER(nextFree);
      else mem[lag + 1] = null;
   }
   return p;
}

void deallocate(word address){
   word addr = address;
   if (freeStart != null) mem[addr + 1] = WORD_OF_POINTER(freeStart);
   else mem[addr + 1] = null;
   freeStart = addr;
}

/*
void deallocate2(word address){
 //printf("deallocating2, at address %d\n", address);
   word addr = address;

   word p = freeStart;
   word lag = null;

   word addrSize = DATA_OF_WORD(mem[addr]);
   for(int i=0; i<addrSize - 2; i++) mem[addr + OBJ_HEADER_SIZE + i] = 0;

   while((p != null) && (p < addr)) {
    //printf("i m looking for insertion point\n");
      lag = p;
      word temp = mem[p + 1];
      if (temp != null) p = POINTER_OF_WORD(temp);
      else p = null;
   }
   if (addr + addrSize == p) {
      word pSize = DATA_OF_WORD(mem[p]);
      addrSize += pSize;
      mem[addr] = WORD_OF_DATA((addrSize));
      word temp = mem[p + 1];
      if (temp != null){
         p = POINTER_OF_WORD(temp);
      }
      else{
         mem[p] = 0;
         mem[p + 1] = 0;
         p = null;
      }
//      printf("nextFree is adjacent to currently being freed %d %d\n", addrSize, DATA_OF_WORD(mem[addr]));
   }

   if (lag == null) {
//      printf("this is going to be the first one\n");
      freeStart = addr;
      if (p != null) mem[addr + 1] = WORD_OF_POINTER(p);
      else mem[addr + 1] = null;
   }
   else{
      word lagSize = DATA_OF_WORD(mem[lag]);
      if (lag + lagSize == addr) {
//       printf("this is adjacent to previous free, addr >? %d lag >? %d\n", addrSize, lagSize);
         mem[addr] = 0;
         mem[addr + 1] = 0;
         lagSize += addrSize;
         mem[lag] = WORD_OF_DATA((lagSize));
         if (p != null) mem[lag + 1] = WORD_OF_POINTER(p);
         else mem[lag + 1] = null;
      }
      else{
//       printf("this is inserted in between, normally\n");
         mem[lag + 1] = WORD_OF_POINTER(addr);
         if (p != null) mem[addr + 1] = WORD_OF_POINTER(p);
         else mem[addr + 1] = null;
      }
   }
}
*/
