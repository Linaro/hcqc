#include "header.hxx"
#include <math.h>

void runCRawLoops_ADI (Index_type len, Real_ptr du1, Real_ptr du2, Real_ptr du3, Real_ptr** u1, Real_ptr** u2, Real_ptr** u3, const Real_type sig, const Real_type a11, const Real_type a12, const Real_type a13, const Real_type a21, const Real_type a22, const Real_type a23, const Real_type a31, const Real_type a32, const Real_type a33)
{

  Index_type nl1 = 0;
  Index_type nl2 = 1;
  Index_type kx;

  for ( kx=1 ; kx<3 ; kx++ ) {
    for (Index_type ky=1 ; ky<len ; ky++ ) {
      du1[ky] = u1[nl1][ky+1][kx] - u1[nl1][ky-1][kx];
      du2[ky] = u2[nl1][ky+1][kx] - u2[nl1][ky-1][kx];
      du3[ky] = u3[nl1][ky+1][kx] - u3[nl1][ky-1][kx];
      u1[nl2][ky][kx]=
	u1[nl1][ky][kx]+a11*du1[ky]+a12*du2[ky]+a13*du3[ky] + sig*
	(u1[nl1][ky][kx+1]-2.0*u1[nl1][ky][kx]+u1[nl1][ky][kx-1]);
      u2[nl2][ky][kx]=
	u2[nl1][ky][kx]+a21*du1[ky]+a22*du2[ky]+a23*du3[ky] + sig*
	(u2[nl1][ky][kx+1]-2.0*u2[nl1][ky][kx]+u2[nl1][ky][kx-1]);
      u3[nl2][ky][kx]=
	u3[nl1][ky][kx]+a31*du1[ky]+a32*du2[ky]+a33*du3[ky] + sig*
	(u3[nl1][ky][kx+1]-2.0*u3[nl1][ky][kx]+u3[nl1][ky][kx-1]);
    }
  }
}
