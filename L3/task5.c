#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int (*f)(const void*, const void*);
int compareString(const void*, const void*);

int main(){

	char *strArr[] = {"daa", "cbab", "bbbb", "bababa", "ccccc", "aaaa"};
	int arrStrLen = sizeof(strArr) / sizeof(char *);
	f = &compareString;
	qsort(strArr, arrStrLen, sizeof(char *), compareString);
	for(int i = 0; i < 6; i++){
		printf("%s \n", strArr[i]);
	}
	return 0;
} 

int compareString(const void* p1, const void* p2){
	
	return strcmp(*(const char**)p1, *(const char**)p2);

}
