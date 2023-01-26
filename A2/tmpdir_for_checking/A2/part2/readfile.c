#include <stdio.h>
#include <stdlib.h>


char* PATH ="little_bin_file"; 

int main(){
	// Open file
	FILE* file = fopen(PATH, "rb");

	// Designating the type sizes in given order, could be done with sizeof() aswell
	int size_buffer[4] = {4, 8, 1, 4};

	// Allocate 17 bytes of memory
	void* adress = malloc(17);

	// There are four entries to be read of different sizes
	for(int i = 0; i < 4; i++){
		// Read from 1 element from file, with size size_buffer[i] at adress
		fread(adress, size_buffer[i], 1, file);

		// Switch case to get correct formatting in print
		switch (i){
		case 0:
			printf("%i\n", *(int*)adress);
			break;
		case 1:
			printf("%lf\n", *(double*)adress);
			break;
		case 2:
			printf("%c\n", *(char*)adress);
			break;
		case 3:
			printf("%f\n", *(float*)adress);
			break;
		}

		// Step the memory adress with size_buffer[i] amount of bytes to get to next slot in memory
		adress += size_buffer[i];
	}
	
	// Return to original adress and free the memory
	adress -= 17;
	free(adress);

	// Close file
	fclose(file);

	return 0;

}
