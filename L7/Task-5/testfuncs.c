#include "testfuncs.h"

// Runnning with a = [0,0.5] gives tihe time 0.5s istad of 1.6s

double f_std(const double * a, int N) {
  int i;
  double sum = 0;
  int counter = 0;
  for(i = 0; i < N; i++) {
    if(a[i] > 0.5 && counter == 3)
      sum += 0.1*a[i];
    if(counter > 5 && a[i] > 0.5)
      counter = 5;
    if(counter > 6 && a[i] > 0.6)
      counter = 6;
    sum += 0.3*a[i] + a[i]*a[i];
    counter++;
  }
  return sum;
}

double f_opt(const double * a, int N) {
  int i;
  double sum = 0;
  int counter = 0;
  for(i = 0; i < N; i++) {
    if(a[i] > 0.5 && counter == 3)
      sum += 0.1*a[i];
    if(counter > 5 && a[i] > 0.5)
      counter = 5;
    if(counter > 6 && a[i] > 0.6)
      counter = 6;
    sum += 0.3*a[i] + a[i]*a[i];
    counter++;
  }
  return sum;
}

