#include <stdio.h>


void (*fpoint)(int);

void print_int_1(int);
void print_int_2(int);

int main(){
	fpoint = &print_int_1;
	fpoint(1);
	fpoint = &print_int_2;
	fpoint(5);





	return 0;
}

void print_int_1(int x){
	printf("Here is the number: %d\n", x);
}

void print_int_2(int x){
	printf("Wow, %d is really an impressive number!\n",x);
}
