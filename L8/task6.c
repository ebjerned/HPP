#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>
double get_wall_seconds();
void* prime_counter();
int is_prime();

typedef struct arguments {
	int* counter;
	int start;
	int end;
} args_t;

int main(int argc, char* argv[]){
	if(argc != 3){
		printf("ERROR: Not enough arguments\n");
		return -1;
	}

	int M = atoi(argv[1]);
	int N = atoi(argv[2]);

	int counter = 1;
	double start = get_wall_seconds();

	int intervall = M/N;
	int surp= 0;
	pthread_t* threads = malloc(N*sizeof(pthread_t));
	int* results = (int*)malloc(4*N);
	args_t* arg_tracker[N];
	int sum=0;
	for(int i = 0; i < N; i++){
		if(i == N-1 || i != 1)
			surp = M%N;
		args_t* args = (args_t*)malloc(sizeof(args_t));
		args->counter = &results[i];
		args->start = i==0 ? i*intervall + 2 : i*intervall+1;
		args->end = (i+1)*intervall +surp;
		arg_tracker[i] =  args;
		pthread_create(&threads[i], NULL, prime_counter, (void*)args);
	}
	for(int i=0; i<N; i++){
		pthread_join(threads[i], NULL);
		sum += results[i];
	}

	printf("There are %i primes in the range 1-%i\n", sum, M); 
	printf("Computed in %lf\n",get_wall_seconds()-start); 
	free(threads); 
	free(results); 
	for(int i=0; i<N; i++){
		free(arg_tracker[i]);
        }
	return 0;
}

int is_prime(int n){
	for(int i = 2; i<n; i++)
		if(n%i ==0) return 0;
	return 1;
}
void* prime_counter(void* arg){
	args_t* args = (args_t*) arg;
//	int* counter = &(args[0]);
	for(int i = args->start; i <= args->end; i++){
		if(is_prime(i)) *(args->counter) += 1;
	}
	printf("%i primes in  range %i - %i\n", *(args->counter), args->start, args->end);

}



double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
