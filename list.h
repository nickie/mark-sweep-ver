#ifndef __LIST_H__
#define __LIST_H__

word insert(word d, word list);

word list(word size);

word append(word x, word y);

word reverse(word list);

int length(word list);

word filter(word (*f)(word), word list);

int equal(word x, word y);

void print(word list);

#endif
