#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

char*** generateVolume(int x, int y, int z);
char*** generateZeroVolume(int x, int y, int z);
char* generateContigousVolume(int x, int y, int z);
int** generateCoordinateArrays(int x, int y, int z);
void freeVolume(char*** volume, int x, int y, int z);
void freeCoordinateArrays(int** arr);
void printVolume(char*** volume, int x, int y, int z);
int** findAlive(char*** volume, int x, int y, int z);
int* findAlive2(char* volume, int x, int y, int z);

void processNeighbours(int** dest, char*** neigh_count, int** all, int x, int y, int z);
double get_wall_seconds();

int main(int argc, char* argv[]){
	
	srand(0);
	int incr = 0;
	int size = 35;
	char*** states = generateVolume(size, size, size);
	char*** neigh_count = generateZeroVolume(size,size,size);
	char*** new_states = generateZeroVolume(size, size, size);
	char* contVol = generateContigousVolume(size, size, size);
	int** alive;
	int** updated = generateCoordinateArrays(size, size, size);
//	printf("Initial state: \n");
//	printVolume(states, size, size, size);
	int* alive2 = findAlive2(contVol,size,size,size);
	alive = findAlive(states,size,size,size);
//	int* all = (int*)malloc(sizeof(int)*alive[0]*27+4);
	int** all = generateCoordinateArrays(alive[0][0]*27, 1, 1);


	processNeighbours(all, neigh_count, alive, size,size,size);
	printf("Setup complete\n\n");
	while(1){
		double time = get_wall_seconds();

		int update_count = 0;

/*		printf("Number of alive cells: %i\n", (alive[0]-1)/3);
		for(int e = 1; e < alive[0]; e+=3){
			printf("Alive %i%i%i\n", alive[e], alive[e+1], alive[e+2]);
		}
		printf("Number of alive cells old: %i\n", (alive_temp[0]-1)/3);
		for(int e = 1; e < alive_temp[0]; e+=3){
			printf("Alive %i%i%i\n", alive_temp[e], alive_temp[e+1], alive_temp[e+2]);
		}*/

//		printf("Count:\n");
//		printVolume(neigh_count, size, size, size);
//		printVolume(states, size, size, size);
		int counter = 0;
		for(int r = 0; r < all[0][0]; r++){
			int i = all[1][r];
			int j = all[2][r];
			int k = all[3][r];
			int count = neigh_count[i][j][k];
//			printf("Cell %i%i%i count %i\n", i, j, k, count);
//			if(count) {
				if(states[i][j][k]){
					if(count != 5 && count != 6 && count !=7){
						new_states[i][j][k] = 0;
						updated[1][update_count] = i;
	                                        updated[2][update_count] = j;
	                                        updated[3][update_count] = k;
	                                        update_count++;
					}
				} else if(count==6){
					new_states[i][j][k] = 1;
					updated[1][update_count] = i;
					updated[2][update_count] = j;
					updated[3][update_count] = k;
					update_count++;
				}
				neigh_count[i][j][k] = 0;
//			}
		}
//		printf("Step %i: \n", incr);
//		printVolume(states, size, size, size);
//		printf("Next step:\n");
//		printVolume(new_states, size, size, size);
//		printf("Number of alive cells next iteration by new way: %i\n", (alive[0]-1)/3);
		//for(int e = 1; e < alive[0]; e+=3){
		//	printf("Alive %i%i%i\n", alive[e], alive[e+1], alive[e+2]);
		//}
		updated[0][0] = update_count;
		processNeighbours(all, neigh_count, updated, size, size, size);

		char*** temp = states;
		states = new_states;
		new_states = temp;
//		free(all);

		if(updated[0][0]==0){
			printf("Everyone has died\n");
			break;
		}
//		free(alive);
		incr++;
		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);
		if (incr == 1){
			printf("Time steps maxed out\n");
			break;
		}
	}
	freeCoordinateArrays(alive);
	freeCoordinateArrays(all);
	freeCoordinateArrays(updated);
	freeVolume(new_states, size, size, size);
	freeVolume(states, size, size, size);
	freeVolume(neigh_count, size, size, size);
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
			for(int k =0; k< x; k++)
				x_slice[k] = (char)round(0.55*(double)rand()/RAND_MAX);
		}
	}
	printf("Generated volume in %lf s\n", get_wall_seconds()-time);
	return z_slices;

}
char* generateContigousVolume(int x, int y, int z){
	double time = get_wall_seconds();
        char* vol = (char*)malloc(z*y*x*sizeof(char));
        for(int i = 0; i < x*y*x; i++){
		vol[i] = (char)round(0.55*(double)rand()/RAND_MAX);
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
//	printf("Number of alive cells of total %i/%i\n", (counter/3), x*y*z);
	printf("Found alive in %lf s\n", get_wall_seconds()-time);
	return alive;
}

int* findAlive2(char* volume, int x, int y, int z){
	double time = get_wall_seconds();
	int* alive =(int*) malloc(sizeof(int)*x*y*z*+4);
	int counter  = 1;
	for(int i = 0; i < x*y*z; i++){
                if(volume[i]){
			alive[counter] = i;
			counter++;
		}
        }
	alive[0] = counter;
//	printf("Number of alive cells of total %i/%i\n", (counter/3), x*y*z);
	printf("Found contigous alive in %lf s\n", get_wall_seconds()-time);
	return alive;
}

// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
void processNeighbours(int** dest, char*** neigh_count, int** alive,  int x, int y, int z){
	double time = get_wall_seconds();
	int** all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	int counter = 1;
	double timecon = 0;

//	char a = 0;
//	a = c2<<1 + 0;
//	char mask = 0xc;
//	if(mask & a) printf("This would birth\n");
//	printf("%x\n",a);
	int limit = alive[0][0];
	for(int i = 0; i < limit; i++){
		int z_coord = alive[1][i];
		int y_coord = alive[2][i];
		int x_coord = alive[3][i];
	

		double temp = get_wall_seconds();

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
		timecon += get_wall_seconds()-temp;

		for(char l = l_l; l < l_u; l++){
                        for(char m = m_l; m < m_u; m++){
                                for(char n = n_l; n < n_u; n++){
					if(!neigh_count[z_coord+l][y_coord+m][x_coord+n]){
						all[1][counter] = z_coord+l;
						all[2][counter] = y_coord+m;
						all[3][counter] = x_coord+n;
						counter++;
					}
					neigh_count[z_coord+l][y_coord+m][x_coord+n] += 1;
				}
			}
		}
	}

//	printf("Counter: %i\n", counter);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();
	all[0][0] = counter;
//	printf("Reducing neighbour array by %li bytes\n", sizeof(int)*(counter));
	printf("Conditional statments in %lf s\n", timecon);
	printf("Number of cells to be counted %i/%i\n", all[0][0], x*y*z*27);
//	all = realloc(all, sizeof(int)*counter+4);

	return;

}



void processNeighbour2(char* vol, int i, int j, int k){


}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
