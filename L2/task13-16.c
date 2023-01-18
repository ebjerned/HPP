#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char const *argv[]){

	char product[50];
	char filename[100];
	double price;
	int count = 0;
	int n;
	typedef struct product {
		char name[50];
		double price;

	} product_t;
	

	strcpy(filename, argv[1]);
	FILE* fp = fopen(filename, "r");
	if (fp == NULL) {
		printf("File not found");
		return 0;
	}
	fscanf(fp, "%i", &n);
	product_t* arr_of_prod = (product_t*)malloc(n*sizeof(product_t));
	while (1){
		if(count ==n) break;
		fscanf(fp, "%s %lf", product, &price);
		strcpy(arr_of_prod[count].name, product);
		arr_of_prod[count].price = price;
		printf("Product: %s, Price: %lf \n", product, price);
		count++;
	}
	return 0;
}
