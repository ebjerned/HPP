/**********************************************************************
 * This program calculates pi using C
 *
 **********************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define NUM_THREADS 16
#define INTERVALS 500000000

void* pi(void* arg);

int main(int argc, char *argv[]) {

  int i;
/*  const int intervals = 500000000;
  double sum, dx, x;

  dx  = 1.0/intervals;
  sum = 0.0;

  for (i = 1; i <= intervals; i++) { 
    x = dx*(i - 0.5);
    sum += dx*4.0/(1.0 + x*x);
  }
*/
  double sum = 0.0;
  pthread_t threads[NUM_THREADS];
  int per_thread_intervall = INTERVALS/NUM_THREADS;
  int** intervall_args[NUM_THREADS];
  printf("Initialized variables\n");

  for(i = 0; i < NUM_THREADS; i++){
	int* intervall =(int*)malloc(8);
	intervall[0] = i*per_thread_intervall+1;
	intervall[1] = (i+1)*per_thread_intervall;
//	printf("Interval: %i - %i\n", intervall[0], intervall[1]);
	pthread_create(&threads[i], NULL, pi, (void*)intervall);
  }
  void* temp_sum=0;
  for(i = 0; i <NUM_THREADS; i++){
	pthread_join(threads[i], &temp_sum);
	sum += *(double*)temp_sum;
	free(temp_sum);
	free(intervall_args[i]);
  }

  printf("PI is approx. %.16f\n",  sum);

  return 0;
}

void* pi(void* arg){
	int* args = (int*)arg;
	int s = args[0];
	int e = args[1];
	double* sum = (double*)malloc(8);
	double dx = 1.0/INTERVALS;
//	printf("Thread interval: %i - %i\n", s, e);
	for(int i = s; i <= e; i++){
		sum[0] += dx*4.0/(1.0+((i-0.5)*dx)*((i-0.5)*dx));
	}
//	printf("%lf\n",sum[0]);
	return (void*)sum;
}
