#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
/*
  Declare global variables here
*/


// MPI varibles  

int ThisTask;
int NTasks;
int ByteCount;
int * ProcArray;
int TaskToGetFrom;
int TaskToSendTo;
int ReadToRow;
int SendFromRow;
int imax;
int * FirstRows;
int * LastRows;
int * flag;
MPI_Status * status;



// Mesh variables  

short int ** SpinMesh; // The mesh of spins, declared 2 bytes / element
short int ** OldSpinMesh; // Copy of last step to preserve Markovian property 
int rand_i;
int rand_j;
int deep_i;
int deep_j;
int deep_max;

// The Energies

double Ediff;
double TopSpin;
double BottomSpin;
double LeftSpin;
double RightSpin;

// Output

FILE * fileout;
