#include <stdio.h>

int main(){
	int dend,div, quot, rem;

	printf("Enter dividend: ");
	scanf(" %i", &dend);
	printf("Enter divisor: ");
	scanf(" %i", &div);
	
	rem = dend % div;
	quot = (dend - rem)/div;
	printf("Quotient = %i\n", quot);
	printf("Remainder = %i \n", rem);

	
	return 0;







}
