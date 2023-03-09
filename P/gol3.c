#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <math.h>

char*** generateVolume(int x, int y, int z);
char*** generateZeroVolume(int x, int y, int z);
void freeVolume(char*** volume, int x, int y, int z);
void printVolume(char*** volume, int x, int y, int z);
int* findAlive(char*** volume, int x, int y, int z);
void addAlive(int* alive, int i, int j, int z);
void removeAlive(int* alive, int i, int j, int k);
int* processNeighbours(char*** neigh_count, int* all, int x, int y, int z);
double get_wall_seconds();

int main(int argc, char* argv[]){
	
	srand(0);
	int incr = 0;
	int size = 35;
	char*** states = generateVolume(size, size, size);
	char*** neigh_count = generateZeroVolume(size,size,size);
	char*** new_states = generateZeroVolume(size, size, size);

	int* alive;
	int* all;
	int* updated;
	char R[4] = {5,7,6,6};
//	printf("Initial state: \n");
//	printVolume(states, size, size, size);
//	alive = findAlive(states,size,size,size);
	while(1){
		alive = findAlive(states,size,size,size);
/*		printf("Number of alive cells: %i\n", (alive[0]-1)/3);
		for(int e = 1; e < alive[0]; e+=3){
			printf("Alive %i%i%i\n", alive[e], alive[e+1], alive[e+2]);
		}
		printf("Number of alive cells old: %i\n", (alive_temp[0]-1)/3);
		for(int e = 1; e < alive_temp[0]; e+=3){
			printf("Alive %i%i%i\n", alive_temp[e], alive_temp[e+1], alive_temp[e+2]);
		}*/
		all = processNeighbours(neigh_count, alive,size,size,size);
//		printf("Count:\n");
//		printVolume(neigh_count, size, size, size);
//		printVolume(states, size, size, size);
		double time = get_wall_seconds();
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
					}
				} else if(count == 6){
					new_states[i][j][k] = 1;
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


		char*** temp = states;
		states = new_states;
		new_states = temp;
		free(all);

		if((alive[0]-1)/3==0){
			printf("Everyone has died\n");
			break;
		}
		free(alive);
		incr++;
		printf("Generation done in %lf s\n\n", get_wall_seconds()-time);
		if (incr == 1) break;
	}
//	free(alive);
//	free(all);
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


// TODO: When loopin through neighbours, count how many times a neighbour is mentioned. This can replace the neighbour checking in the main loop
int* processNeighbours(char*** neigh_count, int* alive,  int x, int y, int z){
	double time = get_wall_seconds();
	int* all = (int*)malloc(sizeof(int)*alive[0]*27+4);
	int* all2 = (int*)malloc(sizeof(int)*alive[0]*27+4);
//	printf("Allocated %li bytes for neighbors\n", sizeof(int)*alive[0]*27+4);
	int counter = 1;
	int c2 = 6;
	double timecon = 0;

	char a = 0;
	a = c2<<1 + 0;
	char mask = 0xc;
	if(mask & a) printf("This would birth\n");
	printf("%x\n",a);
	for(int i = 1; i < alive[0]; i+=3){
		int z_coord = alive[i];
		int y_coord = alive[i+1];
		int x_coord = alive[i+2];
	

		double temp = get_wall_seconds();
		int z1 = z_coord - 1;
		int z3 = z_coord + 1;
		int y1 = y_coord - 1;
		int y3 = y_coord + 1;
		int x1 = x_coord - 1;
		int x3 = x_coord + 1;


		if(z1<0) z1 = 34 - z1;
		if(z1 > 34) z1 = z1 - 34;

		if(z3<0) z1 = 34 - z3;
		if(z3 > 34) z3 = z3 - 34;

		if(y1 < 0) y1 = 34 - y1;
		if(y1 > 34) y1 = y1 - 34;

		if(y3 < 0) y3 = 34 - y3;
		if(y3 > 34) y3 = y3 - 34;

		if(x1 < 0) x1 = 34 - x1;
		if(x1 > 34) x1 = x1 - 34;

		if(x3 < 0) x3 = 34 - x3;
		if(x3 > 34) x3 = x3 - 34;
		
		neigh_count[z1][y1][x1] += 1;
		neigh_count[z1][y1][x_coord] += 1;
		neigh_count[z1][y1][x3] += 1;

		neigh_count[z1][y_coord][x1] += 1;
		neigh_count[z1][y_coord][x_coord] += 1;
		neigh_count[z1][y_coord][x3] += 1;

		neigh_count[z1][y3][x1] += 1;
		neigh_count[z1][y3][x_coord] += 1;
		neigh_count[z1][y3][x3] += 1;

		neigh_count[z_coord][y1][x1] += 1;
		neigh_count[z_coord][y1][x_coord] += 1;
		neigh_count[z_coord][y1][x3] += 1;

		neigh_count[z_coord][y_coord][x1] += 1;
		neigh_count[z_coord][y_coord][x_coord] += 1;
		neigh_count[z_coord][y_coord][x3] += 1;

		neigh_count[z_coord][y3][x1] += 1;
		neigh_count[z_coord][y3][x_coord] += 1;
		neigh_count[z_coord][y3][x3] += 1;

		neigh_count[z3][y1][x1] += 1;
		neigh_count[z3][y1][x_coord] += 1;
		neigh_count[z3][y1][x3] += 1;

		neigh_count[z3][y_coord][x1] += 1;
		neigh_count[z3][y_coord][x_coord] += 1;
		neigh_count[z3][y_coord][x3] += 1;

		neigh_count[z3][y3][x1] += 1;
		neigh_count[z3][y3][x_coord] += 1;
		neigh_count[z3][y3][x3] += 1;



		timecon += get_wall_seconds()-temp;




//		printf("z: %i y: %i x: %i\n", z_coord, y_coord, x_coord);
		for(int l = -1; l < 2; l++){
                        for(int m = -1; m < 2; m++){
                                for(int n = -1; n < 2; n++){
                                        if((unsigned int)(z_coord+l) >= z || (unsigned int)(y_coord+m) >= y || (unsigned int) (x_coord+n) >= x){
//						printf("Blocked z: %i/%i y: %i/%i x: %i/%i\n", z_coord+l, z-1, y_coord+m,y-1, x_coord+n, x-1);
                                                continue;
					}
					all[counter] = z_coord+l;
					all[counter+1] = y_coord+m;
					all[counter+2] = x_coord+n;
//					printf("Counted z: %i/%i y: %i/%i x: %i/%i\n", z_coord+l, z-1, y_coord+m,y-1, x_coord+n, x-1);
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
//	printf("Reducing neighbour array to %li bytes\n", sizeof(int)*counter+4);
	printf("Conditional statments in %lf s\n", timecon);
	printf("Number of cells to be counted %i/%i\n", (all[0]-1)/3, x*y*z*27);
	all = realloc(all, sizeof(int)*counter+4);

	return all;

}



double get_wall_seconds() {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  double seconds = tv.tv_sec + (double)tv.tv_usec / 1000000;
  return seconds;
}
