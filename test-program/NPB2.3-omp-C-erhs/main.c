#include <stdlib.h>

#include "header.h"

int nx, ny, nz;
int nx0, ny0, nz0;
int ist, iend;
int jst, jend;
double tx1, tx2, tx3;
double ty1, ty2, ty3;
double tz1, tz2, tz3;
double dx1, dx2, dx3, dx4, dx5;
double dy1, dy2, dy3, dy4, dy5;
double dz1, dz2, dz3, dz4, dz5;
double dssp;

double ce[5][13];

double rsd[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];
double frct[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];
double flux[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];

extern void erhs (void);

int main ()
{
#if 0
  /* TODO: */
  erhs ()
#endif
  exit (0);
}
