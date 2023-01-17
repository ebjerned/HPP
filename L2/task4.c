#include <stdio.h>

int main(){
	double a, b;
	char op;
	
	printf("Enter operator: ");
	scanf(" %c", &op);

	printf("Input: ");
	scanf(" %lf %lf", &a, &b);
	printf("\n");
	switch (op){
		case '+':
			printf("%lf", a+b);
			break;
		case '-':
			printf("%lf", a-b);
			break;
		case '*':
			printf("%lf", a*b);
			break;
		case '/':
			printf("%lf", a/b);
			break;
		default:
			printf("Operation not recognized \n");

	}
	printf("\n");
	

	return 0;


}
