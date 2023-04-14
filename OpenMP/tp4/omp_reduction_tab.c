#define _POSIX_SOURCE // pour rand_r()

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

// Default dimension for matrix size
#ifndef VAL_NMOLEC
#define VAL_NMOLEC 100
#endif
#ifndef VAL_NMOL
#define VAL_NMOL 100
#endif
#ifndef VAL_N
#define VAL_N 10
#endif

// Initializing an array with zeros. 
void clear(double* array, int size) {
  for(int l=0; l<size; l++) array[l] = 0;
}

int main() {

  const int nmolec=VAL_NMOLEC, nmol=VAL_NMOL, n=VAL_N;

#ifdef _OPENMP
  int nb_taches;
#pragma omp parallel
  { nb_taches = omp_get_num_threads(); }
  fprintf(stdout, "\n\n   Execution reduction_tab en parallele avec %d threads\n", nb_taches);
#endif // _OPENMP

  double tab[nmolec][n][nmol];
// Initialization of the array
  for(int k=0; k<nmolec; k++) {
    for(int j=0; j<n; j++) {
      for(int i=0; i<nmol; i++) {
        tab[k][j][i] = i + j + k;
      }
    }
  }

  double tab1[nmol], tab2[nmol];
  clear(tab2, nmol);

  //CPU time for initial calculation.
  clock_t t_cpu_0 = clock();

  // Reference elapsed time.
  struct timeval t_elapsed_0;
  gettimeofday(&t_elapsed_0, NULL);

  // nested loop to be parallelized
#pragma omp parallel
  {
#pragma omp for
    for(int k=0; k<nmolec; k++) {
      clear(tab1, nmol);
      
      for(int j=0; j<n; j++) {
        for(int i=0; i<nmol; i++) {
          tab1[i] += tab[k][j][i];
        }
      }

      for(int l=0; l<nmol; l++) tab2[l] += 2*tab1[l];
    }
  }

  // Temps elapsed final
  struct timeval t_elapsed_1;
  gettimeofday(&t_elapsed_1, NULL);
  double t_elapsed = (t_elapsed_1.tv_sec - t_elapsed_0.tv_sec) + (t_elapsed_1.tv_usec - t_elapsed_0.tv_usec) / (double)1000000;

  // Temps CPU de calcul final
  clock_t t_cpu_1 = clock();
  double t_cpu = (t_cpu_1 - t_cpu_0) / (double)CLOCKS_PER_SEC;

  // Verification des resultats
  double tab1c[nmol], tab2c[nmol];
  clear(tab2c, nmol);
  for(int k=0; k<nmolec; k++) {
    clear(tab1c, nmol);

    for(int j=0; j<n; j++) {
      for(int i=0; i<nmol; i++) {
        tab1c[i] += tab[k][j][i];
      }
    }

    for(int l=0; l<nmol; l++) tab2c[l] += 2*tab1c[l];
  }

  double err = 0;
  for(int i=0; i<nmol; i++) {
    double curr = fabs(tab2c[i] - tab2[i]) / fabs(tab2c[i]);
    if (curr > err)
      err = curr;
  }

  // Impression du resultat
  fprintf(stdout, "\n\n"
          "   Temps elapsed       : %10.3E sec.\n"
          "   Temps CPU           : %10.3E sec.\n"
          "   Erreur relative     : %10.3E\n",
          t_elapsed, t_cpu, err
          );

  return EXIT_SUCCESS;

}
