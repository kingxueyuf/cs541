/*
 * [yufan xue]
 * [02/09/2014]
 *
 * [Project 0.3 - Program 1 -File Echo Program ]
 */
#include "support.h"

int main(int argc, char **argv) {
    int i;

    for( i = 1; i < argc; ++i ) {
        printf("Processing File: %s\n", argv[i]);
        process_file(argv[i]);
    }

    return 0;
}

int process_file(char *filename) {
    char buffer[LINELEN];
    char *fgets_rtn = NULL;

    // TODO: Open the file here
    FILE *fd = NULL;
    fd = fopen(filename,"r");
    if(NULL ==fd){
	fprintf(stderr,"Error: Cannot open thie file %s for reading!\n",filename);
	return -1;
    }
    

    // TODO: Change loop condition to check for the End-Of-File
    while( 0 == feof(fd) ) {
        // TODO: Instead of reading from 'stdin' you should pass the file descriptor
        /* Read input from the user */
        fgets_rtn = fgets(buffer, LINELEN, fd);
        if( NULL == fgets_rtn) {
            /* Ctrl-D */
            break;
        }

        /* Strip off the new line */
        if( '\n' == buffer[ strlen(buffer) - 1] ) {
            buffer[ strlen(buffer) - 1] = '\0';
        }

        /* Echo the input back the user */
        printf("Line: %s\n", buffer);
    }

    // TODO: Close the file
    fclose(fd);
    return 0;
}
