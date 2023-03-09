#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>
#include <assert.h>

char*** generateVolume(int x, int y, int z);
char*** generateZeroVolume(int x, int y, int z);
char* generateContigousVolume(int x, int y, int z);
void freeVolume(char*** volume, int x, int y, int z);
void printVolume(char*** volume, int x, int y, int z);
int* findAlive(char*** volume, int x, int y, int z);
int* findAlive2(char* volume, int x, int y, int z);

int* processNeighbours(int* dest, char*** neigh_count, int* all, int x, int y, int z);
double get_wall_seconds();

int main(int argc, char* argv[]){
	
	srand(0);
	int incr = 0;
	int size = 35;
	char*** states = generateVolume(size, size, size);
	char*** neigh_count = generateZeroVolume(size,size,size);
	char*** new_states = generateZeroVolume(size, size, size);
	char* contVol = generateContigousVolume(size, size, size);
	int* alive;
	int* updated = (int*)malloc(sizeof(int)*size*size*size*3+4);;
	char R[4] = {5,7,6,6};
//	printf("Initial state: \n");
//	printVolume(states, size, size, size);
	int* alive2 = findAlive2(contVol,size,size,size);
	alive = findAlive(states,size,size,size);
	int* all = (int*)malloc(sizeof(int)*alive[0]*27+4);
	processNeighbours(all, neigh_count, alive, size,size,size);
	printf("Setup complete\n\n");
	while(1){
		double time = get_wall_seconds();

		int update_count = 1;
//		memset(updated, 0, sizeof(int)*size*size*size*3+4);
//		memset(alive, 0, sizeof(int)*size*size*size*3+4);
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
		int counter = 1;
		for(int r = 1; r < all[0]; r += 3){
			int i = all[r];
			int j = all[r+1];
			int k = all[r+2];
			int count = neigh_count[i][j][k];
			if(count) {
				if(states[i][j][k]){
					if(count != 5 && count != 6 && count !=7){
						new_states[i][j][k] = 0;
						updated[update_count] = i;
	                                        updated[update_count+1] = j;
	                                        updated[update_count+2] = k;
	                                        update_count += 3;
					} /*else{
					
					}*/
				} else if(count==6){
					new_states[i][j][k] = 1;
					updated[update_count] = i;
					updated[update_count+1] = j;
					updated[update_count+2] = k;
					update_count += 3;
				}
				neigh_count[i][j][k] = 0;
			}
		}
//		printf("Step %i: \n", incr);
//		printVolume(states, size, size, size);
//		printf("Next step:\n");
//		printVolume(new_states, size, size, size);
//		printf("Number of alive cells next iteration by new way: %i\n", (alive[0]-1)/3);
		//for(int e = 1; e < alive[0]; e+=3){
		//	printf("Alive %i%i%i\n", alive[e], alive[e+1], alive[e+2]);
		//}
		updated[0] = update_count;
		processNeighbours(all, neigh_count, updated, size, size, size);

		char*** temp = states;
		states = new_states;
		new_states = temp;
//		free(all);

		if((updated[0]-1)/3==0){
			printf("Everyone has died\n");
			break;
		}
//		free(alive);
		incr++;
		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);
		if (incr == 1) break;
	}
	free(alive);
	free(all);
	free(updated);
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

int* findAlive(char*** volume, int x, int y, int z){
	double time = get_wall_seconds();
	int* alive =(int*) malloc(sizeof(int)*x*y*z*3+4);
	int counter  = 1;
	for(int i = 0; i < z; i++){
                for(int j = 0; j < y; j++){
                        for(int k = 0; k < x; k++){
				//printf("Checking life at %i %i %i\n", i,j,k);
//				assert(volume[i][j][k] == (volume + z*i + y*j + x*k));
				if(volume[i][j][k]){
					alive[counter] = i;
					alive[counter + 1] = j;
					alive[counter + 2] = k;
					counter += 3;
				}


                        }
                }
        }
	alive[0] = counter;
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
int* processNeighbours(int* dest, char*** neigh_count, int* alive,  int x, int y, int z){
	double time = get_wall_seconds();
	int* all = dest;

//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	int counter = 1;
	double timecon = 0;

//	char a = 0;
//	a = c2<<1 + 0;
//	char mask = 0xc;
//	if(mask & a) printf("This would birth\n");
//	printf("%x\n",a);
	for(int i = 1; i < alive[0]; i+=3){
		int z_coord = alive[i];
		int y_coord = alive[i+1];
		int x_coord = alive[i+2];
	

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

					all[counter] = z_coord+l;
					all[counter+1] = y_coord+m;
					all[counter+2] = x_coord+n;

					neigh_count[z_coord+l][y_coord+m][x_coord+n] += 1;
					counter += 3;
				}
			}
		}
	}

//	printf("Counter: %i\n", counter);
	printf("Found neighbours in %lf s\n", get_wall_seconds()-time);
	time = get_wall_seconds();
	all[0] = counter;
//	printf("Reducing neighbour array by %li bytes\n", sizeof(int)*(counter));
	printf("Conditional statments in %lf s\n", timecon);
	printf("Number of cells to be counted %i/%i\n", (all[0]-1)/3, x*y*z*27);
//	all = realloc(all, sizeof(int)*counter+4);

	return all;

}



void processNeighbour2(char* vol, int i, int j, int k){


}

double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
