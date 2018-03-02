#include "header.h"

static void exact_solution(double xi, double eta, double zeta,
			   double dtemp[5]) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c     this function returns the exact solution at point xi, eta, zeta  
c-------------------------------------------------------------------*/

  int m;

  for (m = 0; m < 5; m++) {
    dtemp[m] =  ce[m][0] +
      xi*(ce[m][1] + xi*(ce[m][4] + xi*(ce[m][7]
					+ xi*ce[m][10]))) +
      eta*(ce[m][2] + eta*(ce[m][5] + eta*(ce[m][8]
					   + eta*ce[m][11])))+
      zeta*(ce[m][3] + zeta*(ce[m][6] + zeta*(ce[m][9] + 
					      zeta*ce[m][12])));
  }
}

/* BT */
void exact_rhs(void) {

/*--------------------------------------------------------------------
--------------------------------------------------------------------*/

/*--------------------------------------------------------------------
c     compute the right hand side based on exact solution
c-------------------------------------------------------------------*/

  double dtemp[5], xi, eta, zeta, dtpp;
  int m, i, j, k, ip1, im1, jp1, jm1, km1, kp1;

/*--------------------------------------------------------------------
c     initialize                                  
c-------------------------------------------------------------------*/
#pragma omp for  
  for (i = 0; i < grid_points[0]; i++) {
    for (j = 0; j < grid_points[1]; j++) {
      for (k = 0; k < grid_points[2]; k++) {
	for (m = 0; m < 5; m++) {
	  forcing[i][j][k][m] = 0.0;
	}
      }
    }
  }

/*--------------------------------------------------------------------
c     xi-direction flux differences                      
c-------------------------------------------------------------------*/
#pragma omp for
  for (j = 1; j < grid_points[1]-1; j++) {
    eta = (double)j * dnym1;
    
    for (k = 1; k < grid_points[2]-1; k++) {
      zeta = (double)k * dnzm1;

      for (i = 0; i < grid_points[0]; i++) {
	xi = (double)i * dnxm1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[i][m] = dtemp[m];
	}

	dtpp = 1.0 / dtemp[0];

	for (m = 1; m <= 4; m++) {
	  buf[i][m] = dtpp * dtemp[m];
	}

	cuf[i]   = buf[i][1] * buf[i][1];
	buf[i][0] = cuf[i] + buf[i][2] * buf[i][2] + 
	  buf[i][3] * buf[i][3];
	q[i] = 0.5*(buf[i][1]*ue[i][1] + buf[i][2]*ue[i][2] +
		    buf[i][3]*ue[i][3]);
      }
               
      for (i = 1; i < grid_points[0]-1; i++) {
	im1 = i-1;
	ip1 = i+1;

	forcing[i][j][k][0] = forcing[i][j][k][0] -
	  tx2*(ue[ip1][1]-ue[im1][1])+
	  dx1tx1*(ue[ip1][0]-2.0*ue[i][0]+ue[im1][0]);

	forcing[i][j][k][1] = forcing[i][j][k][1] -
	  tx2 * ((ue[ip1][1]*buf[ip1][1]+c2*(ue[ip1][4]-q[ip1]))-
		 (ue[im1][1]*buf[im1][1]+c2*(ue[im1][4]-q[im1])))+
	  xxcon1*(buf[ip1][1]-2.0*buf[i][1]+buf[im1][1])+
	  dx2tx1*( ue[ip1][1]-2.0* ue[i][1]+ ue[im1][1]);

	forcing[i][j][k][2] = forcing[i][j][k][2] -
	  tx2 * (ue[ip1][2]*buf[ip1][1]-ue[im1][2]*buf[im1][1])+
	  xxcon2*(buf[ip1][2]-2.0*buf[i][2]+buf[im1][2])+
	  dx3tx1*( ue[ip1][2]-2.0* ue[i][2]+ ue[im1][2]);
                  
	forcing[i][j][k][3] = forcing[i][j][k][3] -
	  tx2*(ue[ip1][3]*buf[ip1][1]-ue[im1][3]*buf[im1][1])+
	  xxcon2*(buf[ip1][3]-2.0*buf[i][3]+buf[im1][3])+
	  dx4tx1*( ue[ip1][3]-2.0* ue[i][3]+ ue[im1][3]);

	forcing[i][j][k][4] = forcing[i][j][k][4] -
	  tx2*(buf[ip1][1]*(c1*ue[ip1][4]-c2*q[ip1])-
	       buf[im1][1]*(c1*ue[im1][4]-c2*q[im1]))+
	  0.5*xxcon3*(buf[ip1][0]-2.0*buf[i][0]+buf[im1][0])+
	  xxcon4*(cuf[ip1]-2.0*cuf[i]+cuf[im1])+
	  xxcon5*(buf[ip1][4]-2.0*buf[i][4]+buf[im1][4])+
	  dx5tx1*( ue[ip1][4]-2.0* ue[i][4]+ ue[im1][4]);
      }

/*--------------------------------------------------------------------
c     Fourth-order dissipation                         
c-------------------------------------------------------------------*/

      for (m = 0; m < 5; m++) {
	i = 1;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (5.0*ue[i][m] - 4.0*ue[i+1][m] +ue[i+2][m]);
	i = 2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (-4.0*ue[i-1][m] + 6.0*ue[i][m] -
	    4.0*ue[i+1][m] +     ue[i+2][m]);
      }

      for (m = 0; m < 5; m++) {
	for (i = 1*3; i <= grid_points[0]-3*1-1; i++) {
	  forcing[i][j][k][m] = forcing[i][j][k][m] - dssp*
	    (ue[i-2][m] - 4.0*ue[i-1][m] +
	     6.0*ue[i][m] - 4.0*ue[i+1][m] + ue[i+2][m]);
	}
      }

      for (m = 0; m < 5; m++) {
	i = grid_points[0]-3;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[i-2][m] - 4.0*ue[i-1][m] +
	   6.0*ue[i][m] - 4.0*ue[i+1][m]);
	i = grid_points[0]-2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[i-2][m] - 4.0*ue[i-1][m] + 5.0*ue[i][m]);
      }

    }
  }

/*--------------------------------------------------------------------
c     eta-direction flux differences             
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i < grid_points[0]-1; i++) {
    xi = (double)i * dnxm1;
    
    for (k = 1; k < grid_points[2]-1; k++) {
      zeta = (double)k * dnzm1;

      for (j = 0; j < grid_points[1]; j++) {
	eta = (double)j * dnym1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[j][m] = dtemp[m];
	}
                  
	dtpp = 1.0/dtemp[0];

	for (m = 1; m <= 4; m++) {
	  buf[j][m] = dtpp * dtemp[m];
	}

	cuf[j]   = buf[j][2] * buf[j][2];
	buf[j][0] = cuf[j] + buf[j][1] * buf[j][1] + 
	  buf[j][3] * buf[j][3];
	q[j] = 0.5*(buf[j][1]*ue[j][1] + buf[j][2]*ue[j][2] +
		    buf[j][3]*ue[j][3]);
      }

      for (j = 1; j < grid_points[1]-1; j++) {
	jm1 = j-1;
	jp1 = j+1;
                  
	forcing[i][j][k][0] = forcing[i][j][k][0] -
	  ty2*( ue[jp1][2]-ue[jm1][2] )+
	  dy1ty1*(ue[jp1][0]-2.0*ue[j][0]+ue[jm1][0]);

	forcing[i][j][k][1] = forcing[i][j][k][1] -
	  ty2*(ue[jp1][1]*buf[jp1][2]-ue[jm1][1]*buf[jm1][2])+
	  yycon2*(buf[jp1][1]-2.0*buf[j][1]+buf[jm1][1])+
	  dy2ty1*( ue[jp1][1]-2.0* ue[j][1]+ ue[jm1][1]);

	forcing[i][j][k][2] = forcing[i][j][k][2] -
	  ty2*((ue[jp1][2]*buf[jp1][2]+c2*(ue[jp1][4]-q[jp1]))-
	       (ue[jm1][2]*buf[jm1][2]+c2*(ue[jm1][4]-q[jm1])))+
	  yycon1*(buf[jp1][2]-2.0*buf[j][2]+buf[jm1][2])+
	  dy3ty1*( ue[jp1][2]-2.0*ue[j][2] +ue[jm1][2]);

	forcing[i][j][k][3] = forcing[i][j][k][3] -
	  ty2*(ue[jp1][3]*buf[jp1][2]-ue[jm1][3]*buf[jm1][2])+
	  yycon2*(buf[jp1][3]-2.0*buf[j][3]+buf[jm1][3])+
	  dy4ty1*( ue[jp1][3]-2.0*ue[j][3]+ ue[jm1][3]);

	forcing[i][j][k][4] = forcing[i][j][k][4] -
	  ty2*(buf[jp1][2]*(c1*ue[jp1][4]-c2*q[jp1])-
	       buf[jm1][2]*(c1*ue[jm1][4]-c2*q[jm1]))+
	  0.5*yycon3*(buf[jp1][0]-2.0*buf[j][0]+
                      buf[jm1][0])+
	  yycon4*(cuf[jp1]-2.0*cuf[j]+cuf[jm1])+
	  yycon5*(buf[jp1][4]-2.0*buf[j][4]+buf[jm1][4])+
	  dy5ty1*(ue[jp1][4]-2.0*ue[j][4]+ue[jm1][4]);
      }

/*--------------------------------------------------------------------
c     Fourth-order dissipation                      
c-------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	j = 1;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (5.0*ue[j][m] - 4.0*ue[j+1][m] +ue[j+2][m]);
	j = 2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (-4.0*ue[j-1][m] + 6.0*ue[j][m] -
	   4.0*ue[j+1][m] +       ue[j+2][m]);
      }

      for (m = 0; m < 5; m++) {
	for (j = 1*3; j <= grid_points[1]-3*1-1; j++) {
	  forcing[i][j][k][m] = forcing[i][j][k][m] - dssp*
	    (ue[j-2][m] - 4.0*ue[j-1][m] +
	     6.0*ue[j][m] - 4.0*ue[j+1][m] + ue[j+2][m]);
	}
      }

      for (m = 0; m < 5; m++) {
	j = grid_points[1]-3;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[j-2][m] - 4.0*ue[j-1][m] +
	   6.0*ue[j][m] - 4.0*ue[j+1][m]);
	j = grid_points[1]-2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[j-2][m] - 4.0*ue[j-1][m] + 5.0*ue[j][m]);
      }

    }
  }


/*--------------------------------------------------------------------
c     zeta-direction flux differences                      
c-------------------------------------------------------------------*/
#pragma omp for
  for (i = 1; i < grid_points[0]-1; i++) {
    xi = (double)i * dnxm1;
    
    for (j = 1; j < grid_points[1]-1; j++) {
      eta = (double)j * dnym1;

      for (k = 0; k < grid_points[2]; k++) {
	zeta = (double)k * dnzm1;

	exact_solution(xi, eta, zeta, dtemp);
	for (m = 0; m < 5; m++) {
	  ue[k][m] = dtemp[m];
	}

	dtpp = 1.0/dtemp[0];

	for (m = 1; m <= 4; m++) {
	  buf[k][m] = dtpp * dtemp[m];
	}

	cuf[k]   = buf[k][3] * buf[k][3];
	buf[k][0] = cuf[k] + buf[k][1] * buf[k][1] + 
	  buf[k][2] * buf[k][2];
	q[k] = 0.5*(buf[k][1]*ue[k][1] + buf[k][2]*ue[k][2] +
		    buf[k][3]*ue[k][3]);
      }

      for (k = 1; k < grid_points[2]-1; k++) {
	km1 = k-1;
	kp1 = k+1;
                  
	forcing[i][j][k][0] = forcing[i][j][k][0] -
	  tz2*( ue[kp1][3]-ue[km1][3] )+
	  dz1tz1*(ue[kp1][0]-2.0*ue[k][0]+ue[km1][0]);

	forcing[i][j][k][1] = forcing[i][j][k][1] -
	  tz2 * (ue[kp1][1]*buf[kp1][3]-ue[km1][1]*buf[km1][3])+
	  zzcon2*(buf[kp1][1]-2.0*buf[k][1]+buf[km1][1])+
	  dz2tz1*( ue[kp1][1]-2.0* ue[k][1]+ ue[km1][1]);

	forcing[i][j][k][2] = forcing[i][j][k][2] -
	  tz2 * (ue[kp1][2]*buf[kp1][3]-ue[km1][2]*buf[km1][3])+
	  zzcon2*(buf[kp1][2]-2.0*buf[k][2]+buf[km1][2])+
	  dz3tz1*(ue[kp1][2]-2.0*ue[k][2]+ue[km1][2]);

	forcing[i][j][k][3] = forcing[i][j][k][3] -
	  tz2 * ((ue[kp1][3]*buf[kp1][3]+c2*(ue[kp1][4]-q[kp1]))-
		 (ue[km1][3]*buf[km1][3]+c2*(ue[km1][4]-q[km1])))+
	  zzcon1*(buf[kp1][3]-2.0*buf[k][3]+buf[km1][3])+
	  dz4tz1*( ue[kp1][3]-2.0*ue[k][3] +ue[km1][3]);

	forcing[i][j][k][4] = forcing[i][j][k][4] -
	  tz2 * (buf[kp1][3]*(c1*ue[kp1][4]-c2*q[kp1])-
		 buf[km1][3]*(c1*ue[km1][4]-c2*q[km1]))+
	  0.5*zzcon3*(buf[kp1][0]-2.0*buf[k][0]
                      +buf[km1][0])+
	  zzcon4*(cuf[kp1]-2.0*cuf[k]+cuf[km1])+
	  zzcon5*(buf[kp1][4]-2.0*buf[k][4]+buf[km1][4])+
	  dz5tz1*( ue[kp1][4]-2.0*ue[k][4]+ ue[km1][4]);
      }

/*--------------------------------------------------------------------
c     Fourth-order dissipation                        
c-------------------------------------------------------------------*/
      for (m = 0; m < 5; m++) {
	k = 1;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (5.0*ue[k][m] - 4.0*ue[k+1][m] +ue[k+2][m]);
	k = 2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (-4.0*ue[k-1][m] + 6.0*ue[k][m] -
	   4.0*ue[k+1][m] +       ue[k+2][m]);
      }

      for (m = 0; m < 5; m++) {
	for (k = 1*3; k <= grid_points[2]-3*1-1; k++) {
	  forcing[i][j][k][m] = forcing[i][j][k][m] - dssp*
	    (ue[k-2][m] - 4.0*ue[k-1][m] +
	     6.0*ue[k][m] - 4.0*ue[k+1][m] + ue[k+2][m]);
	}
      }

      for (m = 0; m < 5; m++) {
	k = grid_points[2]-3;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[k-2][m] - 4.0*ue[k-1][m] +
	   6.0*ue[k][m] - 4.0*ue[k+1][m]);
	k = grid_points[2]-2;
	forcing[i][j][k][m] = forcing[i][j][k][m] - dssp *
	  (ue[k-2][m] - 4.0*ue[k-1][m] + 5.0*ue[k][m]);
      }

    }
  }

/*--------------------------------------------------------------------
c     now change the sign of the forcing function, 
c-------------------------------------------------------------------*/
#pragma omp for  
  for (i = 1; i < grid_points[0]-1; i++) {
    for (j = 1; j < grid_points[1]-1; j++) {
      for (k = 1; k < grid_points[2]-1; k++) {
	for (m = 0; m < 5; m++) {
	  forcing[i][j][k][m] = -1.0 * forcing[i][j][k][m];
	}
      }
    }
  }
}
