/*
 * Name: Wesley Kos
 * Class: CS411 Fall 2011
 * Due Date: 10/7/2011
 * This class calculates the product of two NxN matrices, where N is specificed by the 
 * user upon the execution call. To calculate the product it will use a number of threads
 * specified by the user during the execution call. It will use the diffierent threads to
 * calculate the product and then if the user specifies it it will print out the two 
 * randomly generated matrices A and B and also the resulting matrix.
 */

#include "matmulti.h"
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>

/* This is hte struct which will be passed along with the threads. It contains an ID so 
 * we know which thread number it is, the dimension of hte matrix, the two randomly 
 * generated matrices A nad B as well as the matrix which we will store the result in. 
 * The variable rows is used to determine which threads should handle which rows. 
 */
typedef struct {
  int id;
  int dim;
  double **m_a;
  double **m_b;
  double **m_r;
  int *rows;
} param_t;



/* The main function requires four arguments. 
 * Argument 1: The name of the function pMult
 * Argument 2: Dimension of the matrices, N.
 * Argument 3: The number of threads to use, C.
 * Argument 4: A bool to determine if the matrices should be printed or not.
 */
int main(int argc, const char *argv[] ) {
  if(argc >= 3) { 
    /* dim stores the dimensions of the matrices */
    int dim = atoi(argv[1]);
    /* threads will be the number of threads to use */
    int threads = atoi(argv[2]);
    /* A bool whiuch determines to print or not to print. */
    char *print;
    if(argc == 4) {
      print = atoi(argv[3]);
    }
    else {
      print = 0;
    }
    /* stores the number of rows which each thrtead will have to calculate 
    * the results.*/
    int t[threads];
    /* used to iterate through each matrices */
    int i;
    int row;
    int col; 
    /* Used to determine which threads will handle which rows.*/
    int t_left = threads;
    int r_left = dim;
    /* Used to see if pthread_create() and pthread_join() were executed 
    * successfully */
    long rc;
    /* Used for the return call of pthread_join() */
    void *status;

    /* Seed with unsigned long 47 for drand48(); */
    srand48(47u);

    /** 
    * matrixA nad matrixB will be used as the two matrices that we will 
    * multiply. The matrix result will store the product of the two once 
    * multiplied. 
    **/
    double **matrix_a;
    double **matrix_b;
    double **result;

    /* Allocate storage for arrays. matrixA, matrixB and result. */
    matrix_a = malloc(dim * sizeof(double *));
    matrix_b = malloc(dim * sizeof(double *));
    result = malloc(dim * sizeof(double *));

    /* 
    * for each of the pointeres in the tree matrices we want to allocate     
    * storage for the array's of floats */
    for(row = 0; row < dim; ++row) {
      matrix_a[row] = malloc(dim * sizeof(double));
    }
    for(row = 0; row < dim; ++row) {
      matrix_b[row] = malloc(dim * sizeof(double));
    }
    for(row = 0; row < dim; ++row) {
      result[row] = malloc(dim * sizeof(double));
    }

    /* Fill matrixA with random floats between 0.0 and 1.1 */
    for(row = 0; row < dim; ++row) {
      for(col = 0; col < dim; ++col) {
        matrix_a[row][col] = drand48();
      }
    }

    /* Fill matrixB with random floats between 0.0 and 1.1 */
    for(row = 0; row < dim; ++row) {
      for(col = 0; col < dim; ++col) {
        matrix_b[row][col] = drand48();
      }
    }
    /* 
    * Fill result with 0's to it will be ready for the product of 
    * matrixA * matrixB 
    **/
    for(row = 0; row < dim; ++row) {
      for(col = 0; col < dim; ++col) {
        result[row][col] = 0;
      }
    }

    /* Initialize the p_threads and the attributes for those pthreads */
    pthread_t *thread;
    pthread_attr_t attr;

    thread = (pthread_t *)malloc(threads * sizeof(pthread_t));
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    /* 
    * Here we initlize the tLeft to the number of threads which the user 
    * speicified and the rLeft to the dimensions. We use these two variables 
    * so we can modify those numbers to correctly determine how many rows 
    * each thread will be handling. We store the values each thread will 
    * be holding in t[i].
    **/
    for(i = 0; i < threads; ++i) {
      t[i] = r_left / t_left;
      r_left -= t[i];
      --t_left;
    }

    /* 
    * Initlize our struct to hold the information which our workers will use. 
    **/
    param_t *arg;
    arg = (param_t *)malloc(sizeof(param_t)*threads);

    /* 
    * Here we loop through the number of threads setting each thread up with 
    * the needed information to correctly calculate its portion of hte matrix
    **/
    for(i = 0; i < threads; ++i) {
      arg[i].id = i;
      arg[i].rows = t;
      arg[i].dim = dim;
      arg[i].m_a = &matrix_a;
      arg[i].m_b = &matrix_b;
      arg[i].m_r = &result;
      printf("Main: Creating Thread %d\n", i);
      /* Here we actually create and spawn off the new thread */
      rc = pthread_create(
        &thread[i], &attr, product_worker, (void *)(arg + i)
        );    
      if(rc) {
        printf( 
        "ERROR: Return code from pthread_create() is %ld\n", rc
        );
        exit(-1);
      }
    } 
    /* 
    * Loop through each thead and join it so it will not quit the program 
    * upon completion. 
    **/
    for(i = 0; i < threads; ++i) {
      rc = pthread_join(thread[i], &status);
      if(rc) {
        printf(
        "ERROR: Return code from pthread_join() is %ld\n", rc
        );
      }
      printf(
      "Main: Completed joining thread %d having a status of %ld\n", 
      i, (long)&status
      );
    }
    /* 
    * Here we check to see if the user wants to print out the resulting 
    * matrices. 
    **/
    if(print > 0) {
      printf("Printing out Matrix A: \n");
      print_matrix( matrix_a, dim );
      printf("Printing out Matrix B: \n");
      print_matrix( matrix_b, dim );
      printf("Printing out the result from A * B: \n");
      print_matrix( result, dim );
    }
    printf("Main: Completed the main program. Exitting now.\n");
    return 1;
  }
  else {
    return 0;
  }
}



/*
 * This function it called by each of the pthreads. Once the thread reaches here it will 
 * type cast the arg back to the struct param and then call the Product function. Upon 
 * completion it will make the thread exit.
 */
void *product_worker( void *arg) {
  /* Here we typecast the null pointer back to what it originally was */
  param_t *p = (param_t *)arg; 
  product( p->id, (p->rows), p->dim, *(p->m_a), *(p->m_b), *(p->m_r) );
  pthread_exit(0);
}



/*
 * This function will take take the rows given, figure out what row it should start on 
 * and then calculate the product of matrixA * matrixB from the row it should start at 
 * and finish at the starting row + the number of rows the thread was given.
 */
void product( int id, int *rows, int dim, 
    double **matrix_a, double **matrix_b, double **result ) {
  int i;
  int row; 
  int col;
  int k;
  double sum;
  int starting_row = 0;
  int finish_row = 0; 
  /* We figure out what row we should be starting at. This way the thread only 
  * does the the calculations that we want it too. 
  **/
  for(i = 0; i < id; ++i) {
    starting_row += rows[i];
  }
  /* Here we want to make sure it only goes from row we want to the row we want. 
  * This way we are not having it do needless calculations that some other thread 
  * will do.
  **/
  finish_row = starting_row + rows[id];
  /* 
  * With these loops we will actually add up the correct numbers and then store 
  * them in result 
  **/
  for(row = starting_row; row < finish_row; ++row) {
    for(col = 0; col < dim; ++col) {
      sum = 0.0;
      for(k = 0; k < dim; ++k) {
        sum = sum + (matrix_a[row][k] * matrix_b[k][col]);
//fprintf(stderr, "Id = %d, MA[%d][%d] = %f, MB[%d][%d] = %f, sum = %f\n", id, row, k, matrix_a[row][k], k, col, matrix_b[k][col], sum);

      }
      result[row][col] = sum;
    }
  }
}



/* 
 * This function takes a matrix and the dimensions of htat matrix and then prints out the 
 * matrix on the screen in the form of
 * [ A B C ]
 * [ D E F ]
 * [ G H I ]
 */
void print_matrix( double **matrix, int dim ) {
  int row;
  int col;
  /* 
  * We loop through the matrix given to us and we print it out in the correct format 
  **/
  for(row = 0; row < dim; ++row) {
    printf("[ ");
    for(col = 0; col < dim; ++col) {
      printf(" %f ", matrix[row][col]);
    }
    printf(" ]\n");
  }
  printf("\n");
}