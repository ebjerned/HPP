#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

double* read_input(char* path, int n);
void* acceleration(void* arg);
void acceleration_joiner(double* data_array, double* acc_array, int n, int n_threads);
void solver(double* data_array, double* acc_array, double dt, int n);
void write_output(double* data_array, double* acc_array, int n, char* path);
double get_wall_seconds();

typedef struct data_struct{
	double* data_array;
	double* acc_array;
	int s;
	int n;
	int offset;
} data_t;


int main(int argc, char* argv[]){
	double time = get_wall_seconds();
	double start = time;

	if(argc != 7){
		printf("Not enough arguments. Input arguments: 'N', 'filename', 'nsteps', 'delta_t', 'graphics', 'n_threads'\n");
		return -1;
	}


	int n_particles = atoi(argv[1]);
	char* input_path = argv[2];
	int n_steps = atoi(argv[3]);
	double dt = atof(argv[4]);
//	char graphics = *argv[5];
	int n_threads = atoi(argv[6]);
	double* acc_arr = (double*)malloc(n_particles*2*sizeof(double));

//	printf("Initialized in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	double* data_arr = read_input(input_path, n_particles);

	if (data_arr == NULL) return -1;

//	printf("Input read in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	for(int i = 0; i < n_steps; i++){
		acceleration_joiner(data_arr, acc_arr, n_particles, n_threads);
//		printf("Acceleration computed in %lf s\n", get_wall_seconds()-time);
//		time = get_wall_seconds();
		solver(data_arr, acc_arr, dt, n_particles);
//		printf("Solved in %lf s\n", get_wall_seconds()-time);
	}

//	printf("\n");
//	printf("Problem solved in %lf s, %lf per timestep with %i particles\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_steps, n_particles);
	time = get_wall_seconds();

	write_output(data_arr, acc_arr, n_particles, "result.gal");
//	printf("Output written in %lf s\n", get_wall_seconds()-time);

	free(data_arr);
	free(acc_arr);
//	printf("Program finished in %lf s. Exiting...\n", get_wall_seconds()-start);
	printf("%i\t%lf\n", n_threads, get_wall_seconds()-start);
	return 0;
}

void acceleration_joiner(double* data_array, double* acc_array, int n, int n_threads){
	int i,j;
	pthread_t threads[n_threads];
	double* sub_acc_array[n_threads];
	data_t* argument_data = (data_t*)malloc(sizeof(data_t)*n_threads);

	for(i = 0; i < n_threads; i++){
		sub_acc_array[i] = (double*) malloc(sizeof(double)*n*2);
		memset(sub_acc_array[i], 0, 2*n*8);

		argument_data[i].data_array = data_array;
		argument_data[i].acc_array = sub_acc_array[i];

		argument_data[i].s = n_threads;
		argument_data[i].n = n;
		argument_data[i].offset = i;
		pthread_create(&threads[i], NULL, acceleration, (void*) &argument_data[i]);
	}

	for(i=0; i < n_threads; i++){
		pthread_join(threads[i], NULL);
//		printf("Thread %i acceleretion:\n", i);

		for(j = 0; j < n*2; j++){
//			printf("%lf\n", argument_data[i].acc_array[j]);
			if (i == 0){
				acc_array[j] = argument_data[i].acc_array[j];
			}else{
				acc_array[j] += argument_data[i].acc_array[j];

			}

		}
		free(argument_data[i].acc_array);
	}
	free(argument_data);


}

void* acceleration(void* arg){
	data_t data_struct = *(data_t*) arg;
	double* data_array = (double*)data_struct.data_array;
	double* acc_array = (double*)data_struct.acc_array;
	int s = (int)data_struct.s;
	int n = (int)data_struct.n;
	int offset = (int)data_struct.offset;
	double x_i, y_i, x_j, y_j,m_i, m_j, r_ij;
	double G = 100/(double)n;
	double eps = 0.001;
	int i, j;
	double denom;

	for(i = 6*offset; i < 6*n; i +=6*s){
		// Assign current values to mitigate carry over from previous timestep. Faster than memset
//		printf("Acceleration at particle %i by thread %i \n", i/6, offset);
//		acc_array[2*(i/6)] = 0;
//		acc_array[2*(i/6)+1] = 0;

		// Fetch j-loop invariant values
		x_i = data_array[i];
		y_i = data_array[i+1];
		m_i = data_array[i+2];

		for(j = 0; j < i; j+=6){
//			printf("\tAcceleration at particle %i by thread %i from particle %i\n", i/6,offset, j/6);
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
//			printf("%lf\t%lf\n", acc_array[2*(i/6)],acc_array[2*(i/6)+1]);
		}
	}
	return NULL;
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
//	for(int i = 0; i < 2*n; i++)
//		printf("%lf\n", acc_array[i]);
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
