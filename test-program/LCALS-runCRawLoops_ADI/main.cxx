#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "header.hxx"

extern void runCRawLoops_ADI (Index_type len, Real_ptr du1, Real_ptr du2, Real_ptr du3, Real_ptr** u1, Real_ptr** u2, Real_ptr** u3, const Real_type sig, const Real_type a11, const Real_type a12, const Real_type a13, const Real_type a21, const Real_type a22, const Real_type a23, const Real_type a31, const Real_type a32, const Real_type a33);

int main ()
{
#if 0
  runCRawLoops_ADI (len, du1, du2, du3, u1, u2, u3, sig, a11, a12, a13, a21, a22, a23, a31, a32, a33);
#endif
  exit (0);
}
