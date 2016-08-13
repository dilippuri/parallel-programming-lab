/* Parallel Matrix-Matrix Multiply Program using POSIX Threads */
/* Wes Laurion - 2012 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#define AROWS 8
#define ACOLS 6
#define BROWS 6
#define BCOLS 10
#define PTHREADS 3

/* Define the Matrix data type */
typedef struct Matrix {
int r, c;
int *contents;
int **rows;
}Matrix;

int rows_per_proc;
Matrix A, B, C;
pthread_t tid[PTHREADS];

/* Function Definitions */
Matrix createMatrix(int rows, int cols);
void *MatrixMult(void *tid);
void printMatrices(Matrix A, Matrix B, Matrix C);

void main() {
int rw, cl, i, err;

/* Create and initialize three matrices. First two to be multiplied and third to hold result */
A = createMatrix(AROWS, ACOLS);
B = createMatrix(BROWS, BCOLS);
C = createMatrix(AROWS, BCOLS);

/* Fill matrices A and B with random ints between 0 and 100 */
srand(time(NULL));

for (rw = 0; rw < AROWS; rw++){
for (cl = 0; cl < ACOLS; cl++){ 
A.rows[rw][cl] = rand() % 26;
}
}

for (rw = 0; rw < BROWS; rw++){
for (cl = 0; cl < BCOLS; cl++){ 
B.rows[rw][cl] = rand() % 26;
}
}

/* Rows of C to be calculated by each thread. */
rows_per_proc = AROWS / PTHREADS;

/* Initialize and start pthreads */
for(i = 0; i < PTHREADS; i++){
err = pthread_create(&tid[i], NULL, MatrixMult, void *i);
if(err == 0) printf("Thread %d started successfully!\n", i);
else printf("Thread %d did not start successfully.\n", i);
}

for(i = 0; i < PTHREADS; i++){
err = pthread_join(tid[i], NULL);
if(err == 0) printf("Thread %d joined successfully!\n", i);
else printf("Thread %d did not join successfully.\n", i);
}

printMatrices(A, B, C);
printf("Press Enter to Exit: ");
scanf("Exit");
}

Matrix createMatrix(int rows, int cols){
int i;
Matrix mtx;
mtx.r = rows;
mtx.c = cols;

/* Allocate memory for mtx */
mtx.contents = (int *) malloc(sizeof(int) * rows * cols);
mtx.rows = (int **) malloc(sizeof(int *) * rows);

/* find starting memory address of each row of the matrix */
for (i = 0; i < rows; i++){
mtx.rows[i] = mtx.contents + i*cols;
}

return mtx;
}

void *MatrixMult(void* tid){
int i, j, k, total;
int id = (int)tid;
int row_start = id*rows_per_proc;

if(id < PTHREADS - 1){
for (i = row_start; i < row_start + rows_per_proc; i++){
for (j = 0; j < B.c; j++){
total = 0;
for (k = 0; k < A.c; k++){
total += A.rows[i][k] * B.rows[k][j];
}
C.rows[i][j] = total;
}
}
}else{ //last thread gets all leftover rows
for (i = row_start; i < row_start + (A.r - ((PTHREADS-1)*rows_per_proc)); i++){
for (j = 0; j < B.c; j++){
total = 0;
for (k = 0; k < A.c; k++){
total += A.rows[i][k] * B.rows[k][j];
}
C.rows[i][j] = total;
}
}
}
}

void printMatrices(Matrix A, Matrix B, Matrix C){
int i, j;

/* Print Matrix A */
printf("Matrix A:\n");
for (i = 0; i < A.r; i++){
for (j = 0; j < A.c; j++){
printf("%6d ", A.rows[i][j]);
}
printf("\n\n");
}

/* Print Matrix B */
printf("Matrix B:\n");
for (i = 0; i < B.r; i++){
for (j = 0; j < B.c; j++){
printf("%6d ", B.rows[i][j]);
}
printf("\n\n");
}

/* Print Matrix C = A*B */
printf("Matrix C (A * B):\n");
for (i = 0; i < C.r; i++){
for (j = 0; j < C.c; j++){
printf("%6d ", C.rows[i][j]);
}
printf("\n\n");
}
}
/* end of file */


