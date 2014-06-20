#include "alloc.h"
#include "gc.h"
#include "mark_and_sweep.h"

void dfs (word x)
{
  if (x == null || IS_MARKED(mem[x-OBJ_HEADER_SIZE])) return;

  word prev = null;
  word curr = x-OBJ_HEADER_SIZE;
  mem[curr] = MARK(mem[curr]);
  cnt++;

  mem[curr + 1] = WORD_OF_DATA(0);

  for (;;) {
    word i = DATA_OF_WORD(mem[curr + 1]);
    word size = DATA_OF_WORD(mem[curr]) - OBJ_HEADER_SIZE;
    word y;
    if (i < size) {
      word t = mem[curr + OBJ_HEADER_SIZE + i];
      if (IS_POINTER(t)) {
        y = POINTER_OF_WORD(t);
        if (!IS_MARKED(mem[POINTER_OF_WORD(t)-OBJ_HEADER_SIZE])) {
          mem[curr + OBJ_HEADER_SIZE + i] = prev;
          prev = curr;
          curr = y-OBJ_HEADER_SIZE;
          mem[curr] = MARK(mem[curr]);
          mem[curr + 1] = WORD_OF_DATA(0);
          cnt++;
          continue;
        }
      }
      mem[curr + 1] = WORD_OF_DATA(i+1);
    }
    else {
      y = curr;
      curr = prev;
      if (curr == null) return;
      i = DATA_OF_WORD(mem[curr + 1]);
      prev = mem[curr + OBJ_HEADER_SIZE + i];
      mem[curr + OBJ_HEADER_SIZE + i] = WORD_OF_POINTER(y + OBJ_HEADER_SIZE);
      mem[curr + 1] = WORD_OF_DATA(i+1);
    }
  }
}

void mark ()
{
  cnt = 0;
  word i;
  for (i=0; i<rootp; i++)
    dfs(*root[i]);
}

void sweep ()
{
  swept = 0;
  word p = 0;
  while (p < MEMORY_SIZE) {
    word size = DATA_OF_WORD(mem[p]);
    // if next node not already swept, or not last free node, sweep it
    if ((mem[p+1] != null) && (!(IS_POINTER(mem[p+1])))) {
      if (IS_MARKED(mem[p]))
        mem[p] = UNMARK(mem[p]);
      else {
        deallocate(p);
        swept++;
      }
    }
    p += size;
  }
}
