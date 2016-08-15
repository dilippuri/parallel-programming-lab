#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <malloc.h>
	
	void product( int id, int *rows, int dim, double **matrix_a, double **matrix_b, double **result );
	void *product_worker(void *arg);
	void print_matrix( double **matrix, int dim);