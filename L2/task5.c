#include <stdio.h>
#include <math.h>

int main (){
	float sq;
	printf("Input a square: ");
	scanf(" %f", &sq);
	if(sqrt(sq) == ceilf(sqrt(sq)) && sq > 0) printf("Is perfect square \n");
	return 0;





}
