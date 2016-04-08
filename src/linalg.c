#include "../include/allvars.h"
#include "../include/proto.h"
#include "../include/mc_options.h"
#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>
#include <math.h>


/*
  Compute the Euclidean norm of a vector
*/

double Norm(short int * a,int N)
{
  double norm;
  norm = Dot(a,a,N);
  norm = pow(norm, 0.5);
  return norm;
}


/*
  Just the dot product between two vectors
*/

double Dot(short int * a, short int * b, int N)
{
  int i;
  double dot = 0;
  for (i=0; i<N; i++)
    {
      dot += (double)a[i]*(double)b[i];
    }
  return dot;
}


/*
  Allocate memory to an N X M short int matrix
*/
short int ** MatrixMalloc(int N, int M)
{
  int i;
  short int ** mat = malloc(N * sizeof(short int*));
  ByteCount += N * sizeof(short int *);
  for (i=0; i<N; i++)
    {
      mat[i] = malloc(M * sizeof(short int));
      ByteCount += M * sizeof(short int);
    }
  return mat;
}

/*
  Free an N X M matrix
*/

void MatrixFree(short int ** mat, int N, int M)
{
  int i;

  for (i=0; i < N; i++)
    {
      free(mat[i]);
      ByteCount -= M * sizeof(short int);
    }

  ByteCount -= N * sizeof(short int);

  free(mat);
}


/*
  Allocate memory to an N-dim vector
*/

short int * VectorMalloc(int N)
{
  short int * vec = malloc(N * sizeof(short int));
  ByteCount += N * sizeof(short int);
  return vec;
}

/*
  Free an N-dim vector
*/

void VectorFree(short int * vec, int N)
{
  ByteCount -= N * sizeof(short int);
  free(vec);
}


void PrintMatrix(short int ** matrix, int n, int m)
{
  int i,j;
  short int element;
  printf("Matrix on Task %d: \n", ThisTask);
  for (j=0; j<m; j++)
    {
      for (i=0; i<n; i++)
        {
          element = matrix[i][j];

          if (i == 0)
            {
              printf("[%d, ", (int)element);
            }
          else if (i == n - 1)
            {
              printf(",%d]\n", (int)element);
            }
          else
            {
              printf("%d, ", (int)element);
            }
        }
    }
}


/*
  Print to file
*/

void PrintMatrixToFile(short int ** matrix, int n, int m)
{
  int i,j,  sizeofbase, sizeofrank, nchar_printed;
  short int element, min, max;
  char * filename;

  char * filebase = "OUTPUTS/ising.";

  min = ProcArray[ThisTask];
  if (ThisTask != NTasks -1)
    max = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    max = LATTICE_SIZE - ProcArray[ThisTask];

  sizeofbase = 14 * sizeof(char);
  sizeofrank = 0;

  if (ThisTask < 10)
    {
      sizeofrank = 1 * sizeof(char);
    }
  else if(ThisTask < 100)
    {
      sizeofrank = 2 * sizeof(char);
    }
  else if(ThisTask < 1000)
    {
      sizeofrank = 3 * sizeof(char);
    }
  else if(ThisTask < 10000)
    {
      sizeofrank = 4 * sizeof(char);
    }
  char * rank = malloc(sizeofrank);

  filename = malloc(sizeofbase + sizeofrank);
  for (i = 0; i < 14; i++)
    {
      filename[i] = filebase[i];
    }

  MPI_Barrier(MPI_COMM_WORLD);
  nchar_printed = snprintf(rank,5,"%d",ThisTask);
  MPI_Barrier(MPI_COMM_WORLD);

  for (i=14; i < 15 + nchar_printed; i++)
    {
      filename[i] = rank[i-14];
    }
  fileout = fopen(filename, "wb");

  //fwrite(&min, sizeof(int), 1, fileout);
  //fwrite(&max, sizeof(int), 1, fileout);

  for (i=0; i<imax; i++)
    {
      for (j=0; j<LATTICE_SIZE; j++)
        {
          element = matrix[i][j];
          fwrite(&element, sizeof(short int), 1, fileout);
        }
    }

  fclose(fileout);
  free(filename);
}

/*
  Copy spin mesh to old spin mesh
*/

void SpinMatrixDeepCopy()
{
  //int i, j, max;

  // Copy ghost row too
  if (ThisTask != NTasks-1)
    deep_max = ProcArray[ThisTask + 1] - ProcArray[ThisTask] + 2;
  else
    deep_max = LATTICE_SIZE - ProcArray[ThisTask] + 2;

  for (deep_i=0; deep_i < deep_max; deep_i++)
    for (deep_j=0; deep_j < LATTICE_SIZE; deep_j++)
      {
	//printf("i,j = %d, %d\n", i,j);
	OldSpinMesh[deep_i][deep_j] = SpinMesh[deep_i][deep_j];
      }
  //printf("Matrix copied.\n");
}
