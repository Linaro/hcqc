#include <stdio.h>
#include <stdlib.h>

struct Mat {
  float* m;
  int mnums;
  int mrows;
  int mcols;
  int mdeps;
};

/* prototypes */
typedef struct Mat Matrix;

extern float jacobi(int nn, Matrix* a,Matrix* b,Matrix* c, Matrix* p,Matrix* bnd,Matrix* wrk1,Matrix* wrk2);

int main()
{
#if 0
  gosa = jacobi(nn,&a,&b,&c,&p,&bnd,&wrk1,&wrk2);
#endif
  exit (0);
}
