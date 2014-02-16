
# [YOUR NAME]
# [DATE LAST MODIFIED]
#
CC=gcc
CFLAGS=-Wall -g -O0 -lm

#
# List all of the binary programs you want to build here
# Separate each program with a single space
#
PROGS=finput calc

#
# First target: all
# Compile all programs
#
all: $(PROGS)

#
# Compile Program 1
#
finput: finput.c support.h
	$(CC) -o finput finput.c $(CFLAGS)

#
# Add other targets here..
#
calc: calc.c support.h
	$(CC) -o calc calc.c $(CFLAGS)
#
# Cleanup the files that we have created
#
clean:
	$(RM) $(PROGS)
	$(RM) -rf *.o *.dSYM
