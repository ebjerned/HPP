#include <stdio.h>
#include <pthread.h>

void* the_thread_func(void* arg) {
	/* Do something here? */
	printf("thread starting doing some work.\n");
	long int i;
	double sum;
	for(i = 0; i < 1000000000; i++)
	sum += 1e-7;
	printf("Result of work in thread: sum = %f\n", sum);
  return NULL;
}
void* the_thread_funcB(void* arg) {
	/* Do something here? */
	printf("threadb starting doing some work.\n");
	long int i;
	double sum;
	for(i = 0; i < 100000000; i++)
	sum += 2e-7;
	printf("Result of work in threadB: sum = %f\n", sum);
  return NULL;
}

int main() {
  printf("This is the main() function starting.\n");

  /* Start thread. */
  pthread_t thread;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&thread, NULL, the_thread_func, NULL);
  pthread_t threadB;
  printf("the main() function now calling pthread_create().\n");
  pthread_create(&threadB, NULL, the_thread_funcB, NULL);

  printf("This is the main() function after pthread_create()\n");

  /* Do something here? */
	printf("main() starting doing some work.\n");
	long int i;
	double sum;
	for(i = 0; i < 100000000; i++)
	sum += 1e-7;
	printf("Result of work in main(): sum = %f\n", sum);

  /* Wait for thread to finish. */
  printf("the main() function now calling pthread_join().\n");
  pthread_join(thread, NULL);
  pthread_join(threadB, NULL);

  return 0;
}
