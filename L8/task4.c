#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

double get_wall_seconds();
void* prime_counter();
int is_prime();
int main(int argc, char* argv[]){
	if(argc != 2){
		printf("ERROR: Not enough arguments\n");
		return -1;
	}

	int M = atoi(argv[1]);
	int counter = 1;
	double start = get_wall_seconds();

	pthread_t thread;
	int args[3];
	args[0] = 0;
	args[1] = M/2+1;
	args[2] = M;
	pthread_create(&thread, NULL, prime_counter, args);
	for(int i = 3; i <=M/2; i++){
		if(is_prime(i)) counter++;
	}
	pthread_join(thread, NULL);
	printf("%i\n",args[0]);
	int sum = counter + args[0];
	printf("There are %i primes in the range 1-%i\n", sum, M);
	printf("Computed in %lf\n",get_wall_seconds()-start);
	return 0;
}

int is_prime(int n){
	for(int i = 2; i<n; i++)
		if(n%i ==0) return 0;
	return 1;
}
void* prime_counter(void* arg){
	int* args = (int*) arg;
	int* counter = &(args[0]);
	int s = args[1];
	int e = args[2];
	for(int i = s; i <= e; i++){
		if(is_prime(i)) *counter += 1;
	}
}



double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
