#include <stdlib.h>
#include "sort_funcs.h"

// Swaping to a single malloc does not affect the memoryallocation, which is natural but with
// the less amount of calls calculation time is reduced from 0.132s to 0.071s when n = 20000
// Stack allocation gives 0.036s performance
// n = 1000 => 0.06s
// n = 100 => 0.016s
// n = 50 => 0.012s
// n = 25 => 0.011s
// n = 10 => 0.011s
// n = 5 => 0.013s

void bubble_sort(intType* list, int N) {
  int i, j;
  for(i = 0; i < N-1; i++)
    for(j = 1+i; j < N; j++) {
      if(list[i] > list[j]) {
	// Swap
	intType tmp = list[i];
	list[i] = list[j];
	list[j] = tmp;
      }
    }
}

void merge_sort(intType* list_to_sort, int N) {
  if(N == 1) {
    // Only one element, no sorting needed. Just return directly in this case.
    return;
  }
  int n1 = N / 2;
  int n2 = N - n1;
  // Allocate new lists
// Single allocation
//  intType* tot_list = (intType*)malloc((n1+n2)*sizeof(intType));
//  intType* list1 = &tot_list[0];
//  intType* list2 = &tot_list[n2];
// Double allocation
//  intType* list1 = (intType*)malloc(n1*sizeof(intType));
//  intType* list2 = (intType*)malloc(n2*sizeof(intType));

// Stack Allocation
  intType list1[n1];
  intType list2[n2];

  int i;
  for(i = 0; i < n1; i++)
    list1[i] = list_to_sort[i];
  for(i = 0; i < n2; i++)
    list2[i] = list_to_sort[n1+i];
  // Sort list1 and list2
  if(n1 <= 5){
  	bubble_sort(list1, n1);
	bubble_sort(list2, n2);
  } else {
  	merge_sort(list1, n1);
  	merge_sort(list2, n2);
  }
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
//    Double deallocation
//    free(list1);
//    free(list2);
//    Single deallocation
//    free(tot_list);
}

