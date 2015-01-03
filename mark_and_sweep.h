#ifndef __MARK_AND_SWEEP_H__
#define __MARK_AND_SWEEP_H__

#include "gc.h"

// Object at address p has size size..

/*@ predicate size_obj{L}(integer p, integer size) =
  @   \forall integer p, size;
  @     IS_DATA(\at(mem[p], L))
  @   ==> size == DATA_OF_WORD(\at(mem[p], L));
  @*/

// There are no spaces among the objects in range [a,b]..

/*@ inductive compact_mem{L}(integer a, integer b) {
  @   case compact_mem0{L}:
  @     \forall integer a;
  @        compact_mem{L}(a, a);
  @   case compact_mem1{L}:
  @     \forall integer a, s, b;
  @       size_obj(a, s)
  @     ==> compact_mem{L}(a+s, b)
  @     ==> compact_mem{L}(a, b);
  @ }
  @*/

// Object at place p is a valid object in range [a,b]..

/*@ inductive valid_obj{L}(integer p, integer a, integer b) {
  @   case obj0{L}:
  @     \forall integer a, s, b;
  @       size_obj(a, s)
  @     ==> compact_mem{L}(a+s, b) 
  @     ==> valid_obj{L}(a, a, b);
  @   case obj1{L}:
  @     \forall integer a, s, p, b;
  @       size_obj{L}(a, s)
  @     ==> valid_obj{L}(p, a+s, b)
  @     ==> valid_obj{L}(p, a, b);
  @ }
  @*/

// Object at address p is in the freeList..

/*@ inductive free_obj{L}(integer p, integer fl) {
  @   case free_obj0{L}:
  @     \forall integer p;
  @       valid_obj(p, 0, MEMORY_SIZE)
  @     ==> free_obj{L}(p, p);
  @   case free_obj1{L}:
  @     \forall integer p, t, q;
  @       valid_obj{L}(t, 0, MEMORY_SIZE)
  @     ==> IS_POINTER(\at(mem[t + 1], L))
  @     ==> q == POINTER_OF_WORD(\at(mem[t + 1], L))
  @     ==> free_obj{L}(p, q)
  @     ==> free_obj{L}(p, t);
  @ }
  @*/

//Object at adress o is has the same contents in states L1 and L2..
/*@ predicate same_obj{L1, L2}(integer p) =
  @   \forall integer p, s, i;
  @     size_obj{L1}(p, s)
  @   ==> 0 <= i < s - OBJ_HEADER_SIZE
  @   ==> \at(mem[p + OBJ_HEADER_SIZE + i], L1) == \at(mem[p + OBJ_HEADER_SIZE + i], L2);
  @*/

// TODO documentation
/*@ inductive scan_mem{L}(integer a, integer b, integer inuse, integer free) {
  @   case scan_mem0{L}:
  @     \forall integer a;
  @       scan_mem{L}(a, a, 0, 0);
  @   case scan_mem1{L}:
  @     \forall integer a, b, s, inuse, free;
  @       \at(mem[a+1], L) == null
  @     ==> size_obj(a, s)
  @     ==> scan_mem{L}(a+s, b, inuse, free)
  @     ==> scan_mem{L}(a, b, inuse, free+s);
  @   case scan_mem2{L}:
  @     \forall integer a, b, s, inuse, free;
  @       IS_POINTER(\at(mem[a+1], L))
  @     ==> size_obj(a, s)
  @     ==> scan_mem{L}(a+s, b, inuse, free)
  @     ==> scan_mem{L}(a, b, inuse, free+s);
  @   case scan_mem3{L}:
  @     \forall integer a, b, s, inuse, free;
  @       IS_DATA(\at(mem[a+1], L))
  @     ==> size_obj(a, s)
  @     ==> scan_mem{L}(a+s, b, inuse, free)
  @     ==> scan_mem{L}(a, b, inuse+s, free);
  @ }
  @*/

// Memory, i.e array mem[MEMORY_SIZE] is sane..

/*@ predicate mem_sanity =
  @   \valid_range(mem, 0, MEMORY_SIZE-1)
  @ && \forall integer inuse, free;
  @      scan_mem(0, MEMORY_SIZE, inuse, free)
  @    ==> inuse + free == MEMORY_SIZE;
  @*/

// Object at address b is reachable through object at address a..

/*@ inductive reachable{L}(integer a, integer b) {
  @   case reachable0{L}:
  @     \forall integer a;
  @       valid_obj(a, 0, MEMORY_SIZE)
  @     ==> reachable{L}(a, a);
  @   case reachable1{L}:
  @     \forall integer a, s, t, b, i;
  @       valid_obj(a, 0, MEMORY_SIZE)
  @     ==> size_obj(a, s)
  @     ==> 0 <= i < s - OBJ_HEADER_SIZE
  @     ==> IS_POINTER(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> t == POINTER_OF_WORD(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> reachable{L}(t - OBJ_HEADER_SIZE, b)
  @     ==> reachable{L}(a, b);
  @ }
  @*/

// Object at address b is reachable through object at address a through unmarked objects..

/*@ inductive unmarked_reachable{L}(integer a, integer b) {
  @   case unmarked_reachable0{L}:
  @     \forall integer a;
  @       valid_obj(a, 0, MEMORY_SIZE)
  @     ==> unmarked_reachable{L}(a, a);
  @   case reachable1{L}:
  @     \forall integer a, s, t, b, i;
  @       valid_obj(a, 0, MEMORY_SIZE)
  @     ==> size_obj(a, s)
  @     ==> 0 <= i < s - OBJ_HEADER_SIZE
  @     ==> IS_POINTER(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> t == POINTER_OF_WORD(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> !IS_MARKED(mem[t - OBJ_HEADER_SIZE])
  @     ==> unmarked_reachable{L}(t - OBJ_HEADER_SIZE, b)
  @     ==> unmarked_reachable{L}(a, b);
  @ }
  @*/

// There are cnt reachable but not marked yet objects in range [a,b]..

/*@ axiomatic count_axiomatic
  @{
  @ logic integer count{L}(integer root, integer a, integer b) reads mem[a..b-1];
  @ axiom count0{L}:
  @   \forall integer root, a;
  @     count{L}(root, a, a) == 0;
  @ axiom count1{L}:
  @   \forall integer root, a, b, s;
  @     valid_obj{L}(a, 0, MEMORY_SIZE)
  @   ==> size_obj{L}(a, s)
  @   ==> reachable{L}(root, a)
  @   ==> ! IS_MARKED(mem[a])
  @   ==> count{L}(root, a, b) == count{L}(root, a + s, b) + 1;
  @ axiom count2{L}:
  @   \forall integer root, a, b, s;
  @     valid_obj{L}(a, 0, MEMORY_SIZE)
  @   ==> size_obj{L}(a, s) 
  @   ==> ! reachable{L}(root, a)
  @   ==> count{L}(root, a, b) == count{L}(root, a + s, b);
  @ }
  @*/

// Object at address p is in stack whose top element is object at address s..
/*@ inductive in_stack{L}(integer p, integer s) {
  @   case in_stack0{L}:
  @     \forall integer p;
  @       valid_obj(p, 0, MEMORY_SIZE)
  @     ==> in_stack{L}(p, p);
  @   case free_obj1{L}:
  @     \forall integer p, s, q;
  @       valid_obj{L}(s, 0, MEMORY_SIZE)
  @     ==> IS_POINTER(\at(mem[s + 1], L))
  @     ==> q == POINTER_OF_WORD(\at(mem[s + 1], L))
  @     ==> in_stack{L}(p, q)
  @     ==> in_stack{L}(p, s);
  @ }
  @*/

//Objects at addresses o0, o1 are adjacent in stack whose top element is object at address s..
/*@ inductive adj_in_stack{L}(integer p0, integer p1, integer s) {
  @  case adj_in_stack0{L}:
  @    \forall integer p0, p1, i0, i1; 
  @      IS_DATA(\at(mem[p0 + 1], L))
  @    ==> i0 == DATA_OF_WORD(\at(mem[p0 + 1], L))
  @    ==> IS_POINTER(\at(mem[p0 + OBJ_HEADER_SIZE + i0], L))
  @    ==> p1 == POINTER_OF_WORD(\at(mem[p0 + OBJ_HEADER_SIZE + i0], L))
  @    ==> adj_in_stack{L}(p0, p1, p0);
  @  case adj_in_stack1{L}:
  @    \forall integer p0, p1, s, i, q;
  @      valid_obj(s, 0, MEMORY_SIZE - 1)
  @    && IS_DATA(\at(mem[s + 1], L))
  @    ==> i == DATA_OF_WORD(\at(mem[s + OBJ_HEADER_SIZE + 1], L))
  @    ==> IS_POINTER(\at(mem[s + OBJ_HEADER_SIZE + i], L))
  @    ==> q == POINTER_OF_WORD(\at(mem[s + OBJ_HEADER_SIZE + i], L))
  @    ==> adj_in_stack{L}(p0, p1, q)
  @    ==> adj_in_stack{L}(p0, p1, s);
  @ }
  @*/

void dfs(word x);
void mark();
void sweep();

#endif
