#ifndef __ALLOC_H__
#define __ALLOC_H__

#include "gc.h"

word allocate(word requestSize);
void deallocate(word address);

#endif
