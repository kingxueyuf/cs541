# Yufan xue
# Feb. 23, 2014
#
# CS441/541: Project 1 Part 2
#
#####################################################################
#
# Type "make" or "make myshell" to compile your code
# 
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

CC=gcc
CFLAGS=-Wall -g

all: myshell

myshell: myshell.c myshell.h support.o
	$(CC) -o myshell myshell.c support.o $(CFLAGS)

support.o: support.h support.c
	$(CC) -c -o support.o support.c $(CFLAGS)

clean:
	$(RM) myshell *.o
	$(RM) -rf *.dSYM
