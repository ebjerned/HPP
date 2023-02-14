#include <stdio.h>
#include <omp.h>

void thread_func() {
  printf("This is inside thread_func()!\n");
  int curr = omp_get_thread_num();
  int m = omp_get_num_threads();
  printf("This is thread #%i of %i threads\n", curr,m);
}

int main(int argc, char** argv) {

#pragma omp parallel num_threads(4)
  {
    thread_func();
  }

  return 0;
}
