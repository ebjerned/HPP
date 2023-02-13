#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



void* the_thread_func(void* arg) {
  /* Do something here? */
  double* testdata = (double*)malloc(3*8);
  testdata[0] = 1;
  testdata[1] = 2;
  testdata[2] = 3;

//  return testdata;
	pthread_exit(testdata);
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  if(pthread_create(&thread, NULL, the_thread_func, NULL) != 0) {
    printf("ERROR: pthread_create failed.\n");
    return -1;
  }

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
  double* threaddata;
  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  if(pthread_join(thread, (void**) &threaddata) != 0) {
    printf("ERROR: pthread_join failed.\n");
    return -1;
  }
  for(int i=0; i <3; i++)
	printf("%lf\n", threaddata[i]);
  free(threaddata);
  return 0;
}
