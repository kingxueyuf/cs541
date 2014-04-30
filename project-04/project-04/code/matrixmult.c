/*
 * <AUTHOR>
 * <DATE>
 *
 * CS 441/541 : Memory Management Performance Template
 */
#include "matrixmult.h"

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

	    mtype_t *matrix_a = NULL;
	    mtype_t *matrix_b = NULL;
	    mtype_t *matrix_c = NULL;
	    
	    int N = upper_bound;
	    int i;
	    int logNum = 1;
	    for( i = 2 ; i <= N ; )
	    {
	    	// Allocate a matrix of size: N * N
	    	allocate_matrix(&matrix_a,i);
	    	allocate_matrix(&matrix_b,i);
	    	allocate_matrix(&matrix_c,i);

	    	clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

	    	double FLOPS_ijk = run_experiment_ijk(matrix_a,matrix_b,matrix_c,i);

	    	clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

	    	double FLOPS_ikj = run_experiment_ikj(matrix_a,matrix_b,matrix_c,i);

	    	clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

	    	double FLOPS_jik = run_experiment_jik(matrix_a,matrix_b,matrix_c,i);

	    	clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

			double FLOPS_jki = run_experiment_jki(matrix_a,matrix_b,matrix_c,i);

			clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

			double FLOPS_kij = run_experiment_kij(matrix_a,matrix_b,matrix_c,i);

			clear_matrix(matrix_a, i);
	    	clear_matrix(matrix_b, i);
	    	clear_matrix(matrix_c, i);

			double FLOPS_kji = run_experiment_kji(matrix_a,matrix_b,matrix_c,i);

	    	int matrix_bytes = sizeof(mtype_t) * i * i;

	    	printf("%d 		%d 		%.9f  	%.9f  	%.9f  	%.9f  	%.9f  	%.9f\n", i, matrix_bytes, FLOPS_ijk, FLOPS_ikj, FLOPS_jik, FLOPS_jki, FLOPS_kij, FLOPS_kji);

	    	free(matrix_a);
	    	free(matrix_b);
	    	free(matrix_c);

	    	i = i * 2;
	    	logNum++;
	    }
    	// Finalize the support library
    	support_finalize();
	}

    return 0;
}

double run_experiment_ijk(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
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
				for( k = 0 ; k < N ; ++k)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}

double run_experiment_ikj(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(i = 0 ; i < N ; ++i)
		{
			for(k = 0 ; k < N ; ++k)
			{
				for( j = 0 ; j < N ; ++j)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}


double run_experiment_jik(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(j = 0 ; j < N ; ++j)
		{
			for(i = 0 ; i < N ; ++i)
			{
				for( k = 0 ; k < N ; ++k)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}

double run_experiment_jki(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(j = 0 ; j < N ; ++j)
		{
			for(k = 0 ; k < N ; ++k)
			{
				for( i = 0 ; i < N ; ++i)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}


double run_experiment_kij(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(k = 0 ; k < N ; ++k)
		{
			for(i = 0 ; i < N ; ++i)
			{
				for( j = 0 ; j < N ; ++j)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}

double run_experiment_kji(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N)
{
	int i = 0;
	int j = 0;
	int k = 0;
	int iter;
	hptimer_t start,end;
	double final_time;

	start = get_time();
	for(iter = 0 ; iter < MAX_ITERS ; ++iter)
	{
		for(k = 0 ; k < N ; ++k)
		{
			for( j = 0 ; j < N ; ++j)
			{
				for(i = 0 ; i < N ; ++i)
				{
					matrix_c[ GET_INDEX(i, j, N)] += matrix_a[ GET_INDEX(i, k, N)] * matrix_b[ GET_INDEX(k, j, N)];
				}
			}
		}
	}
	end = get_time();
	final_time = diff_timers(start,end)/MAX_ITERS;
	double FLOPS = ( 2*N*N*N )/final_time;
	return FLOPS/1000000;
}
