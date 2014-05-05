/*
 * [yufan xue]
 * [05/04/2014]
 *
 * [Project 5 - Disk Scheduling Algorithm Simulator ]
 */

#include "scheduler.h"

int head_position = 0;
int head_direction = 0;
char *filename ;
int cylinders_number = 0;
int request_number = 0;
int LINELEN = 256;
int *request_queue;
/*
 *
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

int parse_command(int argc,char **argv)
{
	int i = 1;
	for(; i < argc ; i++)
	{
		if( 0 ==strcmp("-h",argv[i]) ){
		   /*
		 	* To judge the current head position
		 	*/
			i++; //i = i + 1
			if (i >= argc){
				//Invalid input
				//Return 0 -> error
				printf("i >= argc");
				return 0;
			}else if( is_valid_int(argv[i]) ){
				head_position = strtol(argv[i],NULL,10);

			}else{
				printf("Didn't provide current head position\n");
				return 0;
			}
		}else if( 0 ==strcmp("-d",argv[i]) )
		{
			/*
			 * To judge the direction of disk head
			 */
			i++;
			if(i >= argc){
				printf("i >= argc");
				return 0;
			}else if( is_valid_int(argv[i]) ){
				head_direction = strtol(argv[i],NULL,10);
				if( head_direction == 0 || head_direction == 1 ){
						//
				}else{
					printf("Invalid head direction, number can only be '0' or '1'\n");
					return 0;
				}
			}else{
				printf("Invalid head direction, direction must be represent by number\n");
				return 0;
			}
		}else{
			filename = argv[i];
		}
	}
	return 1;
}


void schedule()
{
	int valid = process_file(filename);
	if(!valid){
		printf("Error: Valid input file\n");
	}else{

		OUTPUT_CONFIGURATION();

		int fcfs = FCFS();
		int sstf = SSTF();
		int scan = SCAN();
		int cscan = CSCAN();
		int look = LOOK();
		int clook = CLOOK();

		printf("	%d 		FCFS\n", fcfs);
		printf("	%d 		SSTF\n", sstf);
		printf("	%d 		SCAN\n", scan);
		printf("	%d 		CSCAN\n", cscan);
		printf("	%d 		LOOK\n", look);
		printf("	%d 		CLOOK\n", clook);
	
	}
	free_memory(request_queue);
}

void OUTPUT_CONFIGURATION()
{
	printf("Queue File 			:	%s\n", filename);
	printf("Num. Disk Cylinders 		:	%d\n", cylinders_number);
	printf("Host Position 			:	%d\n", head_position);
	printf("Head Direction			:	%d\n", head_direction);
	printf("------------------------------\n");

}
/*
 * First-come, first-served
 */
int FCFS()
{
	int total_amount = 0 ;
	int i = 0 ;
	int last = head_position;
	for(; i < request_number ; i++){
		total_amount += abs( request_queue[i]- last );
		last = request_queue[i];
	}
	return total_amount;
}

/*
 * Shortest-seek-time-first
 */
int SSTF()
{	
	int* copy_array; 
	copy_array1(request_queue,request_number, &copy_array);
	/*
	 * Ascending sequence
	 */
	sort(copy_array,request_number);


	int total_amount = 0;
	int i = 0; 
	int left = 0;
	int right = 0;
	if( head_position <= copy_array[0])
	{
		// head_position < 0
		total_amount += copy_array[request_number-1] - head_position;
		return total_amount;
	}else if( head_position >= copy_array[request_number-1] )
	{
		// request_number - 1 < head_position
		total_amount += head_position - copy_array[0];
		return total_amount;
	}else{
		for(;i < request_number ; i++)
		{
			if(copy_array[i] >= head_position )
			{
				left = i-1;
				right = i;
				break;
			}
		}
	}

	int current_position = head_position;
	int dis_to_left = 0;
	int dis_to_right = 0;
	while( left != -1 && right != request_number){
		// printf("%d\n", total_amount);
		dis_to_left = abs(current_position- copy_array[left]);
		dis_to_right = abs(current_position - copy_array[right]);
		if(dis_to_left <= dis_to_right)
		{
			total_amount += dis_to_left;
			current_position = copy_array[left];
			left--;
		}else{
			total_amount += dis_to_right;
			current_position = copy_array[right];
			right++;
		}
	}

	if( left == -1)
	{
		for(; right < request_number ; right++){
			total_amount+= abs(copy_array[right] - current_position);
			current_position = copy_array[right];
		}
	}else if( right == request_number ){
		for(; left >= 0 ; left --){
			total_amount += abs(copy_array[left] - current_position);
			current_position = copy_array[left];
		}
	}


	free_memory(copy_array);
	return total_amount;
}


/*
 * Elevator SCAN
 */
int SCAN()
{
	int total_amount=0;
	int* copy_array;
	copy_array1(request_queue,request_number,&copy_array);
	/*
	 * Ascending sequence
	 */
	sort(copy_array,request_number);

	if( head_direction == 0 )
	{
		if(head_position <= copy_array[0])
		{
			//head_position < 0
			total_amount += copy_array[request_number-1] + head_position;
		}else if(head_position <= copy_array[request_number-1])
		{
			// 0 < head_position < request_number - 1
			total_amount += head_position;
			total_amount += copy_array[request_number-1];	
		}else{
			// request_number - 1 < head_position
			total_amount += head_position;
		}
	}else if( head_direction == 1 ){
		if(head_position <= copy_array[0])
		{
			//head_position < 0
			total_amount += copy_array[request_number-1] - head_position;
		}else if(head_position <= copy_array[request_number-1])
		{
			// 0 < head_position < request_number - 1
			total_amount += cylinders_number - 1 - head_position;
			total_amount += cylinders_number - 1 - copy_array[0];	
		}else{
			// request_number - 1 < head_position
			total_amount += cylinders_number - head_position + copy_array[0];
		}
	}
	free_memory(copy_array);
	return total_amount;

}

/*
 * Circular Elevator 
 */
int CSCAN()
{
	int total_amount=0;
	int* copy_array;
	copy_array1(request_queue,request_number,&copy_array);
	/*
	 * Ascending sequence
	 */
	sort(copy_array,request_number);

	int i = 0 ;
	int index = 0;
	for(; i < request_number ; i++){
		if( copy_array[i] >= head_position ){
			index = i;
			break;
		}
	}

	if( head_direction == 0 ){		
		// current_position -> 0 -> cylinders_number -> 
		
		if(head_position >= copy_array[request_number-1])
		{
			// request_number -1 < head_position 
			total_amount += head_position;
		}else if(head_position <= copy_array[0] )
		{
			// head_position < 0
			total_amount += head_position;
			total_amount += cylinders_number - 1 ;
			total_amount += cylinders_number - 1 - copy_array[0];
		}else{
			// 0 < head_position < request_number - 1
			total_amount += head_position;
			total_amount += cylinders_number - 1;
			total_amount += cylinders_number - 1 - copy_array[index];
		}
	}else if( head_direction == 1 ){

		if(head_position >= copy_array[request_number-1]){

			total_amount += cylinders_number - 1 - head_position;
			total_amount += cylinders_number - 1;
			total_amount += copy_array[index-1];

		}else if( head_position <= copy_array[0]){
			total_amount += copy_array[request_number-1] - 1 - head_position;

		}else{
			total_amount += cylinders_number - 1 - head_position;
			total_amount += cylinders_number - 1;
			total_amount += copy_array[index-1];
		}
	}

	free_memory(copy_array);
	return total_amount;
}

int LOOK()
{
	int total_amount = 0;
	int* copy_array;
	copy_array1(request_queue,request_number,&copy_array);
	/*
	 * Ascending sequence
	 */
	sort(copy_array,request_number);

	if( head_direction == 0 ){
		if(head_position <= copy_array[0]){
			// head_position <= 0
			total_amount += copy_array[request_number-1] - head_position;
		}else{
			if( head_position <= copy_array[request_number-1] ){
				// 0 < head_position < request_number -1
				total_amount += (head_position - copy_array[0]);
				total_amount += copy_array[request_number-1] - copy_array[0];
			}else{
				// request_number - 1 < head_position
				total_amount += head_position - copy_array[0];
			}
		}
	}else if( head_direction == 1 ){
		if(head_position <= copy_array[0]){
			// head_position <= 0
			total_amount += copy_array[request_number-1] - head_position;
		}else{
			if( head_position <= copy_array[request_number-1] ){
				// 0 < head_position < request_number -1
				total_amount += copy_array[request_number-1] - head_position;
				total_amount += copy_array[request_number-1] - copy_array[0];
			}else{
				// request_number - 1 < head_position
				total_amount += head_position - copy_array[0];
			}
		}
	}

	free_memory(copy_array);
	return total_amount;
}

int CLOOK()
{
	int total_amount = 0;
	int* copy_array; 
	copy_array1(request_queue,request_number,&copy_array);
	/*
	 * Ascending sequence
	 */
	sort(copy_array,request_number);

	int i = 0;
	int index = 0;
	for(; i < request_number ; i++){
		if(copy_array[i] >= head_position){
			index = i;
			break;
		}
	}

	if(head_direction == 0){
		if(head_position <= copy_array[0]){
			// head_position < 0
			total_amount += copy_array[request_number-1] - head_position;
			total_amount += copy_array[request_number-1] - copy_array[0];
		}else{
			if(head_position <= copy_array[request_number-1]){
				// 0 < head_position < request_number - 1
				total_amount += head_position - copy_array[0];
				total_amount += copy_array[request_number-1] - copy_array[0];
				total_amount += copy_array[request_number-1] - copy_array[index];
			}else{
				// request_number - 1 < request_number -1
				total_amount += head_position - copy_array[0];
			}
		}
	}else if( head_direction == 1 ){
		if(head_position <= copy_array[0]){
			// head_position < 0
			total_amount += copy_array[request_number-1] - head_position;
		}else{
			if(head_position <= copy_array[request_number-1]){
				// 0 < head_position < request_number - 1
				total_amount += copy_array[request_number-1] - head_position;
				total_amount += copy_array[request_number-1] - copy_array[0];
				total_amount += copy_array[index-1] - copy_array[0];
			}else{
				// request_number - 1 < request_number -1
				total_amount += head_position - copy_array[0];
			}
		}
	}


	free_memory(copy_array);
	return total_amount;
}
/*
 * Bubble sort
 * Ascending order
 */
void sort(int* array,int array_length)
{
	int i = 0 ; 
	int j;
	int tmp;
	for(; i <array_length - 1; i++)
	{
		if(array[i] > array[i+1])
		{
			tmp = array[i];
			array[i] = array[i+1];
			array[i+1] = tmp;
			for(j = i ; j >= 1 ; j--)
			{
				if(array[j-1] > array[j])
				{
					tmp = array[j-1];
					array[j-1] = array[j];
					array[j] = tmp;
				}else{
					break;
				}
			}
		}
	}

	// for(i = 0 ; i < request_number ; i ++)
	// {
	// 	printf("%d\n", array[i]);
	// }
}

/*
 *	Copy_array
 *  Return int*
 */
void copy_array1(int* arr_to_copy,int array_length,int **copy_array)
{
	// printf("here\n");
	*copy_array = malloc(sizeof(int)*array_length);
	int i = 0;
	for(; i < array_length ; i++)
	{
		(*copy_array)[i] = arr_to_copy[i];
		// printf("%d\n",arr_to_copy[i]);
	}
	// printf("here\n");
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

	int rownum = 0;
	int pointer = 0;
    while( 0 == feof(fd) ) {
    	/* Read input from the user */
    	fgets_rtn = fgets(buffer, LINELEN, fd);
    	if( NULL == fgets_rtn) {
            /* Ctrl-D */
            break;
        }

    	if( rownum == 0 ){
        	//First Row
        	cylinders_number = strtol(buffer,NULL,10);
        	if(head_position >= cylinders_number){
        		printf("Error: Head position is larger or equal cylinders_number");
        		return 0;
        	}
        }else if( rownum == 1 ){
        	request_number = strtol(buffer,NULL,10);
        	request_queue = (int*)malloc( sizeof(int)* request_number);

        }else{
        	request_queue[pointer] = strtol(buffer,NULL,10);
        	pointer++;
        }
        rownum++;
   	}

	// TODO: Close the file
    fclose(fd);
    return 1;
}


/* 
 * Judge whether the string is a valid int
 */
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

/*
 * Free the memory malloced before
 */
void free_memory(int *array)
{	
	free(array);
}