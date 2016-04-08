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
  //int imax;
  //double Ediff;
  
  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1]-ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  if (i == 0)
    TopSpin = OldSpinMesh[imax + 1][j];
  else
    TopSpin = OldSpinMesh[i-1][j];

  if (i == imax - 1)
    BottomSpin = OldSpinMesh[imax][j];
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
