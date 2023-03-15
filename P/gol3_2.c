#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>
#include <omp.h>

typedef struct dimesions {
	const unsigned int x;
	const unsigned int y;
	const unsigned int z;
} dim_t;

unsigned char*** generateVolume(const dim_t size);
unsigned char*** generateZeroVolume(const dim_t size);
unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z);
void freeVolume(unsigned char*** restrict volume, const dim_t size);
void freeCoordinateArrays(unsigned int** restrict arr);
void printVolume(unsigned char*** restrict volume, dim_t size, char hide_wrapping, char hide_count);
unsigned int** findAlive(unsigned char*** restrict volume, const dim_t size);
unsigned char*** wrappedVolume(const dim_t size);
unsigned char*** wrappedZeroVolume(const dim_t size);
void copyPadding(unsigned char*** restrict vol, const dim_t size);
void processNeighbours(unsigned int** restrict dest, unsigned int*** restrict s_alive, const dim_t size,  unsigned char*** restrict test_byte, unsigned int*** s_updates, unsigned char**** restrict partition, unsigned const char n_threads, unsigned char**** n_counter_thread, unsigned int* partition_break);
void cellLoop(unsigned char*** restrict vol, unsigned int*** restrict s_alive, unsigned int** restrict all, unsigned int*** restrict s_updates, unsigned const char n_threads);
void resizeCoordinateArrays(unsigned int** arr);
void partitions(unsigned char**** restrict partitions, unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned const char n_threads, unsigned int* partition_break);
unsigned char*** partialWrappedVolume(unsigned char*** restrict vol, const dim_t size, unsigned z_start, unsigned z_end);
double get_wall_seconds();




int main(int argc, char* argv[]){

	

	unsigned int timesteps = 1;
	const unsigned size = 5;
	const unsigned wrapped_size = size + 2;
	const dim_t wrapped_dim = {wrapped_size, wrapped_size, wrapped_size};
	unsigned char*** wrapped = wrappedVolume(wrapped_dim);
	copyPadding(wrapped, wrapped_dim);

	unsigned int** alive = findAlive(wrapped,wrapped_dim);
	unsigned char n_threads = atoi(argv[1]);
	unsigned int** all = generateCoordinateArrays(alive[0][0]*27, 1, 1);

	unsigned char*** n_counter_thread[n_threads];
	unsigned int*** s_updates = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));
	unsigned int*** s_alive = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));
	for(int i = 0; i < n_threads; i++){
		s_updates[i] = generateCoordinateArrays(wrapped_size, wrapped_size, wrapped_size);
		s_alive[i] = generateCoordinateArrays(wrapped_size, wrapped_size, wrapped_size);
		n_counter_thread[i] = generateZeroVolume(wrapped_dim);
	}


	unsigned char*** partition = wrappedZeroVolume(wrapped_dim);

	unsigned char**** parts = (unsigned char****)malloc(n_threads*sizeof(unsigned char***));
	unsigned int* partition_break = (unsigned int*)malloc(n_threads*sizeof(int));
	partitions(parts, s_alive, wrapped_dim, wrapped, n_threads, partition_break);
	printf("Setup complete\n\n");

	for(unsigned int t = 0; t < timesteps; t++){
		double gen_start_time = get_wall_seconds();
//		printVolume(partition, wrapped_dim, 1, 1);
		printVolume(wrapped, wrapped_dim, 1,0);
		processNeighbours(all, s_alive, wrapped_dim, wrapped, s_updates, parts, n_threads, n_counter_thread, partition_break);

		printVolume(wrapped, wrapped_dim, 1, 0);	
		cellLoop(wrapped, s_alive, all, s_updates, n_threads);
		printVolume(wrapped, wrapped_dim, 1, 0);	

		copyPadding(wrapped, wrapped_dim);

		resizeCoordinateArrays(alive);
		for(int i = 0; i < n_threads; i++){
			resizeCoordinateArrays(s_updates[i]);
		}
		resizeCoordinateArrays(all);

		printf("Generation done in %lf s\n\n", get_wall_seconds()-gen_start_time);
	}
	freeCoordinateArrays(alive);
	freeCoordinateArrays(all);
	freeVolume(wrapped,wrapped_dim);
	freeVolume(partition, wrapped_dim);
	for(int i = 0; i < n_threads; i++){
		freeCoordinateArrays(s_updates[i]);
		freeVolume(n_counter_thread[i], wrapped_dim);
		const dim_t temp_dim ={wrapped_size, wrapped_size, partition_break[i]};
		freeVolume(parts[i], temp_dim);
		freeCoordinateArrays(s_alive[i]);
		
	}
	free(s_updates);
	free(s_alive);
	free(parts);
	free(partition_break);
	return 0;
}


void cellLoop(unsigned char*** restrict vol, unsigned int*** restrict s_alive, unsigned int** restrict all, unsigned int*** restrict s_updates, unsigned const char n_threads){
		double time = get_wall_seconds();

		
		#pragma omp parallel num_threads(n_threads)
		{
		int alive_count = 0;
		int r, i, j, k;
		char cell, count;
		unsigned int** s_updated = s_updates[omp_get_thread_num()];
		unsigned int** s_living = s_alive[omp_get_thread_num()];
		int limit = s_updated[0][0];
		int section = limit/n_threads;
		int start = omp_get_thread_num()*section;
		int end = (omp_get_thread_num() == (n_threads -1)) ? limit : (omp_get_thread_num()+1)*section;
//		printf("Interval %i - %i, %i\n", start, end, limit);
		for(r = 0; r < limit; r++){
			i = s_updated[1][r];
			j = s_updated[2][r];
			k = s_updated[3][r];

			cell = vol[i][j][k];
			if(cell&1){
				count = (cell & 62) >> 1;
				if(count < 5 || count > 7){
					vol[i][j][k] = count << 1;
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
					vol[i][j][k] = (count << 1) + 1;
				}
			}
			vol[i][j][k] &= ~62;



		}

		s_living[0][0] = alive_count;
		}
		printf("Parallel loop in %lf s\n", get_wall_seconds()-time);
		all[0][0] = 0;
		for(int i = 0; i < n_threads; i++){
			memcpy(&all[1][all[0][0]], s_alive[i][1], s_alive[i][0][0]*sizeof(unsigned int));
			memcpy(&all[2][all[0][0]], s_alive[i][2], s_alive[i][0][0]*sizeof(unsigned int));
			memcpy(&all[3][all[0][0]], s_alive[i][3], s_alive[i][0][0]*sizeof(unsigned int));
			all[0][0] += s_alive[i][0][0];

		}
		printf("Updates: %i\n", all[0][0]);

		printf("Looping done in %lf s, %.16lf per cell\n", get_wall_seconds()-time, (get_wall_seconds()-time)/all[0][0]);

}


unsigned int** findAlive(unsigned char*** restrict volume, const dim_t size){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;

	unsigned int** alive = generateCoordinateArrays(x,y,z);
	unsigned int counter  = 0;
	for(unsigned int i = 0; i < z; i++){
                for(unsigned int j = 0; j < y; j++){
                        for(unsigned int k = 0; k < x; k++){
				if(volume[i][j][k]){
					alive[1][counter] = i;
					alive[2][counter] = j;
					alive[3][counter] = k;
					counter++;
				}


                        }
                }
        }
	alive[0][0] = counter;
	printf("Number of alive cells of total %i\n", counter);
	printf("Found alive in %lf s\n", get_wall_seconds()-time);
	return alive;
}



// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
void processNeighbours(unsigned int** restrict dest, unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned int*** restrict s_updates, unsigned char**** partition, unsigned const char n_threads, unsigned char**** n_counter_thread, unsigned int* partition_break){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
//	unsigned int z_actual = size.z;
	unsigned int** all = dest;
//	printf("Limit neighbors: %i\n", alive[0][0]);

	#pragma omp parallel num_threads(n_threads)
	{
	char thread_ID = omp_get_thread_num();
	unsigned int** updated = s_updates[omp_get_thread_num()];
	unsigned int** alive = s_alive[omp_get_thread_num()];
	char l_l,l,l_u,m_l,m,m_u,n_l,n,n_u;
	unsigned char* cell;
	const unsigned int* z_axis = alive[1];
	const unsigned int* y_axis = alive[2];
	const unsigned int* x_axis = alive[3];
	unsigned int z_coord, y_coord, x_coord;
	unsigned int counter = 0;
	unsigned int limit = alive[0][0];
	unsigned int z = partition_break[thread_ID];//thread_ID == (omp_get_num_threads()-1) ? z_actual-partition_break[thread_ID-1] : partition_break[thread_ID];
//	int section = limit/n_threads;
//	int start = thread_ID*section;
//	int end = (thread_ID == (n_threads -1)) ? limit : (thread_ID+1)*section;
	//printf("Thread %i on interval %i - %i\n", thread_ID, start, end);
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

//		printf("%i: At %i %i %i\n",i, z_coord, y_coord, x_coord);

		for(l = l_l; l < l_u; l++){
                        for(m = m_l; m < m_u; m++){
                                for(n = n_l; n < n_u; n++){
					cell = &(partition[omp_get_thread_num()][z_coord +l][y_coord +m][x_coord +n]);
					char bounce = 0;
					if(thread_ID > 0 && z_coord+l== z-1 && thread_ID == (omp_get_num_threads()-1)) bounce = 1;

					if(!(((*cell)&62) >> 1) && !bounce){
						s_updates[omp_get_thread_num()][1][counter] = z_coord+l;
						s_updates[omp_get_thread_num()][2][counter] = y_coord+m;
						s_updates[omp_get_thread_num()][3][counter] = x_coord+n;
						counter++;
					}

					char val = (((((*cell) & 62) >> 1) + 1) << 1) + ((*cell) & 1);
					*cell = val;

				}
			}
		}
	}
	s_updates[omp_get_thread_num()][0][0] = counter;
	//printf("Thread %i with count %i\n", thread_ID, s_updates[thread_ID][0][0]);
	}
	int z_offset = 0;
	for(int i = 0; i < n_threads; i++){
		z_offset += i==0 ? 0 : partition_break[i-1]-2;
		const dim_t temp_size = {size.x, size.y, partition_break[i]};
//		printVolume(partition[i], temp_size, 0, 0);

		for(int j = 0; j < s_updates[i][0][0]; j++){
			unsigned int z_coord = s_updates[i][1][j];
			unsigned int y_coord = s_updates[i][2][j];
			unsigned int x_coord = s_updates[i][3][j];
			unsigned char* cell = &(test_byte[z_coord + z_offset][y_coord][x_coord]);
			unsigned char thread_sum = (partition[i][z_coord][y_coord][x_coord] & 62)>>1;
			*cell = (((((*cell) & 62) >> 1) - ((*cell)&1) + thread_sum) << 1) +((*cell)&1);
		}
	}

	/*
	all[0][0] = 0;
	for(int i = 0; i < n_threads; i++){
//		printf("Thread unique count: %i\n", counter);
		//s_updates[i][0][0] = counter;
		memcpy(&all[1][all[0][0]], s_updates[i][1], s_updates[i][0][0]*sizeof(unsigned int));
		memcpy(&all[2][all[0][0]], s_updates[i][2], s_updates[i][0][0]*sizeof(unsigned int));
		memcpy(&all[3][all[0][0]], s_updates[i][3], s_updates[i][0][0]*sizeof(unsigned int));
		all[0][0] += s_updates[i][0][0];

	}*/
//	freeVolume(n_counter_thread[0],size);
//	freeVolume(n_counter_thread[1],x,y,z);
//	printf("Count: %i\n", all[0][0]);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	return;

}
void partitions(unsigned char**** restrict partitions, unsigned int*** restrict s_alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned const char n_threads, unsigned int* partitions_break){
	double time = get_wall_seconds();
	unsigned int alive_counter = 0;
	unsigned int alive_per_thread =size.x*size.y*size.z*0.33/n_threads;// alive[0][0]/n_threads;
	unsigned int z_start = 0;
	unsigned int part = 0;
	for(int i = 0; i < size.z; i++){
		for(int j = 0; j < size.y; j++){
			for(int k = 0; k < size.x; k++){
				if(test_byte[i][j][k]&1){
					s_alive[part][1][alive_counter] = i - z_start;// + (part==0 && n_threads != 1);
					s_alive[part][2][alive_counter] = j;
					s_alive[part][3][alive_counter] = k;
					alive_counter++;

				}
			}
		}
		if((alive_counter >= alive_per_thread && (n_threads-part) > 1) || i == size.z-1){
			unsigned int z_s = z_start == 0 ? 0 : (z_start);
			unsigned int z_e = i == (size.z-1) ? size.z - 1 : i+1 ;

//			printf("For thread %i: Interval %i - %i with %i alive\n", part, z_s, z_e, alive_counter);
			partitions_break[part] = z_e - z_s + 1;
			partitions[part] = partialWrappedVolume(test_byte,size, z_s, z_e);
			const dim_t temp_size = {size.x, size.y, z_e - z_s + 1};
			//printVolume(partitions[part], temp_size, 0, 1);
			s_alive[part][0][0] = alive_counter;
			part++;
			alive_counter = 0;
			z_start = i;

		}
	}
	printf("Partitioned volume in %lf s\n", get_wall_seconds()-time);

	return;

}

unsigned char*** partialWrappedVolume(unsigned char*** restrict vol, const dim_t size, unsigned z_start, unsigned z_end){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = z_end - z_start +1;

        unsigned char*** z_slices = (unsigned char***)malloc(z*sizeof(unsigned char**));
        for(unsigned int i = z_start; i <= z_end; i++){
		z_slices[i-z_start] = (unsigned char**)malloc(y*sizeof(unsigned char*));
                for(unsigned int j = 0; j <y; j++){
			z_slices[i - z_start][j] = (unsigned char*)malloc(x*sizeof(unsigned char));
                        memcpy(z_slices[i - z_start][j], vol[i][j], x*sizeof(unsigned char));
                }
        }
	printf("Generated partial wrapped volume in %lf s of size %li B\n", get_wall_seconds()-time, x*y*z*sizeof(unsigned char)+y*z*sizeof(unsigned char*)+z*sizeof(unsigned char**));
        return z_slices;
	


}


double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}

// TODO: Better approch is instead of storing is alive or not, is to store the number of adjacent alive cells directly.
// Loop through all cell and set all cells outside E to 0, if in F set a all adjacent cells to 1 in an change matrix 

unsigned char*** generateVolume(dim_t size){
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
			for(unsigned int k =0; k< x; k++){
				x_slice[k] = (unsigned char)round((double)rand()/RAND_MAX);
//				printf("%i\n", x_slice[k]);
			}
		}
	}
	printf("Generated volume in %lf s\n", get_wall_seconds()-time);
	return z_slices;

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

unsigned char*** wrappedZeroVolume(const dim_t size){
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
                                x_slice[k] = 0; //(unsigned char)round(0.75*(double)rand()/RAND_MAX);
                        }
		}
        }
	printf("Generated wrapped zero volume in %lf s of size %li B\n", get_wall_seconds()-time, x*y*z*sizeof(unsigned char)+y*z*sizeof(unsigned char*)+z*sizeof(unsigned char**));
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
unsigned char*** generateZeroVolume(const dim_t size){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
        unsigned char*** z_slices = (unsigned char***)malloc(z*sizeof(unsigned char**));
        for(int i = 0; i < z; i++){
                z_slices[i] = (unsigned char**)malloc(y*sizeof(unsigned char*));
                unsigned char** y_slice = z_slices[i];
                for(int j = 0; j <y; j++){
                        y_slice[j] = (unsigned char*)malloc(x*sizeof(unsigned char));
                        memset(y_slice[j], 0, x*sizeof(unsigned char));
                }
        }
        printf("Generated zero volume in %lf s of size %li B\n", get_wall_seconds()-time, x*y*z*sizeof(unsigned char)+y*z*sizeof(unsigned char*)+z*sizeof(unsigned char**));;
        return z_slices;

}

unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z){
	unsigned int** arr = (unsigned int**)malloc(sizeof(unsigned int*)*4);
	unsigned long int size = sizeof(unsigned int)*x*y*z;
	arr[0] = (unsigned int*)malloc(sizeof(unsigned int));
	arr[1] = (unsigned int*)malloc(size);
	arr[2] = (unsigned int*)malloc(size);
	arr[3] = (unsigned int*)malloc(size);
	printf("Allocated %li B for coordinate arrays\n", (x*y*z)*sizeof(unsigned int)+sizeof(unsigned int)+sizeof(unsigned int*)*4);
	return arr;
}

void resizeCoordinateArrays(unsigned int** arr){
	int new_size = ((int)(1.2*arr[0][0]))*sizeof(unsigned int);
	arr[1] = realloc(arr[1], new_size);
	arr[2] = realloc(arr[2], new_size);
	arr[3] = realloc(arr[3], new_size);
	
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
	double time = get_wall_seconds();
//	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
	for(unsigned int i = 0; i < z; i++){
		for(unsigned int j = 0; j < y; j++){
				free(volume[i][j]);
		}
		free(volume[i]);
	}
	free(volume);
	printf("Freed volume in %lf s\n", get_wall_seconds()-time);
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
