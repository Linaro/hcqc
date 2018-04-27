#include "header.hxx"
#include <math.h>

double ADomain::loop_length_factor = 1.0;

void runARawLoops_VOL3D_CALC (LoopLength ilength, Real_ptr vol, UnalignedReal_ptr x0, UnalignedReal_ptr x1, UnalignedReal_ptr x2, UnalignedReal_ptr x3, UnalignedReal_ptr x4, UnalignedReal_ptr x5, UnalignedReal_ptr x6, UnalignedReal_ptr x7, UnalignedReal_ptr y0, UnalignedReal_ptr y1, UnalignedReal_ptr y2, UnalignedReal_ptr y3, UnalignedReal_ptr y4, UnalignedReal_ptr y5, UnalignedReal_ptr y6, UnalignedReal_ptr y7, UnalignedReal_ptr z0, UnalignedReal_ptr z1, UnalignedReal_ptr z2, UnalignedReal_ptr z3, UnalignedReal_ptr z4, UnalignedReal_ptr z5, UnalignedReal_ptr z6, UnalignedReal_ptr z7)
{
  ADomain domain(ilength, /* ndims = */ 3);
  const Real_type vnormq = 0.083333333333333333; /* vnormq = 1/12 */

  for (Index_type i = domain.fpz ; i <= domain.lpz ; i++ ) {
    Real_type x71 = x7[i] - x1[i] ;
    Real_type x72 = x7[i] - x2[i] ;
    Real_type x74 = x7[i] - x4[i] ;
    Real_type x30 = x3[i] - x0[i] ;
    Real_type x50 = x5[i] - x0[i] ;
    Real_type x60 = x6[i] - x0[i] ;

    Real_type y71 = y7[i] - y1[i] ;
    Real_type y72 = y7[i] - y2[i] ;
    Real_type y74 = y7[i] - y4[i] ;
    Real_type y30 = y3[i] - y0[i] ;
    Real_type y50 = y5[i] - y0[i] ;
    Real_type y60 = y6[i] - y0[i] ;

    Real_type z71 = z7[i] - z1[i] ;
    Real_type z72 = z7[i] - z2[i] ;
    Real_type z74 = z7[i] - z4[i] ;
    Real_type z30 = z3[i] - z0[i] ;
    Real_type z50 = z5[i] - z0[i] ;
    Real_type z60 = z6[i] - z0[i] ;

    Real_type xps = x71 + x60 ;
    Real_type yps = y71 + y60 ;
    Real_type zps = z71 + z60 ;

    Real_type cyz = y72 * z30 - z72 * y30 ;
    Real_type czx = z72 * x30 - x72 * z30 ;
    Real_type cxy = x72 * y30 - y72 * x30 ;
    vol[i] = xps * cyz + yps * czx + zps * cxy ; 

    xps = x72 + x50 ;
    yps = y72 + y50 ;
    zps = z72 + z50 ;

    cyz = y74 * z60 - z74 * y60 ;
    czx = z74 * x60 - x74 * z60 ;
    cxy = x74 * y60 - y74 * x60 ;
    vol[i] += xps * cyz + yps * czx + zps * cxy ;

    xps = x74 + x30 ;
    yps = y74 + y30 ;
    zps = z74 + z30 ;

    cyz = y71 * z50 - z71 * y50 ;
    czx = z71 * x50 - x71 * z50 ;
    cxy = x71 * y50 - y71 * x50 ;
    vol[i] += xps * cyz + yps * czx + zps * cxy ;

    vol[i] *= vnormq ;

  }
}
