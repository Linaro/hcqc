#include "header.h"

/* LU */
void erhs(void) {

/*--------------------------------------------------------------------
c
c   compute the right hand side based on exact solution
c
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c  local variables
--------------------------------------------------------------------*/
  int i, j, k, m;
  int iglob, jglob;
  int L1, L2;
  int ist1, iend1;
  int jst1, jend1;
  double  dsspm;
  double  xi, eta, zeta;
  double  q;
  double  u21, u31, u41;
  double  tmp;
  double  u21i, u31i, u41i, u51i;
  double  u21j, u31j, u41j, u51j;
  double  u21k, u31k, u41k, u51k;
  double  u21im1, u31im1, u41im1, u51im1;
  double  u21jm1, u31jm1, u41jm1, u51jm1;
  double  u21km1, u31km1, u41km1, u51km1;

  dsspm = dssp;

#pragma omp for
  for (i = 0; i < nx; i++) {
    for (j = 0; j < ny; j++) {
      for (k = 0; k < nz; k++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] = 0.0;
	}
      }
    }
  }

#pragma omp for
  for (i = 0; i < nx; i++) {
    iglob = i;
    xi = ( (double)(iglob) ) / ( nx0 - 1 );
    for (j = 0; j < ny; j++) {
      jglob = j;
      eta = ( (double)(jglob) ) / ( ny0 - 1 );
      for (k = 0; k < nz; k++) {
	zeta = ( (double)(k) ) / ( nz - 1 );
	for (m = 0; m < 5; m++) {
	  rsd[i][j][k][m] =  ce[m][0]
	    + ce[m][1] * xi
	    + ce[m][2] * eta
	    + ce[m][3] * zeta
	    + ce[m][4] * xi * xi
	    + ce[m][5] * eta * eta
	    + ce[m][6] * zeta * zeta
	    + ce[m][7] * xi * xi * xi
	    + ce[m][8] * eta * eta * eta
	    + ce[m][9] * zeta * zeta * zeta
	    + ce[m][10] * xi * xi * xi * xi
	    + ce[m][11] * eta * eta * eta * eta
	    + ce[m][12] * zeta * zeta * zeta * zeta;
	}
      }
    }
  }

/*--------------------------------------------------------------------
c   xi-direction flux differences
--------------------------------------------------------------------*/

  L1 = 0;
  L2 = nx-1;

#pragma omp for
  for (i = L1; i <= L2; i++) {
    for (j = jst; j <= jend; j++) {
      for (k = 1; k < nz - 1; k++) {
	flux[i][j][k][0] = rsd[i][j][k][1];
	u21 = rsd[i][j][k][1] / rsd[i][j][k][0];
	q = 0.50 * (  rsd[i][j][k][1] * rsd[i][j][k][1]
		      + rsd[i][j][k][2] * rsd[i][j][k][2]
		      + rsd[i][j][k][3] * rsd[i][j][k][3] )
	  / rsd[i][j][k][0];
	flux[i][j][k][1] = rsd[i][j][k][1] * u21 + C2 * 
	  ( rsd[i][j][k][4] - q );
	flux[i][j][k][2] = rsd[i][j][k][2] * u21;
	flux[i][j][k][3] = rsd[i][j][k][3] * u21;
	flux[i][j][k][4] = ( C1 * rsd[i][j][k][4] - C2 * q ) * u21;
      }
    }
  }

#pragma omp for
  for (j = jst; j <= jend; j++) {
    for (k = 1; k <= nz - 2; k++) {
      for (i = ist; i <= iend; i++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] =  frct[i][j][k][m]
	    - tx2 * ( flux[i+1][j][k][m] - flux[i-1][j][k][m] );
	}
      }
      for (i = ist; i <= L2; i++) {
	tmp = 1.0 / rsd[i][j][k][0];

	u21i = tmp * rsd[i][j][k][1];
	u31i = tmp * rsd[i][j][k][2];
	u41i = tmp * rsd[i][j][k][3];
	u51i = tmp * rsd[i][j][k][4];

	tmp = 1.0 / rsd[i-1][j][k][0];

	u21im1 = tmp * rsd[i-1][j][k][1];
	u31im1 = tmp * rsd[i-1][j][k][2];
	u41im1 = tmp * rsd[i-1][j][k][3];
	u51im1 = tmp * rsd[i-1][j][k][4];

	flux[i][j][k][1] = (4.0/3.0) * tx3 * 
	  ( u21i - u21im1 );
	flux[i][j][k][2] = tx3 * ( u31i - u31im1 );
	flux[i][j][k][3] = tx3 * ( u41i - u41im1 );
	flux[i][j][k][4] = 0.50 * ( 1.0 - C1*C5 )
	  * tx3 * ( ( u21i * u21i + u31i * u31i + u41i * u41i )
		    - ( u21im1*u21im1 + u31im1*u31im1 + u41im1*u41im1 ) )
	  + (1.0/6.0)
	  * tx3 * ( u21i*u21i - u21im1*u21im1 )
	  + C1 * C5 * tx3 * ( u51i - u51im1 );
      }

      for (i = ist; i <= iend; i++) {
	frct[i][j][k][0] = frct[i][j][k][0]
	  + dx1 * tx1 * (            rsd[i-1][j][k][0]
				     - 2.0 * rsd[i][j][k][0]
				     +       	    rsd[i+1][j][k][0] );
	frct[i][j][k][1] = frct[i][j][k][1]
	  + tx3 * C3 * C4 * ( flux[i+1][j][k][1] - flux[i][j][k][1] )
	  + dx2 * tx1 * (            rsd[i-1][j][k][1]
				     - 2.0 * rsd[i][j][k][1]
				     +           rsd[i+1][j][k][1] );
	frct[i][j][k][2] = frct[i][j][k][2]
	  + tx3 * C3 * C4 * ( flux[i+1][j][k][2] - flux[i][j][k][2] )
	  + dx3 * tx1 * (            rsd[i-1][j][k][2]
				     - 2.0 * rsd[i][j][k][2]
				     +           rsd[i+1][j][k][2] );
	frct[i][j][k][3] = frct[i][j][k][3]
	  + tx3 * C3 * C4 * ( flux[i+1][j][k][3] - flux[i][j][k][3] )
	  + dx4 * tx1 * (            rsd[i-1][j][k][3]
				     - 2.0 * rsd[i][j][k][3]
				     +           rsd[i+1][j][k][3] );
	frct[i][j][k][4] = frct[i][j][k][4]
	  + tx3 * C3 * C4 * ( flux[i+1][j][k][4] - flux[i][j][k][4] )
	  + dx5 * tx1 * (            rsd[i-1][j][k][4]
				     - 2.0 * rsd[i][j][k][4]
				     +           rsd[i+1][j][k][4] );
      }

/*--------------------------------------------------------------------
c   Fourth-order dissipation
--------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	frct[1][j][k][m] = frct[1][j][k][m]
	  - dsspm * ( + 5.0 * rsd[1][j][k][m]
		      - 4.0 * rsd[2][j][k][m]
		      +           rsd[3][j][k][m] );
	frct[2][j][k][m] = frct[2][j][k][m]
	  - dsspm * ( - 4.0 * rsd[1][j][k][m]
		      + 6.0 * rsd[2][j][k][m]
		      - 4.0 * rsd[3][j][k][m]
		      +           rsd[4][j][k][m] );
      }

      ist1 = 3;
      iend1 = nx - 4;
      for (i = ist1; i <=iend1; i++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] = frct[i][j][k][m]
	    - dsspm * (            rsd[i-2][j][k][m]
				   - 4.0 * rsd[i-1][j][k][m]
				   + 6.0 * rsd[i][j][k][m]
				   - 4.0 * rsd[i+1][j][k][m]
				   +           rsd[i+2][j][k][m] );
	}
      }

      for (m = 0; m < 5; m++) {
	frct[nx-3][j][k][m] = frct[nx-3][j][k][m]
	  - dsspm * (             rsd[nx-5][j][k][m]
				  - 4.0 * rsd[nx-4][j][k][m]
				  + 6.0 * rsd[nx-3][j][k][m]
				  - 4.0 * rsd[nx-2][j][k][m]  );
	frct[nx-2][j][k][m] = frct[nx-2][j][k][m]
	  - dsspm * (             rsd[nx-4][j][k][m]
				  - 4.0 * rsd[nx-3][j][k][m]
				  + 5.0 * rsd[nx-2][j][k][m] );
      }
    }
  }

/*--------------------------------------------------------------------
c   eta-direction flux differences
--------------------------------------------------------------------*/

  L1 = 0;
  L2 = ny-1;

#pragma omp for
  for (i = ist; i <= iend; i++) {
    for (j = L1; j <= L2; j++) {
      for (k = 1; k <= nz - 2; k++) {
	flux[i][j][k][0] = rsd[i][j][k][2];
	u31 = rsd[i][j][k][2] / rsd[i][j][k][0];
	q = 0.50 * (  rsd[i][j][k][1] * rsd[i][j][k][1]
		      + rsd[i][j][k][2] * rsd[i][j][k][2]
		      + rsd[i][j][k][3] * rsd[i][j][k][3] )
	  / rsd[i][j][k][0];
	flux[i][j][k][1] = rsd[i][j][k][1] * u31;
	flux[i][j][k][2] = rsd[i][j][k][2] * u31 + C2 * 
	  ( rsd[i][j][k][4] - q );
	flux[i][j][k][3] = rsd[i][j][k][3] * u31;
	flux[i][j][k][4] = ( C1 * rsd[i][j][k][4] - C2 * q ) * u31;
      }
    }
  }

#pragma omp for
  for (i = ist; i <= iend; i++) {
    for (k = 1; k <= nz - 2; k++) {
      for (j = jst; j <= jend; j++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] =  frct[i][j][k][m]
	    - ty2 * ( flux[i][j+1][k][m] - flux[i][j-1][k][m] );
	}
      }
      for (j = jst; j <= L2; j++) {
	tmp = 1.0 / rsd[i][j][k][0];

	u21j = tmp * rsd[i][j][k][1];
	u31j = tmp * rsd[i][j][k][2];
	u41j = tmp * rsd[i][j][k][3];
	u51j = tmp * rsd[i][j][k][4];

	tmp = 1.0 / rsd[i][j-1][k][0];

	u21jm1 = tmp * rsd[i][j-1][k][1];
	u31jm1 = tmp * rsd[i][j-1][k][2];
	u41jm1 = tmp * rsd[i][j-1][k][3];
	u51jm1 = tmp * rsd[i][j-1][k][4];

	flux[i][j][k][1] = ty3 * ( u21j - u21jm1 );
	flux[i][j][k][2] = (4.0/3.0) * ty3 * 
	  ( u31j - u31jm1 );
	flux[i][j][k][3] = ty3 * ( u41j - u41jm1 );
	flux[i][j][k][4] = 0.50 * ( 1.0 - C1*C5 )
	  * ty3 * ( ( u21j  *u21j + u31j  *u31j + u41j  *u41j )
		    - ( u21jm1*u21jm1 + u31jm1*u31jm1 + u41jm1*u41jm1 ) )
	  + (1.0/6.0)
	  * ty3 * ( u31j*u31j - u31jm1*u31jm1 )
	  + C1 * C5 * ty3 * ( u51j - u51jm1 );
      }

      for (j = jst; j <= jend; j++) {
	frct[i][j][k][0] = frct[i][j][k][0]
	  + dy1 * ty1 * (            rsd[i][j-1][k][0]
				     - 2.0 * rsd[i][j][k][0]
				     +           rsd[i][j+1][k][0] );
	frct[i][j][k][1] = frct[i][j][k][1]
	  + ty3 * C3 * C4 * ( flux[i][j+1][k][1] - flux[i][j][k][1] )
	  + dy2 * ty1 * (            rsd[i][j-1][k][1]
				     - 2.0 * rsd[i][j][k][1]
				     +           rsd[i][j+1][k][1] );
	frct[i][j][k][2] = frct[i][j][k][2]
	  + ty3 * C3 * C4 * ( flux[i][j+1][k][2] - flux[i][j][k][2] )
	  + dy3 * ty1 * (            rsd[i][j-1][k][2]
				     - 2.0 * rsd[i][j][k][2]
				     +           rsd[i][j+1][k][2] );
	frct[i][j][k][3] = frct[i][j][k][3]
	  + ty3 * C3 * C4 * ( flux[i][j+1][k][3] - flux[i][j][k][3] )
	  + dy4 * ty1 * (            rsd[i][j-1][k][3]
				     - 2.0 * rsd[i][j][k][3]
				     +           rsd[i][j+1][k][3] );
	frct[i][j][k][4] = frct[i][j][k][4]
	  + ty3 * C3 * C4 * ( flux[i][j+1][k][4] - flux[i][j][k][4] )
	  + dy5 * ty1 * (            rsd[i][j-1][k][4]
				     - 2.0 * rsd[i][j][k][4]
				     +           rsd[i][j+1][k][4] );
      }

/*--------------------------------------------------------------------
c   fourth-order dissipation
--------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	frct[i][1][k][m] = frct[i][1][k][m]
	  - dsspm * ( + 5.0 * rsd[i][1][k][m]
		      - 4.0 * rsd[i][2][k][m]
		      +           rsd[i][3][k][m] );
	frct[i][2][k][m] = frct[i][2][k][m]
	  - dsspm * ( - 4.0 * rsd[i][1][k][m]
		      + 6.0 * rsd[i][2][k][m]
		      - 4.0 * rsd[i][3][k][m]
		      +           rsd[i][4][k][m] );
      }

      jst1 = 3;
      jend1 = ny - 4;

      for (j = jst1; j <= jend1; j++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] = frct[i][j][k][m]
	    - dsspm * (            rsd[i][j-2][k][m]
				   - 4.0 * rsd[i][j-1][k][m]
				   + 6.0 * rsd[i][j][k][m]
				   - 4.0 * rsd[i][j+1][k][m]
				   +           rsd[i][j+2][k][m] );
	}
      }

      for (m = 0; m < 5; m++) {
	frct[i][ny-3][k][m] = frct[i][ny-3][k][m]
	  - dsspm * (             rsd[i][ny-5][k][m]
				  - 4.0 * rsd[i][ny-4][k][m]
				  + 6.0 * rsd[i][ny-3][k][m]
				  - 4.0 * rsd[i][ny-2][k][m]  );
	frct[i][ny-2][k][m] = frct[i][ny-2][k][m]
	  - dsspm * (             rsd[i][ny-4][k][m]
				  - 4.0 * rsd[i][ny-3][k][m]
				  + 5.0 * rsd[i][ny-2][k][m]  );
      }

    }
  }

/*--------------------------------------------------------------------
c   zeta-direction flux differences
--------------------------------------------------------------------*/
#pragma omp for
  for (i = ist; i <= iend; i++) {
    for (j = jst; j <= jend; j++) {
      for (k = 0; k <= nz-1; k++) {
	flux[i][j][k][0] = rsd[i][j][k][3];
	u41 = rsd[i][j][k][3] / rsd[i][j][k][0];
	q = 0.50 * (  rsd[i][j][k][1] * rsd[i][j][k][1]
		      + rsd[i][j][k][2] * rsd[i][j][k][2]
		      + rsd[i][j][k][3] * rsd[i][j][k][3] )
	  / rsd[i][j][k][0];
	flux[i][j][k][1] = rsd[i][j][k][1] * u41;
	flux[i][j][k][2] = rsd[i][j][k][2] * u41;
	flux[i][j][k][3] = rsd[i][j][k][3] * u41 + C2 * 
	  ( rsd[i][j][k][4] - q );
	flux[i][j][k][4] = ( C1 * rsd[i][j][k][4] - C2 * q ) * u41;
      }

      for (k = 1; k <= nz - 2; k++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] =  frct[i][j][k][m]
	    - tz2 * ( flux[i][j][k+1][m] - flux[i][j][k-1][m] );
	}
      }
      for (k = 1; k <= nz-1; k++) {
	tmp = 1.0 / rsd[i][j][k][0];

	u21k = tmp * rsd[i][j][k][1];
	u31k = tmp * rsd[i][j][k][2];
	u41k = tmp * rsd[i][j][k][3];
	u51k = tmp * rsd[i][j][k][4];

	tmp = 1.0 / rsd[i][j][k-1][0];

	u21km1 = tmp * rsd[i][j][k-1][1];
	u31km1 = tmp * rsd[i][j][k-1][2];
	u41km1 = tmp * rsd[i][j][k-1][3];
	u51km1 = tmp * rsd[i][j][k-1][4];

	flux[i][j][k][1] = tz3 * ( u21k - u21km1 );
	flux[i][j][k][2] = tz3 * ( u31k - u31km1 );
	flux[i][j][k][3] = (4.0/3.0) * tz3 * ( u41k 
					    - u41km1 );
	flux[i][j][k][4] = 0.50 * ( 1.0 - C1*C5 )
	  * tz3 * ( ( u21k  *u21k + u31k  *u31k + u41k  *u41k )
		    - ( u21km1*u21km1 + u31km1*u31km1 + u41km1*u41km1 ) )
	  + (1.0/6.0)
	  * tz3 * ( u41k*u41k - u41km1*u41km1 )
	  + C1 * C5 * tz3 * ( u51k - u51km1 );
      }

      for (k = 1; k <= nz - 2; k++) {
	frct[i][j][k][0] = frct[i][j][k][0]
	  + dz1 * tz1 * (            rsd[i][j][k+1][0]
				     - 2.0 * rsd[i][j][k][0]
				     +           rsd[i][j][k-1][0] );
	frct[i][j][k][1] = frct[i][j][k][1]
	  + tz3 * C3 * C4 * ( flux[i][j][k+1][1] - flux[i][j][k][1] )
	  + dz2 * tz1 * (            rsd[i][j][k+1][1]
				     - 2.0 * rsd[i][j][k][1]
				     +           rsd[i][j][k-1][1] );
	frct[i][j][k][2] = frct[i][j][k][2]
	  + tz3 * C3 * C4 * ( flux[i][j][k+1][2] - flux[i][j][k][2] )
	  + dz3 * tz1 * (            rsd[i][j][k+1][2]
				     - 2.0 * rsd[i][j][k][2]
				     +           rsd[i][j][k-1][2] );
	frct[i][j][k][3] = frct[i][j][k][3]
	  + tz3 * C3 * C4 * ( flux[i][j][k+1][3] - flux[i][j][k][3] )
	  + dz4 * tz1 * (            rsd[i][j][k+1][3]
				     - 2.0 * rsd[i][j][k][3]
				     +           rsd[i][j][k-1][3] );
	frct[i][j][k][4] = frct[i][j][k][4]
	  + tz3 * C3 * C4 * ( flux[i][j][k+1][4] - flux[i][j][k][4] )
	  + dz5 * tz1 * (            rsd[i][j][k+1][4]
				     - 2.0 * rsd[i][j][k][4]
				     +           rsd[i][j][k-1][4] );
      }

/*--------------------------------------------------------------------
c   fourth-order dissipation
--------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	frct[i][j][1][m] = frct[i][j][1][m]
	  - dsspm * ( + 5.0 * rsd[i][j][1][m]
		      - 4.0 * rsd[i][j][2][m]
		      +           rsd[i][j][3][m] );
	frct[i][j][2][m] = frct[i][j][2][m]
	  - dsspm * (- 4.0 * rsd[i][j][1][m]
		     + 6.0 * rsd[i][j][2][m]
		     - 4.0 * rsd[i][j][3][m]
		     +           rsd[i][j][4][m] );
      }

      for (k = 3; k <= nz - 4; k++) {
	for (m = 0; m < 5; m++) {
	  frct[i][j][k][m] = frct[i][j][k][m]
	    - dsspm * (           rsd[i][j][k-2][m]
				  - 4.0 * rsd[i][j][k-1][m]
				  + 6.0 * rsd[i][j][k][m]
				  - 4.0 * rsd[i][j][k+1][m]
				  +           rsd[i][j][k+2][m] );
	}
      }

      for (m = 0; m < 5; m++) {
	frct[i][j][nz-3][m] = frct[i][j][nz-3][m]
	  - dsspm * (            rsd[i][j][nz-5][m]
				 - 4.0 * rsd[i][j][nz-4][m]
				 + 6.0 * rsd[i][j][nz-3][m]
				 - 4.0 * rsd[i][j][nz-2][m]  );
        frct[i][j][nz-2][m] = frct[i][j][nz-2][m]
	  - dsspm * (             rsd[i][j][nz-4][m]
				  - 4.0 * rsd[i][j][nz-3][m]
				  + 5.0 * rsd[i][j][nz-2][m]  );
      }
    }
  }
}
