#####################################################################
# Josh Hursey
#
# CS 441/541 : Memory Management Performance Template
#
#####################################################################
#
# Type "make" or "make all" to compile your code
# Type "make clean" to remove the executable (and any object files)
#
#####################################################################

# Our C Compiler
CC=gcc

#
# Compiler Flags
#
CFLAGS=-Wall -O0 -g

#
# Linker Flags
#
# Linker flag for Mac OS X
LDFLAGS=-lm
# Linker flag for Linux
#LDFLAGS=-lm -lrt

#
# All of the programs to compile
#
PROGS=scalarmult matrixmult

all: $(PROGS)

#
# Scalar Multiply
#
scalarmult: support.o scalarmult.c scalarmult.h
	$(CC) -o scalarmult support.o $(CFLAGS) scalarmult.c $(LDFLAGS)

#
# Matrix Multiply
#
matrixmult: support.o matrixmult.c matrixmult.h
	$(CC) -o matrixmult support.o $(CFLAGS) matrixmult.c $(LDFLAGS)

#
# Support Library
#
support.o: support.c support.h
	$(CC) -c -o support.o $(CFLAGS) support.c

#
# Cleanup
#
clean:
	$(RM) $(PROGS) *.o
	$(RM) -rf *.dSYM
