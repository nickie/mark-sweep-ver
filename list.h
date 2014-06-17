#ifndef __LIST_H__
#define __LIST_H__

word insert(word d, word list);

word list(word size);

word append(word x, word y);

word reverse_aux(word list, word acc);

word reverse(word list);

word length_aux(word list, word acc);

word length(word list);

word filter_aux(word (*f)(word), word list, word acc);

word filter(word (*f)(word), word list);

void print(word list);

void print_free(void);

#endif
