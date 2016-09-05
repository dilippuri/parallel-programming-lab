
// OpenMP example program:  producer and consumers

// producer will generate random numbers between 0 and 99; consumers
// will grab them, maintaining a global sums, with everyone stopping
// when sums exceed a given level; of course this is just an
// illustration program, not doing much useful, but it does show the
// principles

// the producer puts the numbers into, and the consumers withdraw from,
// a circular queue, "work"

// on platforms with nonsequential memory consistency, may need a
// "flush" pragma at some points in this program
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define MAXWORK 40

int work[MAXWORK],  // work to be done
    nwork=0,  // number of items in the queue
    nextput=0,  // producer will place number # at work[nextput]
    nextget=-1,  // consumer will obtain next # at work[nextget]
    breaksum,  // sum after which everyone stops
    done = 0,  // value 1 signals producer exceeded breaksum
    psum,csum,  // sums found by producer, consumers
    pwork,  // work done by producer
    *cwork,  // work done by the consumers
    nth,  // number of threads
    debugflag;  // 1 if debug

// wraparound incrementer
void next(int *m)
{  (*m)++;
   if (*m >= MAXWORK) *m = 0;
}

void putwork(int k)
{  int put = 0;
   while (!put)  {
      if (nwork < MAXWORK)  {
         #pragma omp critical
         {  work[nextput] = k;
            if (nwork == 0) nextget = nextput;
            next(&nextput);
            nwork++;
            put = 1;
         }
      }
      // don't use this timeslice spinning and waiting
      else sched_yield();
   }
}

int getwork()
{  int k,get=0;
   while (!get)  {
      // leave if producer exceeded breaksum AND we've used all it produced
      if (done && nwork == 0) return -1;
      if (nwork > 0)  {
         #pragma omp critical
         {  // make sure we didn't, e.g., see work only to have had
            // it snatched away by another consumer when our timeslice
            // suddenly ended
            if (nwork > 0)  {
               k = work[nextget];
               next(&nextget);
               nwork--;
               if (nwork == 0) nextget = -1;
               get = 1;
            }
         }
      }
      // don't waste this timeslice spinning and waiting
      else sched_yield();
   }
   return k;
}

void dowork()
{
   #pragma omp parallel
   {  int me = omp_get_thread_num(),  // my thread number
          num;
      #pragma omp single
      {  int i;
         nth = omp_get_num_threads();
         printf("there are %d threads\n",nth);
         cwork = (int *) malloc(nth*sizeof(int));
         for (i = 1; i < nth; i++) cwork[i]=0;
      }
      // the next 2 lines set up pause for debugger if requested
      if (me == 0 && debugflag) {int wait=0; while (!wait) ; }
      #pragma omp barrier
      if (me == 0)  {  // I'm the producer
         pwork = 0;
         while (1)  {
            num = rand() % 100;
            putwork(num);
            psum += num;
            pwork++;
            if (psum > breaksum)  {
               done = 1;
            //   return NULL;
            }
         }
      }
      else  {  // I'm a consumer
         while (1)  {
            num = getwork();
            if (num == -1);// return NULL;
            cwork[me]++;
            #pragma omp atomic
            csum += num;
         }
      }
   }
}

int main(int argc, char **argv)
{  int i;
   breaksum = atoi(argv[1]);
   debugflag = atoi(argv[2]);
   dowork();
   printf("sum reported by producer:  %d\n",psum);
   printf("sum reported by consumers:  %d\n",csum);
   printf("work done by producer:  %d\n",pwork);
   printf("work done by consumers:\n");
   for (i = 1; i < nth; i++)
      printf("%d\n",cwork[i]);
}
