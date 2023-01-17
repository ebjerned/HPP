#include <stdio.h>

int main(){
	int in, pal, digit, out;
	printf("Input: ");
	scanf(" %i", &in);
	printf("\n");
	pal = in;

	while (pal>0){
		digit = pal % 10;
		out =  out*10 + digit;
		pal = (int)(pal/10);
	}

	if (out == in){
		printf("Output: %i is a palindrome \n", in);
	}else{
		printf("Output: %i is not a palindrome \n", in);
	}



	return 0;

}
