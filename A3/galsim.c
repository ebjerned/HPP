#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
	if(argc != 6){
		printf("Not enough arguments. Input arguments: 'N', 'filename', 'nsteps', 'delta_t', 'graphics'\n");
		return -1;
	}
	int n_particles = atoi(argv[1]);
	char* input_path = argv[2];
	int n_steps = atoi(argv[3]);
	double dt = atof(argv[4]);
	char graphics = *argv[5];






	return 0;
}
