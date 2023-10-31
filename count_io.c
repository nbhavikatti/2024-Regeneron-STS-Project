#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

int main(int argc, char ** argv) {

	int i, j, k, n, v, r, M;
	double cminusdsquare = 0.000;
	double errorval = 0.000;
	double **A, **B, **C, **D;
	double normA, normR, tmp;
	int bi, bj, bk;

	srand(0);

    	n = 10;
	M = 10;
	bi = 1;
	bj = 2;
	bk = 3;
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

	int write = 0;
	int read = 0;
/*************************************************************/

	int ib, jb, kb, io;

	int true_bi, true_bj, true_bk;
//      this counts assumes that three tiles fit in cache

	if (bi*bj + bi*bk + bk*bj > M ) {
		printf("M = %5d; n = %5d; bi = %5d; bj = %5d; bk = %5d;", M, n, bi, bj, bk);
		printf("three tiles do not fit in cache ==> bailing out");
		return -1;
	}
	for(i = 0; i < n; i += bi){
 		for(j = 0; j < n; j += bj){
//			load C in cache
			if ( i + bi > n ) true_bi = n-i+1; else true_bi = bi;
			if ( j + bj > n ) true_bj = n-j+1; else true_bj = bj;
			read += true_bi * true_bj;
			for(k = 0; k < n; k += bk){
//				load A and B in cache
				if ( k + bk > n ) true_bk = n-k+1; else true_bk = bk;
				read += true_bi * true_bk;
				read += true_bk * true_bj;
				for(ib = i; (ib < i + bi)&&(ib < n); ib++){
 					for(jb = j; (jb < j + bj)&&(jb < n); jb++) {
						for(kb = k; (kb < k + bk)&&(kb < n); kb++) {
		 					//D[ib][jb] += A[ib][kb]*B[kb][jb];
						}
					}
				}
			}
//			write C back in main memory
			write += true_bi * true_bj;
		}
	}
	io = read + write;
			
	printf("M = %5d; n = %5d; bi = %5d; bj = %5d; bk = %5d;", M, n, bi, bj, bk);
	printf("read = %10d; write = %10d;", read, write);
	printf("io = %10d;\n", io);

	return 0;
}
