CC=gcc

CFLAGS=-Wall -g
#CFLAGS=-Wall -O3

CFILES=alloc.c gc.c list.c new.c mark_and_sweep.c

HFILES=$(patsubst %.c,%.h,$(CFILES))

all: test

test: test.c $(CFILES) $(HFILES)
	$(CC) $(CFLAGS) -o $@ $< $(CFILES)

clean:
	$(RM) test
