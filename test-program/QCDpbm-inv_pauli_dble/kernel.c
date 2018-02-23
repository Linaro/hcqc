#include <math.h>
#include <float.h>
#include "header.h"

#define DELTA 1.0e-04

static double rr[5];
static complex_dble aa[36], dd[6];

static int fwd_house(double eps)
{
   int i,j,k,itest;
   double r1,r2,r3;
   complex_dble z;

   itest=0;

   for (k=0;k<5;k++)
   {
      r1=aa[6*k+k].re*aa[6*k+k].re+aa[6*k+k].im*aa[6*k+k].im;
      r2=sqrt(r1);

      for (j=(k+1);j<6;j++)
         r1+=(aa[6*j+k].re*aa[6*j+k].re+aa[6*j+k].im*aa[6*j+k].im);

      if (r1>=eps)
         r1=sqrt(r1);
      else
      {
         itest=1;
         r1=1.0;
      }

      if (r2>=(DBL_EPSILON*r1))
      {
         r3=1.0/r2;
         z.re=r3*aa[6*k+k].re;
         z.im=r3*aa[6*k+k].im;
      }
      else
      {
         z.re=1.0;
         z.im=0.0;
      }

      aa[6*k+k].re+=r1*z.re;
      aa[6*k+k].im+=r1*z.im;

      r3=1.0/(r1*(r1+r2));
      rr[k]=r3;
      dd[k].re=-(r1+r2)*r3*z.re;
      dd[k].im= (r1+r2)*r3*z.im;

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

         for (i=k;i<6;i++)
         {
            aa[6*i+j].re-=(z.re*aa[6*i+k].re-z.im*aa[6*i+k].im);
            aa[6*i+j].im-=(z.re*aa[6*i+k].im+z.im*aa[6*i+k].re);
         }
      }
   }

   r1=aa[35].re*aa[35].re+aa[35].im*aa[35].im;

   if (r1>=eps)
      r1=1.0/r1;
   else
   {
      itest=1;
      r1=1.0;
   }

   dd[5].re= r1*aa[35].re;
   dd[5].im=-r1*aa[35].im;

   return itest;
}

static void solv_sys(void)
{
   int i,j,k;
   complex_dble z;

   for (k=5;k>0;k--)
   {
      for (i=(k-1);i>=0;i--)
      {
         z.re=aa[6*i+k].re*dd[k].re-aa[6*i+k].im*dd[k].im;
         z.im=aa[6*i+k].re*dd[k].im+aa[6*i+k].im*dd[k].re;

         for (j=(k-1);j>i;j--)
         {
            z.re+=(aa[6*i+j].re*aa[6*j+k].re-aa[6*i+j].im*aa[6*j+k].im);
            z.im+=(aa[6*i+j].re*aa[6*j+k].im+aa[6*i+j].im*aa[6*j+k].re);
         }

         aa[6*i+k].re=-dd[i].re*z.re+dd[i].im*z.im;
         aa[6*i+k].im=-dd[i].re*z.im-dd[i].im*z.re;
      }
   }
}


static void bck_house(void)
{
   int i,j,k;
   complex_dble z;

   aa[35].re=dd[5].re;
   aa[35].im=dd[5].im;

   for (k=4;k>=0;k--)
   {
      z.re=dd[k].re;
      z.im=dd[k].im;
      dd[k].re=aa[6*k+k].re;
      dd[k].im=aa[6*k+k].im;
      aa[6*k+k].re=z.re;
      aa[6*k+k].im=z.im;

      for (j=(k+1);j<6;j++)
      {
         dd[j].re=aa[6*j+k].re;
         dd[j].im=aa[6*j+k].im;
         aa[6*j+k].re=0.0;
         aa[6*j+k].im=0.0;
      }

      for (i=0;i<6;i++)
      {
         z.re=0.0;
         z.im=0.0;

         for (j=k;j<6;j++)
         {
            z.re+=(aa[6*i+j].re*dd[j].re-aa[6*i+j].im*dd[j].im);
            z.im+=(aa[6*i+j].re*dd[j].im+aa[6*i+j].im*dd[j].re);
         }

         z.re*=rr[k];
         z.im*=rr[k];

         for (j=k;j<6;j++)
         {
            aa[6*i+j].re-=(z.re*dd[j].re+z.im*dd[j].im);
            aa[6*i+j].im+=(z.re*dd[j].im-z.im*dd[j].re);
         }
      }
   }
}

int inv_pauli_dble(pauli_dble *m,pauli_dble *im)
{
   int i,j,itest;
   double eps,sm,*u;
   complex_dble *z;

   sm=0.0;
   u=(*m).u;
   z=(complex_dble*)(u+6);

   for (i=0;i<6;i++)
   {
      aa[6*i+i].re=*u;
      aa[6*i+i].im=0.0;
      sm+=(*u)*(*u);
      u+=1;

      for (j=i+1;j<6;j++)
      {
         aa[6*i+j].re= (*z).re;
         aa[6*i+j].im= (*z).im;
         aa[6*j+i].re= (*z).re;
         aa[6*j+i].im=-(*z).im;
         sm+=2.0*((*z).re*(*z).re+(*z).im*(*z).im);
         z+=1;
      }
   }

   eps=DELTA*sm;

   itest=fwd_house(eps);
   solv_sys();
   bck_house();

   sm=0.0;
   u=(*im).u;
   z=(complex_dble*)(u+6);

   for (i=0;i<6;i++)
   {
      *u=aa[6*i+i].re;
      sm+=(*u)*(*u);
      u+=1;

      for (j=i+1;j<6;j++)
      {
         (*z).re=aa[6*i+j].re;
         (*z).im=aa[6*i+j].im;
         sm+=2.0*((*z).re*(*z).re+(*z).im*(*z).im);
         z+=1;
      }
   }

   if ((eps*sm)>1.0)
      itest=1;

   return itest;
}
