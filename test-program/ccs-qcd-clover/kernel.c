#include <math.h>
#include <complex.h>

#define _NTX  64
#define _NTY  64
#define _NTZ  64
#define _NTT  32

#define _NDIMX  8
#define _NDIMY  8
#define _NDIMZ  8

#define NDIMX _NDIMX
#define NDIMY _NDIMY
#define NDIMZ _NDIMZ

#define NTX _NTX
#define NTY _NTY
#define NTZ _NTZ
#define NTT _NTT

#define NX (NTX/NDIMX)
#define NY (NTY/NDIMY)
#define NZ (NTZ/NDIMZ)
#define NT NTT
#define NX1 (NX+1)
#define NY1 (NY+1)
#define NZ1 (NZ+1)
#define NT1 (NT+1)

#define NTH (NT/2)
#define NDIM 4
#define COL 3
#define SPIN (2<<(NDIM/2))
#define CLSP COL*SPIN
#define CLSPH ((CLSP/2+1)*(CLSP/4))

#define MULT_WD              (1896)
#define MULT_MB_PRE          (1896*2+48)
#define QMULT2C_ACCUM_ADD    (96)
#define QMULT1C_ACCUM_ADD    (96)
#define QPROD                (96)
#define QABS2                (48)
#define QSUB                 (24)
#define QADD                 (24)

int isx[NDIM+1];
int isy[NDIM+1];
int isz[NDIM+1];
int ist[NDIM+1];
int imunu[NDIM+1][NDIM+1];

int ipeo = 0;

void clover (double _Complex ucle_t[COL+1][COL+1][NTH+1][NZ+1][NY+1][NX+1][(NDIM*(NDIM-1)/2)+1],
	     double _Complex uclo_t[COL+1][COL+1][NTH+1][NZ+1][NY+1][NX+1][(NDIM*(NDIM-1)/2)+1],
	     double _Complex ue_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1],
	     double _Complex uo_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1])
{
  double _Complex wce_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1];
  double _Complex wco_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1];
  double _Complex ve_t[COL+1][COL+1][NTH+1][NZ+1][NY+1][NX+1];
  double _Complex vo_t[COL+1][COL+1][NTH+1][NZ+1][NY+1][NX+1];

  int mu,nu,ipl,ic,jc;
  int ix,iy,iz,itb,ith,ieoxyz;
  int ixlow,iylow,izlow;
  int ixhig,iyhig,izhig;
  int itb0;
  int ix1,iy1,iz1,itb1;
  int ix2,iy2,iz2,itb2;
  int ix3,iy3,iz3,itb3;
  int ix4,iy4,iz4,itb4;
  int ix5,iy5,iz5,itb5;
  int ix6,iy6,iz6,itb6;
  double _Complex zid8;
  double _Complex zi = 0.0e0 + 1.0e0 * I;

  /*
    allocate(wce_t[COL][COL][0:NTH][0:NZ1][0:NY1][0:NX1][NDIM])
    allocate(wco_t[COL][COL][0:NTH][0:NZ1][0:NY1][0:NX1][NDIM])
    allocate(ve_t[COL][COL][NTH][NZ][NY][NX])
    allocate(vo_t[COL][COL][NTH][NZ][NY][NX])
  */

  zid8 = -zi / 8.0e0;

  for (mu = 1; mu <= NDIM - 1; mu++) {
    for (nu = mu + 1; nu <= NDIM; nu++) {
      ipl = imunu[mu][nu];
      ixlow = 1 - isx[mu] - isx[nu];
      iylow = 1 - isy[mu] - isy[nu];
      izlow = 1 - isz[mu] - isz[nu];

      for (ix = ixlow; ix <= NX; ix++) {
	for (iy = iylow; iy <= NY; iy++) {
	  for (iz = izlow; iz <= NZ; iz++) {
	    for (ith = 0; ith <= NTH; ith++) {
	      for (jc = 1; jc <= COL; jc ++) {
		for (ic = 1; ic <= COL; ic ++) {
		  ieoxyz = (ipeo + ix + iy + iz) % 2;
		  ix2 = ix + isx[mu];
		  iy2 = iy + isy[mu];
		  iz2 = iz + isz[mu];
		  ix4 = ix + isx[nu];
		  iy4 = iy + isy[nu];
		  iz4 = iz + isz[nu];

		  itb = ith - ieoxyz;
		  if (itb <= 0) {
		    itb = 0;
		  }
		  wce_t[ic][jc][itb][iz][iy][ix][1] = conj (ue_t[1][ic][itb][iz][iy][ix][nu]) * ue_t[1][jc][itb][iz][iy][ix][mu] + conj (ue_t[2][ic][itb][iz][iy][ix][nu]) * ue_t[2][jc][itb][iz][iy][ix][mu] + conj (ue_t[3][ic][itb][iz][iy][ix][nu]) * ue_t[3][jc][itb][iz][iy][ix][mu];

		  itb = ith + ieoxyz - 1;
		  if (itb <= 0) {
		    itb = 0;
		  }
		  wco_t[ic][jc][itb][iz][iy][ix][1] = conj (uo_t[1][ic][itb][iz][iy][ix][nu]) *  uo_t[1][jc][itb][iz][iy][ix][mu] +  conj (uo_t[2][ic][itb][iz][iy][ix][nu]) * uo_t[2][jc][itb][iz][iy][ix][mu] + conj (uo_t[3][ic][itb][iz][iy][ix][nu]) * uo_t[3][jc][itb][iz][iy][ix][mu];
		  itb = ith - ieoxyz;
		  if (itb <= 0) {
		    itb = 0;
		  }
		  itb2 = itb + ieoxyz * ist[mu];
		  itb4 = itb + ieoxyz * ist[nu];
		  wce_t[ic][jc][itb][iz][iy][ix][2] = uo_t[ic][1][itb2][iz2][iy2][ix2][nu] * conj (uo_t[jc][1][itb4][iz4][iy4][ix4][mu]) + uo_t[ic][2][itb2][iz2][iy2][ix2][nu] * conj (uo_t[jc][2][itb4][iz4][iy4][ix4][mu]) + uo_t[ic][3][itb2][iz2][iy2][ix2][nu] * conj (uo_t[jc][3][itb4][iz4][iy4][ix4][mu]);
		  itb = ith + ieoxyz - 1;
		  if (itb <= 0) {
		    itb = 0;
		  }
		  itb2 = itb + (1 - ieoxyz) * ist[mu];
		  itb4 = itb + (1 - ieoxyz) * ist[nu];
		  wco_t[ic][jc][itb][iz][iy][ix][2] = ue_t[ic][1][itb2][iz2][iy2][ix2][nu] * conj (ue_t[jc][1][itb4][iz4][iy4][ix4][mu]) + ue_t[ic][2][itb2][iz2][iy2][ix2][nu] * conj (ue_t[jc][2][itb4][iz4][iy4][ix4][mu]) + ue_t[ic][3][itb2][iz2][iy2][ix2][nu] * conj (ue_t[jc][3][itb4][iz4][iy4][ix4][mu]);
		}
	      }
	    }
	  }
	}
      }

      ixlow = 1 - isx[mu];
      iylow = 1 - isy[mu];
      izlow = 1 - isz[mu];
      ixhig = NX + isx[nu];
      iyhig = NY + isy[nu];
      izhig = NZ + isz[nu];

      for (ix = ixlow; ix <= ixhig; ix++) {
	for (iy = iylow; iy <= iyhig; iy++) {
	  for (iz = izlow; iz <= izhig; iz++) {
	    for (ith = 0; ith <= NTH; ith++) {
	      for (jc = 1; jc <= COL; jc ++) {
		for (ic = 1; ic <= COL; ic ++) {
		  ieoxyz = (ipeo + ix + iy + iz) % 2;
		  ix5 = ix - isx[nu];
		  iy5 = iy - isy[nu];
		  iz5 = iz - isz[nu];
		  ix6 = ix5 + isx[mu];
		  iy6 = iy5 + isy[mu];
		  iz6 = iz5 + isz[mu];

		  itb = ith + (1 - ieoxyz);
		  if (itb >= NTH) {
		    itb = NTH;
		  }
		  itb5 = itb - (1 - ieoxyz) * ist[nu];
		  wce_t[ic][jc][itb][iz][iy][ix][3] = conj (ue_t[1][ic][itb][iz][iy][ix][mu]) * conj (uo_t[jc][1][itb5][iz5][iy5][ix5][nu]) + conj (ue_t[2][ic][itb][iz][iy][ix][mu]) *  conj (uo_t[jc][2][itb5][iz5][iy5][ix5][nu]) + conj (ue_t[3][ic][itb][iz][iy][ix][mu]) * conj (uo_t[jc][3][itb5][iz5][iy5][ix5][nu]);

		  itb = ith + ieoxyz;
		  if (itb >= NTH) {
		    itb = NTH;
		  }
		  itb5 = itb - (ieoxyz) * ist[nu];
		  wco_t[ic][jc][itb][iz][iy][ix][3] = conj (uo_t[1][ic][itb][iz][iy][ix][mu]) * conj (ue_t[jc][1][itb5][iz5][iy5][ix5][nu]) + conj (uo_t[2][ic][itb][iz][iy][ix][mu]) * conj (ue_t[jc][2][itb5][iz5][iy5][ix5][nu]) + conj (uo_t[3][ic][itb][iz][iy][ix][mu]) * conj (ue_t[jc][3][itb5][iz5][iy5][ix5][nu]);

		  itb = ith + (1 - ieoxyz);
		  if (itb >= NTH) {
		    itb = NTH;
		  }
		  itb5 = itb - (1 - ieoxyz) * ist[nu];
		  itb6 = itb5 + ieoxyz * ist[mu];
		  wce_t[ic][jc][itb][iz][iy][ix][4] = uo_t[ic][1][itb5][iz5][iy5][ix5][mu] * ue_t[1][jc][itb6][iz6][iy6][ix6][nu] + uo_t[ic][2][itb5][iz5][iy5][ix5][mu] * ue_t[2][jc][itb6][iz6][iy6][ix6][nu] + uo_t[ic][3][itb5][iz5][iy5][ix5][mu] * ue_t[3][jc][itb6][iz6][iy6][ix6][nu];

		  itb = ith + ieoxyz;
		  if (itb >= NTH) {
		    itb = NTH;
		  }
		  itb5 = itb -  ieoxyz * ist[nu];
		  itb6 = itb5 + (1 - ieoxyz) * ist[mu];
		  wco_t[ic][jc][itb][iz][iy][ix][4] = ue_t[ic][1][itb5][iz5][iy5][ix5][mu] * uo_t[1][jc][itb6][iz6][iy6][ix6][nu] + ue_t[ic][2][itb5][iz5][iy5][ix5][mu] * uo_t[2][jc][itb6][iz6][iy6][ix6][nu] + ue_t[ic][3][itb5][iz5][iy5][ix5][mu] * uo_t[3][jc][itb6][iz6][iy6][ix6][nu];
		}
	      }
	    }
	  }
	}
      }

      for (ix = 1; ix <= NX; ix++) {
	for (iy = 1; iy <= NY; iy++) {
	  for (iz = 1; iz <= NZ; iz++) {
	    for (ith = 1; ith <= NTH; ith++) {
	      for (jc = 1; jc <= COL; jc++) {
		for (ic = 1; ic <= COL; ic++) {
		  ieoxyz = (ipeo + ix + iy + iz) % 2;
		  ix1 = ix + isx[nu];
		  iy1 = iy + isy[nu];
		  iz1 = iz + isz[nu];
		  ix2 = ix - isx[mu];
		  iy2 = iy - isy[mu];
		  iz2 = iz - isz[mu];
		  ix3 = ix - isx[nu];
		  iy3 = iy - isy[nu];
		  iz3 = iz - isz[nu];

		  itb = ith - ieoxyz;
		  itb0 = itb + ieoxyz;
		  itb1 = itb + ieoxyz * ist[nu];
		  itb2 = itb - (1 - ieoxyz) * ist[mu];
		  itb3 = itb - (1 - ieoxyz) * ist[nu];
		  ve_t[ic][jc][itb0][iz][iy][ix] = wco_t[ic][1][itb1][iz1][iy1][ix1][4] * wco_t[1][jc][itb1][iz1][iy1][ix1][3] + wco_t[ic][2][itb1][iz1][iy1][ix1][4] * wco_t[2][jc][itb1][iz1][iy1][ix1][3] + wco_t[ic][3][itb1][iz1][iy1][ix1][4] * wco_t[3][jc][itb1][iz1][iy1][ix1][3] + wco_t[ic][1][itb2][iz2][iy2][ix2][2] * wco_t[1][jc][itb2][iz2][iy2][ix2][1] + wco_t[ic][2][itb2][iz2][iy2][ix2][2] * wco_t[2][jc][itb2][iz2][iy2][ix2][1] + wco_t[ic][3][itb2][iz2][iy2][ix2][2] * wco_t[3][jc][itb2][iz2][iy2][ix2][1] + wco_t[ic][1][itb2][iz2][iy2][ix2][3] * wco_t[1][jc][itb2][iz2][iy2][ix2][4] + wco_t[ic][2][itb2][iz2][iy2][ix2][3] * wco_t[2][jc][itb2][iz2][iy2][ix2][4] + wco_t[ic][3][itb2][iz2][iy2][ix2][3] * wco_t[3][jc][itb2][iz2][iy2][ix2][4] + wco_t[ic][1][itb3][iz3][iy3][ix3][1] * wco_t[1][jc][itb3][iz3][iy3][ix3][2] + wco_t[ic][2][itb3][iz3][iy3][ix3][1] * wco_t[2][jc][itb3][iz3][iy3][ix3][2] + wco_t[ic][3][itb3][iz3][iy3][ix3][1] * wco_t[3][jc][itb3][iz3][iy3][ix3][2];

		  itb = ith+ieoxyz - 1;
		  itb0 = itb+ 1 - ieoxyz;
		  itb1 = itb+(1 - ieoxyz) * ist[nu];
		  itb2 = itb - ieoxyz * ist[mu];
		  itb3 = itb - ieoxyz * ist[nu];

		  vo_t[ic][jc][itb0][iz][iy][ix] = wce_t[ic][1][itb1][iz1][iy1][ix1][4] * wce_t[1][jc][itb1][iz1][iy1][ix1][3] + wce_t[ic][2][itb1][iz1][iy1][ix1][4] * wce_t[2][jc][itb1][iz1][iy1][ix1][3] + wce_t[ic][3][itb1][iz1][iy1][ix1][4] * wce_t[3][jc][itb1][iz1][iy1][ix1][3] + wce_t[ic][1][itb2][iz2][iy2][ix2][2] * wce_t[1][jc][itb2][iz2][iy2][ix2][1] + wce_t[ic][2][itb2][iz2][iy2][ix2][2] * wce_t[2][jc][itb2][iz2][iy2][ix2][1] + wce_t[ic][3][itb2][iz2][iy2][ix2][2] * wce_t[3][jc][itb2][iz2][iy2][ix2][1] + wce_t[ic][1][itb2][iz2][iy2][ix2][3] * wce_t[1][jc][itb2][iz2][iy2][ix2][4] + wce_t[ic][2][itb2][iz2][iy2][ix2][3] * wce_t[2][jc][itb2][iz2][iy2][ix2][4] + wce_t[ic][3][itb2][iz2][iy2][ix2][3] * wce_t[3][jc][itb2][iz2][iy2][ix2][4] + wce_t[ic][1][itb3][iz3][iy3][ix3][1] * wce_t[1][jc][itb3][iz3][iy3][ix3][2] + wce_t[ic][2][itb3][iz3][iy3][ix3][1] * wce_t[2][jc][itb3][iz3][iy3][ix3][2] + wce_t[ic][3][itb3][iz3][iy3][ix3][1] * wce_t[3][jc][itb3][iz3][iy3][ix3][2];
		}
	      }
	    }
	  }
	}
      }

      for (ix = 1; ix <= NX; ix++) {
	for (iy = 1; iy <= NY; iy++) {
	  for (iz = 1; iz <= NZ; iz++) {
	    for (itb = 1; itb <= NTH; itb++) {
	      for (jc = 1; jc <= COL; jc++) {
		for (ic = 1; ic <= COL; ic++) {
		  ucle_t[ic][jc][itb][iz][iy][ix][ipl] = (ve_t[ic][jc][itb][iz][iy][ix] - conj (ve_t[jc][ic][itb][iz][iy][ix])) * zid8;
		  uclo_t[ic][jc][itb][iz][iy][ix][ipl] = (vo_t[ic][jc][itb][iz][iy][ix] - conj (vo_t[jc][ic][itb][iz][iy][ix])) * zid8;
		}
	      }
	    }
	  }
	}
      }
    }
  }

  /* deallocate(wce_t_,wco_t_,ve_t_,vo_t_) */
}
