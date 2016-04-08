#include "../include/allvars.h"
#include "../include/mc_options.h"
#include "../include/proto.h"
#include <stdlib.h>
#include <stdio.h>

/*
  All Ising-specific functions are here
*/


/*
  Gets the change in energy imposing PBC
*/
double DeltaU(int i, int j)
{
  int FirstRowIdx; // The index from which to read FirstRows
  int LastRowIdx; // The index from which to read LastRows

  if (ThisTask != NTasks -1)
    FirstRowIdx = (ThisTask + 1)*LATTICE_SIZE;
  else
    FirstRowIdx = 0;
  
  if (ThisTask != 0)
    LastRowIdx = (ThisTask - 1)*LATTICE_SIZE;
  else
    LastRowIdx = (NTasks - 1)*LATTICE_SIZE;
  
  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1]-ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  if (i == 0)
    TopSpin = LastRows[LastRowIdx + j];
  else
    TopSpin = OldSpinMesh[i-1][j];

  if (i == imax - 1)
    BottomSpin = FirstRows[FirstRowIdx + j];
  else
    BottomSpin = OldSpinMesh[i+1][j];

  if (j == 0)
    LeftSpin = OldSpinMesh[i][LATTICE_SIZE - 1];
  else
    LeftSpin = OldSpinMesh[i][j-1];

  if (j == LATTICE_SIZE - 1)
    RightSpin = OldSpinMesh[i][0];
  else
    RightSpin = OldSpinMesh[i][j+1];
  
  Ediff = 2. * OldSpinMesh[i][j] * (TopSpin + BottomSpin +\
				    LeftSpin + RightSpin);
  return Ediff;
}

void TotalEnergy()
{
  int e_i, e_j;
  double energy = 0.;

  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  for (e_i = 0; e_i < imax; e_i++)
    for (e_j=0; e_j < LATTICE_SIZE; e_j++)
      energy += DeltaU(e_i, e_j)/2.;


  MPI_Allreduce(&energy, &E_arr[currentStep], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  E_arr[currentStep] /= LATTICE_SIZE*LATTICE_SIZE;
}

void TotalMagnetization()
{
  int e_i, e_j;
  double magnetization = 0.;

  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  for (e_i = 0; e_i < imax; e_i++)
    for (e_j=0; e_j < LATTICE_SIZE; e_j++)
      magnetization += SpinMesh[e_i][e_j];


  MPI_Allreduce(&magnetization, &M_arr[currentStep], 1, MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
  M_arr[currentStep] /= LATTICE_SIZE*LATTICE_SIZE;
}


