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


unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z);
void freeVolume(unsigned char*** restrict volume, const dim_t size);
void freeCoordinateArrays(unsigned int** restrict arr);
void resizeCoordinateArrays(unsigned int** arr, unsigned int value);
void printVolume(unsigned char*** restrict volume, dim_t size, char hide_wrapping, char hide_count);
unsigned int countAlive(unsigned char*** restrict volume, const dim_t size);
unsigned char*** wrappedVolume(const dim_t size);
void copyPadding(unsigned char*** restrict vol, const dim_t size);
void processNeighbours(unsigned int*** restrict s_alive, const dim_t size,  unsigned char*** restrict test_byte, unsigned int*** s_updates, unsigned int*** i_updates, unsigned char**** restrict partition, unsigned const char n_threads, unsigned int* partition_break);
unsigned int cellLoop(unsigned char*** restrict vol, unsigned int*** restrict s_alive, unsigned int*** restrict s_updates, unsigned const char n_threads, unsigned int* partition_break);
void partitions(unsigned char**** restrict partitions, unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned const char n_threads, unsigned int* partition_break, unsigned int n_alive);
unsigned char*** partialWrappedVolume(unsigned char*** restrict vol, const dim_t size, unsigned z_start, unsigned z_end);
double get_wall_seconds();




int main(int argc, char* argv[]){

	if(argc != 6){
		printf("ERROR: Not enough arguments\n");
		printf("\tProgram uses following format\n");
		printf("\tSIZE_X SIZE_Y SIZE_Z N_TIMESTEPS N_THREADS\n");
		return -1;
	} 


	//  SIZEX SIZEY SIZEZ TIMESTEPS THREADS
	unsigned int size_x = atoi(argv[1]) + 2;
	unsigned int size_y = atoi(argv[2]) + 2;
	unsigned int size_z = atoi(argv[3]) + 2;	
	unsigned int timesteps = atoi(argv[4]);
	unsigned char n_threads = atoi(argv[5]);
	
	

	const dim_t wrapped_dim = {size_x, size_y, size_z};

	unsigned char*** wrapped = wrappedVolume(wrapped_dim);

	unsigned int n_alive = countAlive(wrapped,wrapped_dim);
	if(!n_alive){
		printf("ERROR: No alive cells found when initializing\n");
		return -1;

	}

	unsigned int*** s_updates = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));	
	unsigned int*** i_updates = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));
	unsigned int*** s_alive = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));
	

	for(int i = 0; i < n_threads; i++){
		s_updates[i] = generateCoordinateArrays(27*n_alive/n_threads, 1, 1);
		i_updates[i] = generateCoordinateArrays(27*n_alive/n_threads, 1, 1);
		s_alive[i] = generateCoordinateArrays(n_alive, 1, 1);
	}


	unsigned char**** parts = (unsigned char****)malloc(n_threads*sizeof(unsigned char***));
	unsigned int* partition_break = (unsigned int*)malloc(n_threads*sizeof(int));
	printf("Setup complete\n\n");
	for(unsigned int t = 0; t < timesteps; t++){
		double gen_start_time = get_wall_seconds();
		for(int i = 0; i < n_threads; i++){
			resizeCoordinateArrays(s_alive[i], n_alive/n_threads);

		}
		partitions(parts, s_alive, wrapped_dim, wrapped, n_threads, partition_break, n_alive);
//		printVolume(wrapped, wrapped_dim, 1, 0);
		processNeighbours(s_alive, wrapped_dim, wrapped, s_updates, i_updates, parts, n_threads, partition_break);
//		printVolume(wrapped, wrapped_dim, 1, 0);

		unsigned int n_alive_approx = cellLoop(wrapped, s_alive, s_updates, n_threads, partition_break);
//		printVolume(wrapped, wrapped_dim, 1, 0);
		copyPadding(wrapped, wrapped_dim);
		//n_alive = countAlive(wrapped, wrapped_dim);
		//printVolume(wrapped, wrapped_dim, 0, 0);
		for(int i = 0; i < n_threads; i++){
			const dim_t temp_dim ={size_x, size_y, partition_break[i]};
			freeVolume(parts[i], temp_dim);
			resizeCoordinateArrays(s_updates[i], n_alive*27/n_threads);
			resizeCoordinateArrays(i_updates[i], n_alive*27/n_threads);
		}
		n_alive = n_alive_approx*1.1;
		if(!n_alive){
			printf("EARLY EXIT: All cells are dead");
			return 0;
		}
		printf("Generation done in %lf s with %i alive \n\n", get_wall_seconds()-gen_start_time, n_alive);
	}

	freeVolume(wrapped,wrapped_dim);
	for(int i = 0; i < n_threads; i++){
		freeCoordinateArrays(s_updates[i]);
		freeCoordinateArrays(i_updates[i]);
		freeCoordinateArrays(s_alive[i]);

	}
	free(s_updates);
	free(i_updates);
	free(s_alive);
	free(parts);
	free(partition_break);
	return 0;
}


unsigned int cellLoop(unsigned char*** restrict vol, unsigned int*** restrict s_alive, unsigned int*** restrict s_updates, unsigned const char n_threads, unsigned int* partition_break){
		double time = get_wall_seconds();
		unsigned int cum_z[n_threads];
		unsigned int z_offset = 0;
		for(int i = 0; i < n_threads; i++){
			z_offset += i==0 ? 0 : partition_break[i-1]-2;
			cum_z[i] = z_offset;
		}
		unsigned int n_alive = 0;
		int alive_count = 0;

		#pragma omp parallel num_threads(n_threads) firstprivate(alive_count) reduction(+:n_alive)
		{
		int r, i, j, k, offset_i;
		char cell, count;
		unsigned int thread_ID = omp_get_thread_num();
		unsigned int** s_updated = s_updates[thread_ID];
		unsigned int** s_living = s_alive[thread_ID];
		int limit = s_updated[0][0];
		unsigned int offset = cum_z[thread_ID];
		for(r = 0; r < limit; r++){

			i = s_updated[1][r];
			j = s_updated[2][r];
			k = s_updated[3][r];
			offset_i = i + offset;

			cell = vol[offset_i][j][k];
			if(cell&1){
				count = ((cell & 62) >> 1);

				if(count < 5 || count > 7){
					vol[offset_i][j][k] = 0;
				} else {
					s_living[1][alive_count] = i;
					s_living[2][alive_count] = j;
					s_living[3][alive_count] = k;
					alive_count++;
				}

			} else {
				count = (cell & 62) >> 1;

				if(count==6){
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
		}

		printf("Looping done in %lf s, %.16lf per cell\n", get_wall_seconds()-time, (get_wall_seconds()-time)/n_alive);
		return n_alive;
}


unsigned int countAlive(unsigned char*** restrict volume, const dim_t size){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;

	unsigned int counter  = 0;
	for(unsigned int i = 0; i < z; i++)
                for(unsigned int j = 0; j < y; j++)
                        for(unsigned int k = 0; k < x; k++)
				counter += volume[i][j][k];
	printf("Counted alive in %lf s\n", get_wall_seconds()-time);
	return counter;
}



// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
void processNeighbours(unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned int*** restrict s_updates, unsigned int*** restrict i_updates, unsigned char**** partition, unsigned const char n_threads, unsigned int* partition_break){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	#pragma omp parallel num_threads(n_threads)
	{
	unsigned int thread_ID = omp_get_thread_num();
	unsigned int** i_updated = i_updates[thread_ID];
	unsigned int** alive = s_alive[thread_ID];
	unsigned char*** part = partition[thread_ID];
	char l_l,l,l_u,m_l,m,m_u,n_l,n,n_u;
	unsigned char* cell;
	const unsigned int* z_axis = alive[1];
	const unsigned int* y_axis = alive[2];
	const unsigned int* x_axis = alive[3];
	unsigned int z_coord, y_coord, x_coord;
	unsigned int counter = 0;
	unsigned int limit = alive[0][0];
	unsigned int z = partition_break[thread_ID];

	for(int i=0; i < limit; i++){
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

					cell = &(part[z_coord+l][y_coord+m][x_coord+n]);

					if(!(((*cell)&62))){
						i_updated[1][counter] = z_coord + l;
						i_updated[2][counter] = y_coord + m;
						i_updated[3][counter] = x_coord + n;
						counter++;
					}

//					char val = (((((*cell) & 62) >> 1) + 1) << 1) + ((*cell) & 1);
//					*cell = val;
					*cell += 2;

				}
			}
		}
		
	}
	i_updated[0][0] = counter;

	}
	int z_offset = 0;
	int total_updates = 0;
	for(int i = 0; i < n_threads; i++){
		unsigned int s_counter = 0;
		z_offset += i==0 ? 0 : partition_break[i-1]-2;
		unsigned int* z_axis = i_updates[i][1];
		unsigned int* y_axis = i_updates[i][2];
		unsigned int* x_axis = i_updates[i][3];
		unsigned char*** part = partition[i];
		for(int j = 0; j < i_updates[i][0][0]; j++){
			unsigned int z_coord = z_axis[j];
			unsigned int y_coord = y_axis[j];
			unsigned int x_coord = x_axis[j];
			
//			*cell = (((((*cell) & 62) >> 1) + thread_sum) << 1) +((*cell)&1);
			if(!(test_byte[z_coord + z_offset][y_coord][x_coord]&62)){
				s_updates[i][1][s_counter] = z_coord;
                                s_updates[i][2][s_counter] = y_coord;
                                s_updates[i][3][s_counter] = x_coord;
                                s_counter++;

			}
			test_byte[z_coord + z_offset][y_coord][x_coord] += (part[z_coord][y_coord][x_coord] & 62);

		}
		s_updates[i][0][0] = s_counter++;
		total_updates += i_updates[i][0][0];
	}
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	return;

}


void partitions(unsigned char**** restrict partitions, unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict vol, unsigned const char n_threads, unsigned int* partitions_break, unsigned int n_alive){
	double time = get_wall_seconds();
	unsigned int alive_counter = 0;
	unsigned int alive_per_thread = n_alive/n_threads;
	unsigned int z_start = 0;
	unsigned int part = 0;
	for(int i = 0; i < size.z; i++){

		for(int j = 0; j < size.y; j++){
			for(int k = 0; k < size.x; k++){
				if(vol[i][j][k]&1){
					s_alive[part][1][alive_counter] = i - z_start;
					s_alive[part][2][alive_counter] = j;
					s_alive[part][3][alive_counter] = k;
					alive_counter++;
				}
			}
		}

		if(((alive_counter >= alive_per_thread)) || (i+(n_threads-part-1)) == size.z-1){
			unsigned int z_s = z_start == 0 ? 0 : (z_start);
			unsigned int z_e = i == (size.z-1) ? size.z - 1 : i+1 ;
			partitions_break[part] = z_e - z_s + 1;
			partitions[part] = partialWrappedVolume(vol, size, z_s, z_e);
			s_alive[part][0][0] = alive_counter;
//			printf("Partition %i with %i alive between z %i-%i\n", part, alive_counter, z_s, z_e);
			part++;
			alive_counter = 0;
			z_start = i;

		}
	}
	printf("Partitioned volume in %lf s\n", get_wall_seconds()-time);

	return;

}

unsigned char*** partialWrappedVolume(unsigned char*** restrict vol, const dim_t size, unsigned z_start, unsigned z_end){
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = z_end - z_start + 1;
        unsigned char*** z_slices = (unsigned char***)malloc(z*sizeof(unsigned char**));
        for(unsigned int i = z_start; i <= z_end; i++){
		z_slices[i-z_start] = (unsigned char**)malloc(y*sizeof(unsigned char*));
                for(unsigned int j = 0; j <y; j++){
			z_slices[i - z_start][j] = (unsigned char*)malloc(x*sizeof(unsigned char));
                        memcpy(z_slices[i - z_start][j], vol[i][j], x*sizeof(unsigned char));
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

unsigned char*** wrappedVolume(const dim_t size){
        double time = get_wall_seconds();
	srand(0);
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
        unsigned char*** z_slices = (unsigned char***)malloc(z*sizeof(unsigned char**));
        for(unsigned int i = 0; i < z; i++){
                z_slices[i] = (unsigned char**)malloc(y*sizeof(unsigned char*));
                unsigned char** y_slice = z_slices[i];
                for(unsigned int j = 0; j <y; j++){
                        y_slice[j] = (unsigned char*)malloc(x*sizeof(unsigned char));
                        unsigned char* x_slice = y_slice[j];
                        for(unsigned int k =0; k < x; k++){
                                x_slice[k] = (unsigned char)round(0.75*(double)rand()/RAND_MAX);
                        }
			if(i == z-1){ 
				memcpy(z_slices[i][j], z_slices[1][j], x*sizeof(unsigned char));
				memcpy(z_slices[0][j], z_slices[z-2][j], x*sizeof(unsigned char));
			}
			z_slices[i][j][0] = z_slices[i][j][x-2];
                        z_slices[i][j][x-1] = z_slices[i][j][1];
                }
		memcpy(z_slices[i][0], z_slices[i][y-2], x*sizeof(unsigned char));
		memcpy(z_slices[i][y-1], z_slices[i][1], x*sizeof(unsigned char));
        }
	printf("Generated wrapped volume in %lf s of size %li B\n", get_wall_seconds()-time, x*y*z*sizeof(unsigned char)+y*z*sizeof(unsigned char*)+z*sizeof(unsigned char**));
        return z_slices;
	

}

void copyPadding(unsigned char*** restrict vol, const dim_t size){
        double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
        for(unsigned int i = 0; i < z; i++){
                for(unsigned int j = 0; j <y; j++){
			if(i == z-1){
				memcpy(vol[i][j], vol[1][j], x*sizeof(unsigned char));
				memcpy(vol[0][j], vol[z-2][j], x*sizeof(unsigned char));
			}
			vol[i][j][0] = vol[i][j][x-2];
                        vol[i][j][x-1] = vol[i][j][1];
                }
		memcpy(vol[i][0], vol[i][y-2], x*sizeof(unsigned char));
		memcpy(vol[i][y-1], vol[i][1], x*sizeof(unsigned char));
        }
	printf("Copied edges of volume in %lf s\n", get_wall_seconds()-time);
	




}


void resizeCoordinateArrays(unsigned int** arr, unsigned int value){
	int new_size = ((int)(1.2*value)*sizeof(unsigned int));
	arr[1] = realloc(arr[1], new_size);
	arr[2] = realloc(arr[2], new_size);
	arr[3] = realloc(arr[3], new_size);
	
}

unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z){
	unsigned int** arr = (unsigned int**)malloc(sizeof(unsigned int*)*4);
	unsigned long int size = sizeof(unsigned int)*x*y*z;
	arr[0] = (unsigned int*)malloc(sizeof(unsigned int));
	arr[1] = (unsigned int*)malloc(size);
	arr[2] = (unsigned int*)malloc(size);
	arr[3] = (unsigned int*)malloc(size);
//	printf("Allocated %li B for coordinate arrays\n", (x*y*z)*sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int*)*4);
	return arr;
}


void freeCoordinateArrays(unsigned int** restrict arr){
	free(arr[0]);
	free(arr[1]);
	free(arr[2]);
	free(arr[3]);
	free(arr);
	return;
}


void freeVolume(unsigned char*** restrict volume, const dim_t size){
	unsigned int y = size.y;
	unsigned int z = size.z;
	for(unsigned int i = 0; i < z; i++){
		for(unsigned int j = 0; j < y; j++){
				free(volume[i][j]);
		}
		free(volume[i]);
	}
	free(volume);
	return;
}

void printVolume(unsigned char*** restrict volume, const dim_t size, char hide_wrapping, char hide_count){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
	int offset = 0;
	if (hide_wrapping) offset = -1;
	for(unsigned int i = hide_wrapping; i < z + offset; i++){
		for(unsigned int j = hide_wrapping; j < y + offset; j++){
			for(unsigned int k = hide_wrapping; k < x+offset; k++){
				printf("%i ", volume[i][j][k]&1);
                                if(k%(x+offset) == (x-1+offset) && !hide_count) printf("\t");
                                if(k%(x+offset) == (x-1+offset) && hide_count) printf("\n");
			}
			if(!hide_count){
				for(unsigned int k = hide_wrapping; k < x+offset; k++){
					printf("%i ", (volume[i][j][k]&62) >> 1);
	                                if(k%(x+offset) == (x-1+offset)) printf("\n");
				}
			}
		}
		printf("\n");
	}
	printf("Printed volume in %lf s\n", get_wall_seconds()-time);
}

