#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

typedef struct dimensions {
	const unsigned int x;
	const unsigned int y;
	const unsigned int z;
} dim_t;

typedef unsigned int uint_t;
typedef unsigned char uchar_t;


uint_t** generateCoordinateArrays(const uint_t x, const uint_t y, const uint_t z);
void freeVolume(uchar_t*** restrict volume, const dim_t size);
void freeCoordinateArrays(uint_t** restrict arr);
void resizeCoordinateArrays(uint_t** arr, uint_t value);
void printVolume(uchar_t*** restrict volume, dim_t size, uchar_t hide_wrapping, uchar_t hide_count);
uint_t countAlive(uchar_t*** restrict volume, const dim_t size);
uchar_t*** wrappedVolume(const dim_t size);
void copyPadding(uchar_t*** restrict vol, const dim_t size);
void processNeighbours(uint_t*** restrict s_alive, const dim_t size,  uchar_t*** restrict test_byte, uint_t*** s_updates, uchar_t**** restrict partition, const uchar_t n_threads, uint_t* partition_break);
uint_t cellLoop(uchar_t*** restrict vol, uint_t*** restrict s_alive, uint_t*** restrict s_updates, const uchar_t n_threads, uint_t* partition_break);
void partitions(uchar_t**** restrict partitions, uint_t*** restrict s_alive,  const dim_t size, uchar_t*** restrict vol, const uchar_t n_threads, uint_t* restrict partitions_break, uint_t n_alive);
uchar_t*** partialWrappedVolume(uchar_t*** restrict vol, const dim_t size, uint_t z_start, uint_t z_end);
double get_wall_seconds();




int main(int argc, char* argv[]){

	if(argc != 6){
		printf("ERROR: Not enough arguments\n");
		printf("\tProgram uses following format\n");
		printf("\tSIZE_X SIZE_Y SIZE_Z N_TIMESTEPS N_THREADS\n");
		return -1;
	} 


	//  SIZEX SIZEY SIZEZ TIMESTEPS THREADS
	uint_t size_x = atoi(argv[1]) + 2;
	uint_t size_y = atoi(argv[2]) + 2;
	uint_t size_z = atoi(argv[3]) + 2;
	uint_t timesteps = atoi(argv[4]);
	uchar_t n_threads = atoi(argv[5]);

	if(n_threads > size_z - 2){
		printf("ERROR: Risk of inefficency with more threads than z-dimension\n");
		return -1;
	}

	const dim_t wrapped_dim = {size_x, size_y, size_z};

	uchar_t*** wrapped = wrappedVolume(wrapped_dim);

	uint_t n_alive = countAlive(wrapped,wrapped_dim);
	if(!n_alive){
		printf("ERROR: No alive cells found when initializing\n");
		return -1;

	}

	uint_t*** s_updates = (uint_t***)malloc(n_threads*sizeof(uint_t**));
	uint_t*** s_alive = (uint_t***)malloc(n_threads*sizeof(uint_t**));


	for(uint_t i = 0; i < n_threads; i++){
		s_updates[i] = generateCoordinateArrays(27*n_alive/n_threads, 1, 1);
		s_alive[i] = generateCoordinateArrays(n_alive, 1, 1);
	}


	uchar_t**** parts = (uchar_t****)malloc(n_threads*sizeof(uchar_t***));
	uint_t* partition_break = (uint_t*)malloc(n_threads*sizeof(uint_t));
//	printf("Setup complete\n\n");

	double total_time = get_wall_seconds();
	double p_time = 0;
	double pn_time = 0;
	double cl_time = 0; 
	double f_time = 0;
	for(uint_t t = 0; t < timesteps; t++){
		double gen_start_time = get_wall_seconds();
		double time = gen_start_time;
		partitions(parts, s_alive, wrapped_dim, wrapped, n_threads, partition_break, n_alive);
		
		p_time += get_wall_seconds() -time;
		time = get_wall_seconds();
		processNeighbours(s_alive, wrapped_dim, wrapped, s_updates, parts, n_threads, partition_break);
		pn_time += get_wall_seconds() -time;
		time = get_wall_seconds();

		uint_t n_alive_approx = cellLoop(wrapped, s_alive, s_updates, n_threads, partition_break);

		cl_time += get_wall_seconds() -time;
		time = get_wall_seconds();

		copyPadding(wrapped, wrapped_dim);

		n_alive = n_alive_approx*1.1;

		for(uint_t i = 0; i < n_threads; i++){
			const dim_t temp_dim ={size_x, size_y, partition_break[i]};
			freeVolume(parts[i], temp_dim);

		}

		if(!n_alive){
			printf("EARLY EXIT: All cells are dead\n");
			return 0;
		}
//		printf("Generation done in %lf s with %i alive \n", get_wall_seconds()-gen_start_time, n_alive);
		f_time += get_wall_seconds() - time;
		time = get_wall_seconds();
	}
	double tot_time = get_wall_seconds()-total_time;
//	printf("All %i timesteps completed in %lf s left with %i alive \n",timesteps, tot_time, n_alive);


	printf("%i %i %i %i %lf %lf %lf %lf %lf\n", n_threads, size_x, size_y, size_z, tot_time, p_time, pn_time, cl_time, f_time);
	freeVolume(wrapped,wrapped_dim);
	for(uint_t i = 0; i < n_threads; i++){
		freeCoordinateArrays(s_updates[i]);
		freeCoordinateArrays(s_alive[i]);

	}
	free(s_updates);
	free(s_alive);
	free(parts);
	free(partition_break);
	return 0;
}


uint_t cellLoop(uchar_t*** restrict vol, uint_t*** restrict s_alive, uint_t*** restrict s_updates, const uchar_t n_threads, uint_t* partition_break){
//		double time = get_wall_seconds();
		uint_t cum_z[n_threads];
		uint_t z_offset = 0;
		for(int i = 0; i < n_threads; i++){
			z_offset += i==0 ? 0 : partition_break[i-1]-2;
			cum_z[i] = z_offset;
		}
		uint_t n_alive = 0;
		uint_t alive_count = 0;

		#pragma omp parallel num_threads(n_threads) firstprivate(alive_count) reduction(+:n_alive)
		{
		uint_t r, i, j, k, offset_i;
		uchar_t cell, count;
		uint_t thread_ID = omp_get_thread_num();
		uint_t** s_updated = s_updates[thread_ID];
		uint_t** s_living = s_alive[thread_ID];
		uint_t limit = s_updated[0][0];
		uint_t offset = cum_z[thread_ID];
		for(r = 0; r < limit; r++){

			i = s_updated[1][r];
			j = s_updated[2][r];
			k = s_updated[3][r];
			offset_i = i + offset;

			cell = vol[offset_i][j][k];
			if(cell&1){
				count = ((cell & 62) >> 1)-1;

				if(count < 5 || count > 7){
					vol[offset_i][j][k] = 0;
				} else {
					s_living[1][alive_count] = i;
					s_living[2][alive_count] = j;
					s_living[3][alive_count] = k;
					alive_count++;
				}


			} else {

				if(((cell & 62) >> 1)==6){
					s_living[1][alive_count] = i;
					s_living[2][alive_count] = j;
					s_living[3][alive_count] = k;
					alive_count++;
					vol[offset_i][j][k] = 1;
				}
			}
			vol[offset_i][j][k] &= ~62;


		}

		s_living[0][0] = alive_count;
		n_alive += alive_count;
//		printf("Alive %i\n", alive_count);
		}

//		printf("Looping done in %lf s, %.16lf per cell\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_alive);
		return n_alive;
}


uint_t countAlive(uchar_t*** restrict volume, const dim_t size){
//	double time = get_wall_seconds();
	const uint_t x = size.x;
	const uint_t y = size.y;
	const uint_t z = size.z;

	uint_t counter  = 0;
	for(uint_t i = 0; i < z; i++)
                for(uint_t j = 0; j < y; j++)
                        for(uint_t k = 0; k < x; k++)
				counter += volume[i][j][k];
//	printf("Counted alive in %lf s\n", get_wall_seconds()-time);
	return counter;
}



// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
void processNeighbours(uint_t*** restrict s_alive,  const dim_t size, uchar_t*** restrict vol, uint_t*** restrict s_updates, uchar_t**** restrict partition, const uchar_t n_threads, uint_t* restrict partition_break){
//	double time = get_wall_seconds();
	const uint_t x = size.x;
	const uint_t y = size.y;
	#pragma omp parallel num_threads(n_threads)
	{
	const uint_t thread_ID = omp_get_thread_num();
	uint_t** s_updated = s_updates[thread_ID];
	uint_t** alive = s_alive[thread_ID];
	uchar_t*** part = partition[thread_ID];
	char l_l,l,l_u,m_l,m,m_u,n_l,n,n_u;
	const uint_t* z_axis = alive[1];
	const uint_t* y_axis = alive[2];
	const uint_t* x_axis = alive[3];
	uint_t z_coord, y_coord, x_coord;
	uint_t counter = 0;
	uint_t limit = alive[0][0];
	uint_t z = partition_break[thread_ID];

	for(uint_t i=0; i < limit; i++){
		z_coord = z_axis[i];
		y_coord = y_axis[i];
		x_coord = x_axis[i];

		l_u = 2;
		l_l = -1;
		m_u = 2;
		m_l = -1;
		n_u = 2;
		n_l = -1;

		if(!z_coord){
			l_l = 0;
		} else if (z_coord == z-1){
			l_u = 1;
		}
		if(!y_coord){
			m_l = 0;
		} else if (y_coord == y-1){
			m_u = 1;
		}
		if(!x_coord){
			n_l = 0;
		} else if (x_coord == x-1){
			n_u = 1;
		}

		for(l = l_l; l < l_u; l++){
                        for(m = m_l; m < m_u; m++){
                                for(n = n_l; n < n_u; n++){

					if(!(part[z_coord+l][y_coord+m][x_coord+n]&62)){
						s_updated[1][counter] = z_coord + l;
						s_updated[2][counter] = y_coord + m;
						s_updated[3][counter] = x_coord + n;
						counter++;
					}

					part[z_coord+l][y_coord+m][x_coord+n] += 2;

				}
			}
		}

	}
	s_updated[0][0] = counter;

	}

//	double serial_time = get_wall_seconds();
	uint_t z_offset = 0;
	uint_t total_updates = 0;
	for(uint_t i = 0; i < n_threads; i++){

		uint_t s_counter = 0;
		z_offset += i==0 ? 0 : partition_break[i-1]-2;
		uint_t* z_axis = s_updates[i][1];
		uint_t* y_axis = s_updates[i][2];
		uint_t* x_axis = s_updates[i][3];
		uchar_t*** part = partition[i];

		for(uint_t j = 0; j < s_updates[i][0][0]; j++){
			uint_t z_coord = z_axis[j];
			uint_t y_coord = y_axis[j];
			uint_t x_coord = x_axis[j];


			if(!(vol[z_coord + z_offset][y_coord][x_coord] & 62)){
				z_axis[s_counter] = z_coord;
                                y_axis[s_counter] = y_coord;
                                x_axis[s_counter] = x_coord;
                                s_counter++;

			}

			vol[z_coord + z_offset][y_coord][x_coord] += (part[z_coord][y_coord][x_coord] & 62);


		}
		s_updates[i][0][0] = s_counter;
		total_updates += s_updates[i][0][0];
	}
//	printf("Found neighbours in %lf s, serial %lf\n", get_wall_seconds()-time, get_wall_seconds()-serial_time);
	return;

}

void partitions(uchar_t**** restrict partitions, uint_t*** restrict s_alive,  const dim_t size, uchar_t*** restrict vol, const uchar_t n_threads, uint_t* restrict partitions_break, uint_t n_alive){
//	double time = get_wall_seconds();
	uint_t section = size.z / n_threads;

	#pragma omp parallel num_threads(n_threads)
	{
	uint_t alive_counter = 0;
	const uint_t thread_ID = omp_get_thread_num();

	const uint_t z_start = (thread_ID==0)? 0 : thread_ID*section+1;
	const uint_t offset = (thread_ID==0)?0:1;
	const uint_t limit = (thread_ID == (n_threads -1)) ? size.z : section*(thread_ID+1)+1;
	for(uint_t i = z_start; i < limit; i++){

		for(uint_t j = 0; j < size.y; j++){
			for(uint_t k = 0; k < size.x; k++){

				if(vol[i][j][k]&1){
					s_alive[thread_ID][1][alive_counter] = i - z_start + offset;
					s_alive[thread_ID][2][alive_counter] = j;
					s_alive[thread_ID][3][alive_counter] = k;
					alive_counter++;
				}
			}
		}


	}
		uint_t z_s = thread_ID == 0 ? 0 : z_start-1;
		uint_t z_e = (limit == (size.z)) ? size.z-1 : limit;
		partitions_break[thread_ID] = z_e - z_s + 1;
		partitions[thread_ID] = partialWrappedVolume(vol, size, z_s, z_e);
		s_alive[thread_ID][0][0] = alive_counter;





	}
//	printf("Partitioned volume in %lf s\n", get_wall_seconds()-time);

	return;

}


uchar_t*** partialWrappedVolume(uchar_t*** restrict vol, const dim_t size, uint_t z_start, uint_t z_end){
	const uint_t x = size.x;
	const uint_t y = size.y;
	const uint_t z = z_end - z_start + 1;
        uchar_t*** z_slices = (uchar_t***)malloc(z*sizeof(uchar_t**));
	long int x_byte_size = x*sizeof(uchar_t);
	long int y_byte_size = y*sizeof(uchar_t*);
        for(uint_t i = z_start; i <= z_end; i++){
		z_slices[i-z_start] = (uchar_t**)malloc(y_byte_size);
                for(uint_t j = 0; j <y; j++){
			z_slices[i - z_start][j] = (uchar_t*)malloc(x_byte_size);
                        memcpy(z_slices[i - z_start][j], vol[i][j], x_byte_size);
                }
        }
        return z_slices;
}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

uchar_t*** wrappedVolume(const dim_t size){
//        double time = get_wall_seconds();
	srand(0);
	const uint_t x = size.x;
	const uint_t y = size.y;
	const uint_t z = size.z;
	long int x_byte_size = x*sizeof(uchar_t);
	long int y_byte_size = y*sizeof(uchar_t*);
        uchar_t*** z_slices = (uchar_t***)malloc(z*sizeof(uchar_t**));
        for(uint_t i = 0; i < z; i++){
                z_slices[i] = (uchar_t**)malloc(y_byte_size);
                uchar_t** y_slice = z_slices[i];
                for(uint_t j = 0; j <y; j++){
                        y_slice[j] = (uchar_t*)malloc(x_byte_size);
                        uchar_t* x_slice = y_slice[j];
                        for(uint_t k =0; k < x; k++){
                                x_slice[k] = (uchar_t)round(0.75*(double)rand()/RAND_MAX);
                        }
			if(i == z-1){ 
				memcpy(z_slices[i][j], z_slices[1][j], x_byte_size);
				memcpy(z_slices[0][j], z_slices[z-2][j], x_byte_size);
			}
			z_slices[i][j][0] = z_slices[i][j][x-2];
                        z_slices[i][j][x-1] = z_slices[i][j][1];
                }
		memcpy(z_slices[i][0], z_slices[i][y-2], x_byte_size);
		memcpy(z_slices[i][y-1], z_slices[i][1], x_byte_size);
        }
//	printf("Generated wrapped volume in %lf s of size %li B\n", get_wall_seconds()-time, x*y*z*sizeof(uchar_t)+y*z*sizeof(uchar_t*)+z*sizeof(uchar_t**));
        return z_slices;


}

void copyPadding(uchar_t*** restrict vol, const dim_t size){
//        double time = get_wall_seconds();
	const uint_t x = size.x;
	const uint_t y = size.y;
	const uint_t z = size.z;
	long int x_byte_size = x*sizeof(uchar_t);
        for(uint_t i = 0; i < z; i++){
                for(uint_t j = 0; j <y; j++){
			if(i == z-1){
				memcpy(vol[i][j], vol[1][j], x_byte_size);
				memcpy(vol[0][j], vol[z-2][j], x_byte_size);
			}
			vol[i][j][0] = vol[i][j][x-2];
                        vol[i][j][x-1] = vol[i][j][1];
                }
		memcpy(vol[i][0], vol[i][y-2], x_byte_size);
		memcpy(vol[i][y-1], vol[i][1], x_byte_size);
        }
//	printf("Copied edges of volume in %lf s\n", get_wall_seconds()-time);





}


void resizeCoordinateArrays(uint_t** restrict arr, uint_t value){
	uint_t new_size = (1.2*value)*sizeof(uint_t);
	arr[1] = realloc(arr[1], new_size);
	arr[2] = realloc(arr[2], new_size);
	arr[3] = realloc(arr[3], new_size);
	
}

uint_t** generateCoordinateArrays(const uint_t x, const uint_t y, const uint_t z){
	uint_t** arr = (uint_t**)malloc(sizeof(uint_t*)*4);
	unsigned long int size = sizeof(uint_t)*x*y*z;
	arr[0] = (uint_t*)malloc(sizeof(uint_t));
	arr[1] = (uint_t*)malloc(size);
	arr[2] = (uint_t*)malloc(size);
	arr[3] = (uint_t*)malloc(size);
//	printf("Allocated %li B for coordinate arrays\n", (x*y*z)*sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int*)*4);
	return arr;
}


void freeCoordinateArrays(uint_t** restrict arr){
	free(arr[0]);
	free(arr[1]);
	free(arr[2]);
	free(arr[3]);
	free(arr);
	return;
}


void freeVolume(uchar_t*** restrict volume, const dim_t size){
	const uint_t y = size.y;
	const uint_t z = size.z;
	for(uint_t i = 0; i < z; i++){
		for(uint_t j = 0; j < y; j++){
				free(volume[i][j]);
		}
		free(volume[i]);
	}
	free(volume);
	return;
}

void printVolume(uchar_t*** restrict volume, const dim_t size, uchar_t hide_wrapping, uchar_t hide_count){
//	double time = get_wall_seconds();
	const uint_t x = size.x;
	const uint_t y = size.y;
	const uint_t z = size.z;
	char offset = 0;
	if (hide_wrapping) offset = -1;
	for(uint_t i = hide_wrapping; i < z + offset; i++){
		for(uint_t j = hide_wrapping; j < y + offset; j++){
			for(uint_t k = hide_wrapping; k < x+offset; k++){
				printf("%i ", volume[i][j][k]&1);
                                if(k%(x+offset) == (x-1+offset) && !hide_count) printf("\t");
                                if(k%(x+offset) == (x-1+offset) && hide_count) printf("\n");
			}
			if(!hide_count){
				for(uint_t k = hide_wrapping; k < x+offset; k++){
					printf("%i ", (volume[i][j][k]&62) >> 1);
	                                if(k%(x+offset) == (x-1+offset)) printf("\n");
				}
			}
		}
		printf("\n");
	}
//	printf("Printed volume in %lf s\n", get_wall_seconds()-time);
}

