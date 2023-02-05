#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <string.h>
double* read_input(const char* restrict path, const int n);
void acceleration(double* restrict data_array, const int n);
void solver(double* restrict data_array, double* restrict acc_array, const double dt, const int n);
void print_array(double* restrict array, const int n, const char* restrict title);
void write_output(double* restrict data_array, double* restrict acc_array, const int n, const char* restrict path);
double* p_x, p_y, mass, v_x, v_y, brightness;
double get_wall_seconds();



int main(int argc, char* argv[]){
	double time = get_wall_seconds();
	double start = time;
	if(argc != 6){
		printf("Not enough arguments. Input arguments: 'N', 'filename', 'nsteps', 'delta_t', 'graphics'\n");
		return -1;
	}
	const int n_particles = atoi(argv[1]);
	const char* input_path = argv[2];
	const int n_steps = atoi(argv[3]);
	const double dt = atof(argv[4]);
	const char graphics = *argv[5];

	double* acc_array = (double*)malloc(n_particles*2*8);
	printf("Initialized in %lf s\n",get_wall_seconds()-time);
	time = get_wall_seconds();

	double* data_arr = read_input(input_path, n_particles);
	printf("Input read in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();

	for(int i = 0; i < n_steps; i++){
		solver(data_arr, acc_array, dt, n_particles);
//		printf("\rSolving timestep %i, average time %lf",i,(get_wall_seconds()-time)/(i+1));
//		fflush(stdout);
	}
	printf("\n");
	printf("Problem solved in %lf s, %lf per timestep with %i particles\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_steps, n_particles);
	time = get_wall_seconds();

	write_output(data_arr, acc_array, n_particles, "output.gal");
	printf("Output written in %lf s\n", get_wall_seconds()-time);
	free(data_arr);
	free(acc_array);
	printf("Program finished in %lf s. Exiting...\n", get_wall_seconds()-start);
	return 0;
}

void acceleration(double* restrict data_array, const int n){
	double sum_x = 0;
	double sum_y = 0;
	double x_i, y_i, x_j, y_j,m_i, m_j, r_ij;
	const double G = 100/(double)n;
	const double eps = 0.001;

	int i, j;
	for(i = 0; i < n*8; i +=8){
//		printf("A # %i\n", i/8);
		x_i = data_array[i];
		y_i = data_array[i+1];
		m_i = data_array[i+2];
		for(j = 0; j < n*8; j+=8){
			if(i==j) continue;
			x_j = data_array[j];
			y_j = data_array[j+1];
			m_j = data_array[j+2];
			r_ij = sqrt((x_i-x_j)*(x_i-x_j)+(y_i-y_j)*(y_i-y_j));
//			double tmp_x = (x_i-x_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));

			sum_x += m_j*(x_i-x_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
			sum_y += m_j*(y_i-y_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
//			printf("%lf\t", -m_i*tmp_x);
		}
		data_array[i+6] = -G*sum_x;
		data_array[i+7] = -G*sum_y;
//		printf("%lf\n%lf\n", sum_x, sum_y);
		sum_x=0;
		sum_y=0;
//		printf("\n");
//		printf("%lf\t%lf\n", data_array[i+6], data_array[i+7]);
	}

}


void acceleration2(double* restrict data_array, double* restrict acc_array, const int n){
	double x_i, y_i, x_j, y_j,m_i, m_j, r_ij;
	const double G = 100/(double)n;
	const double eps = 0.001;
	int i, j;
	double tmp_x, tmp_y;
	memset(acc_array, 0, 2*n*8);


	for(i = 0; i < n*6; i +=6){
		x_i = data_array[i];
		y_i = data_array[i+1];
		m_i = data_array[i+2];


		for(j = 0; j < i; j+=6){
			x_j = data_array[j];
			y_j = data_array[j+1];
			m_j = data_array[j+2];
			r_ij = sqrt((x_i-x_j)*(x_i-x_j)+(y_i-y_j)*(y_i-y_j));
			tmp_x = (x_i-x_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));
			tmp_y = (y_i-y_j)/((r_ij+eps)*(r_ij+eps)*(r_ij+eps));

			acc_array[2*(i/6)] += -G*m_j*tmp_x;
			acc_array[2*(i/6)+1] += -G*m_j*tmp_y;
			acc_array[2*(j/6)] += G*m_i*tmp_x;
			acc_array[2*(j/6)+1] += G*m_i*tmp_y;
		}
	}
}

 
void solver(double* restrict data_array, double* restrict acc_array, const double dt, const int n){
	double time = get_wall_seconds();
	acceleration2(data_array, acc_array, n);
//	printf("Calculated acceleration2 in %lf s\n",get_wall_seconds()-time); 

	int i;
	for(i =0; i < n*6; i+=6){
		
		data_array[i+3] = data_array[i+3] + dt*acc_array[2*(i/6)];
		data_array[i+4] = data_array[i+4] + dt*acc_array[2*(i/6)+1];
		data_array[i] = data_array[i] + dt*data_array[i+3];
		data_array[i+1] = data_array[i+1] + dt*data_array[i+4];
	}
}



double* read_input(const char* restrict path, const int n){
	FILE* file = fopen(path, "rb");
	if (file == NULL){
		printf("ERROR: File path not valid\n");
		return NULL;
	}

	double* input_data = (double*)malloc(n*48);
	fread(input_data, 8, 6*n, file);
	fclose(file);
	return input_data;

}


void print_array(double* restrict array, const int n, const char* restrict title){
	printf("Printing %s\n", title);
	for(int i=0; i < n; i++)
		printf("%lf\n", array[i]);
}

void print_matrix(double* array, const int n, const int m){
	for(int i =0; i<n; i++){
		for(int j = 0; j<m; j++){
			if(!j) printf("\n");
			printf("%lf\t", array[i*n+j]);
			}
		}
}

void write_output(double* restrict data_array, double* restrict acc_array, const int n, const char* restrict path){
	
	FILE* file = fopen(path, "wb");
	fwrite(data_array, 8 ,6*n, file);
	fclose(file);
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
