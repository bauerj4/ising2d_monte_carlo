#include "../include/mc_options.h"
#include "../include/allvars.h"
#include "../include/proto.h"
#include <mpi.h>
#include <stdio.h>

/*

  Program montecarlo:
  
  Do a Monte Carlo simulation for the 2D Ising lattice
  Submitted in partial fulfillment of requirements for
  Queen's University PHYS 870, Winter 2016

*/

int main(int argc, char ** argv)
{
  MPI_Init(&argc, &argv); // Initialize MPI

  Initialize();
  if (ThisTask == 0)
    printf("Initialized.\n");
  MPI_Barrier(MPI_COMM_WORLD);

  //PrintMatrix(SpinMesh, imax, LATTICE_SIZE);


  currentStep = 0;
  TheseIterations = NSTEPS;
  while (Temperature < T_MAX)
    {
      if (ThisTask==0)
	printf("Temperature = %f e / k_B\n", Temperature);
      Metropolis();
      T_arr[currentStep] = Temperature;
      TotalEnergy();
      TotalMagnetization();
      if (ThisTask==0)
        printf("M/N = %2.5f\tE/N = %2.5f\n\n", M_arr[currentStep], E_arr[currentStep]);

      Temperature += Tstep;
      currentStep++;
    }

  if (ThisTask == 2)
    PrintMatrix(SpinMesh, imax, LATTICE_SIZE);
  Finalize();

  MPI_Finalize(); // Close communication

  return 0; // success
}
