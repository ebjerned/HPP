#include <stdio.h>
#include <string.h>

int main(){
	int a, b;
	printf("Enter a and b:");
	scanf("%i %i", &a, &b);

	for(int y=1; y <= b; y++){
		char line[b];
		for(int x=1; x <= a; x++){
			if(y == 1 || y == b){
				strcat(line, "*");
			}else if (x == 1 || x == a){
				strcat(line, "*");
	
			} else{
				strcat(line, ".");
			}
		}
		printf("%s \n", line);
		strcpy(line, "");
	}

	return 0;
}
