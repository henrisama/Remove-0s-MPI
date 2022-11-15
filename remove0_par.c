#include <mpi.h>
#include <stdio.h>

int main(int argc, char **argv)
{
  MPI_Init(&argc, &argv);
  double tini = MPI_Wtime();


  double tfim = MPI_Wtime();
  printf("Tempo=%lf\n", tfim - tini);
  MPI_Finalize();
  return 0;
}
