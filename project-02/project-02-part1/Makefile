#
# Yufan Xue
# CS 441/541: Semaphore Library
#
#####################################################################
#
# Type "make" or "make sum" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g
LDFLAGS=-pthread -lm

PROGS=bf

all: $(PROGS)

bf: bounded-buffer.c bounded-buffer.h semaphore_support.o
	$(CC) -o bounded-buffer $(CFLAGS) $(LDFLAGS) bounded-buffer.c semaphore_support.o

semaphore_support.o: semaphore_support.h semaphore_support.c
	$(CC) -c -o semaphore_support.o semaphore_support.c $(CFLAGS)

clean:
	$(RM) $(PROGS) *.o
	$(RM) -rf *.dSYM
