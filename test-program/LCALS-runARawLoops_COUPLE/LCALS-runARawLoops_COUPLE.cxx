#include "header.hxx"
#include <math.h>

void runARawLoops_COUPLE (Index_type imin, Index_type imax, Index_type jmin, Index_type jmax, Index_type kmin, Index_type kmax,
			  Complex_ptr t0, Complex_ptr t1, Complex_ptr t2, Complex_ptr denac, Complex_ptr denlw)
{
  const Real_type clight=3.e+10;
  const Real_type csound=3.09e+7;
  const Real_type omega0= 0.9;
  const Real_type omegar= 0.9;
  const Real_type dt= 0.208;
  const Real_type c10 = 0.25 * (clight / csound);
  const Real_type fratio = sqrt(omegar / omega0);
  const Real_type r_fratio = 1.0/fratio;
  const Real_type c20 = 0.25 * (clight / csound) * r_fratio;
  const Complex_type ireal(0.0, 1.0);

  for (Index_type k = kmin; k < kmax; k++) {

    for (Index_type j = jmin; j < jmax; j++) {

      Index_type it0=    ((k)*(jmax+1) + (j))*(imax+1) ;
      Index_type idenac= ((k)*(jmax+2) + (j))*(imax+2) ;

      for (Index_type i = imin; i < imax; i++) {

	Complex_type c1 = c10 * denac[idenac+i];
	Complex_type c2 = c20 * denlw[it0+i];

	/* promote to doubles to avoid possible divide by zero
	   errors later on. */
	Real_type c1re = real(c1);  Real_type c1im = imag(c1);
	Real_type c2re = real(c2);  Real_type c2im = imag(c2);

	/* compute lamda = sqrt(|c1|^2 + |c2|^2) using doubles
	   to avoid underflow. */
	Real_type zlam = c1re*c1re + c1im*c1im +
	  c2re*c2re + c2im*c2im + 1.0e-34;
	zlam = sqrt(zlam);
	Real_type snlamt = sin(zlam * dt * 0.5);
	Real_type cslamt = cos(zlam * dt * 0.5);

	Complex_type a0t = t0[it0+i];
	Complex_type a1t = t1[it0+i];
	Complex_type a2t = t2[it0+i] * fratio;

	Real_type r_zlam= 1.0/zlam;
	c1 *= r_zlam;
	c2 *= r_zlam;
	Real_type zac1 = zabs2(c1);
	Real_type zac2 = zabs2(c2);

	/* compute new A0 */
	Complex_type z3 = ( c1 * a1t + c2 * a2t ) * snlamt ;
	t0[it0+i] = a0t * cslamt -  ireal * z3;

	/* compute new A1  */
	Real_type r = zac1 * cslamt + zac2;
	Complex_type z5 = c2 * a2t;
	Complex_type z4 = conj(c1) * z5 * (cslamt-1);
	z3 = conj(c1) * a0t * snlamt;
	t1[it0+i] = a1t * r + z4 - ireal * z3;

	/* compute new A2  */
	r = zac1 + zac2 * cslamt;
	z5 = c1 * a1t;
	z4 = conj(c2) * z5 * (cslamt-1);
	z3 = conj(c2) * a0t * snlamt;
	t2[it0+i] = ( a2t * r + z4 - ireal * z3 ) * r_fratio;

      }  // i loop

    }  // j loop

  }  // k loop
}
