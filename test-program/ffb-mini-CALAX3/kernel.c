void CALAX3(int NP,
	    int NCRS,
	    float A[NCRS+1],
	    float S[NP+1],
	    float AS[NP+1],
	    int IPCRS[NCRS+1],
	    int NPP[NP+1],
	    int JUNROL,
	    int NPPMAX,
	    int NCRS2,
	    float TS[NP+1],
	    float TA[NCRS2+1],
	    int ITPCRS[NCRS2+1])
{
  int IP;
  float BUF;
  int ICRS;
  int K;
  int IP2;

  for (IP = 1; IP <= NP; IP++) {
    AS[IP]=0.0E0;
  }

  if (JUNROL == 0)
    goto L500;

  TS[0]=0.0E0;

  for (IP = 1; IP <= NP; IP++) {
    TS[IP]=S[IP];
  }

  if (NPPMAX > 30)
    goto L500;

  for (IP = 1; IP <= NP; IP++) {
    BUF=0.0E0;
    ICRS=(IP-1)*30;
         BUF=BUF +TA[ICRS+ 1]*TS[ITPCRS[ICRS+ 1]]
                 +TA[ICRS+ 2]*TS[ITPCRS[ICRS+ 2]]
                 +TA[ICRS+ 3]*TS[ITPCRS[ICRS+ 3]]
                 +TA[ICRS+ 4]*TS[ITPCRS[ICRS+ 4]]
                 +TA[ICRS+ 5]*TS[ITPCRS[ICRS+ 5]]
                 +TA[ICRS+ 6]*TS[ITPCRS[ICRS+ 6]]
                 +TA[ICRS+ 7]*TS[ITPCRS[ICRS+ 7]]
                 +TA[ICRS+ 8]*TS[ITPCRS[ICRS+ 8]]
                 +TA[ICRS+ 9]*TS[ITPCRS[ICRS+ 9]]
                 +TA[ICRS+10]*TS[ITPCRS[ICRS+10]]
                 +TA[ICRS+11]*TS[ITPCRS[ICRS+11]]
                 +TA[ICRS+12]*TS[ITPCRS[ICRS+12]]
                 +TA[ICRS+13]*TS[ITPCRS[ICRS+13]]
                 +TA[ICRS+14]*TS[ITPCRS[ICRS+14]]
                 +TA[ICRS+15]*TS[ITPCRS[ICRS+15]]
                 +TA[ICRS+16]*TS[ITPCRS[ICRS+16]]
                 +TA[ICRS+17]*TS[ITPCRS[ICRS+17]]
                 +TA[ICRS+18]*TS[ITPCRS[ICRS+18]]
                 +TA[ICRS+19]*TS[ITPCRS[ICRS+19]]
                 +TA[ICRS+20]*TS[ITPCRS[ICRS+20]]
                 +TA[ICRS+21]*TS[ITPCRS[ICRS+21]]
                 +TA[ICRS+22]*TS[ITPCRS[ICRS+22]]
                 +TA[ICRS+23]*TS[ITPCRS[ICRS+23]]
                 +TA[ICRS+24]*TS[ITPCRS[ICRS+24]]
                 +TA[ICRS+25]*TS[ITPCRS[ICRS+25]]
                 +TA[ICRS+26]*TS[ITPCRS[ICRS+26]]
                 +TA[ICRS+27]*TS[ITPCRS[ICRS+27]]
                 +TA[ICRS+28]*TS[ITPCRS[ICRS+28]]
                 +TA[ICRS+29]*TS[ITPCRS[ICRS+29]]
	   +TA[ICRS+30]*TS[ITPCRS[ICRS+30]];
         AS[IP]=AS[IP]+BUF;
  }

  goto L900;

 L500:
  ICRS=0;
  for (IP = 1; IP <= NP; IP++) {
    BUF=0.0E0;
    for (K = 1; K <= NPP[IP]; K++) {
      ICRS=ICRS+1;
      IP2=IPCRS[ICRS];
      BUF=BUF+A[ICRS]*S[IP2];
    }
    AS[IP]=AS[IP]+BUF;
  }

 L900:
  ;
}
