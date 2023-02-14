#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
void do_some_work() {
  double st = omp_get_wtime();
  printf("Now I am going to do some work...\n");
  long int i;
  long int j = 0;
  for(i = 0; i < 3000000000; i++) {
    j += 3;
  }
  printf("Work done! My result j = %ld in %lf s\n", j, omp_get_wtime()-st);
}

int main(int argc, char** argv) {
if(argc != 2) return -1;
int n = atoi(argv[1]);

#pragma omp parallel num_threads(n)
  {
    do_some_work();
  }

  return 0;
}
