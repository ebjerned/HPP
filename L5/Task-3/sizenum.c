#include <stdio.h>
#include <math.h>

int main(){
	int iter = 170;
	float t=1; // Breaks at iteration 19, at ~10^38 as expected but with alot of errors, is not identical to 10^38 as is should be in theory
	double tt=1; // Breaks at iter 306, ~10^306, but not equal to
	for (int i =0; i < iter; i ++){
		t *= 100;
		tt *= 100;
		printf("Iter: %i Float: %f, Double: %lf\n",i, t, tt);
	}
	printf("Final: %f, %lf\n",t+100, t/1000); // Does not recover information

	double neg = sqrt(-100);
	printf("%lf\n", neg);
	printf("%lf\n", neg*1000);


	int j;
	double epsilon = 1, ref_val;
	for(j =0; j< 10000;j++){
		epsilon *= 0.5;
		ref_val = 1 +epsilon;
		if (ref_val <= 1) break;
	}
	printf("Iter %i, epsilon %lf", j, epsilon);
	return 0;
}
