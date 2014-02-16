/*
 * [yufan xue]
 * [2/6/2014]
 *
 * [Project 0.2 - Program 1 -Echo Program]
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define LINELEN 128

int main(int argc, char **argv){
	char buffer[LINELEN];
	char *fgets_rtn = NULL;
	
	fgets_rtn = fgets(buffer,LINELEN,stdin);
	int counter = 1 ;
	while(fgets_rtn != NULL)
	{
		if( '\n' == buffer[strlen(buffer)-1]){
			buffer[strlen(buffer)-1] = '\0';
		}
		printf("%d) Echo: \"%s\" \n",counter,buffer);	
		counter++ ;
		fgets_rtn = fgets(buffer,LINELEN,stdin);
	}
	return 0;
}
