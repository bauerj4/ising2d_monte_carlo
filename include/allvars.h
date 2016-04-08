#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

/*
  All global variables defined elsewhere,
  but namely in allvars.c
*/


// MPI varibles

extern int ThisTask;
extern int NTasks;
extern int ByteCount;
extern int * ProcArray;
extern int TaskToGetFrom;
extern int TaskToSendTo;
extern int ReadToRow;
extern int SendFromRow;
extern int imax;
extern short int * FirstRows;
extern short int * LastRows;
extern int * flag;
extern MPI_Status * status;


// Mesh variables

extern short int ** SpinMesh; // The mesh of spins, declared 2 bytes / element
extern short int ** OldSpinMesh; // Copy of last step to preserve Markovian property
extern int rand_i;
extern int rand_j;
extern int deep_i;
extern int deep_j;
extern int deep_max;

// The Energies

extern double Ediff;
extern double TopSpin;
extern double BottomSpin;
extern double LeftSpin;
extern double RightSpin;


// Chain parameters

extern int TheseIterations; // Number of iterations to do this time
extern int currentStep;
extern double Temperature;
extern double Tstep;
extern double * T_arr; // Temp
extern double * E_arr; // Energy
extern double * M_arr; // Magnetization
extern double * C_arr; // Heat capacity

// Output

extern FILE * fileout;
