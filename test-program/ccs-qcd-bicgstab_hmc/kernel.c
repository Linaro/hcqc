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

int ipeo = 0;

extern void keep_result (double, double, double);

void bicgstab_hmc (double tol,
		   int iiter,
		   double kappa,
		   double _Complex be_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1],
		   double _Complex xe_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1],
		   double _Complex ue_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1],
		   double _Complex uo_t[COL+1][COL+1][NTH+1][NZ1+1][NY1+1][NX1+1][NDIM+1],
		   int ieo,
		   double _Complex fclinve_t[CLSPH+1][NTH+1][NZ+1][NY+1][NX+1][3],
		   double _Complex fclinvo_t[CLSPH+1][NTH+1][NZ+1][NY+1][NX+1][3])
{
  double flopcount, stcount, ldcount;

  double _Complex rte_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1];
  double _Complex pe_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1];
  double _Complex te_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1];
  double _Complex qe_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1];
  double _Complex re_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1];
  /* double _Complex myo_t[COL+1][SPIN+1][NTH+1][NZ1+1][NY1+1][NX1+1]; */

  int iter,niter;
  int ix,iy,iz,itb,ic,is,ieoxyz;
  int ith;
  double _Complex rho1,rho0,beta,omega,alpha;
  double _Complex ctmp0;
  double rtmp0;
  double rnorm,rsource;

  /*
  allocate(rte_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  allocate( pe_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  allocate( te_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  allocate( qe_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  allocate( re_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  allocate(myo_t(COL,SPIN,0:NTH,0:NZ1,0:NY1,0:NX1))
  */

  flopcount = 0.0e0;
  stcount = 0.0e0;
  ldcount = 0.0e0;

  niter = 5000;

  for (ix = 0; ix <= NX1; ix++) {
    for (iy = 0; iy <= NY1; iy++) {
      for (iz = 0; iz <= NZ1; iz++) {
	for (ith = 0; ith <= NTH; ith++) {
	  for (is =1; is <= SPIN; is++) {
	    for (ic =1; ic <= COL; ic++) {
	      xe_t[ic][is][ith][iz][iy][ix] = 0.0e0 + 0.0e0 * I;
	      re_t[ic][is][ith][iz][iy][ix] = be_t[ic][is][ith][iz][iy][ix];
	      rte_t[ic][is][ith][iz][iy][ix] = be_t[ic][is][ith][iz][iy][ix];
	      pe_t[ic][is][ith][iz][iy][ix] = be_t[ic][is][ith][iz][iy][ix];
	    }
	  }
	}
      }
    }
  }

  rtmp0 = 0.0e0;

  for (ix = 1; ix <= NX; ix++) {
    for (iy = 1; iy <= NY; iy++) {
      for (iz = 1; iz <= NZ; iz++) {
	for (ith = 1; ith <= NTH; ith++) {
	  for (is = 1; is <= SPIN; is++) {
	    for (ic = 1; ic <= COL; ic++) {
	      ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
	      itb  =  ith - ieoxyz;
	      rtmp0 = rtmp0 + powf ((float)(be_t[ic][is][itb][iz][iy][ix]), 2) + powf (cimag (be_t[ic][is][itb][iz][iy][ix]), 2);
	    }
	  }
	}
      }
    }
  }

  rsource = sqrt (rtmp0);
  flopcount = flopcount + QABS2;
  stcount = stcount + COL * SPIN*4;
  ldcount = ldcount + COL * SPIN;

  iter = 0;

  rho0 = pow (rsource,2) + 0.0e0 * I;
  rnorm = 1.0e0;

  while (iter <= niter) {

    /* call mult_mb_pre(kappa,pe_t_,qe_t_,ue_t_,uo_t_,ieo,fclinve_t_,fclinvo_t_,myo_t_) */

    flopcount = flopcount + MULT_MB_PRE;
    ldcount = ldcount + COL * SPIN + COL * COL * 4 * 2 + CLSPH * 2;
    stcount = stcount + COL * SPIN;
    iter = iter + 1;

    ctmp0 = 0.0e0 + 0.0e0 * I;
    rtmp0 = 0.0e0;

    for (ix = 1; ix <= NX; ix++) {
      for (iy = 1; iy <= NY; iy++) {
	for (iz = 1; iz <= NZ; iz++) {
	  for (ith = 1; ith <= NTH; ith++) {
	    for (is = 1; is <= SPIN; is++) {
	      for (ic = 1; ic <= COL; ic++) {
		ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
		itb = ith - ieoxyz;
		ctmp0 = ctmp0 + conj (rte_t[ic][is][itb][iz][iy][ix]) * qe_t[ic][is][itb][iz][iy][ix];
	      }
	    }
	  }
	}
      }
    }

    alpha = rho0 / ctmp0;
    flopcount = flopcount + QPROD;
    ldcount = ldcount + COL * SPIN * 2;

    rtmp0 = 0.0e0;

    for (ix = 1; ix <= NX; ix++) {
      for (iy = 1; iy <= NY; iy++) {
	for (iz = 1; iz <= NZ; iz++) {
	  for (ith = 1; ith <= NTH; ith++) {
	    for (is = 1; is <= SPIN; is++) {
	      for (ic = 1; ic <= COL; ic++) {
		ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
		itb = ith - ieoxyz;
		xe_t[ic][is][itb][iz][iy][ix] = xe_t[ic][is][itb][iz][iy][ix] + alpha * pe_t[ic][is][itb][iz][iy][ix];
		re_t[ic][is][itb][iz][iy][ix] = re_t[ic][is][itb][iz][iy][ix] - alpha * qe_t[ic][is][itb][iz][iy][ix];
		rtmp0 = rtmp0 + powf((float)(re_t[ic][is][itb][iz][iy][ix]), 2) + powf (cimag (re_t[ic][is][itb][iz][iy][ix]), 2);
	      }
	    }
	  }
	}
      }
    }

    flopcount = flopcount + QABS2 + 2 * QMULT1C_ACCUM_ADD;
    ldcount = ldcount + COL * SPIN * 4;
    stcount = stcount + COL * SPIN * 2;

    rnorm = sqrt (rtmp0) / rsource;

    if (rnorm <= tol) {
      return;
    }

    /* call mult_mb_pre(kappa,re_t_,te_t_,ue_t_,uo_t_,ieo,fclinve_t_,fclinvo_t_,myo_t_)*/
    iter = iter + 1;
    flopcount = flopcount + MULT_MB_PRE;
    ldcount = ldcount + COL * SPIN + COL * COL * 4 * 2 + CLSPH * 2;
    stcount = stcount + COL * SPIN;

    rtmp0 =  0.0e0;
    ctmp0 = 0.0e0 + 0.0e0 * I;

    for (ix = 1; ix <= NX; ix++) {
      for (iy = 1; iy <= NY; iy++) {
	for (iz = 1; iz <= NZ; iz++) {
	  for (ith = 1; ith <= NTH; ith++) {
	    for (is = 1; is <= SPIN; is++) {
	      for (ic = 1; ic <= COL; ic++) {
		ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
		itb = ith - ieoxyz;
		rtmp0 = rtmp0 + powf ((float)(te_t[ic][is][itb][iz][iy][ix]), 2) + powf (cimag (te_t[ic][is][itb][iz][iy][ix]), 2);
		ctmp0 = ctmp0 + conj (te_t[ic][is][itb][iz][iy][ix]) * re_t[ic][is][itb][iz][iy][ix];
	      }
	    }
	  }
	}
      }
    }

    flopcount = flopcount + QABS2 + QPROD;
    ldcount = ldcount + COL * SPIN * 3;

    omega = ctmp0 / rtmp0;

    rtmp0 = 0.0e0;
    rho1 = 0.0e0 + 0.0e0 *I;

    for (ix = 1; ix <= NX; ix ++) {
      for (iy = 1; iy <= NY; iy++) {
	for (iz = 1; iz <= NZ; iz++) {
	  for (ith = 1; ith <= NTH; ith++) {
	    for (is = 1; is <= SPIN; is++) {
	      for (ic = 1; ic <= COL; ic++) {
		ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
		itb = ith - ieoxyz;
		xe_t[ic][is][itb][iz][iy][ix] = xe_t[ic][is][itb][iz][iy][ix] + omega * re_t[ic][is][itb][iz][iy][ix];
		re_t[ic][is][itb][iz][iy][ix] = re_t[ic][is][itb][iz][iy][ix] - omega * te_t[ic][is][itb][iz][iy][ix];
		rtmp0 = rtmp0 + powf ((float)(re_t[ic][is][itb][iz][iy][ix]), 2) + powf (cimag (re_t[ic][is][itb][iz][iy][ix]), 2);
		rho1  = rho1 + conj (rte_t[ic][is][itb][iz][iy][ix])  * re_t[ic][is][itb][iz][iy][ix];
	      }
	    }
	  }
	}
      }
    }

    flopcount = flopcount + QABS2 + QPROD + 2 * QMULT1C_ACCUM_ADD;
    ldcount = ldcount + COL * SPIN * 4;
    stcount = stcount + COL * SPIN * 2;

    rnorm = sqrt (rtmp0) / rsource;

    if (rnorm <= tol) {
      return;
    }

    beta = (rho1 / rho0) * (alpha / omega);
    rho0 = rho1;

    for (ix = 1; ix <= NX; ix++) {
      for (iy = 1; iy <= NY; iy++) {
	for (iz = 1; iz <= NZ; iz++) {
	  for (ith = 1; ith <= NTH; ith++) {
	    for (is = 1; is <= SPIN; is++) {
	      for (ic = 1; ic <= COL; ic++) {
		ieoxyz = (ipeo + ix + iy + iz + ieo) % 2;
		itb = ith-ieoxyz;
		pe_t[ic][is][itb][iz][iy][ix] = re_t[ic][is][itb][iz][iy][ix] + beta * (pe_t[ic][is][itb][iz][iy][ix] - omega * qe_t[ic][is][itb][iz][iy][ix]);
	      }
	    }
	  }
	}
      }
    }

    flopcount = flopcount + 2 * QMULT1C_ACCUM_ADD;
    ldcount = ldcount + COL * SPIN * 3;
    stcount = stcount + COL * SPIN;
  }

  iiter = iter;
  flopcount = flopcount * ((double)(NTH * NZ * NY * NX));
  ldcount = ldcount * ((double)(NTH * NZ * NY * NX)) * 2.0e0;
  stcount = stcount * ((double)(NTH * NZ * NY * NX)) * 2.0e0;
  /*  deallocate(rte_t_,pe_t_,te_t_,qe_t_,re_t_,myo_t_) */

  keep_result (flopcount, stcount, ldcount);
  if (iter >= niter) {
    return;
  }
  return;
}
