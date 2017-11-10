const int bc_ndag_T = 17;
const int bc_ndag_B = 16;
const int bc_ndag_N = 15;
const int bc_ndag_S = 14;
const int bc_ndag_E = 13;
const int bc_ndag_W = 12;

const int bc_diag = 9;

const int Active  = 31;

static int ibits (int i, int pos, int len)
{
  int tmp = i >> pos;
  int mask = (1 << len) - 1;
  return (tmp & mask);
}

double psor2sma_core (int sz1,
		      int sz2,
		      int sz3,
		      int g,
		      float p[sz1+g][sz2+g][sz3+g],
		      int ip,
		      int color,
		      float omg,
		      double res,
		      float b[sz1+g][sz2+g][sz3+g],
		      int bp[sz1+g][sz2+g][sz3+g],
		      double flop)
{
  int i, j, k, ix, jx, kx, idx;
  float ndag_e, ndag_w, ndag_n, ndag_s, ndag_t, ndag_b;
  float dd, ss, dp, pp;

  ix = sz1;
  jx = sz2;
  kx = sz3;

  flop = flop + ((double)(ix))*((double)(jx))*((double)(kx)) * 27.0e0 * 0.5e0;

  for (k = 1; i <= kx; k++) {
    for (j = 1; j <= jx; j++) {
      for (i = 1 + (k+j+color+ip) % 2; i <= ix; i += 2) {
	idx = bp[i][j][k];

	ndag_e = (float)(ibits(idx, bc_ndag_E, 1));
	ndag_w = (float)(ibits(idx, bc_ndag_W, 1));
	ndag_n = (float)(ibits(idx, bc_ndag_N, 1));
	ndag_s = (float)(ibits(idx, bc_ndag_S, 1));
	ndag_t = (float)(ibits(idx, bc_ndag_T, 1));
	ndag_b = (float)(ibits(idx, bc_ndag_B, 1));

	dd = 1.0 / ((float)(ibits(idx, bc_diag, 3)));
	pp = p[i][j][k];
	ss = ndag_e * p[i+1][j  ][k  ]
	  + ndag_w * p[i-1][j  ][k  ]
	  + ndag_n * p[i  ][j+1][k  ]
	  + ndag_s * p[i  ][j-1][k  ]
	  + ndag_t * p[i  ][j  ][k+1]
	  + ndag_b * p[i  ][j  ][k-1];
	dp = ( dd*ss + b[i][j][k] - pp ) * omg;
	p[i][j][k] = pp + dp;
	res = res + ((double)(dp*dp)) * ((double)(ibits(idx, Active, 1)));
      }
    }
  }

  return res;
}
