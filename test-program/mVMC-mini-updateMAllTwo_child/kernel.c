int Nsite;
int Ne;
int Nsize;
int Nsite2;
double *SlaterElm;
double *InvM;
double *PfM;

void updateMAllTwo_child(const int ma, const int s, const int mb, const int t,
                         const int raOld, const int rbOld,
                         const int *eleIdx, const int qpStart, const int qpEnd, const int qpidx,
                         double *vecP, double *vecQ, double *vecS, double *vecT) {
  const int msa = ma+s*Ne;
  const int msb = mb+t*Ne;
  const int rsa = eleIdx[msa] + s*Nsite;
  const int rsb = eleIdx[msb] + t*Nsite;
  const int rsaOld = raOld + s*Nsite;
  const int rsbOld = raOld + t*Nsite;
  const int nsize = Nsize;

  const double *sltE = SlaterElm + (qpidx+qpStart)*Nsite2*Nsite2;;
  const double *sltE_a = sltE + rsa*Nsite2;
  const double *sltE_b = sltE + rsb*Nsite2;
  const double mOld_ab = sltE[rsaOld*Nsite2 + rsbOld];

  double *invM = InvM + qpidx*Nsize*Nsize;
  double *invM_a = invM + msa*Nsize;
  double *invM_b = invM + msb*Nsize;
  double *invM_i;
  double invM_ab = invM_a[msb];

  double ratio,det,invDet,bMa;
  double a,b,c,d,e,f;
  double p_i,p_j,q_i,q_j,s_i,s_j,t_i,t_j;

  int msi,msj;
  int rsi;

  for(msi=0;msi<nsize;msi++) {
    vecP[msi]=0.0;
    vecQ[msi]=0.0;
    rsi = eleIdx[msi] + (msi/Ne)*Nsite;
    vecS[msi]=sltE_a[rsi];
    vecT[msi]=sltE_b[rsi];
  }
  vecS[msb] = mOld_ab;

  for(msi=0;msi<nsize;msi++) {
    invM_i = invM + msi*Nsize;
    for(msj=0;msj<nsize;msj++) {
      vecP[msi] += invM_i[msj]*vecS[msj];
      vecQ[msi] += invM_i[msj]*vecT[msj];
    }
  }

  bMa = 0.0;
  for(msi=0;msi<nsize;msi++) {
    bMa += vecT[msi] * vecP[msi];
  }
  ratio = invM_ab*vecT[msa] + invM_ab*bMa
        + vecP[msa]*vecQ[msb] - vecP[msb]*vecQ[msa];
  PfM[qpidx] *= ratio;

  a = -vecP[msa];
  b = vecP[msb];
  c = vecQ[msa];
  d = -vecQ[msb];
  e = -bMa - vecT[msa];
  f = invM_a[msb];

  det = a*d - b*c - e*f;
  invDet = 1.0/det;

  for(msi=0;msi<nsize;msi++) {
    vecS[msi] = invDet * invM_a[msi];
    vecT[msi] = invDet * invM_b[msi];
  }

  for(msi=0;msi<nsize;msi++) {
    invM_i = invM + msi*Nsize;
    p_i = vecP[msi];
    q_i = vecQ[msi];
    s_i = vecS[msi];
    t_i = vecT[msi];

    for(msj=0;msj<nsize;msj++) {
      p_j = vecP[msj];
      q_j = vecQ[msj];
      s_j = vecS[msj];
      t_j = vecT[msj];

      invM_i[msj] += a*(q_i * t_j - q_j * t_i)
        + b*(q_i * s_j - q_j * s_i)
        + c*(p_i * t_j - p_j * t_i)
        + d*(p_i * s_j - p_j * s_i)
        + e*det*(s_i * t_j - s_j * t_i)
        + f*invDet*(p_i * q_j - q_i * p_j);
    }
    invM_i[msa] += -c*t_i -d*s_i - f*invDet*q_i;
    invM_i[msb] += -a*t_i -b*s_i + f*invDet*p_i;
  }

  for(msj=0;msj<nsize;msj++) {
    p_j = vecP[msj];
    q_j = vecQ[msj];
    s_j = vecS[msj];
    t_j = vecT[msj];

    invM_a[msj] += c*t_j + d*s_j + f*invDet*q_j;
    invM_b[msj] += a*t_j + b*s_j - f*invDet*p_j;
  }

  invM_a[msb] += f*invDet;
  invM_b[msa] -= f*invDet;

  return;
}
