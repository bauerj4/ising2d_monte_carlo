#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/mc_options.h"
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
  Functions relating to program initialization
*/

int Initialize()
{
  int rmax;
  // Get MPI rank and comm size

  MPI_Comm_rank(MPI_COMM_WORLD,&ThisTask);
  MPI_Comm_size(MPI_COMM_WORLD,&NTasks);

  // Partition the meshes

  if (ThisTask == 0)
    printf("Communication open on all channels. Partitioning meshes.\n");

  ProcArray = malloc(NTasks * sizeof(int));
  ProcessorPartition1D(ProcArray, LATTICE_SIZE);
  
  if (ThisTask != NTasks - 1)
    rmax = ProcArray[ThisTask + 1] - ProcArray[ThisTask];
  else
    rmax = LATTICE_SIZE - ProcArray[ThisTask];
  // Allocate the meshes with one ghost row

  if (ThisTask == 0)
    printf("Allocating mesh memory...\n");

  // Last two rows are the bottom and top ghost rows respectively
  SpinMesh = MatrixMalloc(rmax + 2, (int)LATTICE_SIZE);
  OldSpinMesh = MatrixMalloc(rmax + 2, (int)LATTICE_SIZE);

  if (ThisTask==0)
    printf("Allocated %f MB / thread on %d threads to the meshes.\n", (double)ByteCount/1.e6, NTasks);
  
  // Set the RNG

  srand(SEED + ThisTask); // Unique to this task

  if (ThisTask == 0)
    printf("Generating initial spin configuration...\n");
  GenerateRandomSpins();
  
  MPI_Barrier(MPI_COMM_WORLD);
  if (ThisTask == 0)
    printf ("Getting ghost row...\n");
  //PrintMatrix(SpinMesh, rmax, LATTICE_SIZE);

  GetGhostRowBottom();
  GetGhostRowTop();
  SpinMatrixDeepCopy();

  return 0; // success
}

void GenerateRandomSpins()
{
  int i, j, rmax, val;

  if (ThisTask == NTasks - 1)
    {
      rmax = LATTICE_SIZE - ProcArray[ThisTask];
    }
  else
    {
      rmax = ProcArray[ThisTask+1] - ProcArray[ThisTask];
    }


  for (i=0; i < rmax+2; i++)
    for (j=0; j < LATTICE_SIZE+2; j++)
      {
	val = (short int) RandomIntInRange(0,1);
	if (val == 0)
	  val = -1;
	SpinMesh[i][j] = val;
	OldSpinMesh[i][j] = SpinMesh[i][j];
      }
}
