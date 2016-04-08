#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/mc_options.h"
#include <stdio.h>
#include <stdlib.h>

int Finalize()
{
  int imax;
  if (ThisTask != NTasks)
    imax = ProcArray[ThisTask + 1] - ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];
  PrintMatrixToFile(SpinMesh, imax, LATTICE_SIZE);

  //MatrixFree(SpinMesh);
  //MatrixFree(OldSpinMesh);
  free(SpinMesh);
  free(OldSpinMesh);
  free(ProcArray);
  return 0; // Success
}
