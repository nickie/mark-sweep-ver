#include <stdio.h>
#include "gc.h"
#include "list.h"

int main (int argc, char *argv[])
{
  mem_init();
  word i;

  int N = 1024;
  word l = list(N);
  root_add(&l);
  //print(l);
  word fl = l;
  root_add(&fl);
  for (i = 0; i < 100000; i++) {
    printf("\r%d", i);
    fl = reverse(fl);
    if (length(fl) != N) {
      fprintf(stderr, "Sanity error (length) after step %i:\n", i);
      print(fl);
      return 1;
    }
    if (i % 2 == 1 && !equal(l, fl)) {
      fprintf(stderr, "Sanity error (not equal) after step %i:\n", i);
      print(fl);
      return 1;
    }
  }
  printf("\ndone!\n");
  return 0;
}
