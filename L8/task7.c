#include <stdio.h>
#include <pthread.h>

void* thread_func();
void* thread_func_2();

int main(){

	pthread_t threadA;
	pthread_t threadB;

	pthread_create(&threadA, NULL, thread_func, NULL);
	pthread_create(&threadB, NULL, thread_func, NULL);
	printf("Main thread\n");
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);

}


void* thread_func(){
	
	pthread_t threadA;
	pthread_t threadB;

	pthread_create(&threadA, NULL, thread_func_2, NULL);
	pthread_create(&threadB, NULL, thread_func_2, NULL);
	printf("1st level thread\n");
	pthread_join(threadA, NULL);
	pthread_join(threadB, NULL);
}

void* thread_func_2(){
	printf("2nd level thread\n");
}
