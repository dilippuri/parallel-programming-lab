
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

#define BUFFER_SIZE 250
#define NUM_PRODUCERS 4
#define NUM_CONSUMERS 4
#define TRANSACTIONS 1000

int item;
int sharedBuffer[BUFFER_SIZE];
int index1 = 0;
int index2 = 0;
int productionCount = 0;
int consumptionCount = 0;
int X=0;

int produceItem(){
    return X++;	
}

void putItemIntoBuffer(int arg_item, int h){
  sharedBuffer[index1] = item;
  printf("Thread=%d->%d added to the buffer at index=%d\n",h, arg_item, index1);	
} 

int removeItemFromBuffer(){
return sharedBuffer[index2];
}

void consumeItem(int arg_item, int j){
printf("Thread=%d->Consumed item:%d\n",j, arg_item);
}

void *producer(int i) {
#pragma omp parellel
{
  printf("Thead:%d",i);	
    while(productionCount < TRANSACTIONS) {
        item = produceItem();
        #pragma omp crtical
    	{
                    int id;
    		id = (int) i;
    		if(index1 != BUFFER_SIZE){
                    putItemIntoBuffer(item,id);
    		}
    		index1 = (index1 + 1) % BUFFER_SIZE;
             }
            
	     productionCount++;
    }
  exit(0);
}
}

void *consumer(int k) {
  #pragma omp parellel
  {
      while (consumptionCount < TRANSACTIONS) {
      	int id; 
      	id = (int) k;
              #pragma omp critical
      	{
  		
      		if(index2 != 0){
                      item = removeItemFromBuffer();
      		}
      		index2= ((index2 + 1) % BUFFER_SIZE);
        }
              //sem_post(&emptyCount);
              consumeItem(item,id);
      	consumptionCount++;
      }
  exit(0);
  }
}

int main(int argc, char *argv[]){

int tid,nthreads;
#pragma omp parallel shared(nthreads) private(tid)

  {

  tid = omp_get_thread_num();

  if (tid == 0)

    {

    nthreads = omp_get_num_threads();

    printf("Number of threads = %d\n", nthreads);

    }

  printf("Thread %d starting...\n",tid);

  #pragma omp sections 
    {
    #pragma omp section
    {
    producer(tid);
      }

    #pragma omp section
      {
	      consumer(tid);
      }

    #pragma omp section
      {
	      producer(tid);
      }

    #pragma omp section
      {
      	consumer(tid);
      }

    #pragma omp section
      {
	        producer(tid);
    }
    #pragma omp section

      {
      	consumer(tid);

      }
    #pragma omp section
      {
	      producer(tid);
      }  

    #pragma omp section
      {
	      consumer(tid);
      }
}
}	return 0;

}
