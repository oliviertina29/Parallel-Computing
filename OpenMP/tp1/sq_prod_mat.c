#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>


// Default dimension for matrix size
#ifndef VAL_M
#define VAL_M 101
#endif
#ifndef VAL_N
#define VAL_N 101
#endif

int main() {

  int m=VAL_M, n=VAL_N;
  double a[m][n], b[n][m], c[m][m];

  struct timeval t_elapsed_0, t_elapsed_1;
  double t_elapsed;

  clock_t t_cpu_0, t_cpu_1;
  double  t_cpu;


  // Initial CPU time.
  t_cpu_0 = clock();

  // Reference elapsed time.
  gettimeofday(&t_elapsed_0, NULL);

  // Initialization of matrices A, B and C.
  for(int i=0; i < m; i++) {
    for(int j=0; j < n; j++) {
      a[i][j] = (i+1)+(j+1);
    }
  }

  for(int i=0; i < n; i++) {
    for(int j=0; j < m; j++) {
      b[i][j] = (i+1)-(j+1);
    }
  }

  for(int i=0; i < m; i++) {
    for(int j=0; j < m; j++) {
      c[i][j] = 0;
    }
  }

  // Matrix product
  for(int i=0; i < m; i++) {
    for(int j=0; j < m; j++) {
      for(int k=0; k < n; k++) {
	c[i][j] = c[i][j] + a[i][k] * b[k][j];
      }
    }
  }

  // Final elapsed time.
  gettimeofday(&t_elapsed_1, NULL);
  t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  // Final CPU time.
  t_cpu_1 = clock();
  t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;

  // Print result.
  fprintf(stdout, "\n\n"
	  "   Values of m and n   : %5d %5d\n"
	  "   Elapsed time        : %10.3E sec.\n"
	  "   CPU time            : %10.3E sec.\n"
	  "   Partial result      : %10.3E %10.3E ... %10.3E %10.3E\n\n",
	  m, n, t_elapsed, t_cpu, c[1][1], c[2][2], c[m-3][m-3], c[m-2][m-2]
	  );

  return EXIT_SUCCESS;
}
