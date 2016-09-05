
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// OpenMP Parallelization of Matrix Multiplication

#define N 5000
#define NUM_OF_THREADS 32
float A[N][N], B[N][N], C[N][N]; // declaring matrices of NxN size


int main ()
{
     /* DECLARING VARIABLES */


int i, j, m; // indices for matrix multiplication


   /* FILLING MATRICES WITH RANDOM NUMBERS */

srand ( time(NULL) );
for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
      A[i][j]= (rand()%10);
      B[i][j]= (rand()%10);
    }
}
      /* MATRIX MULTIPLICATION */

omp_set_num_threads(NUM_OF_THREADS);
printf("Max number of threads: %i \n",omp_get_max_threads());
#pragma omp parallel


printf("Number of threads: %i \n",omp_get_thread_num());
clock_t tic = clock(); // time measure: start mm

#pragma omp parallel for private(m,j)
for(i=0;i<N;i++) {
    for(j=0;j<N;j++) {
        C[i][j]=0.; // set initial value of resulting matrix C = 0
          for(m=0;m<N;m++) {
            C[i][j]=A[i][m]*B[m][j]+C[i][j];
          }
// printf("C: %f \n",C[i][j]);
  }
}
/* TIME MEASURE + OUTPUT */

clock_t toc = clock(); // time measure: end mm
printf("Elapsed: %f seconds\n", (double)(toc - tic) / CLOCKS_PER_SEC);
/* TERMINATE PROGRAM */
return 0;
}
