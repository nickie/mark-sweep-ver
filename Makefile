.PHONY: all verify clean distclean

CC=gcc

CFLAGS=-Wall -g
#CFLAGS=-Wall -O3

CFILES=alloc.c gc.c list.c new.c mark_and_sweep.c

HFILES=$(patsubst %.c,%.h,$(CFILES))

all: test

test: test.c $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -o $@ $< $(CFILES)

JESSIE_CPU=-jessie-cpu-limit 300
JESSIE_OPT=
#JESSIE_OPT=-jessie-why-opt=--split-user-conj -jessie-why-opt=--split-bool-op
#JESSIE_OPT=-jessie-jc-opt=-separation

verify:
	frama-c -jessie $(JESSIE_OPT) $(JESSIE_CPU) mark_and_sweep.c

clean:
	$(RM) *.o *~

distclean: clean
	$(RM) test
	$(RM) -r *.jessie
