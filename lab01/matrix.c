#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define M 50
#define K 50
#define N 50
#define NUM_THREADS 4 // no. of threads

int A [M][K]; /*= {
   {1,4,5,4,45,5,3,6,32,10},
   {2,5,6,4,1,1,1,1,1,1},
   {3,6,8,5,1,1,1,1,1,1},
   {4,6,5,7,1,1,1,1,1,1},
   {1,1,1,1,1,1,1,1,1,1},
   {2,2,2,2,2,2,2,2,2,2},
   {3,4,5,6,4,4,3,5,4,3},
   {6,5,4,5,4,6,5,4,5,4},
   {7,6,6,7,8,6,5,4,3,6},
   {4,3,4,5,6,5,4,3,4,4}
    };*/
int B [K][N]; /*= {
   {1,4,5,4,45,5,3,6,32,10},
   {2,5,6,4,1,1,1,1,1,1},
   {3,6,8,5,1,1,1,1,1,1},
   {4,6,5,7,1,1,1,1,1,1},
   {1,1,1,1,1,1,1,1,1,1},
   {2,2,2,2,2,2,2,2,2,2},
   {3,4,5,6,4,4,3,5,4,3},
   {6,5,4,5,4,6,5,4,5,4},
   {7,6,6,7,8,6,5,4,3,6},
   {4,3,4,5,6,5,4,3,4,4}
	};*/
 //{ {8,7,6,6}, {5,4,3,2}, {3,5,6,8}, {6,5,4,3} };
int C [M][N];

struct v {
   int i; /* row */
   int j; /* column */
};

void *runner(void *param); /* the thread */

int main(int argc, char *argv[]) {

int y,z;
for(y = 0; y < M; y++) {
      for(z = 0; z < K; z++) {
     A[y][z]= 1;
      }
     
   }

   for(y = 0; y < K; y++) {
      for(z = 0; z < N; z++) {
     B[y][z]= 1;
    }
   }




   int i,j, count = 0;
   for(i = 0; i < M; i++) {
      for(j = 0; j < N; j++) {
         //Assign a row and column for each thread
         struct v *data = (struct v *) malloc(sizeof(struct v));
         data->i = i;
         data->j = j;
         /* Now create the thread passing it data as a parameter */
         pthread_t tid;
         tid = (i+j)%NUM_THREADS;
         pthread_attr_t attr; //Set of thread attributes
         //Get the default attributes
         pthread_attr_init(&attr);
         //Create the thread
         pthread_create(&tid,&attr,runner,data);
         //Make sure the parent waits for all thread to complete
          pthread_join(tid, NULL);
         count++;
      }
   }

   //Print out the resulting matrix
   for(i = 0; i < M; i++) {
      for(j = 0; j < N; j++) {
         printf("%d ", C[i][j]);
      }
      printf("\n");
   }
}

//The thread will begin control in this function
void *runner(void *param) {
   struct v *data = param; // the structure that holds our data
   int n, sum = 0; //the counter and sum

   //Row multiplied by column
   for(n = 0; n< K; n++){
      sum += A[data->i][n] * B[n][data->j];
   }
   //assign the sum to its coordinate
   C[data->i][data->j] = sum;
printf("tid : %d \n", (data->i + data->j) % NUM_THREADS );
   //Exit the thread
   pthread_exit(0);
}
