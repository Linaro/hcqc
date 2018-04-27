#include "header.hxx"
#include <math.h>

void runCRawLoops_PIC_1D (Index_type len, Real_ptr vx, Real_ptr xx, Real_ptr xi, Real_ptr ex, Real_ptr ex1, Real_ptr dex, Real_ptr dex1, Real_ptr rh, Real_ptr rx, const Real_type flx, Index_type* ix, Index_type* ir, Index_type* grd)
{
  for (Index_type k=0 ; k<len ; k++ ) {
    vx[k] = 0.0;
    xx[k] = 0.0;
    ix[k] = (Index_type) grd[k];
    xi[k] = (Real_type) ix[k];
    ex1[k] = ex[ ix[k] - 1 ];
    dex1[k] = dex[ ix[k] - 1 ];
  }

  for (Index_type k=0 ; k<len ; k++ ) {
    vx[k] = vx[k] + ex1[k] + ( xx[k] - xi[k] )*dex1[k];
    xx[k] = xx[k] + vx[k]  + flx;
    ir[k] = (Index_type) xx[k];
    rx[k] = xx[k] - ir[k];
    ir[k] = ( ir[k] & (2048-1) ) + 1;
    xx[k] = rx[k] + ir[k];
  }

  for (Index_type k=0 ; k<len ; k++ ) {
    rh[ ir[k]-1 ] += 1.0 - rx[k];
    rh[ ir[k]   ] += rx[k];
  }
}
