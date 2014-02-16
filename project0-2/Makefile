# [yufan xue]
# [2/6/2014]
CC=gcc
CFLAGS=-Wall -g -O0

PROGS=input verify checker

all:$(PROGS)

input:input.c
	$(CC) -o input input.c $(CFLAGS)
verify:verify.c verify.h
	$(CC) -o verify verify.c $(CFLAGS)
checker:checker.c verify.h
	$(CC) -o checker checker.c $(CFLAGS)

clean:
	$(RM) $(PROGS)
	$(RM) -rf *.o *.dSYM
