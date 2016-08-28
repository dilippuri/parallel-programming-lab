#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

struct queueNode{
    int data;
    struct queueNode *nextPtr;
};

typedef struct queueNode QueueNode;
typedef QueueNode *QueueNodePtr;
pthread_t callThd[2];
pthread_mutex_t mutexenQueue;
pthread_mutex_t mutexdeQueue;

void printQueue(QueueNodePtr currentPtr);
int isEmpty(QueueNodePtr headPtr);
void dequeue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr);
void enqueue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr, int value);


void *endeQueueThrd(void *arg){
  int i;
  int j;
  j = (int)arg;
  QueueNodePtr headPtr = NULL;
  QueueNodePtr tailPtr = NULL;

  for (i=0; i<1000 ; i++) {
     if (j%2==0) {
       pthread_mutex_lock (&mutexenQueue);
       enqueue(&headPtr, &tailPtr, i);
       printQueue(headPtr);
       pthread_mutex_unlock (&mutexenQueue);
     }
     else{
       pthread_mutex_lock (&mutexdeQueue);
       dequeue(&headPtr, &tailPtr);
       printQueue(headPtr);
       pthread_mutex_unlock (&mutexdeQueue);
     }
   }
}

int main(void){
  int i;
  void *status;
  pthread_attr_t attr;
  pthread_mutex_init(&mutexenQueue, NULL);
  pthread_mutex_init(&mutexdeQueue, NULL);

  /* Create threads to perform the dotproduct  */
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for(i=0;i<8;i++) {
     pthread_create(&callThd[i], &attr, endeQueueThrd, (void *)i);
   }

  pthread_attr_destroy(&attr);
  /* Wait on the other threads */

  for(i=0;i<2;i++) {
    pthread_join(callThd[i], &status);
    }

  return 0;
}



void enqueue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr, int value){
    QueueNodePtr newPtr;
    newPtr = malloc( sizeof( QueueNode ) );
    if (newPtr != NULL) {
      newPtr->data = value;
      newPtr->nextPtr = NULL;
      if (isEmpty(*headPtr)) {
        *headPtr = newPtr;
      }
      else{
        (*tailPtr)->nextPtr = newPtr;
      }
      *tailPtr = newPtr;
    }
    else{
      printf("%d not inserted. No memory available.\n", value);
    }

}

void dequeue(QueueNodePtr *headPtr, QueueNodePtr *tailPtr){
  int value;
  QueueNodePtr tempPtr;

  value = (*headPtr)->data;
  tempPtr = *headPtr;
  *headPtr = (*headPtr)->nextPtr;

  if (*headPtr == NULL) {
    *tailPtr = NULL;
  }

  free(tempPtr);

}

int isEmpty(QueueNodePtr headPtr){
  return headPtr == NULL;
}

void printQueue(QueueNodePtr currentPtr){
  if (currentPtr == NULL) {
    printf("Queue is empty.\n");
  }
  else{
    printf("The queue is: \n");
    while (currentPtr != NULL) {
      printf("%d --> ", currentPtr->data);
      currentPtr = currentPtr->nextPtr;
    }
    printf("NULL\n\n");
  }
}
