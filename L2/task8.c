#include <stdio.h>

int main(){
	double d;
	int i;
	char c;

	printf("Enter a double, an int and a char: ");
	scanf(" %lf %i %c", &d, &i, &c);

	printf("Double: %lf, %p, %li \n", d, &d, sizeof(d));
	printf("Int: %i, %p, %li \n", i, &i, sizeof(i));
	printf("Char: %c, %p, %li \n", c, &c, sizeof(c));


	return 0;








}
