/*
 * [yufan xue]
 * [04/06/2014]
 *
 * [Project 3 - CPU Scheduling Simulator ]
 */
#include <stdio.h>
#include <string.h> // For strlen
#include <stdlib.h> // For strtol and strtod
#include <math.h>   // For fmod
#include <assert.h>
#include <unistd.h>
#include <ctype.h>

/******************************
 * Structures
 ******************************/
struct process {
    int pid;
    int priority;
    int cpu_burst_time;
    int waiting_time;
    int turnaround_time;
    int remain_time;
    int status;
};
typedef struct process process;


int program(int argc,char **argv);


int parse_command(int argc,char **argv);


void schedule();


int process_file(char *filename);


void FCFS();


void SJF();


void Priority();


void RR();


void commmon_info_echo();


void common_time_echo();


int is_valid_int(char* str);


char** str_split(char* a_str, const char a_delim);


void free_memory();
