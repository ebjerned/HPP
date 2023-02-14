#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>

double* read_input(char* path, int n);
void acceleration(double* data_array, double* acc_array, int n);
void solver(double* data_array, double* acc_array, double dt, int n);
void write_output(double* data_array, double* acc_array, int n, char* path);
double get_wall_seconds();



int main(int argc, char* argv[]){
	double time = get_wall_seconds();
	double start = time;

	if(argc != 6){
		printf("Not enough arguments. Input arguments: 'N', 'filename', 'nsteps', 'delta_t', 'graphics'\n");
		return -1;
	}


	int n_particles = atoi(argv[1]);
	char* input_path = argv[2];
	int n_steps = atoi(argv[3]);
	double dt = atof(argv[4]);
//	char graphics = *argv[5];
	double* acc_arr = (double*)malloc(n_particles*2*sizeof(double));

	printf("Initialized in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	double* data_arr = read_input(input_path, n_particles);

	if (data_arr == NULL) return -1;

	printf("Input read in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	for(int i = 0; i < n_steps; i++){
		acceleration(data_arr, acc_arr, n_particles);
		solver(data_arr, acc_arr, dt, n_particles);
	}

	printf("\n");
	printf("Problem solved in %lf s, %lf per timestep with %i particles\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_steps, n_particles);
	time = get_wall_seconds();

	write_output(data_arr, acc_arr, n_particles, "result.gal");
	printf("Output written in %lf s\n", get_wall_seconds()-time);

	free(data_arr);
	free(acc_arr);
	printf("Program finished in %lf s. Exiting...\n", get_wall_seconds()-start);

	return 0;
}

void acceleration(double* data_array, double* acc_array, int n){
	double x_i, y_i, x_j, y_j,m_i, m_j, r_ij;
	double G = 100/(double)n;
	double eps = 0.001;
	int i, j;
	double denom;

	for(i = 0; i < 6*n; i +=6){
		// Assign current values to mitigate carry over from previous timestep. Faster than memset
		acc_array[2*(i/6)] = 0;
		acc_array[2*(i/6)+1] = 0;

		// Fetch j-loop invariant values
		x_i = data_array[i];
		y_i = data_array[i+1];
		m_i = data_array[i+2];

		for(j = 0; j < i; j+=6){
			x_j = data_array[j];
			y_j = data_array[j+1];
			m_j = data_array[j+2];

			r_ij = sqrt((x_i-x_j)*(x_i-x_j)+(y_i-y_j)*(y_i-y_j));
			denom = G/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));

			// Update x and y components of the acceleration
			acc_array[2*(i/6)] += -m_j*(x_i-x_j)*denom;
			acc_array[2*(i/6)+1] += -m_j*(y_i-y_j)*denom;
			acc_array[2*(j/6)] += m_i*(x_i-x_j)*denom;
			acc_array[2*(j/6)+1] += m_i*(y_i-y_j)*denom;
		}
	}
}


void solver(double* data_array, double* acc_array, double dt, int n){
	int i;
	// Symplectic Euler for the n particles
	for(i = 0; i < 6*n; i+=6){
		// Velocity update
		data_array[i+3] = data_array[i+3] + dt*acc_array[2*(i/6)];
		data_array[i+4] = data_array[i+4] + dt*acc_array[2*(i/6)+1];

		// Position update
		data_array[i] = data_array[i] + dt*data_array[i+3];
		data_array[i+1] = data_array[i+1] + dt*data_array[i+4];
	}
}



double* read_input(char* path, int n){
	FILE* file = fopen(path, "rb");
	if (file == NULL){
		printf("ERROR: File path not valid\n");
		return NULL;
	}
	// Each particle has 6 attributes --> [pos_x, pos_y, m, v_x, v_y, b, ...]

	double* input_data = (double*)malloc(6*n*sizeof(double));
	if(fread(input_data, sizeof(double), 6*n, file) != 6*n){
		printf("ERROR: Mismatch between specified number of particles and read number of particles\n");
		return NULL;
	}
	fclose(file);
	return input_data;

}

void write_output(double* data_array, double* acc_array, int n, char* path){
	FILE* file = fopen(path, "wb");

	if(fwrite(data_array, sizeof(double) ,6*n, file) != 6*n){
		printf("ERROR: Mismatch between specified number of particles and written number of particles\n");
		return;
	}
	fclose(file);
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
