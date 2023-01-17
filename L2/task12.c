#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int is_prime(int);

int main(){
	int* arr = (int*)malloc(10*sizeof(int));
	int counter = 0;
	printf("Input: ");
	while(1){
		int current;
		if(counter%10 == 0 && counter > 0){
			arr = realloc(arr, (counter+10)*sizeof(int));
			printf("Allocating more memory, new size: %li. Keep entering.\nInput: ", (counter+10)*sizeof(int));
		}
		if(!scanf(" %i", &current)) break;
		arr[counter] = current;
		counter++;
	}
	printf("List without primes: [");
	for (int j=0; j < counter; j++){
		if(!is_prime(arr[j])){
			if (j == counter -2){
				printf("%i", arr[j]);
			}else{
				printf("%i, ", arr[j]);
			}
		}
	}
	printf("]\n");
	return 0;

}


int is_prime(int n){
	for(int i = 2; i <= sqrt(n); i++){
		if (n % i == 0) return 0;
	}
	return 1;

}
