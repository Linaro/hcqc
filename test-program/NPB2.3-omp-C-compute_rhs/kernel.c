#include <math.h>

#include "header.h"

/* SP */
void compute_rhs(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

  int i, j, k, m;
  double aux, rho_inv, uijk, up1, um1, vijk, vp1, vm1,
    wijk, wp1, wm1;

/*--------------------------------------------------------------------
c      compute the reciprocal of density, and the kinetic energy, 
c      and the speed of sound. 
c-------------------------------------------------------------------*/

#pragma omp for nowait
  for (i = 0; i <= grid_points[0]-1; i++) {
    for (j = 0; j <= grid_points[1]-1; j++) {
      for (k = 0; k <= grid_points[2]-1; k++) {
	rho_inv = 1.0/u[0][i][j][k];
	rho_i[i][j][k] = rho_inv;
	us[i][j][k] = u[1][i][j][k] * rho_inv;
	vs[i][j][k] = u[2][i][j][k] * rho_inv;
	ws[i][j][k] = u[3][i][j][k] * rho_inv;
	square[i][j][k] = 0.5* (u[1][i][j][k]*u[1][i][j][k] + 
				u[2][i][j][k]*u[2][i][j][k] +
				u[3][i][j][k]*u[3][i][j][k] ) * rho_inv;
	qs[i][j][k] = square[i][j][k] * rho_inv;
/*--------------------------------------------------------------------
c               (do not need speed and ainx until the lhs computation)
c-------------------------------------------------------------------*/
	aux = c1c2*rho_inv* (u[4][i][j][k] - square[i][j][k]);
	aux = sqrt(aux);
	speed[i][j][k] = aux;
	ainv[i][j][k]  = 1.0/aux;
      }
    }
  }

/*--------------------------------------------------------------------
c copy the exact forcing term to the right hand side;  because 
c this forcing term is known, we can store it on the whole grid
c including the boundary                   
c-------------------------------------------------------------------*/

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 0; i <= grid_points[0]-1; i++) {
      for (j = 0; j <= grid_points[1]-1; j++) {
	for (k = 0; k <= grid_points[2]-1; k++) {
	  rhs[m][i][j][k] = forcing[m][i][j][k];
	}
      }
    }
  }

/*--------------------------------------------------------------------
c      compute xi-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	uijk = us[i][j][k];
	up1  = us[i+1][j][k];
	um1  = us[i-1][j][k];

	rhs[0][i][j][k] = rhs[0][i][j][k] + dx1tx1 * 
	  (u[0][i+1][j][k] - 2.0*u[0][i][j][k] + 
	   u[0][i-1][j][k]) -
	  tx2 * (u[1][i+1][j][k] - u[1][i-1][j][k]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dx2tx1 * 
	  (u[1][i+1][j][k] - 2.0*u[1][i][j][k] + 
	   u[1][i-1][j][k]) +
	  xxcon2*con43 * (up1 - 2.0*uijk + um1) -
	  tx2 * (u[1][i+1][j][k]*up1 - 
		 u[1][i-1][j][k]*um1 +
		 (u[4][i+1][j][k]- square[i+1][j][k]-
		  u[4][i-1][j][k]+ square[i-1][j][k])*
		 c2);

	rhs[2][i][j][k] = rhs[2][i][j][k] + dx3tx1 * 
	  (u[2][i+1][j][k] - 2.0*u[2][i][j][k] +
	   u[2][i-1][j][k]) +
	  xxcon2 * (vs[i+1][j][k] - 2.0*vs[i][j][k] +
		    vs[i-1][j][k]) -
	  tx2 * (u[2][i+1][j][k]*up1 - 
		 u[2][i-1][j][k]*um1);

	rhs[3][i][j][k] = rhs[3][i][j][k] + dx4tx1 * 
	  (u[3][i+1][j][k] - 2.0*u[3][i][j][k] +
	   u[3][i-1][j][k]) +
	  xxcon2 * (ws[i+1][j][k] - 2.0*ws[i][j][k] +
		    ws[i-1][j][k]) -
	  tx2 * (u[3][i+1][j][k]*up1 - 
		 u[3][i-1][j][k]*um1);

	rhs[4][i][j][k] = rhs[4][i][j][k] + dx5tx1 * 
	  (u[4][i+1][j][k] - 2.0*u[4][i][j][k] +
	   u[4][i-1][j][k]) +
	  xxcon3 * (qs[i+1][j][k] - 2.0*qs[i][j][k] +
		    qs[i-1][j][k]) +
	  xxcon4 * (up1*up1 -       2.0*uijk*uijk + 
		    um1*um1) +
	  xxcon5 * (u[4][i+1][j][k]*rho_i[i+1][j][k] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i-1][j][k]*rho_i[i-1][j][k]) -
	  tx2 * ( (c1*u[4][i+1][j][k] - 
		   c2*square[i+1][j][k])*up1 -
		  (c1*u[4][i-1][j][k] - 
		   c2*square[i-1][j][k])*um1 );
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order xi-direction dissipation               
c-------------------------------------------------------------------*/

  i = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] +
	    u[m][i+2][j][k]);
      }
    }
  }
  i = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i-1][j][k] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i+1][j][k] + u[m][i+2][j][k]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 3*1; i <= grid_points[0]-3*1-1; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] + 
	       u[m][i+2][j][k] );
	}
      }
    }
  }

  i = grid_points[0]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i+1][j][k] );
      }
    }
  }

  i = grid_points[0]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i-2][j][k] - 4.0*u[m][i-1][j][k] +
	    5.0*u[m][i][j][k] );
      }
    }
  }
#pragma omp barrier
  
/*--------------------------------------------------------------------
c      compute eta-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	vijk = vs[i][j][k];
	vp1  = vs[i][j+1][k];
	vm1  = vs[i][j-1][k];
	rhs[0][i][j][k] = rhs[0][i][j][k] + dy1ty1 * 
	  (u[0][i][j+1][k] - 2.0*u[0][i][j][k] + 
	   u[0][i][j-1][k]) -
	  ty2 * (u[2][i][j+1][k] - u[2][i][j-1][k]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dy2ty1 * 
	  (u[1][i][j+1][k] - 2.0*u[1][i][j][k] + 
	   u[1][i][j-1][k]) +
	  yycon2 * (us[i][j+1][k] - 2.0*us[i][j][k] + 
		    us[i][j-1][k]) -
	  ty2 * (u[1][i][j+1][k]*vp1 - 
		 u[1][i][j-1][k]*vm1);
	rhs[2][i][j][k] = rhs[2][i][j][k] + dy3ty1 * 
	  (u[2][i][j+1][k] - 2.0*u[2][i][j][k] + 
	   u[2][i][j-1][k]) +
	  yycon2*con43 * (vp1 - 2.0*vijk + vm1) -
	  ty2 * (u[2][i][j+1][k]*vp1 - 
		 u[2][i][j-1][k]*vm1 +
		 (u[4][i][j+1][k] - square[i][j+1][k] - 
		  u[4][i][j-1][k] + square[i][j-1][k])
		 *c2);
	rhs[3][i][j][k] = rhs[3][i][j][k] + dy4ty1 * 
	  (u[3][i][j+1][k] - 2.0*u[3][i][j][k] + 
	   u[3][i][j-1][k]) +
	  yycon2 * (ws[i][j+1][k] - 2.0*ws[i][j][k] + 
		    ws[i][j-1][k]) -
	  ty2 * (u[3][i][j+1][k]*vp1 - 
		 u[3][i][j-1][k]*vm1);
	rhs[4][i][j][k] = rhs[4][i][j][k] + dy5ty1 * 
	  (u[4][i][j+1][k] - 2.0*u[4][i][j][k] + 
	   u[4][i][j-1][k]) +
	  yycon3 * (qs[i][j+1][k] - 2.0*qs[i][j][k] + 
		    qs[i][j-1][k]) +
	  yycon4 * (vp1*vp1       - 2.0*vijk*vijk + 
		    vm1*vm1) +
	  yycon5 * (u[4][i][j+1][k]*rho_i[i][j+1][k] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i][j-1][k]*rho_i[i][j-1][k]) -
	  ty2 * ((c1*u[4][i][j+1][k] - 
		  c2*square[i][j+1][k]) * vp1 -
		 (c1*u[4][i][j-1][k] - 
		  c2*square[i][j-1][k]) * vm1);
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order eta-direction dissipation         
c-------------------------------------------------------------------*/

  j = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] +
	    u[m][i][j+2][k]);
      }
    }
  }

  j = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i][j-1][k] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i][j+1][k] + u[m][i][j+2][k]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 3*1; j <= grid_points[1]-3*1-1; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] + 
	       u[m][i][j+2][k] );
	}
      }
    }
  }
 
  j = grid_points[1]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i][j+1][k] );
      }
    }
  }

  j = grid_points[1]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j-2][k] - 4.0*u[m][i][j-1][k] +
	    5.0*u[m][i][j][k] );
      }
    }
  }
#pragma omp barrier  

/*--------------------------------------------------------------------
c      compute zeta-direction fluxes 
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i <= grid_points[0]-2; i++) {
    for (j = 1; j <= grid_points[1]-2; j++) {
      for (k = 1; k <= grid_points[2]-2; k++) {
	wijk = ws[i][j][k];
	wp1  = ws[i][j][k+1];
	wm1  = ws[i][j][k-1];

	rhs[0][i][j][k] = rhs[0][i][j][k] + dz1tz1 * 
	  (u[0][i][j][k+1] - 2.0*u[0][i][j][k] + 
	   u[0][i][j][k-1]) -
	  tz2 * (u[3][i][j][k+1] - u[3][i][j][k-1]);
	rhs[1][i][j][k] = rhs[1][i][j][k] + dz2tz1 * 
	  (u[1][i][j][k+1] - 2.0*u[1][i][j][k] + 
	   u[1][i][j][k-1]) +
	  zzcon2 * (us[i][j][k+1] - 2.0*us[i][j][k] + 
		    us[i][j][k-1]) -
	  tz2 * (u[1][i][j][k+1]*wp1 - 
		 u[1][i][j][k-1]*wm1);
	rhs[2][i][j][k] = rhs[2][i][j][k] + dz3tz1 * 
	  (u[2][i][j][k+1] - 2.0*u[2][i][j][k] + 
	   u[2][i][j][k-1]) +
	  zzcon2 * (vs[i][j][k+1] - 2.0*vs[i][j][k] + 
		    vs[i][j][k-1]) -
	  tz2 * (u[2][i][j][k+1]*wp1 - 
		 u[2][i][j][k-1]*wm1);
	rhs[3][i][j][k] = rhs[3][i][j][k] + dz4tz1 * 
	  (u[3][i][j][k+1] - 2.0*u[3][i][j][k] + 
	   u[3][i][j][k-1]) +
	  zzcon2*con43 * (wp1 - 2.0*wijk + wm1) -
	  tz2 * (u[3][i][j][k+1]*wp1 - 
		 u[3][i][j][k-1]*wm1 +
		 (u[4][i][j][k+1] - square[i][j][k+1] - 
		  u[4][i][j][k-1] + square[i][j][k-1])
		 *c2);
	rhs[4][i][j][k] = rhs[4][i][j][k] + dz5tz1 * 
	  (u[4][i][j][k+1] - 2.0*u[4][i][j][k] + 
	   u[4][i][j][k-1]) +
	  zzcon3 * (qs[i][j][k+1] - 2.0*qs[i][j][k] + 
		    qs[i][j][k-1]) +
	  zzcon4 * (wp1*wp1 - 2.0*wijk*wijk + 
		    wm1*wm1) +
	  zzcon5 * (u[4][i][j][k+1]*rho_i[i][j][k+1] - 
		    2.0*u[4][i][j][k]*rho_i[i][j][k] +
		    u[4][i][j][k-1]*rho_i[i][j][k-1]) -
	  tz2 * ( (c1*u[4][i][j][k+1] - 
		   c2*square[i][j][k+1])*wp1 -
		  (c1*u[4][i][j][k-1] - 
		   c2*square[i][j][k-1])*wm1);
      }
    }
  }

/*--------------------------------------------------------------------
c      add fourth order zeta-direction dissipation                
c-------------------------------------------------------------------*/

  k = 1;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k]- dssp * 
	  ( 5.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] +
	    u[m][i][j][k+2]);
      }
    }
  }

  k = 2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	  (-4.0*u[m][i][j][k-1] + 6.0*u[m][i][j][k] -
	   4.0*u[m][i][j][k+1] + u[m][i][j][k+2]);
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 3*1; k <= grid_points[2]-3*1-1; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] - dssp * 
	    (  u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] + 
	       6.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] + 
	       u[m][i][j][k+2] );
	}
      }
    }
  }
 
  k = grid_points[2]-3;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] + 
	    6.0*u[m][i][j][k] - 4.0*u[m][i][j][k+1] );
      }
    }
  }

  k = grid_points[2]-2;
  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	rhs[m][i][j][k] = rhs[m][i][j][k] - dssp *
	  ( u[m][i][j][k-2] - 4.0*u[m][i][j][k-1] +
	    5.0*u[m][i][j][k] );
      }
    }
  }

  for (m = 0; m < 5; m++) {
#pragma omp for
    for (i = 1; i <= grid_points[0]-2; i++) {
      for (j = 1; j <= grid_points[1]-2; j++) {
	for (k = 1; k <= grid_points[2]-2; k++) {
	  rhs[m][i][j][k] = rhs[m][i][j][k] * dt;
	}
      }
    }
  }
}    
