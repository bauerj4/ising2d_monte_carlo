#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/mc_options.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <mpi.h>

/*
  Use the Metroplis-Hastings algorithm to construct
  the solution at the specified number of iterations.
*/

int Metropolis()
{
  int i,j,n;//, imax;
  double rand, prob;


  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  if (ThisTask == 0)
    printf("Beginning Metropolis iterations.\n");

  n=0;
  i=0;
  while (n < (int)NSTEPS)
    {
      if (ThisTask == 0)
	printf("Metropolis Iteration: %d / %d \t Memory Allocated: %10.2f MB\r", (n+1), NSTEPS, \
	       (double)ByteCount/1.e6);
      i = RandomIntInRange(0, imax);
      j = RandomIntInRange(0, imax);

      Ediff = DeltaU(i,j);
      
      //printf("i,j,Ediff = %d,%d,%f\n",i,j,Ediff);
      
      if (Ediff < 0.)
	  SpinMesh[i][j] *= -1;
      else
	{
	  rand = Uniform();
	  prob = exp(-Ediff / TEMPERATURE);
	  if (rand < prob)
	    SpinMesh[i][j] *= -1;
	}

      MPI_Barrier(MPI_COMM_WORLD);

      GetGhostRowTop();
      GetGhostRowBottom();
      SpinMatrixDeepCopy();
      
      MPI_Barrier(MPI_COMM_WORLD);
      n++;
    }
  return 0;
}
