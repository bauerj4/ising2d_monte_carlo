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

  if (ThisTask == 0)
    {
      printf("\nWriting temperature.bin...\n");
      fileout = fopen("OUTPUTS/temperature.bin","wb");
      fwrite(T_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);

      printf("Writing energy.bin...\n");
      fileout = fopen("OUTPUTS/energy.bin","wb");
      fwrite(E_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);

      printf("Writing magnetization.bin...\n");
      fileout = fopen("OUTPUTS/magnetization.bin","wb");
      fwrite(M_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);

      printf("Writing heat_capacity.bin...\n");
      fileout = fopen("OUTPUTS/heat_capacity.bin","wb");
      fwrite(C_arr, sizeof(double), NTEVALS, fileout);
      fclose(fileout);
    }
  
  //MatrixFree(SpinMesh);
  //MatrixFree(OldSpinMesh);
  free(SpinMesh);
  free(OldSpinMesh);
  free(ProcArray);
  return 0; // Success
}
