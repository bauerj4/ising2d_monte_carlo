/*
  Proc
*/

void ProcessorPartition1D(int * partition_arr, int N);
void GetGhostRowBottom();
void GetGhostRowTop();


/*
  Initialize
*/

int Initialize();
void GenerateRandomSpins();

/*
  Finalize
*/

int Finalize();

/*
  linalg
*/

double Norm(short int * a,int N);
double Dot(short int * a, short int * b, int N);
short int ** MatrixMalloc(int N, int M);
void MatrixFree(short int ** mat, int N, int M);
short int * VectorMalloc(int N);
void VectorFree(short int * vec, int N);
void PrintMatrix(short int ** matrix, int n, int m);
void SpinMatrixDeepCopy();
void PrintMatrixToFile(short int ** matrix, int n, int m);


/*
  Stats
*/

int RandomIntInRange(int a, int b);
double Uniform();

/*
  Ising
*/

double DeltaU(int i, int j);

/*
  Metropolis
*/

int Metropolis();
