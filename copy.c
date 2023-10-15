#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
			int cache_functionLRU (int M, int *global_time, int write_or_not, int id, 
				int *id_Array, int *timestamps, int *write_or_no, int *nextyounger, int *nextolder, int *oldest, int *youngest,
				int *read, int *write, int *index_in_cache) {
				//printf("%s\n", "help me find error");
				//printf("%d ", id_Array[index_in_cache[id]]);
				//printf("%d ", index_in_cache[id_Array[id]]);
				//for (int i = 0; i < 6; i++) {
				//	printf("%d ", id_Array[i]);
				//}
				//printf("\n");
				//for (int i = 0; i < 12; i++) {
				//	printf("%d ", index_in_cache[i]);
				//}
				//printf("\n");
				(*global_time)++;
				//I am index_in_cache[id] and I will become the youngest.
				if (index_in_cache[id] >= 0) {
					timestamps[index_in_cache[id]] = (*global_time);
					write_or_no[index_in_cache[id]] = write_or_not;
					//id_Array[*oldest] = id;
					nextolder[index_in_cache[id]] = *youngest;

					nextolder[nextyounger[index_in_cache[id]]] = nextolder[index_in_cache[id]];
					nextyounger[nextolder[index_in_cache[id]]] = nextyounger[index_in_cache[id]];

					nextyounger[*youngest] = index_in_cache[id];
					*youngest = index_in_cache[id];
				}
			//I am *oldest (I will be replaced by id) and my position will be filled by the *youngest.
				if (index_in_cache[id] == -1) {
					if (write_or_no[*oldest] == 1) (*write)++;
					(*read)++;
					if(id_Array[*oldest] >= 0)
					   index_in_cache[id_Array[*oldest]] = -1;
					id_Array[*oldest] = id;
					write_or_no[*oldest] = write_or_not;
					timestamps[*oldest] = (*global_time);
					nextolder[*oldest] = *youngest;
					nextolder[nextyounger[*oldest]] = -1;
					nextyounger[*youngest] = *oldest;
					*youngest = *oldest;
					*oldest = nextyounger[*oldest];
					nextyounger[*youngest] = -1;
					index_in_cache[id] = *youngest;
					//printf("\n");
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
	int *id_Array;
	id_Array = (int *) malloc( M * sizeof(int));
	for (int i = 0; i < M; i++) {
		id_Array[i] = -1;
	}

	int *timestamps;
	timestamps = (int*) malloc( M * sizeof(int));

	int *write_or_no;
	write_or_no = (int*) malloc( M * sizeof(int));

	int *nextyounger;
	nextyounger = (int*) malloc( M * sizeof(int));

	int *nextolder;
	nextolder = (int*) malloc( M * sizeof(int));

	int *index_in_cache;
	index_in_cache = (int*) malloc( 3*n*n * sizeof(int));

	for (int i = 0; i < 3*n*n; i++) {
		index_in_cache[i] = -1;
	}

	int oldest = 0;
	int youngest = M - 1;

	//start counting from 0 to keep consistency
	for (int i = 0; i < M; i++) {
		timestamps[i] = i;
	}
	int global_time = M - 1;

	for (int i = 0; i < M - 1; i++) {
		nextyounger[i] = i+1;
	}
	nextyounger[M-1] = -1;

	nextolder[0] = -1;
	for (int i = 1; i < M; i++) {
		nextolder[i] = i-1;
	}
	

	for (int i = 0; i < M; i++) {
		write_or_no[i] = 0; 
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
	int write = 0;
	int read = 0;
/*************************************************************/


	for(i = 0; i < n; i++) {
 		for(j = 0; j < n; j++) {
			for(k = 0; k < n; k++) {
				C[i][j] += A[i][k]*B[k][j];
					//cache_functionLRU ( M, &global_time, 0, Aid[i][k], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
					//cache_functionLRU ( M, &global_time, 0, Bid[k][j], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
					//cache_functionLRU ( M, &global_time, 0, Cid[i][j], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
		}
	}
	//for (int p = 0; p < M; p++) if (write_or_no[p] == 1) write++;
	//io = read + write;
		

	
	int ib, jb, kb, b, io;
	b = 1;

	ib = b;
	jb = b;
	kb = b;

	for(i = 0; i < n; i += bi){
 		for(j = 0; j < n; j += bj){
			for(k = 0; k < n; k += bk){
				for(ib = i; (ib < i + bi)&&(ib < n); ib++){
 					for(jb = j; (jb < j + bj)&&(jb < n); jb++) {
						for(kb = k; (kb < k + bk)&&(kb < n); kb++) {
		 					D[ib][jb] += A[ib][kb]*B[kb][jb];
							cache_functionLRU ( M, &global_time, 0, Aid[ib][kb], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
							cache_functionLRU ( M, &global_time, 0, Bid[kb][jb], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
							cache_functionLRU ( M, &global_time, 1, Cid[ib][jb], id_Array, timestamps, write_or_no, nextyounger, nextolder, &oldest, &youngest, &read, &write, index_in_cache);
						}
					}
				}
			}
		}
	}
	for (int p = 0; p < M; p++) if (write_or_no[p] == 1) write++;
	io = read + write;	
/*************************************************************/
for (int i = 0; i < n; i++) {
	for(j = 0; j < n; j++) {
		cminusdsquare += (C[i][j] - D[i][j]) * (C[i][j] - D[i][j]);
		errorval = sqrt(cminusdsquare);
	}
}
	printf("%d, %d, %d,\n", read, write, io);


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

	free(nextyounger);

	free(nextolder);

	free (id_Array);

	free (timestamps);

	free(write_or_no);

	free(index_in_cache);
	}
	return 0;
}
