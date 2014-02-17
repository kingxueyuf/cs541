/*
 * yufan xue
 * Feb. 16, 2014
 *
 * CS441/541: Project 1 Part 1
 *
 */
#ifndef MYSHELL_H
#define MYSHELL_H

#include "support.h"

/* For fork, exec, sleep */
#include <sys/types.h>
#include <unistd.h>
/* For waitpid */
#include <sys/wait.h>

/******************************
 * Defines
 ******************************/



/******************************
 * Structures
 ******************************/


/******************************
 * Global Variables
 ******************************/


/******************************
 * Function declarations
 ******************************/
int interactive_mode();

int batch_mode(int argc, char * argv[]);
#endif /* MYSHELL_H */
