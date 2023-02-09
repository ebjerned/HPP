#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <sys/time.h>
#include <pthread.h>
#define NUM_THREADS 10 
// Speed up is about as expected, being normal time/NUM_THREADS
// Larger than 70x70 matrices are faster running parallell, otherwise serial is faster
void* multiply(void* arg);

static double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

double **A,**B,**C;
int n;

int main(int argc, char *argv[]) {
  int i, j, k;

  if(argc != 2) {
    printf("Please give one argument: the matrix size n\n");
    return -1;
  }

  n = atoi(argv[1]);
  
  //Allocate and fill matrices
  A = (double **)malloc(n*sizeof(double *));
  B = (double **)malloc(n*sizeof(double *));
  C = (double **)malloc(n*sizeof(double *));
  for(i=0;i<n;i++){
    A[i] = (double *)malloc(n*sizeof(double));
    B[i] = (double *)malloc(n*sizeof(double));
    C[i] = (double *)malloc(n*sizeof(double));
  }

  for (i = 0; i<n; i++)
    for(j=0;j<n;j++){
      A[i][j] = rand() % 5 + 1;
      B[i][j] = rand() % 5 + 1;
      C[i][j] = 0.0;
    }

  printf("Doing matrix-matrix multiplication...\n");
  double startTime = get_wall_seconds();

  // Multiply C=A*B
  for(i=0; i<n; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
	C[i][j] += A[i][k] * B[k][j];

  printf("Normal calc: %lf s\n", get_wall_seconds()-startTime);
  for (i = 0; i<n; i++)
    for(j=0;j<n;j++){
      C[i][j] = 0.0;
    }
  double time = get_wall_seconds();
  pthread_t threads[NUM_THREADS];

  for(i = 0; i< NUM_THREADS; i++){
	int* range = (int*)malloc(8);
	range[0] = i*n/NUM_THREADS;
	range[1] = (i+1)*n/NUM_THREADS;
	pthread_create(&threads[i], NULL, multiply, range);
  }
  for(i =0; i<NUM_THREADS; i++){
  	pthread_join(threads[i],NULL);
	
  }
  printf("Parallell calc: %lf s\n", get_wall_seconds()-time);
  double timeTaken = get_wall_seconds() - startTime;
  printf("Elapsed time: %f wall seconds\n", timeTaken);

  // Correctness check (let this part remain serial)
  printf("Verifying result correctness for a few result matrix elements...\n");
  int nElementsToVerify = 5*n;
  double max_abs_diff = 0;
  for(int el = 0; el < nElementsToVerify; el++) {
    i = rand() % n;
    j = rand() % n;
    double Cij = 0;
    for(k = 0; k < n; k++)
      Cij += A[i][k] * B[k][j];
    double abs_diff = fabs(C[i][j] - Cij);
    if(abs_diff > max_abs_diff)
      max_abs_diff = abs_diff;
  }
  printf("max_abs_diff = %g\n", max_abs_diff);
  if(max_abs_diff > 1e-10) {
    for(i = 0; i < 10; i++)
      printf("ERROR: TOO LARGE DIFF. SOMETHING IS WRONG.\n");
    return -1;
  }
  printf("OK -- result seems correct!\n");
  
  // Free memory
  for(i=0;i<n;i++){
    free(A[i]);
    free(B[i]);
    free(C[i]);
  }
  free(A);
  free(B);
  free(C);

  return 0;
}


void* multiply(void* arg){
  int* args = (int*) arg;
  int s = args[0];
  int e = args[1];
  int i,j,k;
  printf("Thread range %i - %i\n", s,e);
  for(i=s; i<e; i++)
    for (j=0; j<n; j++)
      for (k=0; k<n; k++)
	C[i][j] += A[i][k] * B[k][j];
  return NULL;
}

