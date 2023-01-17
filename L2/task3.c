#include <stdio.h>
#include <assert.h>
#include <math.h>

int main(){
	
	int a, b;
	printf("Enter two numbers: ");
	scanf("%i %i", &a, &b);
	printf("\n");
	
	if((a%2 == 0) && (b%2==0)){
		int result = a + b;
		printf("Summation %i", result);
	}else{
		int result = a * b;
		printf("Multiplication%i", result);
	}
	printf("\n");
	
	float c, d, e;
	printf("Enter three numbers: ");
	scanf("%f %f %f", &c, &d, &e);
	float list[3] = {fabs(c), fabs(d), fabs(e)};
	float max = list[0];
	for ( int i = 0; i < 3; i++){
		if (list[i] > max){
			max = list[i];
		}
	}
	printf("Maximum found: %f \n", max);

	printf("\n");
	float f, g, h;
	printf("Enter three numbers: ");
	scanf("%f %f %f", &f, &g, &h);
	float slist[3] = {fabs(f), fabs(g), fabs(h)};
	int index;	
	float smax = slist[0];
	for ( int i = 0; i < 3; i++){
		if (slist[i] > smax){
			smax = slist[i];
			index  = i;
		}
	}
	float mid = 0;
	for (int j = 0; j < 3; j++){
		if(j==index) continue;
		if(list[j]>mid){
			mid = slist[j];
		}
	}
	printf("Second maximum found: %f \n", mid);


	return 0;
}
