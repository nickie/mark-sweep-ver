#ifndef __GC_H__
#define __GC_H__

typedef unsigned int word;

#define IS_DATA(w)         ((w & 0x00000001U) == 0x00000001U)
#define IS_POINTER(w)      ((w & 0x00000001U) == 0x00000000U)

#define WORD_OF_DATA(w)    ((w << 2) | 0x00000001U)
#define DATA_OF_WORD(w)    ((w >> 2) & 0x3FFFFFFFU) 

#define WORD_OF_POINTER(w) ((w << 2) & 0xFFFFFFFCU)
#define POINTER_OF_WORD(w) ((w >> 2) & 0x3FFFFFFFU)

#define IS_MARKED(w)       ((w & 0x00000002U) == 0x00000002U)
#define MARK(w)            (w | 0x00000002U)
#define UNMARK(w)          (w & 0xFFFFFFFDU)

#define MEMORY_SIZE 1 << 13

#define OBJ_HEADER_SIZE 2

extern word null;

extern word roots[2];

extern word *mem;

extern word freeStart;

extern word marked;

extern word cnt;

extern word swept;

extern word rnd;

#endif
