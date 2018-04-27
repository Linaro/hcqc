#include "header.hxx"
#include <math.h>

void runCRawLoops_HYDRO_2D (Index_type len, Real_ptr* za, Real_ptr* zb, Real_ptr* zm, Real_ptr* zp, Real_ptr* zq, Real_ptr* zr, Real_ptr* zu, Real_ptr* zv, Real_ptr* zz, Real_ptr* zrout, Real_ptr* zzout)
{
  const Real_type t = 0.0037;
  const Real_type s = 0.0041;

  Index_type kn = 6;
  Index_type jn = len;
  Index_type k;

  for ( k=1 ; k<kn ; k++ ) {
    for (Index_type j=1 ; j<jn ; j++ ) {
      za[k][j] = ( zp[k+1][j-1] +zq[k+1][j-1] -zp[k][j-1] -zq[k][j-1] )*
	( zr[k][j] +zr[k][j-1] ) / ( zm[k][j-1] +zm[k+1][j-1]);
      zb[k][j] = ( zp[k][j-1] +zq[k][j-1] -zp[k][j] -zq[k][j] ) *
	( zr[k][j] +zr[k-1][j] ) / ( zm[k][j] +zm[k][j-1]);
    }
  }

  for ( k=1 ; k<kn ; k++ ) {
    for (Index_type j=1 ; j<jn ; j++ ) {
      zu[k][j] += s*( za[k][j]   *( zz[k][j] - zz[k][j+1] ) -
		      za[k][j-1] *( zz[k][j] - zz[k][j-1] ) -
		      zb[k][j]   *( zz[k][j] - zz[k-1][j] ) +
		      zb[k+1][j] *( zz[k][j] - zz[k+1][j] ) );
      zv[k][j] += s*( za[k][j]   *( zr[k][j] - zr[k][j+1] ) -
		      za[k][j-1] *( zr[k][j] - zr[k][j-1] ) -
		      zb[k][j]   *( zr[k][j] - zr[k-1][j] ) +
		      zb[k+1][j] *( zr[k][j] - zr[k+1][j] ) );
    }
  }

  for ( k=1 ; k<kn ; k++ ) {
    for (Index_type j=1 ; j<jn ; j++ ) {
      zrout[k][j] = zr[k][j] + t*zu[k][j];
      zzout[k][j] = zz[k][j] + t*zv[k][j];
    }
  }
}
