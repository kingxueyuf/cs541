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
#include <time.h>	//For count time
#include "semaphore_support.h"

/*****************************
 * Defines
 *****************************/



/*****************************
 * Function Declarations
 *****************************/

/*
 *
 */
void *pirate(void *threadid);

void *ninja(void *threadid);



/*
 * Check whether the input is valid
 */
int check_command_line(int argc, char * argv[], int *pirates_number, int* ninjas_number, int* time_to_run);


/*
 * Check whether the string is an valid integer
 */

int is_valid_int(char* str);

