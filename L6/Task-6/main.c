#include <stdio.h>
//#include "func.h"

// Moving f made perf instant
// Setting the attribute const results in the same.
// Moving the functions does not decrease the function calls, and do not produce instant results. But reduces time with 75%

int ff(int k);
int get_counterf();

int main() {
  int n = 20000;
  int i, j;
  int sum = 0;

  for(i = 0; i < n; i++) {

    for(j = 0; j < n; j++) {
      sum += ff(i) + 3*j;
    }
  }
  printf("Result sum: %d\n", sum);
  printf("Function call counter: %d\n", get_counterf());
  return 0;
}



static int function_call_counter = 0;

int ff(int k) {
  // Increment function call counter
  function_call_counter++;
  // Do some expensive operations so that this function takes some time
  int x = 3;
  if(k > 0 && 88/k > 2)
    x += 11;
  if(k > 0)
    x += 777/k + 888 / k + 999 / k;
  return x;
}

int get_counterf() {
  return function_call_counter;
}

