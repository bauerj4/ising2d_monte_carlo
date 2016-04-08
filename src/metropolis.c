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
  //TheseIterations = NSTEPS;
  while (n < (int)TheseIterations)
    {
      if (ThisTask == 0 && (n % 1000 == 0  || n == TheseIterations - 1))
      	printf("Metropolis Iteration: %d / %d \t Memory Allocated: %10.2f MB\r", (n+1), TheseIterations, \
      	       (double)ByteCount/1.e6);
      i = RandomIntInRange(0, imax-1);
      j = RandomIntInRange(0, LATTICE_SIZE);

      Ediff = DeltaU(i,j);
      
      //printf("i,j,Ediff = %d,%d,%f\n",i,j,Ediff);
      
      if (Ediff < 0.)
	{
	  SpinMesh[i][j] *= -1;
	  //printf("Flipping spin on thread %d\n", ThisTask);
	}
      else
	{
	  rand = Uniform();
	  prob = exp(-Ediff / Temperature);
	  if (rand < prob)
	    {
	      //printf("Flipping spin on thread %d\n", ThisTask);
	      SpinMesh[i][j] *= -1;
	    }
	}

      MPI_Barrier(MPI_COMM_WORLD);

      //GetGhostRowTop();
      //GetGhostRowBottom();
      MPI_Allgather(SpinMesh[imax-1],LATTICE_SIZE,MPI_SHORT,LastRows,LATTICE_SIZE,MPI_SHORT,MPI_COMM_WORLD);
      MPI_Allgather(SpinMesh[0],LATTICE_SIZE,MPI_SHORT,FirstRows,LATTICE_SIZE,MPI_SHORT,MPI_COMM_WORLD);
      //printf("Gathered elements from thread %d\n", ThisTask);
      SpinMatrixDeepCopy();
      //printf("Copied\n");

      //break;
      MPI_Barrier(MPI_COMM_WORLD);
      n++;
    }
#ifdef STEP_REDUCTION_POWER
  TheseIterations = (int)((double)NSTEPS * pow(STEP_REDUCTION_POWER, currentStep));
#endif // ITERATION_REDUCTION_POWER     
  if (ThisTask==0)
    printf("\n");
  return 0;
}
