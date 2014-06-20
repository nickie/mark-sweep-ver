#ifndef __GC_H__
#define __GC_H__

// begin stuff for debugging
#undef DEBUG

#ifdef DEBUG
#include <stdio.h>
#define debug(...) fprintf(stderr, __VA_ARGS__)
#else
#define debug(...) do ; while(0)
#define NDEBUG
#endif
// end stuff for debugging

#include <assert.h>

typedef unsigned int word;

#define null 0xFFFFFFFFU

#define IS_DATA(w)         (((w) & 0x00000001U) == 0x00000001U)
#define IS_POINTER(w)      (((w) & 0x00000001U) == 0x00000000U)

#define WORD_OF_DATA(w)    (((w) << 1) | 0x00000001U)
#define DATA_OF_WORD(w)    (((w) >> 1) & 0x7FFFFFFFU)

#define WORD_OF_POINTER(w) ((w) == null ? null : ((w) << 1) & 0xFFFFFFFEU)
#define POINTER_OF_WORD(w) ((w) == null ? null : ((w) >> 1) & 0x7FFFFFFFU)

#define IS_MARKED(w)       (((w) & 0x00000001U) == 0x00000000U)
#define MARK(w)            ((w) & 0xFFFFFFFEU)
#define UNMARK(w)          ((w) | 0x00000001U)

#define MEMORY_SIZE (1 << 20)

#define OBJ_HEADER_SIZE 2
#define ROOT_SIZE 128

extern word * root[ROOT_SIZE];
extern word rootp;

extern word mem[MEMORY_SIZE];

extern word freeStart;

extern word marked;

extern word cnt;

extern word swept;

void gc();

void mem_init();
void root_add(word *p);
void root_pop(int n);
void print_free(void);

#endif
