#include <math.h>

void leaf_function(float*);
float frame_function();

int main(){

	frame_function();


}

float frame_function(){
	float value = 1;
	for(int i = 0; i < 1000000000; i++){
		
		leaf_function(&value);
	}

	return value;

}

void inline leaf_function(float* value){
		
	float r_val = (float) round((*value)/0.055511);
	r_val *= 3.33;
	r_val /= 3;
	
	value = &r_val;

}

