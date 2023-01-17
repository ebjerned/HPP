#include <stdio.h>
#include <stdlib.h>

int main(){
	int count = 0;
	int sum = 0;
	int* arr = (int *)malloc(10*sizeof(int));
	printf("Input: ");
	while (1){
		int current;
		if (count%10 == 0 && count > 0){
			arr = realloc(arr, (count+10)*sizeof(int));
			printf("More space allocated, now size %li. Keep on entering values: \nInput: ", (count+10)*sizeof(int));
		}
		if(!scanf(" %i", &current)) break;
		arr[count] = current;
		sum += current;
		count++;
		
	}
	printf("Sum: %i \n", sum);
return 0;







}
