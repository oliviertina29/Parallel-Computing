#include <stdio.h>
#include <stdlib.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

int main()
{
  int nb_taches = 0;

  #pragma omp parallel
  {
    // Get the rank of the current thread
    int rang = omp_get_thread_num();
    fprintf(stdout, "   Hello from thread of rank %d\n", rang);

    // Get the number of threads
    nb_taches = omp_get_num_threads();
  }

  // Check if executed sequentially or in parallel
  if (nb_taches == 0)
    fprintf(stdout, "   Execution of hello_world sequentially\n");
  else
    fprintf(stdout, "   Execution of hello_world in parallel with %d threads\n", nb_taches);

  return EXIT_SUCCESS;
}
