#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "timer.h"

int     thread_count;
int     m, n;
double* A;
double* x;
double* y;


void Usage (char* prog_name) {
   fprintf(stderr, "usage: %s <thread_count> <m> <n>\n", prog_name);
   exit(0);
}  


void Read_matrix(char* prompt, double A[], int m, int n) {
   int             i, j;

   printf("%s\n", prompt);
   for (i = 0; i < m; i++) 
      for (j = 0; j < n; j++)
         scanf("%lf", &A[i*n+j]);
}  

void Gen_matrix(double A[], int m, int n) {
   int i, j;
   for (i = 0; i < m; i++)
      for (j = 0; j < n; j++)
         A[i*n+j] = random()/((double) RAND_MAX);
}  


void Gen_vector(double x[], int n) {
   int i;
   for (i = 0; i < n; i++)
      x[i] = random()/((double) RAND_MAX);
}  

void Read_vector(char* prompt, double x[], int n) {
   int   i;

   printf("%s\n", prompt);
   for (i = 0; i < n; i++) 
      scanf("%lf", &x[i]);
}  

void *Pth_mat_vect(void* rank) {
   long my_rank = (long) rank;
   int i;
   int j; 
   int local_m = m/thread_count; 
   int my_first_row = my_rank*local_m;
   int my_last_row = my_first_row + local_m;
   register int sub = my_first_row*n;
   double temp;

#  ifdef DEBUG
   printf("Thread %ld > local_m = %d, sub = %d\n",
         my_rank, local_m, sub);
#  endif

   for (i = my_first_row; i < my_last_row; i++) {
      y[i] = 0.0;
      for (j = 0; j < n; j++) {
          temp = A[sub++];
          temp *= x[j];
          y[i] += temp;
      }
   }


   return NULL;
}  


void Print_matrix( char* title, double A[], int m, int n) {
   int   i, j;

   printf("%s\n", title);
   for (i = 0; i < m; i++) {
      for (j = 0; j < n; j++)
         printf("%6.3f ", A[i*n + j]);
      printf("\n");
   }
}  

void Print_vector(char* title, double y[], double m) {
   int   i;

   printf("%s\n", title);
   for (i = 0; i < m; i++)
      printf("%6.3f ", y[i]);
   printf("\n");
}  


int main(int argc, char* argv[]) {
   long       thread;
   pthread_t* thread_handles;
   double start,finish,elapsed;
   if (argc != 4) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   m = strtol(argv[2], NULL, 10);
   n = strtol(argv[3], NULL, 10);

#  ifdef DEBUG
   printf("thread_count =  %d, m = %d, n = %d\n", thread_count, m, n);
#  endif

   thread_handles = malloc(thread_count*sizeof(pthread_t));
   A = malloc(m*n*sizeof(double));
   x = malloc(n*sizeof(double));
   y = malloc(m*sizeof(double));
   
   Gen_matrix(A, m, n);
#  ifdef DEBUG
   Print_matrix("Se genero:", A, m, n); 
#  endif

   Gen_vector(x, n);
#  ifdef DEBUG
   Print_vector("Se genero:", x, n); 
#  endif
   GET_TIME(start);
   for (thread = 0; thread < thread_count; thread++)
      pthread_create(&thread_handles[thread], NULL,
         Pth_mat_vect, (void*) thread);

   for (thread = 0; thread < thread_count; thread++)
      pthread_join(thread_handles[thread], NULL);
   GET_TIME(finish);
   elapsed = finish - start;
   printf("Tiempo(Multiplicacion Matriz-Vector) = %e segundos\n", elapsed);
#  ifdef DEBUG
   Print_vector("El producto es:", y, m); 
#  endif

   free(A);
   free(x);
   free(y);
   free(thread_handles);

   return 0;
}  


