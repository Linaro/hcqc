void FLD3X2(int IMODE,
	    int ME,
	    int NE,
	    int NP,
	    int NEX[9],
	    int N1,
	    float FXYZ[4][NP+1],
	    float FE[NE+1],
	    int NODE[N1+1][NE+1],
	    float DNXI[N1+1][ME+1],
	    float DNYI[N1+1][ME+1],
	    float DNZI[N1+1][ME+1])
{
  int IE;
  int NEHEX;

  if (IMODE != 1)
    return;

  NEHEX=NEX[4];

  for (IE = 1; IE <= NEHEX; IE++) {
    FE[IE] = DNXI[1][IE]*FXYZ[1][NODE[1][IE]]
      + DNXI[2][IE]*FXYZ[1][NODE[2][IE]]
      + DNXI[3][IE]*FXYZ[1][NODE[3][IE]]
      + DNXI[4][IE]*FXYZ[1][NODE[4][IE]]
      + DNXI[5][IE]*FXYZ[1][NODE[5][IE]]
      + DNXI[6][IE]*FXYZ[1][NODE[6][IE]]
      + DNXI[7][IE]*FXYZ[1][NODE[7][IE]]
      + DNXI[8][IE]*FXYZ[1][NODE[8][IE]]

      + DNYI[1][IE]*FXYZ[2][NODE[1][IE]]
      + DNYI[2][IE]*FXYZ[2][NODE[2][IE]]
      + DNYI[3][IE]*FXYZ[2][NODE[3][IE]]
      + DNYI[4][IE]*FXYZ[2][NODE[4][IE]]
      + DNYI[5][IE]*FXYZ[2][NODE[5][IE]]
      + DNYI[6][IE]*FXYZ[2][NODE[6][IE]]
      + DNYI[7][IE]*FXYZ[2][NODE[7][IE]]
      + DNYI[8][IE]*FXYZ[2][NODE[8][IE]]

      + DNZI[1][IE]*FXYZ[3][NODE[1][IE]]
      + DNZI[2][IE]*FXYZ[3][NODE[2][IE]]
      + DNZI[3][IE]*FXYZ[3][NODE[3][IE]]
      + DNZI[4][IE]*FXYZ[3][NODE[4][IE]]
      + DNZI[5][IE]*FXYZ[3][NODE[5][IE]]
      + DNZI[6][IE]*FXYZ[3][NODE[6][IE]]
      + DNZI[7][IE]*FXYZ[3][NODE[7][IE]]
      + DNZI[8][IE]*FXYZ[3][NODE[8][IE]];
  }
}
