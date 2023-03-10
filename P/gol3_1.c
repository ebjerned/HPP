#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

char*** generateVolume(int x, int y, int z);
char*** generateZeroVolume(int x, int y, int z);
char* generateContigousVolume(int x, int y, int z);
char* generateContigousZeroVolume(int x, int y, int z);
int** generateCoordinateArrays(int x, int y, int z);
void freeVolume(char*** volume, int x, int y, int z);
void freeCoordinateArrays(int** arr);
void printVolume(char*** volume, int x, int y, int z);
int** findAlive(char*** volume, int x, int y, int z);
int** findAlive2(char* volume, int x, int y, int z);

void processNeighbours(int** dest, char*** neigh_count, int** all, int x, int y, int z, char*** test_byte);
void processNeighbours2(int** dest, int** all, int x, int y, int z, char* test_byte);
double get_wall_seconds();




int main(int argc, char* argv[]){
	
	srand(0);
	int incr = 0;
	int size = 35;

	int** updated = generateCoordinateArrays(size, size, size);
	char* test_byte = generateContigousVolume(35,35,35);
	char* new_test_byte = generateContigousZeroVolume(size,size,size);
//	printf("Initial state: \n");
//	printVolume(states, size, size, size);
	int** alive = findAlive2(test_byte,size,size,size);

	int** all = generateCoordinateArrays(size*3, size*3, size*3);
	processNeighbours2(all, alive, size,size,size, test_byte);

	printf("Setup complete\n\n");
	while(1){
		double time = get_wall_seconds();

		int update_count = 0;
		int r, i, j, k, index;
		char cell, count;
//		printVolume(neigh_count, size, size, size);
//		printVolume(states, size, size, size);
		for(r = 0; r < all[0][0]; r++){
			i = all[1][r];
			j = all[2][r];
			k = all[3][r];
//			int count = neigh_count[i][j][k];
//			int index = i*x*y + j*x + i;
			index = i*size*size + j*size + k;
			cell = test_byte[index];

			if(cell&1){
				count = (cell & 62) >> 1;

				if(count != 5 && count != 6 && count !=7){
//					new_states[i][j][k] = 0;
					updated[1][update_count] = i;
                                        updated[2][update_count] = j;
                                        updated[3][update_count] = k;
                                        update_count++;
					new_test_byte[index] = 0;
				}
			} else {
				count = (cell & 62) >> 1;

				if(count==6){
	//				new_states[i][j][k] = 1;
					updated[1][update_count] = i;
					updated[2][update_count] = j;
					updated[3][update_count] = k;
					update_count++;
					new_test_byte[index] = 1;
				}
			}
//			neigh_count[i][j][k] = 0;
//			char state = test_byte[index] & 1;
			test_byte[index] &= ~62;
//			assert(((test_byte[index] & 62) >>1 ) == 0);
//			assert(states[i][j][k] == state);
//			assert(new_states[i][j][k] == (new_test_byte[index] & 1));
		
		}

		updated[0][0] = update_count;
		processNeighbours2(all, updated, size, size, size, test_byte);

		memcpy(test_byte, new_test_byte, size*size*size*sizeof(char));

		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);

		incr++;
		if (incr == 2){
			printf("Time steps maxed out\n");
			break;
		}
	}
	freeCoordinateArrays(alive);
	freeCoordinateArrays(all);
	freeCoordinateArrays(updated);
	free(test_byte);
	free(new_test_byte);
	return 0;

}

// TODO: Better approch is instead of storing is alive or not, is to store the number of adjacent alive cells directly.
// Loop through all cell and set all cells outside E to 0, if in F set a all adjacent cells to 1 in an change matrix 

char*** generateVolume(int x, int y, int z){
	double time = get_wall_seconds();
	char*** z_slices = (char***)malloc(z*sizeof(char**));
	for(int i = 0; i < z; i++){
		z_slices[i] = (char**)malloc(y*sizeof(char*));
		char** y_slice = z_slices[i];
		for(int j = 0; j <y; j++){
			y_slice[j] = (char*)malloc(x*sizeof(char));
			char* x_slice = y_slice[j];
			for(int k =0; k< x; k++){
				x_slice[k] = (char)round(0.55*(double)rand()/RAND_MAX);
//				printf("%i\n", x_slice[k]);
			}
		}
	}
	printf("Generated volume in %lf s\n", get_wall_seconds()-time);
	return z_slices;

}
char* generateContigousVolume(int x, int y, int z){
	srand(0);

	double time = get_wall_seconds();
        char* vol = (char*)malloc(z*y*x*sizeof(char));
        for(int i = 0; i < x*y*z; i++){
		vol[i] = (char)round(0.55*(double)rand()/RAND_MAX);
//		printf("%i\n", vol[i]);
        }
        printf("Generated contigous volume in %lf s\n", get_wall_seconds()-time);
        return vol;

}
char*** generateZeroVolume(int x, int y, int z){
	double time = get_wall_seconds();
        char*** z_slices = (char***)malloc(z*sizeof(char**));
        for(int i = 0; i < z; i++){
                z_slices[i] = (char**)malloc(y*sizeof(char*));
                char** y_slice = z_slices[i];
                for(int j = 0; j <y; j++){
                        y_slice[j] = (char*)malloc(x*sizeof(char));
                        memset(y_slice[j], 0, x*sizeof(char));
                }
        }
        printf("Generated zero volume in %lf s\n", get_wall_seconds()-time);
        return z_slices;

}

char* generateContigousZeroVolume(int x, int y, int z){
	double time = get_wall_seconds();
	char* vol = (char*)malloc(sizeof(char)*x*y*z);
	memset(vol, 0, sizeof(char)*x*y*z);
        printf("Generated contigous zero volume in %lf s\n", get_wall_seconds()-time);

	return vol;
}


int** generateCoordinateArrays(int x, int y, int z){
	int** arr = (int**)malloc(sizeof(int*)*4);
	long int size = sizeof(int)*x*y*z;
	arr[0] = (int*)malloc(sizeof(int));
	arr[1] = (int*)malloc(size);
	arr[2] = (int*)malloc(size);
	arr[3] = (int*)malloc(size);

	return arr;
}

void freeCoordinateArrays(int** arr){
	free(arr[0]);
	free(arr[1]);
	free(arr[2]);
	free(arr[3]);
	free(arr);
	return;
}


void freeVolume(char*** volume, int x, int y, int z){
	double time = get_wall_seconds();
	for(int i = 0; i < z; i++){
		for(int j = 0; j < y; j++){
				free(volume[i][j]);
		}
		free(volume[i]);
	}
	free(volume);
	printf("Freed volume in %lf s\n", get_wall_seconds()-time);
	return;
}

void printVolume(char*** volume, int x, int y, int z){
	double time = get_wall_seconds();
	for(int i = 0; i < z; i++){
		for(int j = 0; j < y; j++){
			for(int k = 0; k < x; k++){
				printf("%i ", volume[i][j][k]);
                                if(k%x == x-1) printf("\n");
			}
		}
		printf("\n");
	}
	printf("Printed volume in %lf s\n", get_wall_seconds()-time);
}

int** findAlive(char*** volume, int x, int y, int z){
	double time = get_wall_seconds();

	int** alive = generateCoordinateArrays(x,y,z);
	int counter  = 0;
	for(int i = 0; i < z; i++){
                for(int j = 0; j < y; j++){
                        for(int k = 0; k < x; k++){
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

int** findAlive2(char* volume, int x, int y, int z){
	double time = get_wall_seconds();
	int** alive = generateCoordinateArrays(x,y,z);
	int counter  = 0;
	int x_counter = 0;
	int y_counter = 0;
	int z_counter = 0;

	for(int i = 0; i < x*y*z; i++){
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
void processNeighbours(int** dest, char*** neigh_count, int** alive,  int x, int y, int z, char*** test_byte){
	double time = get_wall_seconds();
	int** all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	int counter = 0;


	int limit = alive[0][0];
	for(int i = 0; i < limit; i++){
		int z_coord = alive[1][i];
		int y_coord = alive[2][i];
		int x_coord = alive[3][i];
	


		char l_u = 2;
		char l_l = -1;
		char m_u = 2;
		char m_l = -1;
		char n_u = 2;
		char n_l = -1;

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

		for(char l = l_l; l < l_u; l++){
                        for(char m = m_l; m < m_u; m++){
                                for(char n = n_l; n < n_u; n++){
//					double temp = get_wall_seconds();

					char* neighbour = &(neigh_count[z_coord+l][y_coord+m][x_coord+n]);
					char* cell = &(test_byte[z_coord+l][y_coord+m][x_coord+n]);

					if(!(*neighbour)){
						all[1][counter] = z_coord+l;
						all[2][counter] = y_coord+m;
						all[3][counter] = x_coord+n;
						counter++;
					}
					*neighbour += 1;

					char state = (*cell) & 1;
					char count = (((*cell) & 62) >> 1) + 1;
					*cell = (count << 1) + state;
//					timecon += get_wall_seconds()-temp;

				}
			}
		}
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



void processNeighbours2(int** dest, int** alive,  int x, int y, int z, char* test_byte){
	double time = get_wall_seconds();
	int** all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	int counter = 0;

	int limit = alive[0][0];
	int* z_axis = alive[1];
	int* y_axis = alive[2];
	int* x_axis = alive[3];
	int z_coord, y_coord, x_coord;
	char l_l, l, l_u, m_l, m, m_u, n_l, n, n_u;
	char* cell;
	for(int i = 0; i < limit; i++){
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
					if(!((*cell)&62)){
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
