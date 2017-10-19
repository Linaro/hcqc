#include <stdio.h>
#include <stdlib.h>

extern void initmt();
extern float jacobi(int nn);

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

  if (6.710e-3 < gosa && gosa < 6.712e-3) {
    exit (0);
  } else {
    exit (1);
  }
}
