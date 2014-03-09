/*
 * Yufan Xue
 * Mar. 09, 2014
 *
 * CS 441/541 project-02-part2
 *
 */
#include "pirates-ninjas.h"

/*
 * 
 */
semaphore_t *pirate_queue_semaphore;
semaphore_t *ninja_queue_semaphore;

/*
 *
 */
char * queue[];

/*
 *
 */
int currrent_pirates_number = 0;
int currrent_ninjas_number = 0;

/*
 * Semaphore(s)
 */
semaphore_t department;
semaphore_t team;
semaphore_t mutex;


int main(int argc, char * argv[]) {
    int ret;
    int stderr;
    /*
     * Initialize:
     * - 
     * - 
     * - 
     */
    srandom(time(NULL));

    /*  Number of Pirates */
    int pirates_number = 0;

    /*  Number of Ninjas */
    int ninjas_number = 0;
    
    /*  Time to run in seconds */
    int time_to_run = 0;

    int valid = check_command_line(argc,argv,&pirates_number,&ninjas_number,&time_to_run);

    pthread_t pirates_threads[pirates_number];

    pthread_t ninjas_threads[ninjas_number];

    if(valid)
    {   
    
        /*
         * Initialize:
         * - 
         * - 
         * - 
         * - 
         */

        if( 0 != (ret = semaphore_create(&department, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&team, 2)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }
        if( 0 != (ret = semaphore_create(&mutex, 1)) ) {
            fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
            return -1;
        }

        pirate_queue_semaphore = (semaphore_t**)malloc(sizeof(semaphore_t*)* (time_to_run*5));
        for(int i = 0 ; i < time_to_run* 5 ; i++)
        {
            if( 0 != (ret = semaphore_create(pirate_queue_semaphore[i], 0)) ) {
                fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
                return -1;
            }
        }

        ninja_queue_semaphore = (semaphore_t**)malloc(sizeof(semaphore_t*)* (time_to_run*5));
        for(int i = 0 ; i < time_to_run* 5 ; i++)
        {
            if( 0 != (ret = semaphore_create(ninja_queue_semaphore[i], 0)) ) {
                fprintf(stderr, "Error: semaphore_create() failed with %d\n", ret);
                return -1;
            }
        }

        queue = (char**)malloc(sizeof(char*) * 1);


        /*
         * Create threads
         */
        /* Create Pirates */
        for(int i = 0 ; i <pirates_number ; i++)
        {      
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&pirates_threads[i],
                             NULL,
                             pirate,
                             (void *)(intptr_t)i);
            if(0 != ret ) {
                fprintf(stderr, "Error: Cannot Create thread\n");
                exit(-1);
            }
        }

        /* Create Ninjas */
        for(int i = 0 ; i <ninjas_number ;i++)
        {
            // printf("In Main(): Creating thread %d\n", i);
            ret = pthread_create(&ninjas_threads[i],
                             NULL,
                             ninja,
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
    for(int i = 0; i < pirates_number; ++i ) {
        ret = pthread_join(pirates_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }
    /* Join consumer thread */
    for(int i = 0; i < ninjas_number; ++i ) {
        ret = pthread_join(ninjas_threads[i], NULL);
        if( 0 != ret ) {
            fprintf(stderr, "Error: Cannot Join Thread %d\n", i);
            exit(-1);
        }
    }

    /*
     * Cleanup
     */
    if( 0 != (ret = semaphore_destroy(&department)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    if( 0 != (ret = semaphore_destroy(&team)) ) {
        fprintf(stderr, "Error: semaphore_destroy() failed with %d\n", ret);
        return -1;
    }
    
    pthread_exit(NULL);

    return 0;
}

void *pirate(void *threadid)
{
    int ret;
    int tid = (intptr_t)threadid;
    while(1)
    {
        printf("Pirate      %d | Waiting \n",tid);
        /*
         * Critical Section to update buffer
         */
         /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        currrent_pirates_number++;

        int queue_size = sizeof(queue)/sizeof(queue[0]);

        queue = realloc( (queue_size+1)*sizeof(char*) );

        queue[queue_size-1] = malloc(sizeof(char) * 1);
        
        *queue[queue_size-1] = 'p';

        if( 0 != (ret = semaphore_post(pirate_queue_semaphore[queue_size-1])) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if( 0 != (ret = semaphore_post(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if(queue_size-1==0)
        {
            if( 0 != (ret = semaphore_wait(&department)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
            if( 0 != (ret = semaphore_wait(&team)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }

            printf("Pirate      %d | Costume preparation\n",tid);


            if( 0 != (ret = semaphore_post(&team)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }

        }else
        {
            if(queue[queue_size-2] =='p')
            {
                /* last one is pirate */
                if( 0 != (ret = semaphore_wait(pirate_queue_semaphore[queue_size-2])) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
                if( 0 != (ret = semaphore_wait(&team)) ) {
                    fprintf(stderr, "Error: semaphorew_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                printf("Pirate      %d | Costume preparation\n",tid);
                int random_num = random() % 5000;
                usleep(random_num);

                if( 0 != (ret = semaphore_post(&team)) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
            }else if( queue[queue_size-2] == 'q')
            {
                if( 0 != (ret = semaphore_wait(&department)) ) {
                    fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                if( 0 != (ret = semaphore_wait(&team)) ) {
                    fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                printf("Pirate      %d | Costume preparation\n",tid);
                int random_num = random() % 5000;
                usleep(random_num);

                if( 0 != (ret = semaphore_post(&team)) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
            }
        }
        currrent_pirates_number--;
        if(currrent_pirates_number ==0)
        {
            if( 0 != (ret = semaphore_post(&department)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
        }

        printf("Pirate      %d | Leaving\n",tid);
        /* Just to simulate some work, so we can see the threads synchronize */
        int random_num = random() % 1000;
        usleep(random_num);
    }    
    pthread_exit(NULL);
}

void *ninja(void *threadid)
{
    int ret;
    int tid = (intptr_t)threadid;
    while(1)
    {
        printf("Ninja       %d | Waiting \n",tid);
        /*
         * Critical Section to update buffer
         */
         /*  Wait the semaphore mutex  */
        if( 0 != (ret = semaphore_wait(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        currrent_ninjas_number++;

        int queue_size = sizeof(queue)/sizeof(queue[0]);

        queue = realloc( (queue_size+1)*sizeof(char*) );

        queue[queue_size-1] = malloc(sizeof(char) * 1);
        
        *queue[queue_size-1] = 'p';

        if( 0 != (ret = semaphore_post(ninja_queue_semaphore[queue_size-1])) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if( 0 != (ret = semaphore_post(&mutex)) ) {
            fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
            pthread_exit(NULL);
        }

        if(queue_size-1==0)
        {
            if( 0 != (ret = semaphore_wait(&department)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
            if( 0 != (ret = semaphore_wait(&team)) ) {
                fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }

            printf("Ninja       %d | Costume preparation\n",tid);


            if( 0 != (ret = semaphore_post(&team)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }

        }else
        {
            if(queue[queue_size-2] =='p')
            {
                /* last one is pirate */
                if( 0 != (ret = semaphore_wait(ninja_queue_semaphore[queue_size-2])) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
                if( 0 != (ret = semaphore_wait(&team)) ) {
                    fprintf(stderr, "Error: semaphorew_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                printf("Ninja       %d | Costume preparation\n",tid);
                int random_num = random() % 5000;
                usleep(random_num);

                if( 0 != (ret = semaphore_post(&team)) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
            }else if( queue[queue_size-2] == 'q')
            {
                if( 0 != (ret = semaphore_wait(&department)) ) {
                    fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                if( 0 != (ret = semaphore_wait(&team)) ) {
                    fprintf(stderr, "Error: semaphore_wait() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }

                printf("Ninja       %d | Costume preparation\n",tid);
                int random_num = random() % 5000;
                usleep(random_num);

                if( 0 != (ret = semaphore_post(&team)) ) {
                    fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                    pthread_exit(NULL);
                }
            }
        }
        currrent_ninjas_number--;
        if(currrent_pirates_number ==0)
        {
            if( 0 != (ret = semaphore_post(&department)) ) {
                fprintf(stderr, "Error: semaphore_post() failed with %d in thread %d\n", ret, tid);
                pthread_exit(NULL);
            }
        }

        printf("Pirate      %d | Leaving\n",tid);
        /* Just to simulate some work, so we can see the threads synchronize */
        int random_num = random() % 1000;
        usleep(random_num);
    }    
    pthread_exit(NULL);

}

int check_command_line(int argc, char * argv[], int *pirates_number, int* ninjas_number, int* time_to_run)
{

    switch(argc)
    {
        case 2:
        /* time to run */
        if( !is_valid_int(argv[1]))
            return 0;
        else
        {
            int convert_int = strtol(argv[1],NULL,10);
            if(convert_int < 0)
                return 0;
            else
            {
                *time_to_run = convert_int;
                *pirates_number = 5;
                *ninjas_number = 5;
                return 1;
            }
        }
        break;

        case 3:
        if( is_valid_int(argv[1]) && is_valid_int(argv[2]) )
        {
            int convert_int1 = strtol(argv[1],NULL,10);
            int convert_int2 = strtol(argv[2],NULL,10);
            if( convert_int1 > 0 && convert_int2 >0)
            {
                *time_to_run = convert_int1;
                *pirates_number = convert_int2;
                return 1;

            }else
                return 0;
        }else
            return 0;
        break;
        
        case 4:
        if( is_valid_int(argv[1]) && is_valid_int(argv[2]) && is_valid_int(argv[3]) )
        {
            int convert_int1 = strtol(argv[1],NULL,10);
            int convert_int2 = strtol(argv[2],NULL,10);
            int convert_int3 = strtol(argv[3],NULL,10);
            if( convert_int1 > 0 && convert_int2 >0 && convert_int3 > 0)
            {
                *time_to_run = convert_int1;
                *pirates_number = convert_int2;
                *ninjas_number = convert_int3;
                return 1;

            }else
                return 0;
        }else
            return 0;
        break;
        
        default:
        return 0;
        break;
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


