#include <stdlib.h>
#include "sort_funcs.h"
#include <omp.h>
void bubble_sort(int* list, int N) {
  int i, j;
  for(i = 0; i < N-1; i++)
    for(j = 1+i; j < N; j++) {
      if(list[i] > list[j]) {
	// Swap
	int tmp = list[i];
	list[i] = list[j];
	list[j] = tmp;
      }
    }
}

void merge_sort(int* list_to_sort, int N, int M) {
  if(N == 1) {
    // Only one element, no sorting needed. Just return directly in this case.
    return;
  }
  int n1 = N / 2;
  int n2 = N - n1;
  // Allocate new lists


  int* tot_list = (int*)malloc(N*sizeof(int));
  int* list1 = tot_list;
  int* list2 = &tot_list[n1];

  int i;
  for(i = 0; i < n1; i++)
    list1[i] = list_to_sort[i];
  for(i = 0; i < n2; i++)
    list2[i] = list_to_sort[n1+i];
  // Sort list1 and list2
  if (M>=2){
	  #pragma omp parallel num_threads(2)
	  if(omp_get_thread_num()%2 == 0){
		  merge_sort(list1, n1,M/2);
	  } else {
		  merge_sort(list2, n2,M/2);
	  }
	  int i1 = 0;
	  int i2 = 0;
	  i = 0;
	  while(i1 < n1 && i2 < n2) {
	    if(list1[i1] < list2[i2]) {
	      list_to_sort[i] = list1[i1];
	      i1++;
	    }
	    else {
	      list_to_sort[i] = list2[i2];
	      i2++;
	    }
	    i++;
	  }
	  while(i1 < n1)
	    list_to_sort[i++] = list1[i1++];
	  while(i2 < n2)
	    list_to_sort[i++] = list2[i2++];
	  free(tot_list);
	  
  } else {
	 merge_sort(list1, n1,1);
	 merge_sort(list2, n2,1);
	  
	  // Merge!
	  int i1 = 0;
	  int i2 = 0;
	  i = 0;
	  while(i1 < n1 && i2 < n2) {
	    if(list1[i1] < list2[i2]) {
	      list_to_sort[i] = list1[i1];
	      i1++;
	    }
	    else {
	      list_to_sort[i] = list2[i2];
	      i2++;
	    }
	    i++;
	  }
	  while(i1 < n1)
	    list_to_sort[i++] = list1[i1++];
	  while(i2 < n2)
	    list_to_sort[i++] = list2[i2++];

	  free(tot_list);
  }
}

