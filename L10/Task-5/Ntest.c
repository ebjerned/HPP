#include <stdio.h>
#include <omp.h>

const long int NA = 4000000000;
const long int NB = 8000000000;

long int thread_func_A() {
  double st = omp_get_wtime();
  long int i;
  long int sum = 0;
  for(i = 0; i < NA; i++)
    sum += 7;
  printf("Thread a done in %lf s\n", omp_get_wtime()-st);
  return sum;
}

long int thread_func_B() {
  double st = omp_get_wtime();
  long int i;
  long int sum = 0;
  for(i = 0; i < NB; i++)
    sum += 7;
  printf("Thread b done in %lf s\n", omp_get_wtime()-st);
  return sum;
}

int main() {
  printf("This is the main() function starting.\n");

  long int result_A;
  long int result_B;

#pragma omp parallel num_threads(2)
  {
    int id = omp_get_thread_num();
    if(id == 0)
      result_A = thread_func_A();
    else
      result_B = thread_func_B();
  }

  printf("This is the main() function after the parallel block.\n");

  printf("result_A : %ld\n", result_A);
  printf("result_B : %ld\n", result_B);
  long int totalSum = result_A + result_B;
  printf("totalSum : %ld\n", totalSum);

  return 0;
}
