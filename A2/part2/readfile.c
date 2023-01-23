#include <stdio.h>
#include <stdlib.h>


char* PATH ="little_bin_file"; 

int main(){
	FILE* file = fopen(PATH, "rb");
	FILE* check = fopen("check_file", "wb");
	int size_buffer[4] = {4, 8, 1, 4};

	void* adress =(void*) malloc(17);
	for(int i = 0; i < 4; i++){
		fread(adress, size_buffer[i], 1, file);
		fwrite(adress, size_buffer[i], 1, check);
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
		adress += size_buffer[i];
	}
	fclose(file);
	fclose(check);
	free(adress);
	return 0;

}
