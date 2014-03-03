/*
 * Josh Hursey
 * Oct. 10, 2013
 *
 * CS 441/541: Example - add some numbers
 *
 */
#include "bounded-buffer.h"

/*
 * Buffer of random integers
 * Shared between all threads in the process
 */
int *buffer;

/*
 * Semaphore(s)
 */
semaphore_t empty;
semaphore_t mutex;
semaphore_t full;

/*
 * Pointers to buffer[]
 */
int in = 0; /* Next Free */
int out = 0; /* First Full */

/*
 * Time for each Thread to sleep
 */
int time_to_live = 0;

/*
 *
 */
int buffer_size;


int main(int argc, char * argv[]) {
    int ret;
    /*
     * Initialize:
     * - buffer
     * - random number generator
     * - semaphores
     */
    srandom(time(NULL));

    int producer_number = 0;

    int consumer_number = 0;

    buffer_size = 0;

    int valid = check_command_line(argc,argv,&producer_number,&consumer_number,&buffer_size,&time_to_live);

    pthread_t producer_threads[producer_number];

    pthread_t consumer_threads[consumer_number];

    if(valid)
    {   
    
        /*
         * Initialize:
         * - buffer[] = malloc(buffer_size)
         * - mutex = 1
         * - empty = buffer_size
         * - full = 0
         */

        buffer = (int *)malloc(sizeof(int) * buffer_size );

        if (buffer==NULL) {
            printf("Error allocating memory!\n"); //print an error message
            return 1; //return with failure
        }

        if( 0 != (ret = semaphore_create(&mutex, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&empty, buffer_size)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&full, 0)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }

        /*
         * Print out initial buffer
         */
        printf("Initial Buffer:                       ");
        output_buffer();

        /*
         * Create threads
         */
        /* Create Producer */
        for(int i = 0 ; i <producer_number ; i++)
        {      
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&producer_threads[i],
                             NULL,
                             produce,
                             (void *)(intptr_t)i);
            if(0 != ret ) {
                fprintf(stderr, "Error: Cannot Create thread\n");
                exit(-1);
            }
        }

        /* Create Consumer */
        for(int i = 0 ; i <consumer_number ;i++)
        {
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&consumer_threads[i],
                             NULL,
                             consume,
                             (void *)(intptr_t)i);
            if(0 != ret ) {
                fprintf(stderr, "Error: Cannot Create thread\n");
                exit(-1);
            }
        }

    }else{
        printf("Error: valid input command line\n");
        pthread_exit(NULL);
    }

    /*
     * Join Thread(s)
     */
     /* Join producer thread */
    for(int i = 0; i < producer_number; ++i ) {
        ret = pthread_join(producer_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }
    /* Join consumer thread */
    for(int i = 0; i < consumer_number; ++i ) {
        ret = pthread_join(consumer_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }

    /*
     * Cleanup
     */
    if( 0 != (ret = semaphore_destroy(&mutex)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&empty)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&full)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }

    pthread_exit(NULL);

    return 0;
}

void *produce(void *threadid)
{
    int ret;
    while(1)
    {
        int tid = (intptr_t)threadid;
        /*
         * Critical Section to update buffer
         */
        /*  Wait the semaphore empty  */
        if( 0 != (ret = semaphore_wait(&empty)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        
        /* Critical Section start */

        int random_num = random() % 10;
        buffer[in] = random_num;
        in = (in + 1) % buffer_size;

        printf("Producer    %d: Produced    %d          ",tid,random_num);
        output_buffer();

        sleep(time_to_live); /* Just to simulate some work, so we can see the threads synchronize */
        
        /* Critical Section end */

        /* Signal the semaphore mutex */
        if( 0 != (ret = semaphore_post(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        /* Signal the semaphore empty */
        if( 0 != (ret = semaphore_post(&full)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);
    
}

void *consume(void *threadid)
{
    int ret;
    while(1)
    {
        int tid = (intptr_t)threadid;
        /*
         * Critical Section to update buffer
         */
        /*  Wait the semaphore empty  */
        if( 0 != (ret = semaphore_wait(&full)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        
        /* Critical Section start */
        int consume_num = buffer[out];
        buffer[out] = -1;
        out = (out + 1) % buffer_size;

        printf("Consumer    %d: Consumed    %d          ",tid,consume_num);
        output_buffer();

        sleep(time_to_live); /* Just to simulate some work, so we can see the threads synchronize */
        
        /* Critical Section end */

        /* Signal the semaphore mutex */
        if( 0 != (ret = semaphore_post(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
        /* Signal the semaphore empty */
        if( 0 != (ret = semaphore_post(&empty)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }
    }
    pthread_exit(NULL);

}

int check_command_line(int argc, char * argv[], int *producer_number, int* consumer_number, int* buffer_size, int* time_to_live)
{
    if(argc == 5)
    {
        //BUFFER_SIZE was setted by input command line
        int i = 1;
        for( ; i<argc; i++)
        {
            if( !is_valid_int(argv[i]) )
            {
                //Invalid input
                return 0;
            }else
            {
                /*
                 * Valid integer
                 * Then check whether it is positive integer
                 */
                int convert_int = strtol(argv[i],NULL,10); //Convert char to int
                if(convert_int <= 0 )
                {
                    //Invalid input
                    return 0;
                }

            }
        }
        //All parameters are valid
        *time_to_live = strtol(argv[1],NULL,10);
        *producer_number = strtol(argv[2],NULL,10);
        *consumer_number = strtol(argv[3],NULL,10);
        *buffer_size = strtol(argv[4],NULL,10);


        //Output initial variables
        printf("Buffer Size                 :   %d\n",*buffer_size);
        printf("Time To Live (seconds)      :   %d\n",*time_to_live);
        printf("Number of Producer threads  :   %d\n",*producer_number);
        printf("Number of Consumer threads  :   %d\n",*consumer_number);
        printf("----------------------------------\n");
        //Return success
        return 1;
    }else if(argc == 4){
        //Input does not set the BUFFER_SIZE
        int i = 1;
        for( ; i<argc; i++)
        {
            if( !is_valid_int(argv[i]) )
            {
                //Invalid input
                return 0;
            }else
            {
                /*
                 * Valid int 
                 * Then check wether it is positive integer
                 */
                int convert_int = strtol(argv[i],NULL,10); //Convert char to int
                if(convert_int <= 0 )
                {
                    //Invalid input
                    return 0;
                }

            }
        }
        //All parameters are valid
        *time_to_live = strtol(argv[1],NULL,10);
        *producer_number = strtol(argv[2],NULL,10);
        *consumer_number = strtol(argv[3],NULL,10);
        *buffer_size = 10; //BUFFER_SIZE is set to 10 default

        printf("Buffer Size                 :   %d\n",*buffer_size);
        printf("Time To Live (seconds)      :   %d\n",*time_to_live);
        printf("Number of Producer threads  :   %d\n",*producer_number);
        printf("Number of Consumer threads  :   %d\n",*consumer_number);
        printf("----------------------------------\n");

        return 1;
    }else
    {
        //Invalid input
        return 0;
    }
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

void output_buffer()
{
    printf("[");
    for(int i = 0 ; i <buffer_size ; i++)
    {
        printf("    %d",buffer[i]);
        if(i == in)
            printf("^");
        if(i == out)
            printf("v");
    }
    printf("    ]\n");
}


