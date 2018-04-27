#include "header.hxx"
#include <math.h>

void runARawLoops_ENERGY_CALC (Index_type len, const Real_type rho0, const Real_type e_cut, const Real_type emin, const Real_type q_cut, Real_ptr e_new, Real_ptr e_old, Real_ptr delvc, Real_ptr p_new, Real_ptr p_old, Real_ptr q_new, Real_ptr q_old, Real_ptr work, Real_ptr compHalfStep, Real_ptr pHalfStep, Real_ptr bvc, Real_ptr pbvc, Real_ptr ql_old, Real_ptr qq_old, Real_ptr vnewc)
{
  for (Index_type i=0 ; i<len ; i++ ) {
    e_new[i] = e_old[i] - 0.5 * delvc[i] * 
      (p_old[i] + q_old[i]) + 0.5 * work[i];
  }

  for (Index_type i=0 ; i<len ; i++ ) {
    if ( delvc[i] > 0.0 ) {
      q_new[i] = 0.0 ;
    }
    else {
      Real_type vhalf = 1.0 / (1.0 + compHalfStep[i]) ;
      Real_type ssc = ( pbvc[i] * e_new[i]
			+ vhalf * vhalf * bvc[i] * pHalfStep[i] ) / rho0 ;

      if ( ssc <= 0.1111111e-36 ) {
	ssc = 0.3333333e-18 ;
      } else {
	ssc = sqrt(ssc) ;
      }

      q_new[i] = (ssc*ql_old[i] + qq_old[i]) ;
    }
  }

  for (Index_type i=0 ; i<len ; i++ ) {
    e_new[i] = e_new[i] + 0.5 * delvc[i]
      * ( 3.0*(p_old[i] + q_old[i])
	  - 4.0*(pHalfStep[i] + q_new[i])) ;
  }

  for (Index_type i=0 ; i<len ; i++ ) {
    e_new[i] += 0.5 * work[i];

    if ( fabs(e_new[i]) < e_cut ) { e_new[i] = 0.0  ; }

    if ( e_new[i]  < emin ) { e_new[i] = emin ; }
  }

  for (Index_type i=0 ; i<len ; i++ ) {
    Real_type q_tilde ;

    if (delvc[i] > 0.0) { 
      q_tilde = 0. ; 
    }
    else {
      Real_type ssc = ( pbvc[i] * e_new[i]
                        + vnewc[i] * vnewc[i] * bvc[i] * p_new[i] ) / rho0 ;

      if ( ssc <= 0.1111111e-36 ) {
	ssc = 0.3333333e-18 ;
      } else {
	ssc = sqrt(ssc) ;
      }

      q_tilde = (ssc*ql_old[i] + qq_old[i]) ;
    }

    e_new[i] = e_new[i] - ( 7.0*(p_old[i] + q_old[i])
			    - 8.0*(pHalfStep[i] + q_new[i])
			    + (p_new[i] + q_tilde)) * delvc[i] / 6.0 ;

    if ( fabs(e_new[i]) < e_cut ) {
      e_new[i] = 0.0  ;
    }
    if ( e_new[i]  < emin ) {
      e_new[i] = emin ;
    }
  }

  for (Index_type i=0 ; i<len ; i++ ) {
    if ( delvc[i] <= 0.0 ) {
      Real_type ssc = ( pbvc[i] * e_new[i]
			+ vnewc[i] * vnewc[i] * bvc[i] * p_new[i] ) / rho0 ;

      if ( ssc <= 0.1111111e-36 ) {
	ssc = 0.3333333e-18 ;
      } else {
	ssc = sqrt(ssc) ;
      }

      q_new[i] = (ssc*ql_old[i] + qq_old[i]) ;

      if (fabs(q_new[i]) < q_cut) q_new[i] = 0.0 ;
    }
  }
}
