#include "../include/proto.h"
#include "../include/allvars.h"
#include "../include/mc_options.h"
#include <stdlib.h>
#include <stdio.h>

/*
  Functions to do with the RNG and stats
*/

int RandomIntInRange(int a, int b)
{
  // val is just d in [0, RAND_MAX] mod (a-b + 1) + b

  return  (random() % (b+1-a));
}

double Uniform()
{
  return ((double) random() / (double) RAND_MAX);
}
