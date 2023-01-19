#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int (*f)(const char**, const char**);
int compareString(const char**, const char**);

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

int compareString(const char** p1, const char** p2){
	return strcmp(*p1, *p2);

}
