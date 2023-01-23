#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int binomial_coefficient(int n, int k);
int* row_coefficients(int n);
void print_int_array(int* arr, int n_elem);
void print_n_rows(int n);

int main(int argc, char* argv[]) {
	if(argc != 2){
		fprintf(stderr, "Wrong number of arguments: Given %i expected 1. Exiting... \n", argc-1);
		exit(-1);
	}
	int n = atoi(argv[1]);
	print_n_rows(n);
	return 0;
}

int binomial_coefficient(int n, int k){
	if(k == 0 || k == n) return 1;
//	if(n >= k) k = n-k;
	float coeff = 1;
	for(int i= 1; i <= k; i++){
		coeff *= (float)(n+1-i)/(float)i;
//		printf("n: %i, k: %i. %f\n",n,i, coeff);
	}
//	printf("%i \n", coeff);
	return (int)coeff;
}

int* row_coefficients(int n){
	int* row = (int*)malloc((n+1)*sizeof(int));

	// TODO Use symmetry over centre to reduce iterations
	int symmetry_limit = ceil((n+1)/2);
	for(int k = 0; k <= symmetry_limit; k++){
		row[k] = binomial_coefficient(n, k);
		row[n-k] = row[k];
	}
	return row;
}

void print_int_array(int* arr, int n_elem){
	for (int i = 0; i < n_elem; i++)
		printf("%i \t", arr[i]);
	printf("\n");
}

void print_n_rows(int n){
	for (int i = 0; i < n; i++){
		int* curr = row_coefficients(i);
		print_int_array(curr, i+1);
		free(curr);
	}
	
}
