#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* the_thread_func(void* arg) {
  /* Do something here? */
  double* arr = (double*) arg;
  printf("%lf, %lf, %lf\n", arr[0], arr[1], arr[2]);
  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  double data_for_thread[3];
  data_for_thread[0] = 5.7;
  data_for_thread[1] = 9.2;
  data_for_thread[2] = 1.6;

  double* data_for_threadB = (double*)malloc(sizeof(double)*3);
  data_for_threadB[0]= 1;
  data_for_threadB[1]= 10;
  data_for_threadB[2]= 100;

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, data_for_thread);
  pthread_t threadB;
  pthread_create(&threadB, NULL, the_thread_func, data_for_threadB);


  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);
  pthread_join(threadB, NULL);
  free(data_for_threadB);
  return 0;
}
