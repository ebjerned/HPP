#include <stdio.h>
#include <stdlib.h>
#include <time.h>

double** allocate_matrix(int n);
void deallocate_matrix(double** theMatrix, int n);
void fill_matrix(double** theMatrix, int n);
void print_matrix(double** theMatrix, int n);
double get_min_value(double** theMatrix, int n);
double get_max_value(double** theMatrix, int n);
double get_wall_seconds();

int main()
{
  int n;
  printf("\nEnter the dimension for a square matrix:");
  scanf("%d",&n);
  printf("n = %d\n", n);
  double start = get_wall_seconds();
  double** matrixA = allocate_matrix(n);
  double all_A = get_wall_seconds();
  printf("Allocated first matrix in %lf s\n", all_A-start);
  fill_matrix(matrixA, n);
  double fill_A = get_wall_seconds();
  printf("Filled first matrix in %lf s\n", fill_A-all_A);
  double minValue = get_min_value(matrixA, n);
  double min = get_wall_seconds();
  printf("Found minimum in %lf s\n", min-fill_A);
  double maxValue = get_max_value(matrixA, n);
  double max = get_wall_seconds();
  printf("Found maximum in %lf s\n", max-min);
  //print_matrix(matrixA, n);
  printf("Min value: %14.9f  Max value: %14.9f\n", minValue, maxValue);
  deallocate_matrix(matrixA, n);
  double deall_A = get_wall_seconds();
  printf("Deallocated first matrix in %lf s\n", deall_A-max);
  double** matrixB = allocate_matrix(n);
  deallocate_matrix(matrixB,n);
  double deall_B = get_wall_seconds();
  printf("Allocated and deallocated second matrix in %lf s\n", deall_B-deall_A);
  return 0;
}


double** allocate_matrix(int n)
{
  double** theMatrix;
  int i;
  theMatrix = calloc(n , sizeof(double *));
  for(i = 0; i < n; i++)
    theMatrix[i] = calloc(n , sizeof(double));
  return theMatrix;
}

void deallocate_matrix(double** theMatrix, int n)
{
  int i;
  for(i = 0; i < n; i++)
    free(theMatrix[i]);
  free(theMatrix);
}

void fill_matrix(double** theMatrix, int n)
{
  int j, i;
  for(j = 0; j < n; j++)
    for(i = 0 ; i < n ; i++)
      theMatrix[i][j] = 10 * (double)rand() / RAND_MAX;
}

void print_matrix(double** theMatrix, int n)
{
  int i, j;
  for(i = 0 ; i < n; i++)
    {
      for(j = 0 ; j < n ; j++)
	printf("%2.3f " , theMatrix[i][j]);
      putchar('\n');
    }
}

double get_max_value(double** theMatrix, int n)
{
  int i, j;
  double max = 0.0;
  max = theMatrix[0][0];
  for(j = 0; j < n ; j++)
    for(i = 0 ;i < n; i++)
      if(max < theMatrix[i][j])
	max = theMatrix[i][j];
  return max;
}

double get_min_value(double** theMatrix, int n)
{
  int i, j;
  double min = 0.0;
  min = theMatrix[0][0];
  for(j = 0; j < n; j++)
    for(i = 0; i < n; i++)
      if(min > theMatrix[i][j])
	min = theMatrix[i][j];
  return min;
}

double get_wall_seconds(){
	struct timeval tv;
	gettimeofday(&tv, NULL);
	double seconds = tv.tv_sec + (double)tv.tv_usec/1000000;
	return seconds;
}
