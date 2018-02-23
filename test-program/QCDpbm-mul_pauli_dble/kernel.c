#include "header.h"

static weyl_dble rs;

void mul_pauli_dble(pauli_dble *m,weyl_dble *s,weyl_dble *r)
{
   rs.c1.c1.re=
      (*m).u[ 0]*(*s).c1.c1.re+
      (*m).u[ 6]*(*s).c1.c2.re-(*m).u[ 7]*(*s).c1.c2.im+
      (*m).u[ 8]*(*s).c1.c3.re-(*m).u[ 9]*(*s).c1.c3.im+
      (*m).u[10]*(*s).c2.c1.re-(*m).u[11]*(*s).c2.c1.im+
      (*m).u[12]*(*s).c2.c2.re-(*m).u[13]*(*s).c2.c2.im+
      (*m).u[14]*(*s).c2.c3.re-(*m).u[15]*(*s).c2.c3.im;

   rs.c1.c1.im=
      (*m).u[ 0]*(*s).c1.c1.im+
      (*m).u[ 6]*(*s).c1.c2.im+(*m).u[ 7]*(*s).c1.c2.re+
      (*m).u[ 8]*(*s).c1.c3.im+(*m).u[ 9]*(*s).c1.c3.re+
      (*m).u[10]*(*s).c2.c1.im+(*m).u[11]*(*s).c2.c1.re+
      (*m).u[12]*(*s).c2.c2.im+(*m).u[13]*(*s).c2.c2.re+
      (*m).u[14]*(*s).c2.c3.im+(*m).u[15]*(*s).c2.c3.re;

   rs.c1.c2.re=
      (*m).u[ 6]*(*s).c1.c1.re+(*m).u[ 7]*(*s).c1.c1.im+
      (*m).u[ 1]*(*s).c1.c2.re+
      (*m).u[16]*(*s).c1.c3.re-(*m).u[17]*(*s).c1.c3.im+
      (*m).u[18]*(*s).c2.c1.re-(*m).u[19]*(*s).c2.c1.im+
      (*m).u[20]*(*s).c2.c2.re-(*m).u[21]*(*s).c2.c2.im+
      (*m).u[22]*(*s).c2.c3.re-(*m).u[23]*(*s).c2.c3.im;

   rs.c1.c2.im=
      (*m).u[ 6]*(*s).c1.c1.im-(*m).u[ 7]*(*s).c1.c1.re+
      (*m).u[ 1]*(*s).c1.c2.im+
      (*m).u[16]*(*s).c1.c3.im+(*m).u[17]*(*s).c1.c3.re+
      (*m).u[18]*(*s).c2.c1.im+(*m).u[19]*(*s).c2.c1.re+
      (*m).u[20]*(*s).c2.c2.im+(*m).u[21]*(*s).c2.c2.re+
      (*m).u[22]*(*s).c2.c3.im+(*m).u[23]*(*s).c2.c3.re;

   rs.c1.c3.re=
      (*m).u[ 8]*(*s).c1.c1.re+(*m).u[ 9]*(*s).c1.c1.im+
      (*m).u[16]*(*s).c1.c2.re+(*m).u[17]*(*s).c1.c2.im+
      (*m).u[ 2]*(*s).c1.c3.re+
      (*m).u[24]*(*s).c2.c1.re-(*m).u[25]*(*s).c2.c1.im+
      (*m).u[26]*(*s).c2.c2.re-(*m).u[27]*(*s).c2.c2.im+
      (*m).u[28]*(*s).c2.c3.re-(*m).u[29]*(*s).c2.c3.im;

   rs.c1.c3.im=
      (*m).u[ 8]*(*s).c1.c1.im-(*m).u[ 9]*(*s).c1.c1.re+
      (*m).u[16]*(*s).c1.c2.im-(*m).u[17]*(*s).c1.c2.re+
      (*m).u[ 2]*(*s).c1.c3.im+
      (*m).u[24]*(*s).c2.c1.im+(*m).u[25]*(*s).c2.c1.re+
      (*m).u[26]*(*s).c2.c2.im+(*m).u[27]*(*s).c2.c2.re+
      (*m).u[28]*(*s).c2.c3.im+(*m).u[29]*(*s).c2.c3.re;

   rs.c2.c1.re=
      (*m).u[10]*(*s).c1.c1.re+(*m).u[11]*(*s).c1.c1.im+
      (*m).u[18]*(*s).c1.c2.re+(*m).u[19]*(*s).c1.c2.im+
      (*m).u[24]*(*s).c1.c3.re+(*m).u[25]*(*s).c1.c3.im+
      (*m).u[ 3]*(*s).c2.c1.re+
      (*m).u[30]*(*s).c2.c2.re-(*m).u[31]*(*s).c2.c2.im+
      (*m).u[32]*(*s).c2.c3.re-(*m).u[33]*(*s).c2.c3.im;

   rs.c2.c1.im=
      (*m).u[10]*(*s).c1.c1.im-(*m).u[11]*(*s).c1.c1.re+
      (*m).u[18]*(*s).c1.c2.im-(*m).u[19]*(*s).c1.c2.re+
      (*m).u[24]*(*s).c1.c3.im-(*m).u[25]*(*s).c1.c3.re+
      (*m).u[ 3]*(*s).c2.c1.im+
      (*m).u[30]*(*s).c2.c2.im+(*m).u[31]*(*s).c2.c2.re+
      (*m).u[32]*(*s).c2.c3.im+(*m).u[33]*(*s).c2.c3.re;

   rs.c2.c2.re=
      (*m).u[12]*(*s).c1.c1.re+(*m).u[13]*(*s).c1.c1.im+
      (*m).u[20]*(*s).c1.c2.re+(*m).u[21]*(*s).c1.c2.im+
      (*m).u[26]*(*s).c1.c3.re+(*m).u[27]*(*s).c1.c3.im+
      (*m).u[30]*(*s).c2.c1.re+(*m).u[31]*(*s).c2.c1.im+
      (*m).u[ 4]*(*s).c2.c2.re+
      (*m).u[34]*(*s).c2.c3.re-(*m).u[35]*(*s).c2.c3.im;

   rs.c2.c2.im=
      (*m).u[12]*(*s).c1.c1.im-(*m).u[13]*(*s).c1.c1.re+
      (*m).u[20]*(*s).c1.c2.im-(*m).u[21]*(*s).c1.c2.re+
      (*m).u[26]*(*s).c1.c3.im-(*m).u[27]*(*s).c1.c3.re+
      (*m).u[30]*(*s).c2.c1.im-(*m).u[31]*(*s).c2.c1.re+
      (*m).u[ 4]*(*s).c2.c2.im+
      (*m).u[34]*(*s).c2.c3.im+(*m).u[35]*(*s).c2.c3.re;

   rs.c2.c3.re=
      (*m).u[14]*(*s).c1.c1.re+(*m).u[15]*(*s).c1.c1.im+
      (*m).u[22]*(*s).c1.c2.re+(*m).u[23]*(*s).c1.c2.im+
      (*m).u[28]*(*s).c1.c3.re+(*m).u[29]*(*s).c1.c3.im+
      (*m).u[32]*(*s).c2.c1.re+(*m).u[33]*(*s).c2.c1.im+
      (*m).u[34]*(*s).c2.c2.re+(*m).u[35]*(*s).c2.c2.im+
      (*m).u[ 5]*(*s).c2.c3.re;

   rs.c2.c3.im=
      (*m).u[14]*(*s).c1.c1.im-(*m).u[15]*(*s).c1.c1.re+
      (*m).u[22]*(*s).c1.c2.im-(*m).u[23]*(*s).c1.c2.re+
      (*m).u[28]*(*s).c1.c3.im-(*m).u[29]*(*s).c1.c3.re+
      (*m).u[32]*(*s).c2.c1.im-(*m).u[33]*(*s).c2.c1.re+
      (*m).u[34]*(*s).c2.c2.im-(*m).u[35]*(*s).c2.c2.re+
      (*m).u[ 5]*(*s).c2.c3.im;

   *r=rs;
}
