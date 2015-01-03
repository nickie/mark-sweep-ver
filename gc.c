#include <stdio.h>
#include "gc.h"
#include "mark_and_sweep.h"

word * root[ROOT_SIZE];
word rootp;

word mem[MEMORY_SIZE];
word freeStart;

word marked;

word cnt;
word swept;

void mem_init()
{
   mem[0] = WORD_OF_DATA(MEMORY_SIZE);
   mem[1] = WORD_OF_POINTER(null);
   freeStart = 0;
   rootp = 0;
}

void gc() {
  printf("\nGC!\n");
  mark();
  sweep();
}

void root_add (word *p)
{
  assert(rootp < ROOT_SIZE);
  root[rootp++] = p;
}

void root_pop (int n)
{
  assert(n <= rootp);
  rootp -= n;
}

void print_free ()
{
  // no need to keep track of rootset
  word list = freeStart;
  printf("free: [");
  while (list != null) {
    word x = DATA_OF_WORD(mem[list]);
    printf("(%d, %d)", list, x);
    list = POINTER_OF_WORD(mem[list+1]);
  }
  printf("]\n");
}
