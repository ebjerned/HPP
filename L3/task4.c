#include <stdio.h>
#include <stdlib.h>
int (*f)(const void*, const void*);
//void qsort (void*, size_t, size_t, int (*compar)());
int compareDouble (const void*, const void*);


int main(){
	double arr[9]={1, 3, 2, 4, 5, 6, 7, 8, 9}; 
	f = &compareDouble;
	qsort(arr, 9, sizeof(double), (*f));
	for (int i = 0; i < 9; i++){
		printf("%lf \n", arr[i]);
	}
	return 0;
}
int compareDouble(const void* p1, const void* p2){
	if (*(double*)p1 > *(double*)p2)  return -1;
	if (*(double*)p1 == *(double*)p2) return 0;
	if (*(double*)p1 < *(double*)p2) return 1;
	return 0;

}

/* Not needed to implement own qsort
void qsort (void* base, size_t num, size_t size, int (*compar)()){
	double* ptr = base;
	double* prev = ptr;
	double* curr = ptr;
	double* next = ptr;
	int result = 0;
	for (int i = 0; i < num; i++){
		if (i == num -1){
			result = compar(prev, curr);
//			printf("Edge case handled\n");
			if(result == 1) swap_pointers(&prev, &curr);
			
		} else {
			prev = curr;
			curr = next;
			next = ++ptr;
//			printf("%i %i \n", *curr, *next);

			result = compar(curr, next);
			if(result == 1) swap_pointers(&curr, &next);
		}
		printf("%lf %lf %i %lf \n", *curr, *next,  result, *ptr);
		
		
	}
}


void swap_pointers(char** s1, char** s2){
	char *temp = *s1;
	*s1 = *s2;
	*s2 = temp;
}


*/









