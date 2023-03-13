#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

unsigned char*** generateVolume(const unsigned int x, const unsigned int y, const unsigned int z);
unsigned char*** generateZeroVolume(const unsigned int x, const unsigned int y, const unsigned int z);
unsigned char* generateContigousVolume(const unsigned int x, const unsigned int y, const unsigned int z);
unsigned char* generateContigousZeroVolume(const unsigned int x, const unsigned int y,const unsigned int z);
unsigned int** generateCoordinateArrays(const unsigned int x, const unsigned int y, const unsigned int z);
void freeVolume(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z);
void freeCoordinateArrays(unsigned int** restrict arr);
void printVolume(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z, char hide_wrapping);
unsigned int** findAlive(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z);
unsigned int** findAlive2(unsigned char* restrict volume, const unsigned int x, const unsigned int y, const unsigned int z);
unsigned char*** wrappedVolume(const unsigned int x, const unsigned int y, const unsigned int z);
void copyPadding(unsigned char*** restrict vol, const unsigned int x, const unsigned int y, const unsigned int z);
void processNeighbours(unsigned int** restrict dest, unsigned int** restrict all, const unsigned int x, const unsigned int y, const unsigned int z, unsigned char*** restrict test_byte);
void processNeighbours2(unsigned int** restrict dest, unsigned int** restrict all, const unsigned int x, const unsigned int y, const unsigned int z, unsigned char* restrict test_byte);
double get_wall_seconds();




int main(int argc, char* argv[]){
	
	srand(0);
	unsigned int timesteps = 4;
	const unsigned size = 35;
	const unsigned wrapped_size = size + 2;
	unsigned int** updated = generateCoordinateArrays(wrapped_size, wrapped_size, wrapped_size);
	unsigned char*** test_byte = generateVolume(size, size, size);
	unsigned char*** wrapped = wrappedVolume(wrapped_size, wrapped_size, wrapped_size);
	printf("Wrapped state: \n");
//	printVolume(test_byte, size, size, size);
	unsigned int** alive = findAlive(wrapped,wrapped_size,wrapped_size,wrapped_size);

	unsigned int** all = generateCoordinateArrays(wrapped_size*3, wrapped_size*3, wrapped_size*3);
	processNeighbours(all, alive, wrapped_size, wrapped_size, wrapped_size, wrapped);
//	printVolume(wrapped, size, size, size, 0);
//	printVolume(wrapped, size, size, size, 1);



	printf("Setup complete\n\n");
	for(unsigned int t = 0; t < timesteps; t++){
		double time = get_wall_seconds();

		int update_count = 0;
		int balance = 0;
		int r, i, j, k, index;
		char cell, count;
//		printVolume(wrapped, size, size, size, 1);
		int limit = all[0][0];
		for(r = 0; r < limit; r++){
			i = all[1][r];
			j = all[2][r];
			k = all[3][r];
//			int count = neigh_count[i][j][k];
//			int index = i*x*y + j*x + i;
//			index = (i*size + j)*size + k;
//			cell = test_byte[index];
			cell = wrapped[i][j][k];
			if(cell&1){
				count = (cell & 62) >> 1;
				if(count != 5 && count != 6 && count !=7){
					wrapped[i][j][k] = count << 1;
					balance--;
				} else {
					updated[1][update_count] = i;
					updated[2][update_count] = j;
					updated[3][update_count] = k;
					update_count++;
					balance++;
				}

			} else {
				count = (cell & 62) >> 1;

				if(count==6){
					updated[1][update_count] = i;
					updated[2][update_count] = j;
					updated[3][update_count] = k;
					update_count++;
					wrapped[i][j][k] = (count << 1) + 1;
				}
			}
//			test_byte[index] &= ~62;
//			new_test_byte[i][j][k] &= ~62;
			wrapped[i][j][k] &= ~62;

		}

		updated[0][0] = update_count;
		processNeighbours(all, updated, wrapped_size, wrapped_size, wrapped_size, wrapped);
		copyPadding(wrapped, wrapped_size, wrapped_size, wrapped_size);


		printf("Next state\n");
		printf("Balance %i\n", balance);
//		printVolume(wrapped,size,size,size, 0);	
//		memcpy(test_byte, new_test_byte, size*size*size*sizeof(char));
//		char*** temp = test_byte;
//		test_byte = new_test_byte;
//		new_test_byte = temp;

		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);
	}
	freeCoordinateArrays(alive);
	freeCoordinateArrays(all);
	freeCoordinateArrays(updated);
 	freeVolume(test_byte,size,size,size);
 	freeVolume(wrapped,wrapped_size,wrapped_size,wrapped_size);
	
	return 0;

}

unsigned int** findAlive(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z){
	double time = get_wall_seconds();

	unsigned int** alive = generateCoordinateArrays(x,y,z);
	unsigned int counter  = 0;
	for(unsigned int i = 0; i < z; i++){
                for(unsigned int j = 0; j < y; j++){
                        for(unsigned int k = 0; k < x; k++){
				//printf("Checking life at %i %i %i\n", i,j,k);
//				assert(volume[i][j][k] == (volume + z*i + y*j + x*k));
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

unsigned int** findAlive2(unsigned char* restrict volume, const unsigned int x, const unsigned int y, const unsigned int z){
	double time = get_wall_seconds();
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
void processNeighbours(unsigned int** restrict dest, unsigned int** restrict alive,  const unsigned int x, const unsigned int y, const unsigned int z, unsigned char*** restrict test_byte){
	double time = get_wall_seconds();
	unsigned int** all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	unsigned int counter = 0;


	unsigned int limit = alive[0][0];

	char l_l,l,l_u,m_l,m,m_u,n_l,n,n_u;
	unsigned char* cell;
	unsigned const int* z_axis = alive[1];
	const unsigned int* y_axis = alive[2];
	const unsigned int* x_axis = alive[3];
	unsigned int z_coord, y_coord, x_coord, i;
	for(i = 0; i < limit; i++){
		z_coord = z_axis[i];
		y_coord = y_axis[i];
		x_coord = x_axis[i];
		
//		test_byte[z_coord][y_coord][x_coord] &= ~62;


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

/*		l_l = -1;
		l_u = 2;
		m_l= -1;
		m_u = 2;
		n_l = -1;
		n_u = 2;*/
		for(l = l_l; l < l_u; l++){
                        for(m = m_l; m < m_u; m++){
                                for(n = n_l; n < n_u; n++){
//					double temp = get_wall_seconds();

					cell = &(test_byte[z_coord+l][y_coord+m][x_coord+n]);

//					printf("At %i%i%i\n", z_coord+l, y_coord+m, x_coord+n);
//					assert(!(*neighbour) == (!((*cell)&62)));
//					printf("%x", !((*cell)&62));
					if(!(((*cell)&62) >> 1)){
//						int index = counter * !(cell&62);
						all[1][counter] = z_coord+l;
						all[2][counter] = y_coord+m;
						all[3][counter] = x_coord+n;
						counter++;
					}

//					char state = (*cell) & 1;
//					char count = (((*cell) & 62) >> 1) + 1;
//					*cell = (count << 1) + state;

					*cell = (((((*cell) & 62) >> 1) + 1) << 1) + ((*cell) & 1);

				}
			}
		}
	}
	for(i = 0; i < counter; i++){
		z_coord = all[1][i];
		y_coord = all[2][i];
		x_coord = all[3][i];
		cell = &(test_byte[z_coord][y_coord][x_coord]);
		*cell = (((((*cell) & 62) >> 1) - ((*cell)&1)) << 1) +((*cell)&1);
	}
//	printf("Counter: %i\n", counter);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();
	all[0][0] = counter;
//	printf("Reducing neighbour array by %li bytes\n", sizeof(int)*(counter));
	printf("Number of cells to be counted %i/%i\n", all[0][0], x*y*z*27);
//	all = realloc(all, sizeof(int)*counter+4);

	return;

}

void processNeighbours2(unsigned int** restrict dest, unsigned int** restrict alive,  const unsigned int x, const unsigned int y, const unsigned int z, unsigned char* restrict test_byte){
	double time = get_wall_seconds();
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

unsigned char*** generateVolume(const unsigned int x, const unsigned int y, const unsigned int z){
	srand(0);
	double time = get_wall_seconds();
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


unsigned char*** wrappedVolume(const unsigned int x, const unsigned int y, const unsigned int z){
	srand(0);
        double time = get_wall_seconds();
        unsigned char*** z_slices = (unsigned char***)malloc(z*sizeof(unsigned char**));
        for(unsigned int i = 0; i < z; i++){
                z_slices[i] = (unsigned char**)malloc(y*sizeof(unsigned char*));
                unsigned char** y_slice = z_slices[i];
                for(unsigned int j = 0; j <y; j++){
                        y_slice[j] = (unsigned char*)malloc(x*sizeof(unsigned char));
                        unsigned char* x_slice = y_slice[j];
                        for(unsigned int k =0; k < x; k++){
                                x_slice[k] = (unsigned char)round(0.65*(double)rand()/RAND_MAX);
                        }
//			if(i == z_p-2) 
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

void copyPadding(unsigned char*** restrict vol, const unsigned int x, const unsigned int y, const unsigned int z){
        double time = get_wall_seconds();

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
	printf("Generated wrapped volume in %lf s\n", get_wall_seconds()-time);
	




}
unsigned char* generateContigousVolume(const unsigned int x, const unsigned int y, const unsigned int z){
	srand(0);

	double time = get_wall_seconds();
        unsigned char* vol = (unsigned char*)malloc(z*y*x*sizeof(unsigned char));
        for(unsigned int i = 0; i < x*y*z; i++){
		vol[i] = (unsigned char)round(0.55*(double)rand()/RAND_MAX);
//		printf("%i\n", vol[i]);
        }
        printf("Generated contigous volume in %lf s\n", get_wall_seconds()-time);
        return vol;

}
unsigned char*** generateZeroVolume(const unsigned int x, const unsigned int y, const unsigned int z){
	double time = get_wall_seconds();
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

unsigned char* generateContigousZeroVolume(const unsigned int x, const unsigned int y, const unsigned int z){
	double time = get_wall_seconds();
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


void freeVolume(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z){
	double time = get_wall_seconds();
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

void printVolume(unsigned char*** restrict volume, const unsigned int x, const unsigned int y, const unsigned int z, char hide_wrapping){
	double time = get_wall_seconds();
	int offset = 2;
	if (hide_wrapping) offset = 1;
	for(unsigned int i = hide_wrapping; i < z + offset; i++){
		for(unsigned int j = hide_wrapping; j < y + offset; j++){
			for(unsigned int k = hide_wrapping; k < x+offset; k++){
				printf("%i ", volume[i][j][k]&1);
                                if(k%(x+offset) == (x-1+offset)) printf("\t");
			}

			for(unsigned int k = hide_wrapping; k < x+offset; k++){
				printf("%i ", (volume[i][j][k]&62) >> 1);
                                if(k%(x+offset) == (x-1+offset)) printf("\n");
			}
		}
		printf("\n");
	}
	printf("Printed volume in %lf s\n", get_wall_seconds()-time);
}
