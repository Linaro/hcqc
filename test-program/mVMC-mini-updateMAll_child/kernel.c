int Nsite;
int Ne;
int Nsize;
int Nsite2;
double *SlaterElm;
double *InvM;
double *PfM;

void updateMAll_child(const int ma, const int s, const int *eleIdx,
                      const int qpStart, const int qpEnd, const int qpidx,
                      double *vec1, double *vec2) {
  const int msa = ma+s*Ne;
  const int rsa = eleIdx[msa] + s*Nsite;
  const int nsize = Nsize;

  int msi,msj,rsj;

  const double *sltE_a;
  double sltE_aj;
  double *invM;
  double *invM_i,*invM_j,*invM_a;

  double vec1_i,vec2_i;
  double invVec1_a;
  double tmp;

  sltE_a = SlaterElm + (qpidx+qpStart)*Nsite2*Nsite2 + rsa*Nsite2;

  invM = InvM + qpidx*Nsize*Nsize;
  invM_a = invM + msa*Nsize;

  for(msi=0;msi<nsize;msi++) vec1[msi] = 0.0;

  for(msj=0;msj<nsize;msj++) {
    rsj = eleIdx[msj] + (msj/Ne)*Nsite;
    sltE_aj = sltE_a[rsj];
    invM_j = invM + msj*Nsize;

    for(msi=0;msi<nsize;msi++) {
      vec1[msi] += -invM_j[msi] * sltE_aj;
    }
  }

  tmp = vec1[msa];
  PfM[qpidx] *= -tmp;
  invVec1_a = -1.0/tmp;

  for(msi=0;msi<nsize;msi++) {
    vec2[msi] = invM_a[msi] * invVec1_a;
  }

  for(msi=0;msi<nsize;msi++) {
    invM_i = invM + msi*Nsize;
    vec1_i = vec1[msi];
    vec2_i = vec2[msi];

    for(msj=0;msj<nsize;msj++) {
      invM_i[msj] += vec1_i * vec2[msj] - vec1[msj] * vec2_i;
    }

    invM_i[msa] -= vec2_i;
  }

  for(msj=0;msj<nsize;msj++) {
    invM_a[msj] += vec2[msj];
  }

  return;
}
