#include <stdio.h>
#include <stdlib.h>


char* PATH ="little_bin_file"; 

int main()
{
/*	int** buffer = (int**)malloc(4);
	int size_buffer[4] = {(int)sizeof(int), (int)sizeof(double), (int)sizeof(char), (int)sizeof(float)};
	FILE* file = fopen(PATH, "rb");
	for(int i = 0; i < 4; i++){
		fseek(file, size_buffer[i], SEEK_CUR);
		fread(buffer[i], size_buffer[i], 1, file);
		printf("%i \n",*buffer[i]); 
	}
	for (int i=0; i<17; i++)
		printf("%u", buffer[i]);
	
	fclose(file);
*/

	FILE* file = fopen(PATH, "rb");

	fseek(file, 0, SEEK_SET);
	int* int_p=malloc(sizeof(int));
	fread(int_p, sizeof(int), 1, file);
	printf("Int: %i \n", *int_p);

	//fseek(file, 4, SEEK_SET);
	double* double_p =malloc(sizeof(double));
	fread(double_p, sizeof(double), 1,file);
	printf("Double: %lf \n", *double_p);

	//fseek(file, 12, SEEK_SET);
	char* char_p =malloc(sizeof(char));
	fread(char_p, sizeof(char), 1,file);
	printf("Char: %c \n", *char_p);

	//fseek(file, 13, SEEK_SET);
	float* float_p =malloc(sizeof(float));
	fread(float_p, sizeof(float), 1,file);
	printf("Float: %f \n", *float_p);

	fclose(file);
	FILE* returnFile = fopen("return_file", "wb");
	fwrite(int_p, sizeof(int), 1, returnFile);
	fwrite(double_p, sizeof(double), 1, returnFile);
	fwrite(char_p, sizeof(char), 1, returnFile);
	fwrite(float_p, sizeof(float), 1, returnFile);
	fclose(returnFile);
/*

	FILE* file = fopen(PATH,"rb");
	unsigned char buffer[17];
	fread(buffer, sizeof(buffer),1,file);
	for(int i=0; i<17; i++)
		printf("%x ", buffer[i]);

*/


}
