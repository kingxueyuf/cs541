/*
 * [yufan xue]
 * [05/04/2014]
 *
 * [Project 5 - Disk Scheduling Algorithm Simulator ]
 */
#include <stdio.h>
#include <string.h> // For strlen
#include <stdlib.h> // For strtol and strtod
#include <math.h>   // For fmod
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

/*
 * program function 
 * It will 1) check the command line
 * then run the schedule algorithm based on the commond line input parameters
 */
int program(int argc,char **argv);

/*
 * parse_command function
 * Parse the command 
 * The command input parameters include
 * 1)filename
 * 2)current head position
 * 3)current direction of travel of the disk head
 * Attention --> the command is not ordered !!!
 */
int parse_command(int argc,char **argv);

/*
 * schedule function
 * After parse command program will run the schedule function
 * There exists 4 scheduling algorithm
 * 1) FCFS
 * 2) SJF
 * 3) Priority
 * 4) RR
 */
void schedule();


/* 
 * Judge whether the string is a valid int
 */
int is_valid_int(char* str);


/*
 * process_file
 * Process the file 
 * Build the struct to store the process information
 */
int process_file(char *filename) ;

void OUTPUT_CONFIGURATION();

int FCFS();
int SSTF();
int SCAN();
int CSCAN();
int LOOK();
int CLOOK();

void sort(int* array,int array_length);


void copy_array1(int* arr_to_copy,int array_length,int **copy_array);

/*
 * Free the memory malloced before
 */
void free_memory();
