# Yufan Xue
# CS 441/541:  Make File
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

all: scheduler

scheduler: scheduler.c scheduler.h
	$(CC) -o scheduler scheduler.c $(CFLAGS)

clean:
	$(RM) scheduler *.o
	$(RM) -rf *.dSYM
