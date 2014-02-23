/*
 * Yufan xue
 * Feb. 23, 2014
 *
 * CS441/541: Project 1 Part 2
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
#define JOB_STATE_UNKNOWN 0
#define JOB_STATE_RUNNING 1
#define JOB_STATE_DONE    2

#define PROMPT ("myshell$ ")


/******************************
 * Structures
 ******************************/
struct bg_job_t {
    char * command;
    int    state;
    pid_t  pid;
    int    jobid;
};
typedef struct bg_job_t bg_job_t;

/******************************
 * Global Variables
 ******************************/
/*
 * Debugging toggle
 */
int is_debug = FALSE;

/*
 * Interactive or batch mode
 */
int is_batch = FALSE;

/*
 * Batch file names
 */
int num_batch_files = 0;
char **batch_files = NULL;

/*
 * Counts
 */
int total_jobs_display_ctr = 0;
int total_jobs    = 0;
int total_jobs_bg = 0;
int total_history = 0;

/*
 * If we are exiting
 */
int exiting = FALSE;

/*
 * For tracking background jobs
 */
int num_bg_jobs   = 0;
bg_job_t *bg_jobs = NULL;

/******************************
 * Function declarations
 ******************************/
/*
 * Parse command line arguments passed to myshell upon startup.
 *  argc : Number of command line arguments
 *  argv : Array of pointers to strings
 * Returns 0 on success, ow error
 */
int parse_args_main(int argc, char **argv);

/*
 * Main routine for batch mode
 * Returns 0 on success, ow error
 */
int batch_mode(void);

/*
 * Main routine for interactive mode
 * Returns 0 on success, ow error
 */
int interactive_mode(void);

/*
 * Split possible multiple jobs on a command line, then call parse_and_run()
 *  command : command line string (may contain multiple jobs)
 * Return 0 on success, ow error
 */
int split_parse_and_run(char * command);

/*
 * Parse and execute a single job given to the prompt.
 *   loc_job : job to execute
 * Return 0 on success, ow error
 */
int parse_and_run(job_t * loc_job);

/*
 * Launch a job
 *  binary : String name of the binary
 *  argc : Number of command line arguments for the job
 *  argv : Array of pointers to strings of arguments
 *  is_background : Is this a background job
 * Returns 0 on success, ow error
 */
int launch_job(job_t * loc_job);

/*
 * Built-in 'exit' command
 * Returns 0 on success, ow error
 */
int builtin_exit(void);

/*
 * Built-in 'jobs' command
 * Returns 0 on success, ow error
 */
int builtin_jobs(void);

/*
 * Built-in 'history' command
 * Returns 0 on success, ow error
 */
int builtin_history(void);

/*
 * Ensure all backgrounded jobs finish correctly
 * Returns 0 on success, ow error
 */
int finish_bg_jobs(void);

#endif /* MYSHELL_H */
