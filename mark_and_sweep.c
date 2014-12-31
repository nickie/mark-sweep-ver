#include "alloc.h"
#include "gc.h"
#include "mark_and_sweep.h"

/*@ requires mem_sanity;
  @ requires  
  @  (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && reachable(root, o) ==> ! IS_MARKED(mem[o])
  @  );
  @ requires root == null || 0 <= root < MEMORY_SIZE;
  @ ensures mem_sanity;
  @ ensures
  @  (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && reachable(root, o)
  @   ==> IS_MARKED(mem[o])
  @  );
  @  ensures
  @  (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && ! reachable(root, o)
  @   ==> mem[o] == \at(mem[o], Old)
  @  );
  @  ensures
  @   (\forall integer o, s;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && size_obj(o, s)
  @   ==> \forall integer i;
  @          0 <= i < s - OBJ_HEADER_SIZE
  @       ==> mem[o + OBJ_HEADER_SIZE + i] == \at(mem[o + OBJ_HEADER_SIZE + i], Old)
  @   );
  @*/

void dfs (word root)
{
  if (root == null || IS_MARKED(mem[root-OBJ_HEADER_SIZE])) return;

  word prev = null;
  word curr = root - OBJ_HEADER_SIZE;
  mem[curr] = MARK(mem[curr]);
  cnt++;

  mem[curr + 1] = WORD_OF_DATA(0);

/*@ loop invariant
  @   (\forall integer o;
  @     valid_obj(o, 0, MEMORY_SIZE)
  @   ==> reachable{Pre}(root, o)
  @   ==> reachable(curr, o) || reachable(prev, o));
  @
  @  loop invariant
  @    (\forall integer root, o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @    ==> (reachable(curr, o) || reachable(prev, o)) ==> reachable{Pre}(root, o));
  @
  @  loop invariant
  @    (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @     ==> ! reachable{Pre}(root, o)
  @     ==> \at(mem[o], Here) == \at(mem[o], Pre));
  @
  @  loop invariant
  @    (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @      && in_stack(o, prev)
  @      ==> IS_MARKED(mem[o]));
  @
  @  loop invariant
  @    (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @      && reachable{Pre}(root, o)
  @      && !IS_MARKED(mem[o])
  @      ==> unmarked_reachable(curr, o) 
  @         || \exists integer q; 
  @            in_stack(q, prev) 
  @         ==> unmarked_reachable(q, o));
  @
  @  loop invariant
  @    (\forall integer o, s;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @      && ! in_stack(o, prev)
  @      ==> same_obj{Pre, Here}(o));
  @
  @  loop invariant
  @    (\forall integer o0, o1, s1, i1;
  @      valid_obj(o0, 0, MEMORY_SIZE)
  @      && valid_obj(o1, 0, MEMORY_SIZE)
  @      && adj_in_stack(o0, o1, prev)
  @      ==> size_obj(o1, s1)
  @      ==> i1 == DATA_OF_WORD(\at(mem[o1 + 1], Here))
  @      ==> \at(mem[o1 + OBJ_HEADER_SIZE + i1], Pre) == WORD_OF_POINTER(o0 + OBJ_HEADER_SIZE)
  @          && \forall integer j1;
  @               0 <= j1 < s1 - OBJ_HEADER_SIZE && j1 != i1
  @               ==> \at(mem[o1 + OBJ_HEADER_SIZE + j1], Pre) == \at(mem[o1 + OBJ_HEADER_SIZE + j1], Here));
  @
  @  loop invariant mem_sanity;
  @  loop variant count(root, 0, MEMORY_SIZE);
  @*/

  for (;;) {
    word i = DATA_OF_WORD(mem[curr + 1]);
    word size = DATA_OF_WORD(mem[curr]) - OBJ_HEADER_SIZE;
    word y;
    if (i < size) {
      word t = mem[curr + OBJ_HEADER_SIZE + i];
      if (IS_POINTER(t)) {
        y = POINTER_OF_WORD(t);
        if (!IS_MARKED(mem[y - OBJ_HEADER_SIZE])) {
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

/*@ requires mem_sanity;
  @ requires
  @  (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && \exists integer r; 0 <= r < ROOT_SIZE ==> reachable(*root[r] - OBJ_HEADER_SIZE, o) ==> ! IS_MARKED(mem[o])
  @  );
  @ requires
  @  (\forall integer r;
  @     0 <= r < ROOT_SIZE
  @   ==> *root[r] == null || 0 <= *root[r] < MEMORY_SIZE
  @  );
  @ ensures mem_sanity;
  @ ensures
  @  (\forall integer o;
  @     valid_obj(o, 0, MEMORY_SIZE)
  @   ==> \exists integer r; 0 <= r < ROOT_SIZE ==> reachable(*root[r], o)
  @   ==> IS_MARKED(mem[o])
  @  );
  @ ensures
  @  (\forall integer o;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   ==> \forall integer r; 0 <= r < ROOT_SIZE ==> ! reachable(*root[r], o)
  @   ==> mem[o] == \at(mem[o], Old)
  @  );
  @  ensures
  @   (\forall integer o, s;
  @      valid_obj(o, 0, MEMORY_SIZE)
  @   && size_obj(o, s)
  @   ==> \forall integer i;
  @          0 <= i < s - OBJ_HEADER_SIZE
  @       ==> mem[o + OBJ_HEADER_SIZE + i] == \at(mem[o + OBJ_HEADER_SIZE + i], Old)
  @   );
  @*/

void mark ()
{
  cnt = 0;
  word i;

/*@ loop invariant mem_sanity;
  @ loop invariant
  @  (\forall integer j;
  @     0 <= j < i
  @   ==> \forall integer o;
  @         valid_obj(o, 0, MEMORY_SIZE)
  @       ==> reachable(*root[j], o)
  @       ==> IS_MARKED(mem[o])
  @  );
  @ loop invariant
  @  (\forall integer j;
  @     0 <= j < i
  @   ==> \forall integer o;
  @         valid_obj(o, 0, MEMORY_SIZE)
  @       ==> ! reachable(*root[j], o)
  @       ==> mem[o] == \at(mem[o], Pre)
  @  );
  @ loop variant rootp-i;
  @*/
  for (i=0; i<rootp; i++)
    dfs(*root[i]);
}

/*@ requires mem_sanity;
  @ requires
  @  (\forall integer o;
  @     valid_obj(o, 0, MEMORY_SIZE - 1)
  @   ==> \forall integer r;
  @         0 <= r < ROOT_SIZE
  @       ==> reachable(*root[r] - OBJ_HEADER_SIZE, o)
  @       ==> IS_MARKED(mem[o])
  @  );
  @ requires 0 <= freeStart < MEMORY_SIZE - 1;
  @ ensures mem_sanity;
  @ ensures
  @  (\forall integer o;
  @     valid_obj(o, 0, MEMORY_SIZE)
  @   ==> (IS_MARKED(\at(mem[o], Pre)) ==> ! IS_MARKED(\at(mem[o], Here)))
  @   &&  (! IS_MARKED(\at(mem[o], Pre)) ==> free_obj(o, freeStart))
  @  );
  @ ensures 0 <= freeStart < MEMORY_SIZE;
  @*/

void sweep ()
{
  swept = 0;
  word p = 0;

/*@ loop invariant mem_sanity;
  @ loop invariant
  @  (\forall integer o;
  @     valid_obj(o, 0, p)
  @   ==> (IS_MARKED(\at(mem[o], Pre)) ==> ! IS_MARKED(\at(mem[o], Here)))
  @   && ! IS_MARKED(\at(mem[o], Pre)) ==> free_obj(o, freeStart)
  @  );
  @ loop variant MEMORY_SIZE - p;
  @*/
  while (p < MEMORY_SIZE) {
    word size = DATA_OF_WORD(mem[p]);
    // if next node not already swept, or not last free node, sweep it
    if ((mem[p+1] != null) && (!(IS_POINTER(mem[p+1])))) {
      if (IS_MARKED(mem[p]))
        mem[p] = UNMARK(mem[p]);
      else {
        //deallocate(p);
        mem[p+1] = WORD_OF_POINTER(freeStart);
        freeStart = p;
        swept++;
      }
    }
    p += size;
  }
}
