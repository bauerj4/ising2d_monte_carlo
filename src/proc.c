#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/mc_options.h"
#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>

// Partition N elements in an even load; N need not divide NTasks

void ProcessorPartition1D(int * partition_arr, int N)
{
  int n,nremainder, mymin;
  n = N/NTasks;
  nremainder = N % NTasks;

  if (ThisTask < nremainder)
    {
      mymin = ThisTask * (n + 1);
    }
  else
    {
      mymin = n * ThisTask + nremainder;
    }

  printf("I am thread %d / %d and my min is %d\n", ThisTask + 1, NTasks, mymin);
  MPI_Allgather(&mymin, 1, MPI_INT, partition_arr, 1, MPI_INT, MPI_COMM_WORLD);
}

void GetGhostRowTop()
{
  //MPI_Request request;
  /*
  int TaskToGetFrom;
  int TaskToSendTo;
  int ReadToRow;
  int SendFromRow;
  int imax;
  MPI_Status * status;*/
  int size = LATTICE_SIZE + 2* MPI_BSEND_OVERHEAD;

  if (ThisTask != NTasks - 1)
    imax = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    imax = LATTICE_SIZE - ProcArray[ThisTask];

  if (ThisTask != NTasks - 1)
    ReadToRow = ProcArray[ThisTask+1] - ProcArray[ThisTask] + 1;
  else
    ReadToRow = LATTICE_SIZE - ProcArray[ThisTask] + 1;

  if (ThisTask != NTasks - 1)
    TaskToSendTo = ThisTask + 1;
  else
    TaskToSendTo = 0;

  if (ThisTask != 0)
    TaskToGetFrom = ThisTask - 1;
  else
    TaskToGetFrom = NTasks - 1;

  SendFromRow = imax - 1;
  
  //printf("Thread %d receiving from thread %d to row %d from row %d...\n", ThisTask, TaskToGetFrom, ReadToRow, SendFromRow);  

  MPI_Buffer_attach(SpinMesh[SendFromRow], size);
  MPI_Send(SpinMesh[SendFromRow], LATTICE_SIZE, MPI_SHORT, TaskToSendTo, 0, MPI_COMM_WORLD);
  MPI_Recv(SpinMesh[ReadToRow], LATTICE_SIZE, MPI_SHORT, TaskToGetFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //MPI_Test(request, flag, status);
  //MPI_Wait(MPI_REQUEST_NULL, status);
  //MPI_Request_free(&request);
  MPI_Buffer_detach(SpinMesh[SendFromRow], &size);
  MPI_Barrier(MPI_COMM_WORLD);
}

void GetGhostRowBottom()
{
  //MPI_Request request;

  /*
  int TaskToGetFrom;
  int TaskToSendTo;
  int ReadToRow;
  int SendFromRow;
  MPI_Status * status;
  */
  int size = LATTICE_SIZE + 2* MPI_BSEND_OVERHEAD;


  if (ThisTask != NTasks - 1)
    ReadToRow = ProcArray[ThisTask+1] - ProcArray[ThisTask];
  else
    ReadToRow = LATTICE_SIZE - ProcArray[ThisTask];

  if (ThisTask != 0)
    TaskToSendTo = ThisTask - 1;
  else
    TaskToSendTo = NTasks - 1;

  TaskToGetFrom = (ThisTask + 1) % NTasks; // Periodicity

  if (ThisTask != NTasks -1)
    SendFromRow = 0;//ProcArray[ThisTask+1]-ProcArray[ThisTask];
  else
    SendFromRow = 0;//LATTICE_SIZE - ProcArray[ThisTask];

  // Non-blocking send, non-blocking receive, thread barrier

  //printf("Sending from thread %d to thread %d...\n", ThisTask, TaskToSendTo);
  MPI_Buffer_attach(SpinMesh[SendFromRow],size);
  MPI_Send(SpinMesh[SendFromRow], LATTICE_SIZE, MPI_SHORT, TaskToSendTo, 0, MPI_COMM_WORLD);
  //printf("Thread %d receiving from thread %d to row %d from row %d...\n", ThisTask, TaskToGetFrom, ReadToRow, SendFromRow);
  MPI_Recv(SpinMesh[ReadToRow], LATTICE_SIZE, MPI_SHORT, TaskToGetFrom, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  //printf("Thread %d is done communicating.\n", ThisTask);
  //MPI_Test(request, flag, status);
  //MPI_Wait(MPI_REQUEST_NULL, status);
  //MPI_Request_free(&request);
  MPI_Buffer_detach(SpinMesh[SendFromRow], &size);
  MPI_Barrier(MPI_COMM_WORLD);
}
