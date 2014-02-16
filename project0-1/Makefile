# @author robin-yufan xue
# @date   2/2/2014
CC=gcc
CFLAGES=-Wall -g -O0

# List all of the binary programs you want build here
# Sparate each program with a single space
#
PROGS=hello cmdline

# First target: all
# Compile all programs
#
all: $(PROGS)

# Complie Hello World
# 
hello: hello.c
	$(CC) -o hello hello.c $(CFLAGES)

cmdline: cmdline.c
	$(CC) -o cmdline cmdline.c $(CFLAGES)
# 
# Add other targets here..
#

# CLeanup the files that we have created
#
clean:
	$(RM) $(PROGS)
	$(RM) -rf *.o *.dSYM

