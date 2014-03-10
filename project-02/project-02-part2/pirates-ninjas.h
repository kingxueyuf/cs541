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
 * Pirate thread run this function
 * To find pseudo code and design detail, please see REAMME.pdf
 */
void *pirate(void *threadid);
/*
 * Ninja thread run this function
 * To find pseudo code and design detail, please see REAMME.pdf
 */
void *ninja(void *threadid);
/*
 * Check command input 
 * Return 1 -> true
 *        0 -> false
 */
int check_command_line(int argc, char * argv[], int *pirates_number, int* ninjas_number, int* time_to_run);
/* 
 * Check whether the string is a valid int
 * Return 1 -> true
 *        2 -> false
 */
int is_valid_int(char* str);

