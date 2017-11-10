void GRAD3X(int MCOLOR,
	    int MCPART,
	    int NCOLOR[5],
	    int NCPART[MCOLOR+1][5],
	    int LLOOP[MCPART+1][MCOLOR+1][5],
	    int ME,
	    int N,
	    int N1,
	    int NE,
	    int NP,
	    int NODE[N1+1][NE+1],
	    float S[NE+1],
	    float DNXYZ[4][N1+1][ME+1],
	    float CM[NP+1],
	    int IPART,
	    int NDOM,
	    int MBPDOM,
	    int LDOM[NDOM+1],
	    int NBPDOM[NDOM+1],
	    int IPSLF[MBPDOM+1][NDOM+1],
	    int IPSND[MBPDOM+1][NDOM+1],
	    int NPFIX,
	    int LPFIX[NPFIX+1],
	    int NPSYMT,
	    int LPSYMT[NPSYMT+1],
	    float XPSYMT[NPSYMT+1],
	    float YPSYMT[NPSYMT+1],
	    float ZPSYMT[NPSYMT+1],
	    float FXYZ[4][NP+1],
	    float RX[N+1][ME+1],
	    float RY[N+1][ME+1],
	    int MWRK,
	    float WRKN[MWRK+1][10])
{
  int ICOLOR,ICPART,IEE,IES, IP1,IP2,IP3,IP4,IP5,IP6,IP7,IP8, MAXBUF,IP,IE,IBP;
  float SWRK, COEF;

  MAXBUF = NE*(N+1);

  for (IP = 1; IP <= NP; IP++) {
    FXYZ[1][IP] = 0.0E0;
    FXYZ[2][IP] = 0.0E0;
    FXYZ[3][IP] = 0.0E0;
  }

  for (ICOLOR = 1; ICOLOR <= NCOLOR[4]; ICOLOR++) {
    for (ICPART = 1; ICPART <= NCPART[ICOLOR][4]; ICPART++) {
      IES=LLOOP[ICPART  ][ICOLOR][4];
      IEE=LLOOP[ICPART+1][ICOLOR][4]-1;
      for (IE = IES; IE <= IEE; IE++) {
	IP1=NODE[1][IE];
	IP2=NODE[2][IE];
	IP3=NODE[3][IE];
	IP4=NODE[4][IE];
	IP5=NODE[5][IE];
	IP6=NODE[6][IE];
	IP7=NODE[7][IE];
	IP8=NODE[8][IE];

	SWRK = S[IE];

	FXYZ[1][IP1]=FXYZ[1][IP1]-SWRK*DNXYZ[1][1][IE];
	FXYZ[2][IP1]=FXYZ[2][IP1]-SWRK*DNXYZ[2][1][IE];
	FXYZ[3][IP1]=FXYZ[3][IP1]-SWRK*DNXYZ[3][1][IE];

	FXYZ[1][IP2]=FXYZ[1][IP2]-SWRK*DNXYZ[1][2][IE];
	FXYZ[2][IP2]=FXYZ[2][IP2]-SWRK*DNXYZ[2][2][IE];
	FXYZ[3][IP2]=FXYZ[3][IP2]-SWRK*DNXYZ[3][2][IE];

	FXYZ[1][IP3]=FXYZ[1][IP3]-SWRK*DNXYZ[1][3][IE];
	FXYZ[2][IP3]=FXYZ[2][IP3]-SWRK*DNXYZ[2][3][IE];
	FXYZ[3][IP3]=FXYZ[3][IP3]-SWRK*DNXYZ[3][3][IE];

	FXYZ[1][IP4]=FXYZ[1][IP4]-SWRK*DNXYZ[1][4][IE];
	FXYZ[2][IP4]=FXYZ[2][IP4]-SWRK*DNXYZ[2][4][IE];
	FXYZ[3][IP4]=FXYZ[3][IP4]-SWRK*DNXYZ[3][4][IE];

	FXYZ[1][IP5]=FXYZ[1][IP5]-SWRK*DNXYZ[1][5][IE];
	FXYZ[2][IP5]=FXYZ[2][IP5]-SWRK*DNXYZ[2][5][IE];
	FXYZ[3][IP5]=FXYZ[3][IP5]-SWRK*DNXYZ[3][5][IE];

	FXYZ[1][IP6]=FXYZ[1][IP6]-SWRK*DNXYZ[1][6][IE];
	FXYZ[2][IP6]=FXYZ[2][IP6]-SWRK*DNXYZ[2][6][IE];
	FXYZ[3][IP6]=FXYZ[3][IP6]-SWRK*DNXYZ[3][6][IE];

	FXYZ[1][IP7]=FXYZ[1][IP7]-SWRK*DNXYZ[1][7][IE];
	FXYZ[2][IP7]=FXYZ[2][IP7]-SWRK*DNXYZ[2][7][IE];
	FXYZ[3][IP7]=FXYZ[3][IP7]-SWRK*DNXYZ[3][7][IE];

	FXYZ[1][IP8]=FXYZ[1][IP8]-SWRK*DNXYZ[1][8][IE];
	FXYZ[2][IP8]=FXYZ[2][IP8]-SWRK*DNXYZ[2][8][IE];
	FXYZ[3][IP8]=FXYZ[3][IP8]-SWRK*DNXYZ[3][8][IE];
      }
    }
  }

  for (IP = 1; IP <= NP; IP++) {
    WRKN[IP][4]=FXYZ[1][IP];
    WRKN[IP][5]=FXYZ[2][IP];
    WRKN[IP][6]=FXYZ[3][IP];
  }

  /* CALL DDCOMX */

  for (IP = 1; IP <= NP; IP++) {
    FXYZ[1][IP]=WRKN[IP][4];
    FXYZ[2][IP]=WRKN[IP][5];
    FXYZ[3][IP]=WRKN[IP][6];
  }

  for (IP = 1; IP <= NP; IP++) {
    FXYZ[1][IP]=FXYZ[1][IP]*CM[IP];
    FXYZ[2][IP]=FXYZ[2][IP]*CM[IP];
    FXYZ[3][IP]=FXYZ[3][IP]*CM[IP];
  }

  for (IBP = 1; IBP <= NPFIX; IBP++) {
    IP=LPFIX[IBP];
    FXYZ[1][IP]=0.0E0;
    FXYZ[2][IP]=0.0E0;
    FXYZ[3][IP]=0.0E0;
  }

  for (IBP = 1; IBP <= NPSYMT; IBP++) {
    COEF= XPSYMT[IBP]*FXYZ[1][LPSYMT[IBP]]
      +YPSYMT[IBP]*FXYZ[2][LPSYMT[IBP]]
      +ZPSYMT[IBP]*FXYZ[3][LPSYMT[IBP]];
    FXYZ[1][LPSYMT[IBP]] = FXYZ[1][LPSYMT[IBP]]-COEF*XPSYMT[IBP];
    FXYZ[2][LPSYMT[IBP]] = FXYZ[2][LPSYMT[IBP]]-COEF*YPSYMT[IBP];
    FXYZ[3][LPSYMT[IBP]] = FXYZ[3][LPSYMT[IBP]]-COEF*ZPSYMT[IBP];
  }
}
