#include "testfuncs.h"
#include <stdio.h>

// Out of rf = 4, 7, 12. 4 gives the best performance at about 40% faster than original
// Using -funroll-loops is about the same performance as doin it manually
// With -O3 and flag, they are the same, with just O3 f_opt is just slightly better than f_std

void f_std(const double * __restrict a, 
	   const double * __restrict b, 
	   double * __restrict c, int N) {
  int i;
  double x = 0.3;
  for(i = 0; i < N; i++) {
    c[i] = a[i]*a[i] + b[i] + x;
  }
}

void f_opt(const double * __restrict a, 
	   const double * __restrict b, 
	   double * __restrict c, int N) {
  int i;
  int unroll_factor = 4;
  double x = 0.3;
  
  for(i = 0; i < N-(N%unroll_factor); i+=unroll_factor) {
//    printf("Unrolled index: %i\n", i);
    c[i] = a[i]*a[i] + b[i] + x;
    c[i+1] = a[i+1]*a[i+1] + b[i+1] + x;
    c[i+2] = a[i+2]*a[i+2] + b[i+2] + x;
    c[i+3] = a[i+3]*a[i+3] + b[i+3] + x;
/*    c[i+4] = a[i+4]*a[i+4] + b[i+4] + x;
    c[i+5] = a[i+5]*a[i+5] + b[i+5] + x;
    c[i+6] = a[i+6]*a[i+6] + b[i+6] + x;
    c[i+7] = a[i+7]*a[i+7] + b[i+7] + x;
    c[i+8] = a[i+8]*a[i+8] + b[i+8] + x;
    c[i+9] = a[i+9]*a[i+9] + b[i+9] + x;
    c[i+10] = a[i+10]*a[i+10] + b[i+10] + x;
    c[i+11] = a[i+11]*a[i+11] + b[i+11] + x;*/
  }
  for(int j = N-(N%unroll_factor); j < N; j++){
//    printf("Index: %i\n", j);
    c[j] = a[j]*a[j] + b[j] + x;
  }
}

