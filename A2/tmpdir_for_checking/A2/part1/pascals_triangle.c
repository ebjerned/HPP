#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int binomial_coefficient(int n, int k);
int* row_coefficients(int n);
void print_int_array(int* arr, int n_elem);
void print_n_rows(int n);

int main(int argc, char* argv[]) {
	// Check n of arguments
	if(argc != 2){
		fprintf(stderr, "Wrong number of arguments: Given %i expected 1. Exiting... \n", argc-1);
		exit(-1);
	}

	int n = atoi(argv[1]);

	print_n_rows(n);

	return 0;
}

int binomial_coefficient(int n, int k){
	// Special case
	if(k == 0 || k == n) return 1;

	// Has to be float during calulations to allow non-integer division
	float coeff = 1;
	float n_float = (float) n;

	// Use given algorithm
	for(float i= 1; i <= k; i++){
		coeff *= (n_float+1-i)/i;
	}
	// In the end all coefficient will be integers
	return (int)coeff;
}

int* row_coefficients(int n){
	// Allocate mempry for the entire row
	int* row = (int*)malloc((n+1)*sizeof(int));

	// Binomial coefficients are symmetric around the centre, use this to reduce amount of coefficient calculations
	int symmetry_limit = ceil((n+1)/2);

	// Assemble row of coefficients
	for(int k = 0; k <= symmetry_limit; k++){
		row[k] = binomial_coefficient(n, k);
		row[n-k] = row[k];
	}

	return row;
}

void print_int_array(int* arr, int n_elem){
	// Prints the values of an integer array
	for (int i = 0; i < n_elem; i++)
		printf("%i \t", arr[i]);
	printf("\n");
}

void print_n_rows(int n){
	// Prints a number of rows of the binomial coefficients
	for (int i = 0; i < n; i++){
		int* curr = row_coefficients(i);
		print_int_array(curr, i+1);
		free(curr);
	}
}
