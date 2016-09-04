#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main (int argc, char *argv[]){

/* Fork a team of threads giving them their own copies of variables */
#pragma omp parallel // starts a new team
{
   //Work0(); // this function would be run by all threads.

   #pragma omp sections // divides the team into sections
   {
     // everything herein is run only once.
     {
//       sleep(20);
       printf("Hello World from thread = %d\n", omp_get_thread_num());
       /* Only master thread does this */
       if (omp_get_thread_num() == 0){
         printf("Number of threads = %d\n", omp_get_num_threads());
         }
     }
     #pragma omp section
     {
//           sleep(10);
           printf("Hello World from thread = %d\n", omp_get_thread_num());
           /* Only master thread does this */
           if (omp_get_thread_num() == 0){
             printf("Number of threads = %d\n", omp_get_num_threads());
             }
      }
     #pragma omp section
     {
//           sleep(5);
           printf("Hello World from thread = %d\n", omp_get_thread_num());
           /* Only master thread does this */
           if (omp_get_thread_num() == 0){
             printf("Number of threads = %d\n", omp_get_num_threads());
             }
      }
      #pragma omp section
      {
//             sleep(30);
            printf("Hello World from thread = %d\n", omp_get_thread_num());
            /* Only master thread does this */
            if (omp_get_thread_num() == 0){
              printf("Number of threads = %d\n", omp_get_num_threads());
              }
       }
   }

   //Work5(); // this function would be run by all threads.
}
/*
  #pragma omp parallel sections
  {
      #pragma omp section
      {
        sleep(10);
        printf("Hello World from thread = %d\n", omp_get_thread_num());
         /* Only master thread does this */
/*        if (omp_get_thread_num() == 0){
          printf("Number of threads = %d\n", omp_get_num_threads());
          }
      }
      #pragma omp section
      {
        sleep(20);
        printf("Hello World from thread = %d\n", omp_get_thread_num());
        /* Only master thread does this */
/*        if (omp_get_thread_num() == 0){
          printf("Number of threads = %d\n", omp_get_num_threads());
          }
      }
      #pragma omp section
      {
        sleep(5);
        printf("Hello World from thread = %d\n", omp_get_thread_num());
        /* Only master thread does this */
/*        if (omp_get_thread_num() == 0){
          printf("Number of threads = %d\n", omp_get_num_threads());
          }
      }
      #pragma omp section
      {
        sleep(30);
        printf("Hello World from thread = %d\n", omp_get_thread_num());
        /* Only master thread does this */
/*        if (omp_get_thread_num() == 0){
          printf("Number of threads = %d\n", omp_get_num_threads());
          }
      }
    }*/

  }  /* All threads join master thread and disband */
