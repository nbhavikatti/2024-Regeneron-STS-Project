#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

	srand(0);

    n = 10;
	M = 200;
	bj = 1;
	bi = 1;
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
	int *isincache;
	isincache = (int *) malloc( M * sizeof(int));
	for (int i = 0; i < M; i++) {
		isincache[i] = 3*n*n;
	}

	int *timestamps;
	timestamps = (int*) malloc( M * sizeof(int));

	//increment for EVERY read and write

	int *changed;
	changed = (int*) malloc( M * sizeof(int));

	//0 means no change (A or B)
	//1 means change (C)

	//start counting from 0 to keep consistency
	for (int i = 0; i < M; i++) {
		timestamps[i] = i;
	}
	int time = M - 1;
	for (int i = 0; i < M; i++) {
		changed[i] = 0; 
	}

int **Aid;
Aid = (int **)malloc(n * sizeof(int *));

int **Bid;
Bid = (int **)malloc(n * sizeof(int *));

int **Cid;
Cid = (int **)malloc(n * sizeof(int *));

for (i = 0; i < n; i++) {
    Aid[i] = (int *)malloc(n * sizeof(int));
    Bid[i] = (int *)malloc(n * sizeof(int));
    Cid[i] = (int *)malloc(n * sizeof(int));
}

for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) {
        Aid[i][j] = 0*n*n + n*j + i;
		Bid[i][j] = 1*n*n + n*j + i;
		Cid[i][j] = 2*n*n + n*j + i;
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

//	printf(" A=\n");
//	for(i = 0; i < n; i++) {
// 		for(j = 0; j < n; j++)
//			printf(" %+7.3f ", A[i][j]);
//		printf(" \n");
//	}

//	printf(" B=\n");
//	for(i = 0; i < n; i++) {
// 		for(j = 0; j < n; j++)
//			printf(" %+7.3f ", B[i][j]);
//		printf(" \n");
//	}
	int write = 0;
	int read = 0;
/*************************************************************/

//cache function called n^3 times.
	for(i = 0; i < n; i++) {
 		for(j = 0; j < n; j++) {
			for(k = 0; k < n; k++) {
				C[i][j] += A[i][k]*B[k][j];
							//cache_functionLRU ( M, &time, 0, Aid[i][k], isincache, timestamps, changed, &read, &write);
							//cache_functionLRU ( M, &time, 0, Bid[k][j], isincache, timestamps, changed, &read, &write);
							//cache_functionLRU ( M, &time, 1, Cid[i][j], isincache, timestamps, changed, &read, &write);
			}
		}
	}
	//if changed[p] = value from C
	//for (int p = 0; p < M; p++) if (changed[p] == 1) write++;
		

	
	int ib, jb, kb, io, b;
	b = 1;

	ib = b;
	jb = b;
	kb = b;


	for(i = 0; i < n; i += bi){
 		for(j = 0; j < n; j += bj){
			for(k = 0; k < n; k += bk){
			//bring tiles A, B, and C in cache = 3n^2
				//ib < i + b: stay in current bxb block. ib < n: stay in matrix C.
				//cache size needed for computations: 3b^2
				for(ib = i; (ib < i + bi)&&(ib < n); ib++){
 					for(jb = j; (jb < j + bj)&&(jb < n); jb++) {
						for(kb = k; (kb < k + bk)&&(kb < n); kb++) {
		 					D[ib][jb] += A[ib][kb]*B[kb][jb];
							cache_functionLRU ( M, &time, 0, Aid[ib][kb], isincache, timestamps, changed, &read, &write);
							cache_functionLRU ( M, &time, 0, Bid[kb][jb], isincache, timestamps, changed, &read, &write);
							cache_functionLRU ( M, &time, 1, Cid[ib][jb], isincache, timestamps, changed, &read, &write);
						}
					}
				}
			}
		}
	}
	for (int p = 0; p < M; p++) if (changed[p] == 1) write++;
	io = read + write;
			//evict tiles A, B, and C cache = 3n^2
	
			
/*************************************************************/

//printf(" C=\n");
	//for(i = 0; i < n; i++) {
		//for(j = 0; j < n; j++)
			//printf(" %+7.3f ", C[i][j]);
		//printf(" \n");
	//}

//printf(" D=\n");
//	for(i = 0; i < n; i++) {
// 		for(j = 0; j < n; j++)
//			printf(" %+7.3f ", D[i][j]);
//		printf(" \n");
//	}

//check C - D = 0 using square root of sum of squares of corresponding entries

for (int i = 0; i < n; i++) {
	for(j = 0; j < n; j++) {
		cminusdsquare += (C[i][j] - D[i][j]) * (C[i][j] - D[i][j]);
		errorval = sqrt(cminusdsquare);
	}
}
	
	

//	Free memory

//for (int r = 0; r < M; r++) {
    //    printf("%d ", isincache[r]);
    //}
//printf("\n");

//for (i = 0; i < n; i++) {
    //for (j = 0; j < n; j++) {
        //printf(" %-5d ", Aid[i][j]); // Adjust width as needed
    //}
    //printf("\n");
//}
	//printf("error = %+7.3e; ", errorval);
	//printf("M = %5d; n = %5d; bi = %5d; bj = %5d; bk = %5d;", M, n, bi, bj, bk);
	//printf("read = %10d; write = %10d;\n",` read, write);
	//printf("Communication = %10d;\n", io);
	//printf(" %10d,", io);
	//printf(" %10d %10d\n", read, write);
	printf("%d, %d, %d, %d,\n", bk, read, write, io);


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

	for(i = 0; i < n; i++){
		free( Aid[i] );
	}
	free( Aid );

	for(i = 0; i < n; i++){
		free( Bid[i] );
	}
	free( Bid );

	for(i = 0; i < n; i++){
		free( Cid[i] );
	}
	free( Cid );

	free (isincache);

	free (timestamps);

	free(changed);

	return 0;
}
