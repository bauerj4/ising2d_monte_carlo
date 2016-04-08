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
extern int * FirstRows;
extern int * LastRows;
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


// Output

extern FILE * fileout;
