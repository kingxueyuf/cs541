/*
 * <AUTHOR>
 * <DATE>
 *
 * CS 441/541 : Memory Management Performance Template
 */
#include "scalarmult.h"

int MAX_ITERS = 10000;
int main(int argc, char * argv[]) {


	if(argc != 2)
	{
		printf("please input valid upper bound size\n");
	}else
	{
		int upper_bound = strtol(argv[1],NULL,10);
		// Initialize the support library
	    support_init();

	    mtype_t *matrix = NULL;
	    int N = upper_bound;
	    int i;
	    int logNum = 1;
	    for( i = 2 ; i <= N ; )
	    {
	    	// Allocate a matrix of size: N * N
	    	allocate_matrix(&matrix,i);

	    	// Clear/Initalize the matrix
	    	clear_matrix(matrix, i);

	    	srand(time(NULL));
			int r = rand();

	    	double FLOPS_1 = run_experiment_ij(matrix,r,i);

	    	clear_matrix(matrix, i);

	    	double FLOPS_2 = run_experiment_ji(matrix,r,i);

	    	int matrix_bytes = sizeof(mtype_t) * i * i;

	    	printf("%d 		%d 		%.9f  	%.9f\n", i, matrix_bytes, FLOPS_1, FLOPS_2);

	    	free(matrix);

	    	i = i * 2;
	    	logNum++;
	    }
	    
	    // Finalize the support library
	    support_finalize();
	}

    return 0;
}


double run_experiment_ij(mtype_t *matrix,
                         mtype_t scalar, int N) {
	int i = 0;
	int j = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(i = 0 ; i < N ; ++i)
		{
			for(j = 0 ; j < N ; ++j)
			{
				matrix[ GET_INDEX(i, j, N)] = matrix[ GET_INDEX(i, j, N)] * scalar;
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( N*N )/final_time;
	return FLOPS/1000000;
}


double run_experiment_ji(mtype_t *matrix,
                         mtype_t scalar, int N) {
	int i = 0;
	int j = 0;	
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(j = 0 ; j < N ; ++j)
		{
			for(i = 0 ; i < N; ++i)
			{
				matrix[ GET_INDEX(i, j, N)] = matrix[ GET_INDEX(i, j, N)] * scalar;
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( N*N )/final_time;
	return FLOPS/1000000;
}
