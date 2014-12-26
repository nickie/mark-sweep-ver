#ifndef __MARK_AND_SWEEP_H__
#define __MARK_AND_SWEEP_H__

#include "gc.h"

// Object at address p has size size..

/*@ predicate sz_of_obj{L}(integer p, integer size) =
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

// There is inuse words of objects in-use and free words of free objects in range [a,b]..

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
  @    \valid(mem+(0..MEMORY_SIZE))
  @ && scan_mem(0, MEMORY_SIZE, inuse, free)
  @ && inuse + free == MEMORY_SIZE;
  @*/

//@ type list = Nil | Cons (integer, list);

// Object at address b is reachable through object at address a..

/*@ inductive reachable{L}(integer a, integer b) {
  @   case reachable0{L}:
  @     \forall integer a;
  @       valid_obj(a, 0, MEMORY_SIZE)
  @     ==> reachable{L}(a, a);
  @   case reachable1{L}:
  @     \forall integer a, s, t;
  @       valid_obj(a, 0, MEMORY_SIZE) && size_obj(a, s)
  @     ==>  \exists integer i;
  @            0 <= i < s - OBJ_HEADER_SIZE
  @          ==> IS_POINTER(mem[a + OBJ_HEADER_SIZE + i])
  @          ==> t == POINTER_OF_WORD(mem[a + OBJ_HEADER_SIZE + i])
  @          ==> reachable{L}(t - OBJ_HEADER_SIZE, b)
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
  @     \forall integer a, s, t;
  @       valid_obj(a, 0, MEMORY_SIZE) && size_obj(a, s)
  @     ==>  \exists integer i;
  @            0 <= i < s - OBJ_HEADER_SIZE
  @          ==> IS_POINTER(mem[a + OBJ_HEADER_SIZE + i])
  @          ==> t == POINTER_OF_WORD(mem[a + OBJ_HEADER_SIZE + i])
  @          ==> !IS_MARKED(mem[t - OBJ_HEADER_SIZE])
  @          ==> unmarked_reachable{L}(t - OBJ_HEADER_SIZE, b)
  @     ==> unmarked_reachable{L}(a, b);
  @ }
  @*/

// There are cnt reachable but not marked yet objects in range [a,b]..

/*@ axiomatic count_axiomatic
  @{
  @ logic integer count{L}(integer root, integer a, integer b) reads mem[a..b-1];
  @ axiom count0{L}:
  @   \forall integer a;
  @     count(a, a) == 0;
  @ axiom count1{L}:
  @   \forall integer a, b, s;
  @     \valid_obj(a, 0, MEMORY_SIZE)
  @   ==> size_obj{L}(a, s)
  @   ==> reachable{L}(root, a)
  @   ==> ! IS_MARKED(mem[a])
  @   ==> count(root, a, b) == count(root, a + s, b) + 1;
  @ axiom count2{L}:
  @   \forall integer a, b, s;
  @     valid_obj(a, 0, MEMORY_SIZE)
  @   ==> size_obj{L}(a, s)   
  @   ==> ! reachable{L}(root, a)
  @   ==> count(root, a, b) == count(root, a + s, b);
  @ }
  @*/

// s is the stack containing object addresses starting from object at address a..

/*@ inductive c_stack{L}(integer a, list s) {
  @   case stack0{L}:
  @     \forall integer a, i;
  @       i == DATA_OF_WORD(\at(mem[a+1], L))
  @     ==> IS_DATA(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> c_stack{L}(a, Cons(a, Nil));
  @   case stack1{L}:
  @     \forall integer a, i, t, list s;
  @       i == DATA_OF_WORD(\at(mem[a+1], L))
  @     ==> IS_POINTER(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> t == POINTER_OF_WORD(mem[a + OBJ_HEADER_SIZE + i])
  @     ==> c_stack{L}(t, ts)
  @     ==> c_stack{L}(a, Cons(a, ts));
  @ }
  @*/

//(*TODO*)
// in_stack(integer o, list stack)
//(*TODO*)
// adj_in_stack(integer o0, integer o2, list stack)

void dfs(word x);
void mark();
void sweep();

#endif
