#include <stdio.h>
#include <stdlib.h>

void print_array(int*, int);

int main(){

	int *arr;
	int n;

	scanf(" %d", &n);
	arr = (int *)malloc(n*sizeof(int));
	for(int i = 0; i < n; i++){
		arr[i] = rand();
	}
	print_array(arr, n);

	return 0;
}


void print_array(int* arr, int n){
	for (int j = 0; j < n; j++){
		out[j] = arr[j];
		printf("%i", arr[j]);
		printf("\n");
	}


}
