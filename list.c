#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "alloc.h"
#include "gc.h"
#include "new.h"
#include "list.h"

word insert(word d, word list)
{
  root_add(&list);
  word n = new(2);
  // no need to add n to the rootset
  mem[n] = WORD_OF_DATA(d);
  mem[n+1] = WORD_OF_POINTER(list);
  root_pop(1);
  return n;
}

word list(word size)
{
  word i;
  word list = null;
  root_add(&list);
  for (i=0; i<size; i++)
    list = insert(i, list);
  root_pop(1);
  return list;
}

word append(word x, word y)
{
  // no need to keep track of rootset
  if (y == null) return x;
  if (x == null) return y;
  word p = x;
  while (POINTER_OF_WORD(mem[p+1]) != null)
    p = POINTER_OF_WORD(mem[p+1]);
  mem[p+1] = WORD_OF_POINTER(y);
  return x;
}

int length (word list)
{
  // no need to keep track of rootset
  int n = 0;
  while (list != null) {
    list = POINTER_OF_WORD(mem[list+1]);
    n++;
  }
  return n;
}

word reverse (word list)
{
  root_add(&list);
  word acc = null;
  root_add(&acc);
  while (list != null) {
    word d = DATA_OF_WORD(mem[list]);
    acc = insert(d, acc);
    list = POINTER_OF_WORD(mem[list+1]);
  }
  root_pop(2);
  return acc;  
}

word f (word x) {
  return x % 3 == 0;
}

word filter (word (*f)(word), word list)
{
  root_add(&list);
  word acc = null;
  root_add(&acc);
  while (list != null) {
    word d = DATA_OF_WORD(mem[list]);
    if (f(d) != 0)
      acc = insert(d, acc);
    list = POINTER_OF_WORD(mem[list+1]);
  }
  root_pop(2);
  return acc;
}

int equal(word x, word y)
{
  // no need to keep track of rootset
  while (x != null && y != null) {
    word dx = DATA_OF_WORD(mem[x]);
    word dy = DATA_OF_WORD(mem[y]);
    if (dx != dy) return 0;
    x = POINTER_OF_WORD(mem[x+1]);
    y = POINTER_OF_WORD(mem[y+1]);
  }
  return x == y;
}

void print (word list)
{
  // no need to keep track of rootset
  printf("[");
  while (list != null) {
    word d = DATA_OF_WORD(mem[list]);
    printf("(%d, %d)", list, d);
    list = POINTER_OF_WORD(mem[list+1]);
  }
  printf("]\n");
}
