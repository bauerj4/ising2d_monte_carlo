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

  FirstRows = VectorMalloc(LATTICE_SIZE * NTasks);
  LastRows = VectorMalloc(LATTICE_SIZE * NTasks);

  if (ThisTask == 0)
    printf("Communication open on all channels. Partitioning meshes.\n");

  ProcArray = malloc(NTasks * sizeof(int));
  ProcessorPartition1D(ProcArray, LATTICE_SIZE);
  
  if (ThisTask != NTasks - 1)
    rmax = ProcArray[ThisTask + 1] - ProcArray[ThisTask];
  else
    rmax = LATTICE_SIZE - ProcArray[ThisTask];

  Temperature = T_MIN;
  Tstep = (T_MAX - T_MIN)/((double)NTEVALS);
  T_arr = malloc(NTEVALS * sizeof(double));
  E_arr= malloc(NTEVALS * sizeof(double));
  M_arr= malloc(NTEVALS * sizeof(double));
  C_arr= malloc(NTEVALS * sizeof(double));

  // Allocate the meshes with one ghost row

  if (ThisTask == 0)
    printf("Allocating mesh memory...\n");

  // Last two rows are the bottom and top ghost rows respectively
  SpinMesh = MatrixMalloc(rmax , (int)LATTICE_SIZE);
  OldSpinMesh = MatrixMalloc(rmax , (int)LATTICE_SIZE);

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

  //GetGhostRowBottom();
  // GetGhostRowTop();

  MPI_Allgather(SpinMesh[rmax-1],LATTICE_SIZE,MPI_SHORT,LastRows,LATTICE_SIZE,MPI_SHORT,MPI_COMM_WORLD);
  MPI_Allgather(SpinMesh[0],LATTICE_SIZE,MPI_SHORT,FirstRows,LATTICE_SIZE,MPI_SHORT,MPI_COMM_WORLD);

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


  for (i=0; i < rmax; i++)
    for (j=0; j < LATTICE_SIZE; j++)
      {
	val = (short int) RandomIntInRange(0,1);
	if (val == 0)
	  val = -1;
#ifndef START_MAGNETIZED
	SpinMesh[i][j] = val;
	OldSpinMesh[i][j] = SpinMesh[i][j];
#else
	SpinMesh[i][j] = -1;
	OldSpinMesh[i][j] = -1;
#endif
      }
}
