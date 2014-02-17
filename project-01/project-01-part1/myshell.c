/*
 * yufan xue
 * Feb. 16, 2014
 *
 * CS441/541: Project 1 Part 1
 *
 */
#include "myshell.h"
#define LINELEN 1024
int main(int argc, char * argv[]) {

    /*
     * Below is just an example of how to use the support functionality.
     * You will want to remove this code when you get started, but
     * it may serve as a helpful reference while you are getting started.
     */
     /*
      * mode = 0 -> interactive mode
      * mode = 1 -> batch mode
      */
    if(argc ==1)
    {
      // mode = 0 -> interactive mode
      interactive_mode();
    }else
    {
      // mode = 1 -> batch mode;
      batch_mode(argc,argv);
    }

    return 0;
}

int interactive_mode()
{
    char command[LINELEN] ;
    char *fgets_rtn = NULL;
    printf("$myshell ");
    fgets_rtn = fgets(command,LINELEN,stdin);
    
    int num_jobs = 0;
    job_t *loc_jobs = NULL;
    int i, j;
    int counter = 0;
    int total_jobs = 0;
    int background = 0;
    while(fgets_rtn != NULL)
    {
            if( '\n' == command[strlen(command)-1]){
                command[strlen(command)-1] = '\0';
            }
            num_jobs = 0;
            i = 0;
            j = 0;
            /*
             * Allocate some space for the job_t array
             * Necessary so split_input_into_jobs() function has a memory
             * handle to work with.
             */
            loc_jobs = (job_t*)malloc(sizeof(job_t) * 1);
            if( NULL == loc_jobs ) {
                fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
                exit(-1);
            }
            int is_concurrent = concurrent(command);
            /*
             * Split out the individual jobs from the larger command
             */
            split_input_into_jobs(command, &num_jobs, &loc_jobs);

            /*
             * For each of those jobs split out the argument set
             */
            for( i = 0; i < num_jobs; ++i ) {
                      // printf("here");

                counter ++;
                /*
                 * Display the command
                 */
                split_job_into_args( &(loc_jobs[i]) );
                if(is_concurrent)
                {
                    if(build_in_command(loc_jobs[i].argv[0]))
                    {
                        printf("Job %d^: \"%s\"", counter,loc_jobs[i].argv[0]);
                        if(is_exit(loc_jobs[i].argv[0]))
                        {  
                            printf("\n");
                            printf("Total number of jobs: %d \n", total_jobs);
                            printf("Total number of jobs in history: %d \n", counter);
                            printf("Total number of jobs in background: %d \n", background);
                            exit(0);
                        }
                    }else
                    {
                        total_jobs++;
                        printf("Job %d : \"%s\"", counter,loc_jobs[i].argv[0]);
                    }
                }else
                {
                    total_jobs++;
                    background++;
                    printf("Job %d*: \"%s\"", counter,loc_jobs[i].argv[0]);
                }
                /*
                 * Display the arguments
                 */
                fflush(NULL);
                for( j = 1; j < loc_jobs[i].argc; ++j ) {
                    printf(" [%s]", loc_jobs[i].argv[j]);
                    fflush(NULL);
                }
                printf("\n");
                fflush(NULL);
            }

            /*
             * Cleanup
             */
            if( NULL != loc_jobs ) {
                      // printf("here");
                /* Cleanup struct fields */
                for( i = 0; i < num_jobs; ++i ) {
                    /* .full_command */
                    if( NULL != loc_jobs[i].full_command ) {
                        free( loc_jobs[i].full_command );
                        loc_jobs[i].full_command = NULL;
                    }

                    /* .argv */
                    if( NULL != loc_jobs[i].argv ) {
                        for( j = 0; j < loc_jobs[i].argc; ++j ) {
                            if( NULL != loc_jobs[i].argv[j] ) {
                                free( loc_jobs[i].argv[j] );
                                loc_jobs[i].argv[j] = NULL;
                            }
                        }
                        free( loc_jobs[i].argv );
                        loc_jobs[i].argv = NULL;
                    }

                    /* .argc */
                    loc_jobs[i].argc = 0;
                }

                /* Free the array */
                free(loc_jobs);
                loc_jobs = NULL;
            }
            printf("$myshell ");
            fgets_rtn = fgets(command,LINELEN,stdin);
    }
    return 0;
}

int batch_mode(int argc, char * argv[])
{
    int num_jobs = 0;
    job_t *loc_jobs = NULL;
    int i, j;
    int counter = 0;
    int total_jobs = 0;
    int background = 0;

    for( i = 1; i < argc; ++i ) {
      printf("Processing File: %s\n", argv[i]);
      char command[LINELEN] ;
      char *fgets_rtn = NULL;
      FILE *fd = NULL;
      fd = fopen(argv[i],"r");
      if(NULL ==fd){
        fprintf(stderr,"Error: Cannot open thie file %s for reading!\n",argv[i]);
        return -1;
      }
      while( 0 == feof(fd) ) {
         fgets_rtn = fgets(command, LINELEN, fd);
         if( NULL == fgets_rtn) {
            /* Ctrl-D */
            break;
        }
         if( '\n' == command[strlen(command)-1]){
                command[strlen(command)-1] = '\0';
            }
            num_jobs = 0;
            i = 0;
            j = 0;
            /*
             * Allocate some space for the job_t array
             * Necessary so split_input_into_jobs() function has a memory
             * handle to work with.
             */
            loc_jobs = (job_t*)malloc(sizeof(job_t) * 1);
            if( NULL == loc_jobs ) {
                fprintf(stderr, "Error: Failed to allocate memory! Critical failure on %d!", __LINE__);
                exit(-1);
            }
            int is_concurrent = concurrent(command);
            /*
             * Split out the individual jobs from the larger command
             */
            split_input_into_jobs(command, &num_jobs, &loc_jobs);

            /*
             * For each of those jobs split out the argument set
             */
            for( i = 0; i < num_jobs; ++i ) {

                counter ++;
                /*
                 * Display the command
                 */
                split_job_into_args( &(loc_jobs[i]) );
                if(is_concurrent)
                {
                    if(build_in_command(loc_jobs[i].argv[0]))
                    {
                        printf("Job %d^: \"%s\"", counter,loc_jobs[i].argv[0]);
                        if(is_exit(loc_jobs[i].argv[0]))
                        {  
                            printf("\n");
                            printf("Total number of jobs: %d \n", total_jobs);
                            printf("Total number of jobs in history: %d \n", counter);
                            printf("Total number of jobs in background: %d \n", background);
                            exit(0);
                        }
                    }else
                    {
                        total_jobs++;
                        printf("Job %d : \"%s\"", counter,loc_jobs[i].argv[0]);
                    }
                }else
                {
                    total_jobs++;
                    background++;
                    printf("Job %d*: \"%s\"", counter,loc_jobs[i].argv[0]);
                }
                /*
                 * Display the arguments
                 */
                fflush(NULL);
                for( j = 1; j < loc_jobs[i].argc; ++j ) {
                    printf(" [%s]", loc_jobs[i].argv[j]);
                    fflush(NULL);
                }
                printf("\n");
                fflush(NULL);
            }

            /*
             * Cleanup
             */
            if( NULL != loc_jobs ) {
                /* Cleanup struct fields */
                for( i = 0; i < num_jobs; ++i ) {
                    /* .full_command */
                    if( NULL != loc_jobs[i].full_command ) {
                        free( loc_jobs[i].full_command );
                        loc_jobs[i].full_command = NULL;
                    }

                    /* .argv */
                    if( NULL != loc_jobs[i].argv ) {
                        for( j = 0; j < loc_jobs[i].argc; ++j ) {
                            if( NULL != loc_jobs[i].argv[j] ) {
                                free( loc_jobs[i].argv[j] );
                                loc_jobs[i].argv[j] = NULL;
                            }
                        }
                        free( loc_jobs[i].argv );
                        loc_jobs[i].argv = NULL;
                    }

                    /* .argc */
                    loc_jobs[i].argc = 0;
                }

                /* Free the array */
                free(loc_jobs);
                loc_jobs = NULL;
            }
      }
    }
    printf("\n");
    printf("Total number of jobs: %d \n", total_jobs);
    printf("Total number of jobs in history: %d \n", counter);
    printf("Total number of jobs in background: %d \n", background);
  return 0;
}
