/*
 * [yufan xue]
 * [04/06/2014]
 *
 * [Project 3 - CPU Scheduling Simulator ]
 */

#include "scheduler.h"

int command_type = -1; 
int quantum = -1;
char *filename ;
int LINELEN = 256;
process** process_arr; // pointer to an process* array
int process_num= 0;

/*
 */
int main(int argc, char **argv) {

    program(argc,argv);

    return 0;
}

/*
 * program function 
 * It will 1) check the command line
 * then run the schedule algorithm based on the commond line input parameters
 */

int program(int argc,char **argv)
{
	// 1 --> ture 0 --> false
	int valid = parse_command(argc,argv);
	if(!valid)
	{
		printf("Invalid input, system will exit");
		return 0;
	}
	schedule();
	return 0;
}

/*
 * parse_command function
 * Parse the command 
 * The command input parameters include
 * 1)filename
 * 2)which scheduling algorithm to use
 * 3)quantum
 * Attention --> the command is not ordered !!!
 */
int parse_command(int argc,char **argv)
{
	int i = 1;
	/*
	 * 1 --> FCFS
	 * 2 --> SJF
	 * 3 --> Priority
	 * 4 --> RR
	 */
	for( ; i < argc ; i++)
	{
		if( 0 ==strcmp("-s",argv[i]) ){
			if(command_type == -1)
			{
				//haven't set command type
				i = i + 1;
				if(i >= argc){
					printf("i >= argc");
					return 0;
				}else if( is_valid_int(argv[i]) ){
					int convert_int = strtol(argv[i],NULL,10);
					if( convert_int >= 1 && convert_int <= 4){
						printf("%d\n", convert_int);
						command_type = convert_int;
					}else{
						printf("valid int but range overflow");
						return 0;
					}
				}else{
					printf("invalid int\n");
					return 0;
				}
			}
		}else if( 0 ==strcmp("-q",argv[i]) ){
			if(quantum == -1)
			{
				//haven't set command type
				i = i + 1;
				if(i >= argc){
					return 0;
				}else if( is_valid_int(argv[i]) ){
					int convert_int2 = strtol(argv[i],NULL,10);
					if( convert_int2 >= 1){
						quantum = convert_int2;
					}else{
						return 0;
					}
				}else{
					return 0;
				}
			}
		}else{
			filename = argv[i];
		}
	}
	int str_len = strlen (filename);
	if( command_type != -1 && str_len > 4 )
		return 1;
	else 
	{
		printf("%d,\n", command_type);
		printf("invalid filename\n");
		return 0;
	}
}

/*
 * schedule function
 * After parse command program will run the schedule function
 * There exists 4 scheduling algorithm
 * 1) FCFS
 * 2) SJF
 * 3) Priority
 * 4) RR
 */
void schedule(){
	int valid = process_file(filename);
	if(!valid)
		printf("valid input file\n");
	switch(command_type)
	{
		case 1:	
			printf("Scheduler 	: 1 FCFS\n");
			commmon_info_echo();
			FCFS();
			common_time_echo();
			break;
		case 2:
			printf("Scheduler 	: 2 SJF\n");
			commmon_info_echo();
			SJF();
			common_time_echo();
			break;
		case 3:
			printf("Scheduler 	: 3 Priority\n");
			commmon_info_echo();
			Priority();
			common_time_echo();
			break;
		case 4:
			printf("Scheduler 	: 4 RR\n");
			commmon_info_echo();
			RR();
			common_time_echo();
			break;
	}
	free_memory();
}


/*
 * process_file
 * Process the file 
 * Build the struct to store the process information
 */
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
    int rownum = 0;
    while( 0 == feof(fd) ) {
        /* Read input from the user */
        fgets_rtn = fgets(buffer, LINELEN, fd);
        if( NULL == fgets_rtn) {
            /* Ctrl-D */
            break;
        }
        // /* Strip off the new line */
        // if( '\n' == buffer[ strlen(buffer) - 1] ) {
        //     buffer[ strlen(buffer) - 1] = '\0';
        // }
        if(rownum == 0)
        {
        	//First Row
        	process_num = strtol(buffer,NULL,10);
        	process_arr = (process**)malloc( sizeof(process*)*process_num );
        }else{
        	process* current_process = (process*)malloc( sizeof(process) );
        	process_arr[ rownum - 1] =  current_process;
			char** tokens;
			printf("split=[%s]\n\n", buffer);
			tokens = str_split(buffer, ' ');
			if (tokens)
			{
			    printf("pid=[%s]\n", *(tokens + 0));
			   	int pid = strtol(*(tokens + 0),NULL,10);
			   	current_process->pid = pid;
			    free(*(tokens + 0));

			    printf("cpu burst time =[%s]\n", *(tokens + 1));
			   	int cpu_time = strtol(*(tokens + 1),NULL,10);
			   	current_process->cpu_burst_time = cpu_time;
			    free(*(tokens + 1));

			    printf("priority =[%s]\n", *(tokens + 2));
			   	int priority = strtol(*(tokens + 2),NULL,10);
			   	current_process->priority = priority;
			    free(*(tokens + 2));

			    printf("\n");
			    free(tokens);
			}
    	}
    	rownum++;
    }

    // TODO: Close the file
    fclose(fd);
    return 0;
}

/*
 * FCFS function
 * First-Come, First-Served scheduling algorithm
 */
void FCFS()
{
	/*
	 * Start Running ...
	 */
	int i = 0;
	int tmp_time;
	int waiting_time = 0;
	int turnaround_time = 0;
	for( ; i < process_num ; i++)
	{
		process_arr[i]->waiting_time = waiting_time;
		tmp_time = process_arr[i]->cpu_burst_time;
		// sleep(tmp_time);
		turnaround_time += tmp_time;
		process_arr[i]->turnaround_time = turnaround_time;
		waiting_time += tmp_time;
	}
	/*
	 * Finish Running ...
	 * 
	 */
}

/*
 * SJF function
 * Shortest-Job First scheduling algorithm
 */
void SJF()
{
	/*
	 * Make a Array Copy of process_arr to process_arr_tmp
	 */
	process** process_arr_tmp = (process**)malloc( sizeof(process*)*process_num );
	int i =0;
	for( ; i < process_num ; i++)
	{
		process_arr_tmp[i] = process_arr[i];
	}
	/*
	 * Sort process_arr_tmp By CPU burst time
	 * Sort Algorithm : bubble sort
	 */
	i = 0;
	int j = 0;
	for( ; i< process_num-1; i++)
	{
		if(process_arr_tmp[i]->cpu_burst_time > process_arr_tmp[i+1]->cpu_burst_time)
		{
			//swap
			process* tmp = process_arr_tmp[i];
			process_arr_tmp[i] = process_arr_tmp[i+1];
			process_arr_tmp[i+1] = tmp;
			for( j = i ; j > 0 ; j--)
			{
				if(process_arr_tmp[j]->cpu_burst_time < process_arr_tmp[j-1]->cpu_burst_time)
				{
					//swap
					tmp = process_arr_tmp[j];
					process_arr_tmp[j] = process_arr_tmp[j-1];
					process_arr_tmp[j-1] = tmp;
				}else 
					break;
			}
		}
	}
	/*
	 * Finish Sort
	 * Then Run It with Ordered Array process_arr_tmp
	 */

	 /*
	  * Start Running ...
	  */
	i = 0;
	int tmp_time;
	int waiting_time = 0;
	int turnaround_time = 0;
	for( ; i < process_num ; i++)
	{
		process_arr_tmp[i]->waiting_time = waiting_time;
		tmp_time = process_arr_tmp[i]->cpu_burst_time;
		// sleep(tmp_time);
		turnaround_time += tmp_time;
		process_arr_tmp[i]->turnaround_time = turnaround_time;
		waiting_time += tmp_time;
	}
	/*
	 * Finish Running ...
	 */	
}

/*
 * Priority function
 */
void Priority()
{
	/*
	 * Make a Array Copy of process_arr to process_arr_tmp
	 */
	process** process_arr_tmp = (process**)malloc( sizeof(process*)*process_num );
	int i =0;
	for( ; i < process_num ; i++)
	{
		process_arr_tmp[i] = process_arr[i];
	}
	/*
	 * Sort process_arr_tmp By Priority
	 * Sort Algorithm : bubble sort
	 */
	i = 0;
	int j = 0;
	for( ; i< process_num-1; i++)
	{
		if(process_arr_tmp[i]->priority > process_arr_tmp[i+1]->priority)
		{
			//swap
			process* tmp = process_arr_tmp[i];
			process_arr_tmp[i] = process_arr_tmp[i+1];
			process_arr_tmp[i+1] = tmp;
			for( j = i ; j > 0 ; j--)
			{
				if(process_arr_tmp[j]->priority < process_arr_tmp[j-1]->priority)
				{
					//swap
					tmp = process_arr_tmp[j];
					process_arr_tmp[j] = process_arr_tmp[j-1];
					process_arr_tmp[j-1] = tmp;
				}else 
					break;
			}
		}
	}
	/*
	 * Finish Sort
	 * Then Run It with Ordered Array process_arr_tmp
	 */

	 /*
	  * Start Running ...
	  */
	i = 0;
	int tmp_time;
	int waiting_time = 0;
	int turnaround_time = 0;
	for( ; i < process_num ; i++)
	{
		process_arr_tmp[i]->waiting_time = waiting_time;
		tmp_time = process_arr_tmp[i]->cpu_burst_time;
		// sleep(tmp_time);
		turnaround_time += tmp_time;
		process_arr_tmp[i]->turnaround_time = turnaround_time;
		waiting_time += tmp_time;
	}
	/*
	 * Finish Running ...
	 */
}

/*
 * RR function
 * Round-Robin
 */
void RR()
{
	int array[process_num];
	int i = 0;
	for( ; i < process_num ; i++)
	{
		array[i] = process_arr[i]->cpu_burst_time;
	}
	int time_use = 0;
	int finished_process_num = 0;
	i = 0;
	while(1)
	{
		if(array[i] != 0)
		{
			if( i == 0 && array[i] == process_arr[i]->cpu_burst_time - quantum )
				process_arr[i]->waiting_time = time_use - quantum;

			if( array[i] == process_arr[i]->cpu_burst_time)
			{
				process_arr[i]->waiting_time = time_use;
			}
			if( quantum < array[i])
			{	
				// sleep(quantum);
				time_use += quantum;
				array[i] = array[i] - quantum;
			}else
			{
				// sleep(array[i]);
				time_use += array[i];
				process_arr[i]->turnaround_time = time_use;
				array[i] = 0;
				finished_process_num ++;
				if(finished_process_num == process_num)
					break;
			}
		}
		i = ( i + 1 ) % process_num;
	}
}

/*
 * 
 */
void commmon_info_echo()
{
	if(quantum == -1)
		printf("Quantum 	: 0\n");
	else
		printf("Quantum 	: %d\n",quantum);
	printf("Sch. File 	: %s\n", filename);
	printf("-------------------------------\n");
	printf("Arrival Order:");
	int i = 0;
	for( ; i < process_num ; i++)
	{
		printf("	%d,",process_arr[i]->pid);
	}
	printf("\n");
	printf("Process Information:\n");	
	i = 0 ;
	for( ; i < process_num ; i++)
	{
		printf("%d 		%d 		%d\n",process_arr[i]->pid,
			process_arr[i]->cpu_burst_time,
			process_arr[i]->priority);
	}
	printf("-------------------------------\n");
	printf("Running...\n");
	printf("-------------------------------\n");
}

void common_time_echo()
{
	/*
	 * Print time for each process
	 */
	int i = 0 ;
	int waiting_time_total = 0;
	int turnaround_time_total = 0;
	for( ; i < process_num ; i++)
	{
		waiting_time_total += process_arr[i]->waiting_time;
		turnaround_time_total += process_arr[i]->turnaround_time;
		printf("%d 		%d 		%d 		%d  	%d\n",process_arr[i]->pid,
			process_arr[i]->cpu_burst_time,
			process_arr[i]->priority,
			process_arr[i]->waiting_time,
			process_arr[i]->turnaround_time);
	}
	/*
	 * Calculate Average Time
	 */
	 float average_waiting_time = (float)waiting_time_total / process_num;
	 float average_turnaround_time = (float)turnaround_time_total / process_num;
	 printf("Avg. Waiting Time 		:	%.2f\n", average_waiting_time);
	 printf("Avg. Turnaround Time 		:	%.2f\n",average_turnaround_time);
	 printf("-------------------------------\n");
}

int is_valid_int(char* str){
	//TODO
	int start = 0;
		
	while( *str != '\0')
	{
		if(start)
		{
			if(!isdigit(*str)){
				return 0;
			}
		}else if(*str == '+' || *str== '-'|| isdigit(*str)){
			//printf("here");
			start =1;
		}else if(!isspace(*str))
		{
			return 0;
		}
		str++;			
	}
	return 1;
}

char** str_split(char* a_str, const char a_delim)
{
    char** result    = 0;
    size_t count     = 0;
    char* tmp        = a_str;
    char* last_comma = 0;
    char delim[2];
    delim[0] = a_delim;
    delim[1] = 0;

    /* Count how many elements will be extracted. */
    while (*tmp)
    {
        if (a_delim == *tmp)
        {
            count++;
            last_comma = tmp;
        }
        tmp++;
    }

    /* Add space for trailing token. */
    count += last_comma < (a_str + strlen(a_str) - 1);

    /* Add space for terminating null string so caller
       knows where the list of returned strings ends. */
    count++;

    result = malloc(sizeof(char*) * count);

    if (result)
    {
        size_t idx  = 0;
        char* token = strtok(a_str, delim);

        while (token)
        {
            assert(idx < count);
            *(result + idx++) = strdup(token);
            token = strtok(0, delim);
        }
        assert(idx == count - 1);
        *(result + idx) = 0;
    }

    return result;
}

void free_memory()
{	
	int i = 0;
	for( ; i < process_num ; i++)
	{
		free(process_arr[i]);
	}
	free(process_arr);
}

