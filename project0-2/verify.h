/*
 * [yufan xue]
 * [2/6/14]
 * 
 * [Project 0.2 - Program 2 -Input Verification]
 */
#include <stdio.h>
#include <string.h>	//For strlen
#include <stdlib.h>	//For strtol and strtod
#include <ctype.h>	//For isdigit

/*
 *
 * Takes as input a string to check if it is a syntacticly correct integer.
 *
 * Parameters:
 * 	str : String to check
 *
 * Returns
 * 	0 if it is not valid integer
 * 	1 if it is a valid integer
 */

int is_valid_int(char *str); 	
int is_valid_float(char *str);