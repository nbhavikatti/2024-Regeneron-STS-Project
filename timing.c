#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

static inline double get_current_time( )
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return (double)tp.tv_sec+(1.e-6)*tp.tv_usec;
}

			int cache_functionLRU (int M, int *time, int ischanged, int id, 
						int *isincache, int *timestamps, int *changed,
						int *read, int *write) {
				(*time)++;
				int itisincache = 0;
				for (int p = 0; p < M; p++) {
					if (id == isincache[p]) {
						timestamps[p] = (*time);
						itisincache = 1;
						changed[p] = ischanged;
					} 
				}
				
				if (itisincache == 0) {
					int oldest_time = (*time);
					int oldest_loc = M;
					for (int p = 0; p < M; p++) {
						if (timestamps[p] < oldest_time) {
							oldest_time = timestamps[p];
							oldest_loc = p;
						}
					}
					if (changed[oldest_loc] == 1) (*write)++;
					(*read)++;
					isincache[oldest_loc] = id;
					changed[oldest_loc] = ischanged;
					timestamps[oldest_loc] = (*time);
				}
				return 0;
			}



int main(int argc, char ** argv) {

	int i, j, k, n, v, r, M;
	double cminusdsquare = 0.000;
	double errorval = 0.000;
	double **A, **B, **C, **D;
	double normA, normR, tmp;
	int bi, bj, bk;
	double elapsed;
	srand(0);

    n = 10;
	M = 200;
	bi = 1;
	bj = 1;
	bk = 1;
	for(i = 1; i < argc; i++){
		if( strcmp( *(argv + i), "-n") == 0) {
			n  = atoi( *(argv + i + 1) );
			i++;
		}
		if( strcmp( *(argv + i), "-bi") == 0) {
			bi  = atoi( *(argv + i + 1) );
			i++;
		}
		if( strcmp( *(argv + i), "-bj") == 0) {
			bj  = atoi( *(argv + i + 1) );
			i++;
		}
		if( strcmp( *(argv + i), "-bk") == 0) {
			bk  = atoi( *(argv + i + 1) );
			i++;
		}
		if( strcmp( *(argv + i), "-M") == 0) {
			M  = atoi( *(argv + i + 1) );
			i++;
		}
	}
	A = (double **) malloc( n * sizeof(double*));
 	for(i = 0; i < n; i++){
		A[i] = (double *) malloc( n * sizeof(double));
	}

	B = (double **) malloc( n * sizeof(double*));
 	for(i = 0; i < n; i++){
		B[i] = (double *) malloc( n * sizeof(double));
	}

	C = (double **) malloc( n * sizeof(double*));
 	for(i = 0; i < n; i++){
		C[i] = (double *) malloc( n * sizeof(double));
	}

	D = (double **) malloc( n * sizeof(double*));
 	for(i = 0; i < n; i++){
		D[i] = (double *) malloc( n * sizeof(double));
	}

 	for(i = 0; i < n; i++) {
 		for(j = 0; j < n; j++) {
			A[i][j] = (double)rand() / (double)(RAND_MAX) - 0.5e+00;
		}
	}
 	for(i = 0; i < n; i++) {
 		for(j = 0; j < n; j++) {
			B[i][j] = (double)rand() / (double)(RAND_MAX) - 0.5e+00;
		}
	}

	for(i = 0; i < n; i++)
 		for(j = 0; j < n; j++)
			C[i][j] = 0.0e+00;

	for(i = 0; i < n; i++)
 		for(j = 0; j < n; j++)
			D[i][j] = 0.0e+00;
	int write = 0;
	int read = 0;
/*************************************************************/	
	int ib, jb, kb, io, b;

	elapsed = -get_current_time();

	for(i = 0; i < n; i += bi){
 		for(j = 0; j < n; j += bj){
			for(k = 0; k < n; k += bk){	
				for(ib = i; (ib < i + bi)&&(ib < n); ib++){
 					for(jb = j; (jb < j + bj)&&(jb < n); jb++) {
						for(kb = k; (kb < k + bk)&&(kb < n); kb++) {
		 					D[ib][jb] += A[ib][kb]*B[kb][jb];
						}
					}
				}
			}
		}
	}
	elapsed += get_current_time();
	printf(" n = %5d; bi = %5d; bj = %5d; bk = %5d;", n, bi, bj, bk);
	printf("elapsed: %f\n", elapsed);	

	for(i = 0; i < n; i++){
		free( C[i] );
	}
	free( C );

 	for(i = 0; i < n; i++){
		free( B[i] );
	}
	free( B );

 	for(i = 0; i < n; i++){
		free( A[i] );
	}
	free( A );
	return 0;
}
