/* buffer.h */
typedef int buffer_item;
#define BUFFER_SIZE 30

/* main.c */

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
//#include "buffer.h"

#define RAND_DIVISOR 100000000
#define TRUE 1

/* The mutex lock */
pthread_mutex_t mutex;

/* the semaphores */
sem_t full, empty;

//cond var
pthread_cond_t can_produce; // signaled when items are removed
pthread_cond_t can_consume;

/* the buffer */
buffer_item buffer[BUFFER_SIZE];

/* buffer counter */
int counter;

//pthread_t tid;       //Thread ID
//pthread_attr_t attr; //Set of thread attributes

void *producer(void *param); /* the producer thread */
void *consumer(void *param); /* the consumer thread */

void initializeData() {

   /* Create the mutex lock */
   pthread_mutex_init(&mutex, NULL);
   
   /* Create the full semaphore and initialize to 0 */
   sem_init(&full, 0, 0);

   /* Create the empty semaphore and initialize to BUFFER_SIZE */
   sem_init(&empty, 0, BUFFER_SIZE);

//can_produce = PTHREAD_COND_INITIALIZER;
//can_consume = PTHREAD_COND_INITIALIZER;
   pthread_cond_init (&can_produce, NULL);
   pthread_cond_init (&can_consume, NULL);

   /* Get the default attributes */
   //pthread_attr_init(&attr);

   /* init buffer */
   counter = 0;
}

/* Producer Thread */
void *producer(void *param) {
   buffer_item item;
int i = (int)param;
//int *i = (int*)param;
printf("helo %d\n",i);
int count=250;
   while(count>0) {
//int x= *i;
     

      /* generate a random number */
      item = count;

      /* acquire the empty lock */
      sem_wait(&empty);
      /* acquire the mutex lock */
      pthread_mutex_lock(&mutex);
//to check if the buffer is full.
      if(counter == BUFFER_SIZE){
       pthread_cond_wait(&can_produce,&mutex);
      }
      if(insert_item(item)) {
	printf("Producer error");
         fprintf(stderr, " Producer report error condition\n");
      }
      else {
         printf("producer %d produced %d\n",i, item);
pthread_cond_signal(&can_consume);
	count--;
      }

      /* release the mutex lock */
      pthread_mutex_unlock(&mutex);
      /* signal full */
      sem_post(&full);

   }
}

/* Consumer Thread */
void *consumer(void *k) {
   buffer_item item;
	int ii = (int)k;
   int count=250;
   while(count>0) {
      /* sleep for a random period of time */
      int rNum = rand() / RAND_DIVISOR;
      //sleep(rNum);

      /* aquire the full lock */
      sem_wait(&full);
      /* aquire the mutex lock */
      pthread_mutex_lock(&mutex);
      
if(counter==0){//if empty
	pthread_cond_wait(&can_consume, &mutex);
	}
count--;	
      if(remove_item(&item)) {
         fprintf(stderr, "Consumer report error condition\n");
      }
      else {
         printf("consumer %d consumed %d\n",ii, item);
      }
      /* release the mutex lock */
pthread_cond_signal(&can_produce);//notify
      pthread_mutex_unlock(&mutex);
      /* signal empty */
      sem_post(&empty);
   }
}

/* Add an item to the buffer */
int insert_item(buffer_item item) {
   /* When the buffer is not full add the item
      and increment the counter*/
   if(counter < BUFFER_SIZE) {
      buffer[counter] = item;
      counter++;
      //return 0;
   }
   else { /* Error the buffer is full */
	printf("Buffer is full nothing to do \n");
   }
	return 0;
}

/* Remove an item from the buffer */
int remove_item(buffer_item *item) {
   /* When the buffer is not empty remove the item
      and decrement the counter */
   if(counter > 0) {
      *item = buffer[(counter-1)];
      counter--;
      //return 0;
   }
   else { /* Error buffer empty */
      
   }
return 0;
}

int main(int argc, char *argv[]) {

   /* Initialize the app */
   initializeData();


   /* Sleep for the specified amount of time in milliseconds */
  // sleep(mainSleepTime);

//

int prod,cons;
prod=4;
cons=4;

pthread_t producerThread[prod];
pthread_t consumerThread[cons];


//std::thread producerThread[prod];
//std::thread consumerThread[cons];
int i,j,k,l;
for( i=0;i<prod;i++){
	int rc = pthread_create(&producerThread[i],NULL,producer,(void*) i);

}
for( j=0;j<cons;j++){
	int ra = pthread_create(&consumerThread[j],NULL,consumer,(void*) j);
	//pthread_join(consumerThread[j],NULL);
}


for( k=0;k<prod;k++){
	pthread_join(producerThread[k],NULL);

}



for( l=0;l<cons;l++){
	pthread_join(consumerThread[l],NULL);

}

//
   /* Exit the program */
   printf("Exit the program\n");
   exit(0);
}
