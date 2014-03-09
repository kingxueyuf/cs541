/*
 * Yufan Xue
 * Mar. 09, 2014
 *
 * CS 441/541: project-02-part2
 *
 */
#include <stdio.h>
#include <stdlib.h>   
#include <time.h>
#include <unistd.h>  
#include <errno.h>
#include <pthread.h>
#include <ctype.h>	//For isdigit
#include "semaphore_support.h"

/*****************************
 * Defines
 *****************************/



/*****************************
 * Function Declarations
 *****************************/
/*
 * Thread Start Function
 * Add a segment of numbers in the buffer
 * Store the result in the segment of 'subtotals' for this thread.
 *  *threadid : integer artificial thread ID
 */
void *add_numbers(void *threadid);


/*
 * Check whether the input is valid
 */
int check_command_line(int argc, char * argv[], int *producer_number, int* consumer_number, int* buffer_size, int* time_to_live);


/*
 * Check whether the string is an valid integer
 */

int is_valid_int(char* str);

/*
 *
 */
void *consume(void *threadid);

/*
 *
 */
void *produce(void *threadid);

/*
 *
 */
void output_buffer();

