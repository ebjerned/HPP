#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>

double** read_input(char* path, int n);
void acceleration(double* xpos, double* ypos, double* m, double* xvel, double* yvel, double* acc_arrayx, double* acc_arrayy, int n);
void solver(double* xpos, double* ypos, double* m, double* xvel, double* yvel, double* acc_arrayx, double* acc_arrayy, double dt, int n);
void write_output(double** input_data, int n, char* path);
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
	double* acc_arrx = (double*)malloc(n_particles*sizeof(double));
	double* acc_arry = (double*)malloc(n_particles*sizeof(double));

	printf("Initialized in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	double** data_arr = read_input(input_path, n_particles);

	if (data_arr == NULL) return -1;
	double* xpos = data_arr[0];
	double* ypos = data_arr[1];
	double* m = data_arr[2];
	double* xvel = data_arr[3];
	double* yvel = data_arr[4];

	printf("Input read in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();


	for(int i = 0; i < n_steps; i++){
		acceleration(xpos, ypos, m, xvel, yvel, acc_arrx, acc_arry, n_particles);
		solver(xpos, ypos, m, xvel, yvel, acc_arrx, acc_arry, dt, n_particles);
	}

	printf("\n");
	printf("Problem solved in %lf s, %lf per timestep with %i particles\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_steps, n_particles);
	time = get_wall_seconds();

	write_output(data_arr, n_particles, "result.gal");
	printf("Output written in %lf s\n", get_wall_seconds()-time);
	free(xpos);
	free(ypos);
	free(m);
	free(xvel);
	free(yvel);
	free(data_arr);
	free(acc_arrx);
	free(acc_arry);
	printf("Program finished in %lf s. Exiting...\n", get_wall_seconds()-start);

	return 0;
}

void acceleration(double* xpos, double* ypos, double* m, double* xvel, double* yvel, double* acc_arrayx, double* acc_arrayy, int n){
	double x_i, y_i, x_j, y_j,m_i, m_j, r_ij, distx, disty, denomx, denomy;
	double G = 100/(double)n;
	double eps = 0.001;
	int i, j;
	double denom;

	for(i = 0; i < n; i ++){
		// Assign current values to mitigate carry over from previous timestep. Faster than memset
		acc_arrayx[i] = 0;
		acc_arrayy[i] = 0;

		// Fetch j-loop invariant values
		x_i = xpos[i];
		y_i = ypos[i];
		m_i = m[i];

		for(j = 0; j < i; j++){
			x_j = xpos[j];
			y_j = ypos[j];
			m_j = m[j];
			distx = x_i-x_j;
			disty = y_i-y_j;
			r_ij = sqrt(distx*distx+disty*disty);
			denom = G/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
			denomx = distx*denom;
			denomy = disty*denom;

			// Update x and y components of the acceleration
			acc_arrayx[i] += -m_j*denomx;
			acc_arrayx[j] += m_i*denomx;
			acc_arrayy[i] += -m_j*denomy;
			acc_arrayy[j] += m_i*denomy;
		}
	}
}


void solver(double* xpos, double* ypos, double* m, double* xvel, double* yvel, double* acc_arrayx, double* acc_arrayy, double dt, int n){
	int i;
	// Symplectic Euler for the n particles
	for(i = 0; i < n; i++){
		// Velocity update
		xvel[i] = xvel[i] + dt*acc_arrayx[i];
		yvel[i] = yvel[i] + dt*acc_arrayy[i];
	}
	for(i = 0; i < n; i++){
		// Position update
		xpos[i] = xpos[i] + dt*xvel[i];
		ypos[i] = ypos[i] + dt*yvel[i];
	}
}



double** read_input(char* path, int n){
	FILE* file = fopen(path, "rb");
	if (file == NULL){
		printf("ERROR: File path not valid\n");
		return NULL;
	}
	// Each particle has 6 attributes --> [pos_x, pos_y, m, v_x, v_y, b, ...]

	double** input_data = (double**)malloc(6*sizeof(double*));
	double* xpos = (double*)malloc(n*sizeof(double));
	double* ypos = (double*)malloc(n*sizeof(double));
	double* m = (double*)malloc(n*sizeof(double));
	double* xvel = (double*)malloc(n*sizeof(double));
	double* yvel = (double*)malloc(n*sizeof(double));
	double* brig = (double*)malloc(n*sizeof(double));
/*	if(fread(input_data, sizeof(double), 6*n, file) != 6*n){
		printf("ERROR: Mismatch between specified number of particles and read number of particles\n");
		return NULL;
	}
	fclose(file);*/
	for(int i = 0; i < n; i++){
		fread(xpos, sizeof(double), 1, file);
		fread(ypos, sizeof(double), 1, file);
		fread(m, sizeof(double), 1, file);
		fread(xvel, sizeof(double), 1, file);
		fread(yvel, sizeof(double), 1, file);
		fread(brig, sizeof(double), 1, file);
	}
	fclose(file);
	input_data[0] = xpos;
	input_data[1] = ypos;
	input_data[2] = m;
	input_data[3] = xvel;
	input_data[4] = yvel;
	input_data[5] = brig;

	return input_data;

}

void write_output(double** input_data, int n, char* path){
	FILE* file = fopen(path, "wb");

	/*if(fwrite(data_array, sizeof(double) ,6*n, file) != 6*n){
		printf("ERROR: Mismatch between specified number of particles and written number of particles\n");
		return;
	}*/

	double*	xpos = input_data[0];
	double* ypos = input_data[1];
	double* m = input_data[2];
	double* xvel = input_data[3];
	double* yvel = input_data[4];
	double* brig = input_data[5];	
	for(int i = 0; i < n; i++){
		fwrite(&xpos[i], sizeof(double), 1, file);
		fwrite(&ypos[i], sizeof(double), 1, file);
		fwrite(&m[i], sizeof(double), 1, file);
		fwrite(&xvel[i], sizeof(double), 1, file);
		fwrite(&yvel[i], sizeof(double), 1, file);
		fwrite(&brig[i], sizeof(double), 1, file);
	}

	fclose(file);
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
