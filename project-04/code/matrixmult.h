/*
 * <AUTHOR>
 * <DATE>
 *
 * CS 441/541 : Memory Management Performance Template
 */
#include "support.h"

 double run_experiment_ijk(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);

 double run_experiment_ikj(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);

 double run_experiment_jik(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);

 double run_experiment_jki(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);

 double run_experiment_kij(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);

 double run_experiment_kji(mtype_t *matrix_a,
						  mtype_t *matrix_b,
						  mtype_t *matrix_c, int N);
