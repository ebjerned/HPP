#include <stdio.h>


int main(){
	char[20] product;
	int price;
	FILE* fp = fopen("data.txt", "r");

	fscanf(fp, "%s %lf", product, &price);
}