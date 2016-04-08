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
  //int imax;
  //double Ediff;
  //printf("Ising (i,j) = %d, %d\n",i,j);


  //PrintMatrix(&FirstRows, 1, LATTICE_SIZE * NTasks);
  //PrintMatrix(&LastRows, 1, LATTICE_SIZE * NTasks);

  if (ThisTask != NTasks -1)
    FirstRowIdx = (ThisTask + 1)*LATTICE_SIZE;
  else
    FirstRowIdx = 0;
  
  if (ThisTask != 0)
    LastRowIdx = (ThisTask - 1)*LATTICE_SIZE;
  else
    LastRowIdx = (NTasks - 1)*LATTICE_SIZE;
  
  //printf("Thread %d searching idx pair First, Last = %d, %d \n",ThisTask, FirstRowIdx+j, LastRowIdx +j);
  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1]-ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  if (i == 0)
    TopSpin = LastRows[LastRowIdx + j];
  else
    TopSpin = OldSpinMesh[i-1][j];

  //printf("Got top spin on thread %d\n", ThisTask);
  if (i == imax - 1)
    BottomSpin = FirstRows[FirstRowIdx + j];
  else
    BottomSpin = OldSpinMesh[i+1][j];
  //printf("Got bottom spin on thread %d\n", ThisTask);

  if (j == 0)
    LeftSpin = OldSpinMesh[i][LATTICE_SIZE - 1];
  else
    LeftSpin = OldSpinMesh[i][j-1];
  //printf("Got left spin on thread %d\n", ThisTask);

  if (j == LATTICE_SIZE - 1)
    RightSpin = OldSpinMesh[i][0];
  else
    RightSpin = OldSpinMesh[i][j+1];
  //printf("Got right spin on thread %d\n", ThisTask);

  
  Ediff = 2. * OldSpinMesh[i][j] * (TopSpin + BottomSpin +\
				    LeftSpin + RightSpin);
  return Ediff;
}
