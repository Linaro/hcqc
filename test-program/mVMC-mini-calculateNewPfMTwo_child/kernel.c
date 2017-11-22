int Nsite;
int Ne;
int Nsize;
int Nsite2;
double *SlaterElm;
double *InvM;
double *PfM;

void calculateNewPfMTwo_child(const int ma, const int s, const int mb, const int t,
                              double *pfMNew, const int *eleIdx,
                              const int qpStart, const int qpEnd, const int qpidx,
                              double *vec_a, double *vec_b) {
  const int msa = ma+s*Ne;
  const int msb = mb+t*Ne;
  const int rsa = eleIdx[msa] + s*Nsite;
  const int rsb = eleIdx[msb] + t*Nsite;
  const int nsize = Nsize;

  int msi,msj;
  int rsi;

  double p_a,p_b,q_a,q_b,bMa;
  double ratio,tmp;

  const double *sltE;
  const double *sltE_a;
  const double *sltE_b;

  double *invM;
  const double *invM_a, *invM_b, *invM_i;
  double invM_ab,invM_ai,invM_bi;

  double vec_ba,vec_ai,vec_bi;

  sltE = SlaterElm + (qpidx+qpStart)*Nsite2*Nsite2;
  sltE_a = sltE + rsa*Nsite2;
  sltE_b = sltE + rsb*Nsite2;

  for(msi=0;msi<nsize;msi++) {
    rsi = eleIdx[msi] + (msi/Ne)*Nsite;
    vec_a[msi] = sltE_a[rsi];
    vec_b[msi] = sltE_b[rsi];
  }
  vec_ba = vec_b[msa];

  invM = InvM + qpidx*Nsize*Nsize;
  invM_a = invM + msa*Nsize;
  invM_b = invM + msb*Nsize;
  invM_ab = invM_a[msb];

  p_a = p_b = q_a = q_b = bMa = 0.0;

  for(msi=0;msi<nsize;msi++) {
    vec_ai = vec_a[msi];
    vec_bi = vec_b[msi];
    invM_ai = invM_a[msi];
    invM_bi = invM_b[msi];

    p_a += invM_ai * vec_ai;
    p_b += invM_bi * vec_ai;
    q_a += invM_ai * vec_bi;
    q_b += invM_bi * vec_bi;
  }

  for(msi=0;msi<nsize;msi++) {
    invM_i = invM + msi*Nsize;
    tmp = 0.0;
    for(msj=0;msj<nsize;msj++) {
      tmp += invM_i[msj] * vec_a[msj];
    }
    bMa += vec_b[msi] * tmp;
  }

  ratio = invM_ab*vec_ba + invM_ab*bMa + p_a*q_b - p_b*q_a;

  pfMNew[qpidx] = ratio*PfM[qpidx];

  return;
}
