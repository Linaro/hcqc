#include <stdio.h>
#include <stdlib.h>

#define STORAGE_CLASS
#include "header.h"

static void initmt();
extern float jacobi(int nn);

static void
initmt()
{
	int i,j,k;

  imax = MIMAX-1;
  jmax = MJMAX-1;
  kmax = MKMAX-1;

  for(i=0 ; i<MIMAX ; i++)
    for(j=0 ; j<MJMAX ; j++)
      for(k=0 ; k<MKMAX ; k++){
        a[0][i][j][k]=0.0;
        a[1][i][j][k]=0.0;
        a[2][i][j][k]=0.0;
        a[3][i][j][k]=0.0;
        b[0][i][j][k]=0.0;
        b[1][i][j][k]=0.0;
        b[2][i][j][k]=0.0;
        c[0][i][j][k]=0.0;
        c[1][i][j][k]=0.0;
        c[2][i][j][k]=0.0;
        p[i][j][k]=0.0;
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=0.0;
      }

  for(i=0 ; i<imax ; i++)
    for(j=0 ; j<jmax ; j++)
      for(k=0 ; k<kmax ; k++){
        a[0][i][j][k]=1.0;
        a[1][i][j][k]=1.0;
        a[2][i][j][k]=1.0;
        a[3][i][j][k]=1.0/6.0;
        b[0][i][j][k]=0.0;
        b[1][i][j][k]=0.0;
        b[2][i][j][k]=0.0;
        c[0][i][j][k]=1.0;
        c[1][i][j][k]=1.0;
        c[2][i][j][k]=1.0;
        p[i][j][k]=(float)(i*i)/(float)((imax-1)*(imax-1));
        wrk1[i][j][k]=0.0;
        bnd[i][j][k]=1.0;
      }
}

int main()
{
  int nn;
  float  gosa;

  initmt();
  nn = 3;
  gosa = jacobi(nn);
#if 0
  printf ("%e\n", gosa);
#endif

  if (6.710e-3 < gosa && gosa < 6.714e-3) {
    exit (0);
  } else {
    exit (1);
  }
}
