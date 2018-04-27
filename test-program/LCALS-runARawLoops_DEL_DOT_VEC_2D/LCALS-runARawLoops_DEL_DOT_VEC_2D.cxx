#include "header.hxx"
#include <math.h>

double ADomain::loop_length_factor = 1.0;

void runARawLoops_DEL_DOT_VEC_2D (LoopLength ilength, UnalignedReal_ptr x1, UnalignedReal_ptr x2, UnalignedReal_ptr x3, UnalignedReal_ptr x4, UnalignedReal_ptr y1, UnalignedReal_ptr y2, UnalignedReal_ptr y3, UnalignedReal_ptr y4, UnalignedReal_ptr fx1, UnalignedReal_ptr fx2, UnalignedReal_ptr fx3, UnalignedReal_ptr fx4, UnalignedReal_ptr fy1, UnalignedReal_ptr fy2, UnalignedReal_ptr fy3, UnalignedReal_ptr fy4, Real_ptr div)
{
  ADomain domain(ilength, /* ndims = */ 2);
  const Real_type ptiny = 1.0e-20;
  const Real_type half  = 0.5;

  for (Index_type ii = 0 ; ii < domain.n_real_zones ; ii++ ) {

    Index_type i  = domain.real_zones[ii] ;

    Real_type xi  = half * ( x1[i]  + x2[i]  - x3[i]  - x4[i]  ) ;
    Real_type xj  = half * ( x2[i]  + x3[i]  - x4[i]  - x1[i]  ) ;

    Real_type yi  = half * ( y1[i]  + y2[i]  - y3[i]  - y4[i]  ) ;
    Real_type yj  = half * ( y2[i]  + y3[i]  - y4[i]  - y1[i]  ) ;

    Real_type fxi = half * ( fx1[i] + fx2[i] - fx3[i] - fx4[i] ) ;
    Real_type fxj = half * ( fx2[i] + fx3[i] - fx4[i] - fx1[i] ) ;

    Real_type fyi = half * ( fy1[i] + fy2[i] - fy3[i] - fy4[i] ) ;
    Real_type fyj = half * ( fy2[i] + fy3[i] - fy4[i] - fy1[i] ) ;

    Real_type rarea  = 1.0 / ( xi * yj - xj * yi + ptiny ) ;

    Real_type dfxdx  = rarea * ( fxi * yj - fxj * yi ) ;

    Real_type dfydy  = rarea * ( fyj * xi - fyi * xj ) ;

    Real_type affine = ( fy1[i] + fy2[i] + fy3[i] + fy4[i] ) /
      ( y1[i]  + y2[i]  + y3[i]  + y4[i]  ) ;

    div[i] = dfxdx + dfydy + affine ;

  }
}
