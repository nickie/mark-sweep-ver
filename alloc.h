#include "gc.h"

#ifndef __ALLOC_H_
#define __ALLOC_H__

word allocate(word requestSize);
void deallocate(word address);

#endif
