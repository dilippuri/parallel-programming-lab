#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100     
#define PRODUCERS_COUNT 4   // No. of producers
#define CONSUMERS_COUNT 4   // No. of consumers
#define TRANSACTIONS 1000   // No. of Production and consumption interations

pthread_mutex_t mutex;
sem_t fillCount;            
sem_t emptyCount;           
int item;
int sharedBuffer[BUFFER_SIZE];
int index1 = 0;
int index2 = 0;
int productionCount = 0;
int consumptionCount = 0;
int X=0;

int produceItem(){       // increments the no of items produced
    return X++;	
}

void putItemIntoBuffer(int arg_item){  // inserts the produced item to the buffer  
  sharedBuffer[index1] = item;
  printf("%d added at index=%d  of buffer \n", arg_item, index1);	
} 

int removeItemFromBuffer(){           // removes the item from the buffer
return sharedBuffer[index2];           
}

void consumeItem(int arg_item){          
printf("Consumed item:%d\n", arg_item);
}

void *producer() {
    while(productionCount < TRANSACTIONS) {
        item = produceItem();                         
        sem_wait(&emptyCount);              //decrements the empty count
            pthread_mutex_lock(&mutex);     // acquires lock
                putItemIntoBuffer(item);
		index1 = (index1 + 1) % BUFFER_SIZE;
            pthread_mutex_unlock(&mutex);   // Removes lock 
        sem_post(&fillCount);               // increments the fill count
	productionCount++;
    }
    pthread_exit(0);
}

void *consumer() {
    while (consumptionCount < TRANSACTIONS) {
        sem_wait(&fillCount);                 //decrements the fill count
            pthread_mutex_lock(&mutex);         // acquires lock
                item = removeItemFromBuffer();
		index2= ((index2 + 1) % BUFFER_SIZE);
            pthread_mutex_unlock(&mutex);       // Removes lock
        sem_post(&emptyCount);                // increments the empty count  
        consumeItem(item);              
	consumptionCount++;
    }
    pthread_exit(0);
}


int main(){

	pthread_t producers[PRODUCERS_COUNT], consumers[CONSUMERS_COUNT];
	pthread_mutex_init(&mutex, NULL);
	sem_init(&fillCount,0,1);
	sem_init(&emptyCount,0,BUFFER_SIZE);
	int i;
	for(i=0;i<PRODUCERS_COUNT;i++){
		pthread_create(&producers[i],NULL,producer,NULL);      //creating producer threads
	}
	for(i=0;i<CONSUMERS_COUNT;i++){
		pthread_create(&consumers[i],NULL,consumer,NULL);      //creating consumer threads
	}
	for(i=0;i<PRODUCERS_COUNT;i++){
		pthread_join(producers[i],NULL);                       //waits for all the producer threads to finish up
	}

	for(i=0;i<CONSUMERS_COUNT;i++){
		pthread_join(consumers[i],NULL);                       // waits for all the consumer threads to finish up 
	}

	pthread_mutex_destroy(&mutex);
	sem_destroy(&fillCount);
	sem_destroy(&emptyCount);	
	return 0;
}
