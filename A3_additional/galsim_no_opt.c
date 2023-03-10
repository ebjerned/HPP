#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
double* read_input(char* path);
void acceleration(double* data_array, int n);
void solver(double* data_array, double dt, int n);
void print_array(double* arr, int n, char* title);
void write_output(double* data_array, int n, char* path);
double* p_x, p_y, mass, v_x, v_y, brightness;
double get_wall_seconds();



int main(int argc, char* argv[]){
	double time = get_wall_seconds();
	if(argc != 6){
		printf("Not enough arguments. Input arguments: 'N', 'filename', 'nsteps', 'delta_t', 'graphics'\n");
		return -1;
	}
	int n_particles = atoi(argv[1]);
	char* input_path = argv[2];
	int n_steps = atoi(argv[3]);
	double dt = atof(argv[4]);
	char graphics = *argv[5];
	printf("Initialized in %lf s\n",get_wall_seconds()-time);
	time = get_wall_seconds();
	double* data_arr = read_input(input_path);
	printf("Input read in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();
	for(int i = 0; i < n_steps; i++)
		solver(data_arr, dt, n_particles);
	printf("Problem solved in %lf s, %lf per timestep with %i particles\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_steps, n_particles);
	write_output(data_arr, n_particles, "output.gal");

	free(data_arr);

	return 0;
}

void acceleration(double* data_array, int n){
	double sum_x = 0;
	double sum_y = 0;
	double x_i, y_i, x_j, y_j, m_j, r_ij;
	double G = (double)100/(double)n;
	double eps = 0.001;

	for(int i = 0; i < n*8; i +=8){
//		printf("A # %i\n", i/8);
		x_i = data_array[i];
		y_i = data_array[i+1];
		for(int j =0; j < n*8; j+=8){
			if(i==j) continue;
			x_j = data_array[j];
			y_j = data_array[j+1];
			m_j = data_array[j+2];
			r_ij = sqrt((x_i-x_j)*(x_i-x_j)+(y_i-y_j)*(y_i-y_j));
			sum_x += m_j*(x_i-x_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
			sum_y += m_j*(y_i-y_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
		}
		data_array[i+6] = -G*sum_x;
		data_array[i+7] = -G*sum_y;
//		printf("%lf\t%lf\n", data_array[i+6], data_array[i+7]);
		sum_x = 0;
		sum_y = 0;
	}

}


void solver(double* data_array, double dt, int n){
	acceleration(data_array, n);
	for(int i =0; i < n*8; i+=8){

		data_array[i+3] = data_array[i+3] + dt*data_array[i+6];
		data_array[i+4] = data_array[i+4] + dt*data_array[i+7];
		data_array[i] = data_array[i] + dt*data_array[i+3];
		data_array[i+1] = data_array[i+1] + dt*data_array[i+4];

	}
//	print_array(data_array, n*8, "Next timestep");


}



double* read_input(char* path){
	FILE* file = fopen(path, "rb");
	if (file == NULL){
		printf("ERROR: File path not valid\n");
		return NULL;
	}
	fseek(file, 0, SEEK_END);
	long int file_size = ftell(file);
	int n = file_size/(6*8);
	printf("Number of particles: %i in file %s\n", n, path);
	fseek(file, 0, SEEK_SET);
	double* input_data = (double*)malloc(file_size*8/6);
	for(int i = 0; i < n; i++){
		fread(&(input_data[i*8]), 8, 6, file);

	}
	//FOR DEBUGGING
//	print_array(input_data, n*8, "Input Data");

	fclose(file);
	return input_data;

}


void print_array(double* arr, int n, char* title){
	printf("Printing %s\n", title);
	for(int i=0; i < n; i++)
		printf("%lf\n",arr[i]);
}

void write_output(double* data_array, int n, char* path){
	
	FILE* file = fopen(path, "wb");
	fseek(file,0, SEEK_END);
	for(int i=0; i<n; i++)
		fwrite(&(data_array[i*8]), 8 ,6, file);
	fclose(file);
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
