/*
 * [yufan xue]
 * [02/09/2014]
 *
 * [Project 0.3 ]
 */
#include <stdio.h>
#include <string.h> // For strlen
#include <stdlib.h> // For strtol and strtod
#include <ctype.h>  // For isdigit
#include <math.h>   // For fmod

/*
 * Max length of the buffer for fgets
 */
int LINELEN = 256;

/*
 * Process all of the instructions in the filename specified.
 *
 * Parameters:
 *   filename: Filename to process
 *
 * Returns:
 *   0 on success
 *  -1 if we cannot open the file
 */
int process_file(char *filename);

int is_valid_int(char *str);    
int is_valid_float(char *str);
int is_valid_operator(char *str);
int is_operator(char *str);
