#include <stdio.h>

int main(){
	for(int i = 100; i >= 0; i-=4){
		printf("For-loop: %d \n", i);

	}
	int j = 100;
	while (j >= 0){
		printf("While-loop: %d \n", j);
		j-=4;
		
	}
	int k = 100;
	do {
		printf("Do-while loop: %d \n", k);
		k -= 4;
	} while (k >= 0);

}
