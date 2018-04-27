#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "header.hxx"

extern void runARawLoops_COUPLE (Index_type imin, Index_type imax, Index_type jmin, Index_type jmax, Index_type kmin, Index_type kmax, Complex_ptr t0, Complex_ptr t1, Complex_ptr t2, Complex_ptr denac, Complex_ptr denlw);

int main ()
{
#if 0
  runARawLoops_COUPLE (imin, imax, jmin, jmax, kmin, kmax, t0, t1, t2, denac, denlw);
#endif
  exit (0);
}
