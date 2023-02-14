#include <stdlib.h>
#include <stdio.h>


#ifdef _OPENMP
	#include <omp.h>
#endif

int is_prime(int);
void prime_counter(int*, int);

int main(int argc, char* argv[]){
	if(argc != 3) return -1;
	int M = atoi(argv[1]);
	int N = atoi(argv[2]);
	int result[N];
	int sum = 0;
	#pragma omp parallel num_threads(N)
	{
		prime_counter(result, M);
	}
	for(int i = 0; i < N; i++){
		sum += result[i];
	}
	printf("Number of primes %i\n", sum);
}


int is_prime(int n){
	for(int i = 2; i<n; i++)
		if(n%i ==0) return 0;
	return 1;
}

void prime_counter(int* result, int lim){
	#ifdef _OPENMP
	int counter = omp_get_thread_num()==0 ? 1 : 0;
	for(int i = omp_get_thread_num()+3; i <= lim; i += omp_get_num_threads()){
		if(is_prime(i)) counter++;
	}
	result[omp_get_thread_num()] = counter;
	#else
	printf("OpenMP not supported\n");
	#endif
}
