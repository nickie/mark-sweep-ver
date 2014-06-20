#include "gc.h"
#include "alloc.h"

word allocate (word size)
{
  size += OBJ_HEADER_SIZE;
  word nextFree;
  word p = freeStart;
  word lag = null;

  while ((p != null) && (DATA_OF_WORD(mem[p]) < size)){
    lag = p;
    p = POINTER_OF_WORD(mem[p+1]);
  }
  if (p == null) return null;

  word unused = DATA_OF_WORD(mem[p]) - size;
  if (unused > OBJ_HEADER_SIZE) {
    nextFree = p+size;
    mem[nextFree] = WORD_OF_DATA(unused);
    mem[nextFree+1] = mem[p+1];
  }
  else {
    size += unused;
    nextFree = POINTER_OF_WORD(mem[p+1]);
  }

  if (lag == null)
    freeStart = nextFree;
  else
    mem[lag+1] = WORD_OF_POINTER(nextFree);
  mem[p] = WORD_OF_DATA(size);
  mem[p + 1] = WORD_OF_DATA(0);
  // initialize the object
  word i;
  for (i = OBJ_HEADER_SIZE; i < size; i++)
    mem[p + i] = null;
  return p + OBJ_HEADER_SIZE;
}

void deallocate (word address)
{
  // this version does not fix fragmentation
  mem[address+1] = WORD_OF_POINTER(freeStart);
  freeStart = address;
}
