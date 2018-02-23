#include <math.h>
#include <float.h>
#include "header.h"

static complex_dble aa[36];

double det_pauli_dble(pauli_dble *m)
{
   int i,j,k;
   double sm,eps,*u;
   double r1,r2,r3;
   complex_dble det,z,w,*pz;

   sm=0.0;   
   u=(*m).u;
   pz=(complex_dble*)(u+6);

   for (i=0;i<6;i++)
   {
      aa[6*i+i].re=*u;
      aa[6*i+i].im=0.0;
      sm+=(*u)*(*u);
      u+=1;

      for (j=i+1;j<6;j++)
      {
         aa[6*i+j].re= (*pz).re;
         aa[6*i+j].im= (*pz).im;
         aa[6*j+i].re= (*pz).re;
         aa[6*j+i].im=-(*pz).im;
         sm+=2.0*((*pz).re*(*pz).re+(*pz).im*(*pz).im);
         pz+=1;
      }
   }

   eps=DBL_EPSILON*sqrt(sm);
   det.re=1.0;
   det.im=0.0;

   for (k=0;k<5;k++)
   {
      r1=aa[6*k+k].re*aa[6*k+k].re+aa[6*k+k].im*aa[6*k+k].im;
      r2=sqrt(r1);

      for (j=(k+1);j<6;j++)
         r1+=(aa[6*j+k].re*aa[6*j+k].re+aa[6*j+k].im*aa[6*j+k].im);

      r1=sqrt(r1);
      
      if (r1<=eps)
         return 0.0;
      
      if (r2>=(DBL_EPSILON*r1))
      {
         r3=1.0/r2;
         z.re=r1*r3*aa[6*k+k].re;
         z.im=r1*r3*aa[6*k+k].im;
      }
      else
      {
         z.re=r1;
         z.im=0.0;
      }

      w.re=det.re*z.re-det.im*z.im;
      w.im=det.re*z.im+det.im*z.re;
      det.re=w.re;
      det.im=w.im;
      
      aa[6*k+k].re+=z.re;
      aa[6*k+k].im+=z.im;
      r3=1.0/(r1*(r1+r2));

      for (j=(k+1);j<6;j++)
      {
         z.re=0.0;
         z.im=0.0;

         for (i=k;i<6;i++)
         {
            z.re+=(aa[6*i+k].re*aa[6*i+j].re+aa[6*i+k].im*aa[6*i+j].im);
            z.im+=(aa[6*i+k].re*aa[6*i+j].im-aa[6*i+k].im*aa[6*i+j].re);
         }

         z.re*=r3;
         z.im*=r3;

         for (i=(k+1);i<6;i++)
         {
            aa[6*i+j].re-=(z.re*aa[6*i+k].re-z.im*aa[6*i+k].im);
            aa[6*i+j].im-=(z.re*aa[6*i+k].im+z.im*aa[6*i+k].re);
         }
      }
   }

   return det.re*aa[35].re-det.im*aa[35].im;
}
