#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "header.hxx"

extern void runCRawLoops_HYDRO_2D (Index_type len, Real_ptr* za, Real_ptr* zb, Real_ptr* zm, Real_ptr* zp, Real_ptr* zq, Real_ptr* zr, Real_ptr* zu, Real_ptr* zv, Real_ptr* zz, Real_ptr* zrout, Real_ptr* zzout);

int main ()
{
#if 0
  runCRawLoops_HYDRO_2D (len, za, zb, zm, zp, zq, zr, zu, zv, zz, zrout, zzout);
#endif
  exit (0);
}
