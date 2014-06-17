#include "gc.h"
#include "mark_and_sweep.h"

word null = 0xFFFFFFFFU;

word roots[2];

word *mem;
word freeStart;

word marked;

word cnt;
word swept;

word rnd = 0;

void gc() {
   mark();
   sweep();
}
