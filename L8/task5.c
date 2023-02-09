#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>



typedef struct data{
	int index;
	double someData;
	} data_t;

void* indexing(void*arg);

int main(int argc, char* argv[]){
	if(argc != 2){
		printf("ERROR: Not enough arguments\n");
		return -1;
	}
	int N = atoi(argv[1]);
	pthread_t* threads = malloc(N*sizeof(pthread_t));
	data_t* iter_data;
	for(int i = 0 ; i < N; i++){
		iter_data = malloc(sizeof(data_t));
		(*iter_data).index = i;
		(*iter_data).someData = i;
		pthread_create(&threads[i], NULL, indexing, iter_data);

	}
	for(int i = 0; i<N; i++){
		pthread_join(threads[i], NULL);
	}
	free(threads);

}


void* indexing(void* arg){
	data_t* data = (data_t*) arg;
	printf("Thread with index %i has data %lf\n", data->index, data->someData);
}
