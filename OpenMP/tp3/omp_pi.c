#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

// Function to be integrated
double f(double a) {
  return 4/(1 + a*a);
}

// Default size dimension of matrices
#ifndef VAL_N
#define VAL_N 30000000
#endif

int main() {

  //                  __
  //  Goal: calculate || using the mid-point rectangle method.
  //
  //                   / 1
  //                  |       4            __
  //                  |   ----------  dx = ||
  //                  |    1 + x**2
  //                 / 0

#ifdef _OPENMP
  int nb_taches;
#pragma omp parallel
  { nb_taches = omp_get_num_threads(); }
  fprintf(stdout, "\n\n   Jacobi execution in parallel with %d threads\n", nb_taches);
#endif // _OPENMP

  int n=VAL_N;
  double Pi_calcule;

  // Length of the integration interval.
  double h = 1.0 / n;

  // Initial CPU time.
  clock_t t_cpu_0 = clock();

  // Reference elapsed time.
  struct timeval t_elapsed_0;
  gettimeofday(&t_elapsed_0, NULL);

  // Artificial loop, do not modify.
  for(int k=0; k<100; k++) {

    // Calculation of Pi
    Pi_calcule = 0;
#pragma omp .for
    for(int i = 0; i<n; i++) {
      double x = h * (i + 0.5);
      Pi_calcule += f(x);
    }
    Pi_calcule = h * Pi_calcule;

  }

  // Final elapsed time.
  struct timeval t_elapsed_1;
  gettimeofday(&t_elapsed_1, NULL);
  double t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  // Final CPU time
  clock_t t_cpu_1 = clock();
  double t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;

  // Difference between the estimated and calculated value of Pi.
  double Pi_estime = acos(-1);   // Estimated value of Pi
  double ecart = fabs(Pi_estime - Pi_calcule);

  // Printing the results
  fprintf(stdout, "\n\n"
	  "   Number of intervals        : %10d\n"
	  "   | Estimated Pi - Calculated Pi | : %10.3E\n"
	  "   Elapsed time               : %10.3E sec.\n"
	  "   CPU time                   : %10.3E sec.\n",
	  n, ecart, t_elapsed, t_cpu
	  );

  return EXIT_SUCCESS;
}
