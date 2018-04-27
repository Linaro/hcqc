#include "header.hxx"
#include <math.h>

void runCRawLoops_PIC_2D (Index_type len, Real_ptr* p, Real_ptr* b, Real_ptr* c, Real_ptr y, Real_ptr z, Index_type* e, Index_type* f, Real_ptr* h)
{
  for (Index_type ip=0 ; ip<len ; ip++ ) {
    Index_type i1, j1, i2, j2;
    i1 = (Index_type) p[ip][0];
    j1 = (Index_type) p[ip][1];
    i1 &= 64-1;
    j1 &= 64-1;
    p[ip][2] += b[j1][i1];
    p[ip][3] += c[j1][i1];
    p[ip][0] += p[ip][2];
    p[ip][1] += p[ip][3];
    i2 = (Index_type) p[ip][0];
    j2 = (Index_type) p[ip][1];
    i2 = ( i2 & 64-1 ) ;
    j2 = ( j2 & 64-1 ) ;
    p[ip][0] += y[i2+32];
    p[ip][1] += z[j2+32];
    i2 += e[i2+32];
    j2 += f[j2+32];
    h[j2][i2] += 1.0;
  }
}
