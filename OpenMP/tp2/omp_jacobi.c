#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

// Default dimension for matrix size
#ifndef VAL_N
#define VAL_N 101
#endif
#ifndef VAL_D
#define VAL_D 100
#endif

// Function to initialize an array with random numbers
void random_number(double* array, int size) {
  for(int i=0; i<size; i++) {
    // Generate a number in the interval [0, 1[
    double r = (double)rand() / (double)(RAND_MAX - 1);
    array[i] = r;
  }
}

int main() {

  int n=VAL_N, diag=VAL_D;
  int i, j, iteration=0;
  double a[n][n];
  double x[n], x_courant[n], b[n];
  double norme;

  struct timeval t_elapsed_0, t_elapsed_1;
  double t_elapsed;

  clock_t t_cpu_0, t_cpu_1;
  double  t_cpu;

#ifdef _OPENMP
  int nb_taches;
#pragma omp parallel
  { nb_taches = omp_get_num_threads(); }
  fprintf(stdout, "\n\n   Parallel Jacobi execution with %d threads\n", nb_taches);
#endif // _OPENMP

  // Initialization of the matrix and the right-hand side
  srand(421); // for reproducible results
  random_number(&a[0][0], n*n);
  random_number(&b[0], n);

  // Strengthen the diagonal of the matrix
  for(int i=0; i<n; i++) {
    a[i][i] += diag;
  }

  // Initial solution
  for(int i=0; i<n; i++) {
    x[i] = 1.0;
  }

  // Initial CPU time
  t_cpu_0 = clock();

  // Reference elapsed time
  gettimeofday(&t_elapsed_0, NULL);

  // Jacobi method solution
  while(1) {
    iteration++;

#pragma omp for
    for(int i=0; i<n; i++) {
      x_courant[i] = 0;
      for(j=0; j<i; j++) {
	x_courant[i] += a[j][i]*x[j];
      }
      for(j=i+1; j<n; j++) {
	x_courant[i] += a[j][i]*x[j];
      }
      x_courant[i] = (b[i] - x_courant[i])/a[i][i];
    }

    // Convergence test
    {
      double absmax = 0;
      for(int i=0; i<n; i++) {
	double curr = fabs(x[i] - x_courant[i]);
	if (curr > absmax)
	  absmax = curr;
      }
      norme = absmax / n;
    }
    if( (norme <= DBL_EPSILON) || (iteration >= n) ) break;

    // Copy x_courant into x
    memcpy(x, x_courant, n*sizeof(double));
  }

  // Final elapsed time
  gettimeofday(&t_elapsed_1, NULL);
  t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  // Final CPU time
  t_cpu_1 = clock();
  t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;

  // results
  fprintf(stdout, "\n\n"
	  "   Taille du systeme   : %5d\n"
	  "   Iterations          : %4d\n"
	  "   Norme               : %10.3E\n"
	  "   Temps elapsed       : %10.3E sec.\n"
	  "   Temps CPU           : %10.3E sec.\n",
	  n, iteration, norme, t_elapsed, t_cpu
	  );

  return EXIT_SUCCESS;
}
