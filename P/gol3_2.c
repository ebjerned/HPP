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
unsigned char* generateContigousVolume(const dim_t size);
unsigned char* generateContigousZeroVolume(const dim_t size);
unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z);
void freeVolume(unsigned char*** restrict volume, const dim_t size);
void freeCoordinateArrays(unsigned int** restrict arr);
void printVolume(unsigned char*** restrict volume, dim_t size, char hide_wrapping, char hide_count);
unsigned int** findAlive(unsigned char*** restrict volume, const dim_t size);
unsigned int** findAlive2(unsigned char* restrict volume, const dim_t size);
unsigned char*** wrappedVolume(const dim_t size);
unsigned char*** wrappedZeroVolume(const dim_t size);
void copyPadding(unsigned char*** restrict vol, const dim_t size);
void processNeighbours(unsigned int** restrict dest, unsigned int** restrict all, const dim_t size,  unsigned char*** restrict test_byte, unsigned int*** s_updates, unsigned char*** restrict partition, unsigned const char n_threads, unsigned char**** n_counter_thread);
void processNeighbours2(unsigned int** restrict dest, unsigned int** restrict all, const dim_t size, unsigned char* restrict test_byte);
void cellLoop(unsigned char*** restrict vol, unsigned int** restrict updated, unsigned int** restrict all, unsigned int*** restrict s_updates, unsigned const char n_threads);
double get_wall_seconds();




int main(int argc, char* argv[]){
	
	
	
	unsigned int timesteps = 1;
	const unsigned size = 200;
	const unsigned wrapped_size = size + 2;
	const dim_t wrapped_dim = {wrapped_size, wrapped_size, wrapped_size};
//	unsigned int** updated = generateCoordinateArrays(wrapped_size, wrapped_size, wrapped_size);

	unsigned char*** wrapped = wrappedVolume(wrapped_dim);

	unsigned int** alive = findAlive(wrapped,wrapped_dim);
	unsigned char n_threads = 2;
	unsigned int** all = generateCoordinateArrays(wrapped_size*3, wrapped_size*3, wrapped_size*3);

//	processNeighbours(all, alive, wrapped_size, wrapped_size, wrapped_size, wrapped);

	unsigned int*** s_updates = (unsigned int***)malloc(n_threads*sizeof(unsigned int**));
	for(int i = 0; i < n_threads; i++)
		s_updates[i] = generateCoordinateArrays(wrapped_size, wrapped_size, wrapped_size);

	unsigned char*** n_counter_thread[n_threads];
	for(int i = 0; i < n_threads; i++)
		n_counter_thread[i] = generateZeroVolume(wrapped_dim);

	unsigned char*** partition = wrappedZeroVolume(wrapped_dim);
	unsigned int partition_counter = 0;
	unsigned char ID = 0;

	for(int i = 0; i < wrapped_size; i++){
		for(int j = 0; j < wrapped_size; j++){
			for(int k = 0; k < wrapped_size; k++){
				if(wrapped[i][j][k]) partition_counter++;
				partition[i][j][k] = ID;
				if(partition_counter == (alive[0][0]/n_threads+1)) {
					ID++;
					partition_counter = 0;
				}
			}
		}
	}

	unsigned int** updated = alive;
	printf("Setup complete\n\n");
	for(unsigned int t = 0; t < timesteps; t++){
		double time = get_wall_seconds();
//		printVolume(partition, wrapped_dim, 1, 1);

		processNeighbours(all, updated, wrapped_dim, wrapped, s_updates, partition, n_threads, n_counter_thread);

//		printVolume(wrapped, wrapped_dim, 1, 0);	

		cellLoop(wrapped, updated, all, s_updates, n_threads);
		printf("Looping done in %lf s, %.16lf per cell\n", get_wall_seconds()-time, (get_wall_seconds()-time)/all[0][0]);
		copyPadding(wrapped, wrapped_dim);



		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);
	}
	freeCoordinateArrays(alive);
	freeCoordinateArrays(all);
//	freeCoordinateArrays(updated);
 	freeVolume(wrapped,wrapped_dim);
	freeVolume(partition, wrapped_dim);
	for(int i = 0; i < n_threads; i++){
		freeCoordinateArrays(s_updates[i]);
		freeVolume(n_counter_thread[i], wrapped_dim);
	}
	free(s_updates);
	
	return 0;
}

void cellLoop(unsigned char*** restrict vol, unsigned int** restrict updated, unsigned int** restrict all, unsigned int*** restrict s_updates, unsigned const char n_threads){
		double time = get_wall_seconds();

		int limit = all[0][0];
		int section = limit/n_threads;
		#pragma omp parallel num_threads(n_threads)
		{
		int update_count = 0;
		int r, i, j, k;
		char cell, count;

		unsigned int** s_updated = s_updates[omp_get_thread_num()];
		int start = omp_get_thread_num()*section;
		int end = (omp_get_thread_num() == (n_threads -1)) ? limit : (omp_get_thread_num()+1)*section;
//		printf("Interval %i - %i, %i\n", start, end, limit);
		for(r = start; r < end; r++){
			i = all[1][r];
			j = all[2][r];
			k = all[3][r];

			cell = vol[i][j][k];
			if(cell&1){
				count = (cell & 62) >> 1;
				if(count < 5 || count > 7){
					vol[i][j][k] = count << 1;
				} else {
					s_updated[1][update_count] = i;
					s_updated[2][update_count] = j;
					s_updated[3][update_count] = k;
					update_count++;
				}

			} else {
				count = (cell & 62) >> 1;

				if(count==6){
					s_updated[1][update_count] = i;
					s_updated[2][update_count] = j;
					s_updated[3][update_count] = k;
					update_count++;
					vol[i][j][k] = (count << 1) + 1;
				}
			}
			vol[i][j][k] &= ~62;



		}

		s_updated[0][0] = update_count;
		}
		printf("Parallel loop in %lf s\n", get_wall_seconds()-time);
		updated[0][0] = 0;
		for(int i = 0; i < n_threads; i++){
			memcpy(&updated[1][updated[0][0]], s_updates[i][1], s_updates[i][0][0]*sizeof(unsigned int));
			memcpy(&updated[2][updated[0][0]], s_updates[i][2], s_updates[i][0][0]*sizeof(unsigned int));
			memcpy(&updated[3][updated[0][0]], s_updates[i][3], s_updates[i][0][0]*sizeof(unsigned int));
			updated[0][0] += s_updates[i][0][0];

		}
		printf("Updates: %i\n", updated[0][0]);


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

unsigned int** findAlive2(unsigned char* restrict volume, const dim_t size){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;

	unsigned int** alive = generateCoordinateArrays(x,y,z);
	unsigned int counter  = 0;
	unsigned int x_counter = 0;
	unsigned int y_counter = 0;
	unsigned int z_counter = 0;

	for(unsigned int i = 0; i < x*y*z; i++){
                if(volume[i]){
			alive[1][counter] = z_counter;
			alive[2][counter] = y_counter;
			alive[3][counter] = x_counter;
			counter++;
		}
//		printf("%i %i %i\n", z_counter, y_counter, x_counter);
		x_counter++;
		if(x_counter == x){
			y_counter++;
			x_counter = 0;
			if(y_counter == y){
				z_counter++;
				y_counter = 0;
			}
		}
        }
	alive[0][0] = counter;
	printf("Number of alive cells of total %i\n", counter);
	printf("Found contigous alive in %lf s\n", get_wall_seconds()-time);
	return alive;
}

// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
void processNeighbours(unsigned int** restrict dest, unsigned int** restrict alive,  const dim_t size, unsigned char*** restrict test_byte, unsigned int*** restrict s_updates, unsigned char*** partition, unsigned const char n_threads, unsigned char**** n_counter_thread){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
	unsigned int** all = dest;
	unsigned int limit = alive[0][0];
	printf("Limit neighbors: %i\n", alive[0][0]);

	#pragma omp parallel num_threads(n_threads)
	{
	unsigned int** updated = s_updates[omp_get_thread_num()];
	char l_l,l,l_u,m_l,m,m_u,n_l,n,n_u;
	unsigned char* cell;
	const unsigned int* z_axis = alive[1];
	const unsigned int* y_axis = alive[2];
	const unsigned int* x_axis = alive[3];
	unsigned int z_coord, y_coord, x_coord, i;
	unsigned int counter = 0;
	char thread_ID = omp_get_thread_num();
	int section = limit/n_threads;
	int start = thread_ID*section;
	int end = (thread_ID == (n_threads -1)) ? limit : (thread_ID+1)*section;
	printf("Thread %i on interval %i - %i\n", thread_ID, start, end);
	for(i = start; i < end; i++){
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
//					printf("\tAt %i%i%i\n", z_coord+l, y_coord+m, x_coord+n);

//					cell = &(test_byte[z_coord+l][y_coord+m][x_coord+n]);
					cell = &(n_counter_thread[omp_get_thread_num()][z_coord +l][y_coord +m][x_coord +n]);
					char allowed = (partition[z_coord+l][y_coord+m][x_coord+n] == thread_ID);
					if(!(*cell) && allowed){
						s_updates[omp_get_thread_num()][1][counter] = z_coord+l;
						s_updates[omp_get_thread_num()][2][counter] = y_coord+m;
						s_updates[omp_get_thread_num()][3][counter] = x_coord+n;
						counter++;
					}
					*cell += 1;
					
//					char val = (((((*cell) & 62) >> 1) + 1) << 1) + ((*cell) & 1);
					
//					*cell = val;

				}
			}
		}
	}
	s_updates[omp_get_thread_num()][0][0] = counter;
	printf("Thread %i with count %i\n", thread_ID, s_updates[thread_ID][0][0]);
	}

	all[0][0] = 0;
	for(int i = 0; i < n_threads; i++){
//		printf("Thread unique count: %i\n", counter);
		//s_updates[i][0][0] = counter;
		memcpy(&all[1][all[0][0]], s_updates[i][1], s_updates[i][0][0]*sizeof(unsigned int));
		memcpy(&all[2][all[0][0]], s_updates[i][2], s_updates[i][0][0]*sizeof(unsigned int));
		memcpy(&all[3][all[0][0]], s_updates[i][3], s_updates[i][0][0]*sizeof(unsigned int));
		all[0][0] += s_updates[i][0][0];

	}
	for(int i = 0; i < all[0][0]; i++){
		unsigned int z_coord = all[1][i];
		unsigned int y_coord = all[2][i];
		unsigned int x_coord = all[3][i];
		unsigned char* cell = &(test_byte[z_coord][y_coord][x_coord]);
		unsigned char thread_sum = 0;
		for(int j = 0; j < n_threads; j++){
			thread_sum += n_counter_thread[j][z_coord][y_coord][x_coord];
		}
		*cell = (((((*cell) & 62) >> 1) - ((*cell)&1) + thread_sum) << 1) +((*cell)&1);
	}
//	freeVolume(n_counter_thread[0],size);
//	freeVolume(n_counter_thread[1],x,y,z);
	printf("Count: %i\n", all[0][0]);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	return;

}

void processNeighbours2(unsigned int** restrict dest, unsigned int** restrict alive,  const dim_t size, unsigned char* restrict test_byte){
	double time = get_wall_seconds();

	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;

	unsigned int** all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	unsigned int counter = 0;

	unsigned int limit = alive[0][0];
	const unsigned int* z_axis = alive[1];
	const unsigned int* y_axis = alive[2];
	const unsigned int* x_axis = alive[3];
	unsigned int z_coord, y_coord, x_coord;
	char l_l, l, l_u, m_l, m, m_u, n_l, n, n_u;
	unsigned char* cell;
	for(unsigned int i = 0; i < limit; i++){
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
//					char* neighbour = &(neigh_count[z_coord+l][y_coord+m][x_coord+n]);


					cell = &test_byte[(z_coord+l)*x*y + (y_coord+m)*x + (x_coord + n)];
//					assert(!(*neighbour) == (!((*cell)&62)));
					if(!(((*cell)&62) >> 1)){
//						int index = counter * !((*cell)&62);
						all[1][counter] = z_coord+l;
						all[2][counter] = y_coord+m;
						all[3][counter] = x_coord+n;
						counter++;
					}
//					*neighbour += 1;



//					char state = (*cell) & 1;
//					char count = (((*cell) & 62) >> 1) + 1;
//					*cell = (count << 1) + state;
					*cell = (((((*cell) & 62) >> 1) + 1 ) << 1) + ((*cell) & 1);

//					assert(*neighbour == (((*cell) & 62) >> 1));
				}
			}
		}
	}

//	printf("Counter: %i\n", counter);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	all[0][0] = counter;
//	printf("Reducing neighbour array by %li bytes\n", sizeof(int)*(counter));
	printf("Number of cells to be counted %i/%i\n", all[0][0], x*y*z*27);
//	all = realloc(all, sizeof(int)*counter+4);

	return;

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
	printf("Generated wrapped volume in %lf s\n", get_wall_seconds()-time);
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
	printf("Generated wrapped zero volume in %lf s\n", get_wall_seconds()-time);
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
unsigned char* generateContigousVolume(const dim_t size){
	double time = get_wall_seconds();
	srand(0);
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
        unsigned char* vol = (unsigned char*)malloc(z*y*x*sizeof(unsigned char));
        for(unsigned int i = 0; i < x*y*z; i++){
		vol[i] = (unsigned char)round(0.55*(double)rand()/RAND_MAX);
//		printf("%i\n", vol[i]);
        }
        printf("Generated contigous volume in %lf s\n", get_wall_seconds()-time);
        return vol;

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
        printf("Generated zero volume in %lf s\n", get_wall_seconds()-time);
        return z_slices;

}

unsigned char* generateContigousZeroVolume(const dim_t size){
	double time = get_wall_seconds();
	unsigned int x = size.x;
	unsigned int y = size.y;
	unsigned int z = size.z;
	unsigned char* vol = (unsigned char*)malloc(sizeof(unsigned char)*x*y*z);
	memset(vol, 0, sizeof(unsigned char)*x*y*z);
        printf("Generated contigous zero volume in %lf s\n", get_wall_seconds()-time);

	return vol;
}





unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z){
	unsigned int** arr = (unsigned int**)malloc(sizeof(unsigned int*)*4);
	unsigned long int size = sizeof(unsigned int)*x*y*z;
	arr[0] = (unsigned int*)malloc(sizeof(unsigned int));
	arr[1] = (unsigned int*)malloc(size);
	arr[2] = (unsigned int*)malloc(size);
	arr[3] = (unsigned int*)malloc(size);

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
	double time = get_wall_seconds();
	unsigned int x = size.x;
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
