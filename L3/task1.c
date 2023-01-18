#include <stdio.h>


int main(){
	int n = 5;
	int mat[5][5];
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			if(i - j == 0) mat[j][i] = 0;
			if(i - j < 0) mat[j][i] = 1;
			if(i - j > 0) mat[j][i] = -1;
		}
	}
	for(int i = 0; i < n; i++){
		for(int j = 0; j < n; j++){
			printf("%i ", mat[j][i]);
			if (j==4) printf("\n");
			
		}
	}









}
