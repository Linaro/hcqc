int TIME_SPLIT = 1;

extern void cnvvar_rhokin_ijkl (int ADM_lall,
				int ADM_lall_pl,
				int ADM_gall_1d,
				int ADM_gall,
				int ADM_gall_pl,
				int ADM_kall,
				double M1   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M1_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				double M2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				double M3   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M3_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				double M4   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M4_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				double M5   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M5_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				double M6   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				double M6_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1]);

extern void src_advection_convergence (int ADM_lall,
				       int ADM_lall_pl,
				       int ADM_gall_1d,
				       int ADM_gall,
				       int ADM_gall_pl,
				       int ADM_kall,
				       double M1   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M1_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       double M2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       double M3   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M3_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       double M4   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M4_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       double M5   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M5_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       double M6   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				       double M6_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				       int I_SRC_horizontal);

extern void BNDCND_rhow (int LLL,
			 int GGG,
			 int KKK,
			 int l,
			 double M1[GGG+1][   KKK+1][LLL   +1],
			 double M2[GGG+1][   KKK+1][LLL   +1],
			 double M3[GGG+1][   KKK+1][LLL   +1],
			 double M4[GGG+1][   KKK+1][LLL   +1],
			 int x);

extern void src_flux_convergence (int ADM_lall,
				  int ADM_lall_pl,
				  int ADM_gall_1d,
				  int ADM_gall,
				  int ADM_gall_pl,
				  int ADM_kall,
				  double rhogvx_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				  double rhogvx_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				  double rhogvy_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				  double rhogvy_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				  double rhogvz_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				  double rhogvz_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				  double rhogw_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				  double rhogw_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				  double drhog_split       [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
				  double drhog_split_pl    [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
				  int I_SRC_horizontal);

extern void  vi_rhow (int ADM_lall,
		      int ADM_lall_pl,
		      int ADM_gall_1d,
		      int ADM_gall,
		      int ADM_gall_pl,
		      int ADM_kall,
		      double rhogw_split2    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double rhogw_split2_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double rhogw_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double rhogw_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double pregd_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double pregd_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double rhog_split      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double rhog_split_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double drhog1    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double drhog1_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double grhogw      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double grhogw_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double dpre1     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
		      double dpre1_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
		      double dt);

void vi_path2 (int ADM_lall,
	       int ADM_lall_pl,
	       int ADM_gall_1d,
	       int ADM_gall,
	       int ADM_gall_pl,
	       int ADM_kall,
	       int ADM_prc_me,
	       int ADM_prc_pl,
	       int I_SRC_horizontal,
	       int I_SRC_default,
	       int CNST_RAIR,
	       int CNST_CV,
	       int VMTR_C2Wfact,
	       int VMTR_C2Wfact_pl,
	       double rhog_split2     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhog_split2_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvx_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvx_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvy_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvy_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvz_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvz_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogw_split2    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogw_split2_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhoge_split2    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhoge_split2_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	       double phi    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double phi_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	       double rhog_split      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhog_split_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvx_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvx_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvy_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvy_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvz_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvz_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogw_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogw_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhoge_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhoge_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double pregd_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double pregd_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	       double rhog     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhog_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvx   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvx_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvy   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvy_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogvz   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogvz_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double rhogw    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double rhogw_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double eth      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double eth_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	       double grhog       [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double grhog_pl    [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double grhogw      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double grhogw_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double grhoge      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double grhoge_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	       double grhogetot   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	       double grhogetot_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	       double dt)
{
  double drhog_split       [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double drhog_split_pl    [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double drhoge_split      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double drhoge_split_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double drhogetot_split   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double drhogetot_split_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];

  double drhog1    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double drhog1_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double drhoge1   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double drhoge1_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double dpre1     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double dpre1_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];

  double wk_rhog     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double wk_rhog_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double wk_rhogvx   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double wk_rhogvx_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double wk_rhogvy   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double wk_rhogvy_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double wk_rhogvz   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double wk_rhogvz_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double wk_rhogw    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double wk_rhogw_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];

  double rhogkin0   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double rhogkin0_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double rhogkin1   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double rhogkin1_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double rhogkin2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double rhogkin2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];
  double ethtot     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1];
  double ethtot_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1];

  int ij, k, l;

  cnvvar_rhokin_ijkl (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall, rhog, rhog_pl, rhogvx, rhogvx_pl, rhogvy, rhogvy_pl, rhogvz, rhogvz_pl, rhogw, rhogw_pl, rhogkin0, rhogkin0_pl);

  for (l = 1; l <= ADM_lall; l++) {
    for (k = 1; k <= ADM_kall; k++) {
      for (ij = 1; ij <= ADM_gall; ij++) {
	wk_rhog [ij][k][l] = rhog [ij][k][l] + rhog_split [ij][k][l];
	wk_rhogvx[ij][k][l] = rhogvx[ij][k][l] + rhogvx_split[ij][k][l];
	wk_rhogvy[ij][k][l] = rhogvy[ij][k][l] + rhogvy_split[ij][k][l];
	wk_rhogvz[ij][k][l] = rhogvz[ij][k][l] + rhogvz_split[ij][k][l];
	wk_rhogw [ij][k][l] = rhogw [ij][k][l] + rhogw_split [ij][k][l];
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (l = 1; l <= ADM_lall_pl; l++) {
      for (k = 1; k <= ADM_kall; k++) {
	for (ij = 1; ij <= ADM_gall_pl; ij++) {
	  wk_rhog_pl [ij][k][l] = rhog_pl [ij][k][l] + rhog_split_pl [ij][k][l];
	  wk_rhogvx_pl[ij][k][l] = rhogvx_pl[ij][k][l] + rhogvx_split_pl[ij][k][l];
	  wk_rhogvy_pl[ij][k][l] = rhogvy_pl[ij][k][l] + rhogvy_split_pl[ij][k][l];
	  wk_rhogvz_pl[ij][k][l] = rhogvz_pl[ij][k][l] + rhogvz_split_pl[ij][k][l];
	  wk_rhogw_pl [ij][k][l] = rhogw_pl [ij][k][l] + rhogw_split_pl [ij][k][l];
	}
      }
    }
  }

  cnvvar_rhokin_ijkl (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall, wk_rhog, wk_rhog_pl, wk_rhogvx, wk_rhogvx_pl, wk_rhogvy, wk_rhogvy_pl, wk_rhogvz, wk_rhogvz_pl, wk_rhogw, wk_rhogw_pl, rhogkin1, rhogkin1_pl);

  if (TIME_SPLIT) {
    src_flux_convergence (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
			  rhogvx_split2, rhogvx_split2_pl,
			  rhogvy_split2, rhogvy_split2_pl,
			  rhogvz_split2, rhogvz_split2_pl,
			  rhogw_split, rhogw_split_pl,
			  drhog_split, drhog_split_pl,
			  I_SRC_horizontal);
  } else {
    for (int i = 1; i <= ADM_gall; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall; k++) {
	  drhog_split [i][j][k] = 0.0e0;
	}
      }
    }
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  drhog_split_pl[i][j][k] = 0.0e0;
	}
      }
    }
  }

  if (TIME_SPLIT) {
    src_advection_convergence (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
			       rhogvx_split2, rhogvx_split2_pl,
			       rhogvy_split2, rhogvy_split2_pl,
			       rhogvz_split2, rhogvz_split2_pl,
			       rhogw_split, rhogw_split_pl,
			       eth, eth_pl,
			       drhoge_split, drhoge_split_pl,
			       I_SRC_horizontal);
  } else {
    for (int i = 1; i <= ADM_gall; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall; k++) {
	  drhoge_split [i][j][k] = 0.0e0;
	}
      }
    }
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  drhoge_split_pl[i][j][k] = 0.0e0;
	}
      }
    }
  }


  for (l = 1; l <= ADM_lall; l++) {
    for (k = 1; k <= ADM_kall; k++) {
      for (ij = 1; ij <= ADM_gall; ij++) {
	drhog1[ij][k][l] = grhog[ij][k][l] + drhog_split[ij][k][l];
	drhoge1[ij][k][l] = grhoge[ij][k][l] + drhoge_split[ij][k][l];
	dpre1[ij][k][l] = drhoge1[ij][k][l] * CNST_RAIR / CNST_CV;
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (l = 1; l <= ADM_lall_pl; l++) {
      for (k = 1; k <= ADM_kall; k++) {
	for (ij = 1; ij <= ADM_gall_pl; ij++) {
	  drhog1_pl[ij][k][l] = grhog_pl[ij][k][l] + drhog_split_pl[ij][k][l];
	  drhoge1_pl[ij][k][l] = grhoge_pl[ij][k][l] + drhoge_split_pl[ij][k][l];
	  dpre1_pl[ij][k][l] = drhoge1_pl[ij][k][l] * CNST_RAIR / CNST_CV;
	}
      }
    }
  }

  for (l = 1; l <= ADM_lall; l++) {
    for (k = 1; k <= ADM_kall; k++) {
      for (ij = 1; ij <= ADM_gall; ij++) {
	rhogw_split2[ij][k][l] = 0.0e0;
      }
    }
    BNDCND_rhow (ADM_lall,
		 ADM_gall,
		 ADM_kall,
		 l,
		 rhogvx_split2,
		 rhogvy_split2,
		 rhogvz_split2,
		 rhogw_split2,
		 VMTR_C2Wfact);
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (l = 1; l <= ADM_lall_pl; l++) {
      for (k = 1; k <= ADM_kall; k++) {
	for (ij = 1; ij <= ADM_gall_pl; ij++) {
	  rhogw_split2_pl[ij][k][l] = 0.0e0;
	}
      }

      BNDCND_rhow (ADM_lall_pl,
		   ADM_gall_pl,
		   ADM_kall,
		   l,
		   rhogvx_split2_pl,
		   rhogvy_split2_pl,
		   rhogvz_split2_pl,
		   rhogw_split2_pl,
		   VMTR_C2Wfact_pl);
    }
  }

  vi_rhow (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
	   rhogw_split2, rhogw_split2_pl,
	   rhogw_split, rhogw_split_pl,
	   pregd_split, pregd_split_pl,
	   rhog_split, rhog_split_pl,
	   drhog1, drhog1_pl,
	   grhogw, grhogw_pl,
	   dpre1, dpre1_pl,
	   dt);

  src_flux_convergence (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
			rhogvx_split2, rhogvx_split2_pl,
			rhogvy_split2, rhogvy_split2_pl,
			rhogvz_split2, rhogvz_split2_pl,
			rhogw_split2, rhogw_split2_pl,
			drhog_split, drhog_split_pl,
			I_SRC_default);

  for (int i = 1; i <= ADM_gall; i++) {
    for (int j = 1; j <= ADM_kall; j++) {
      for (int k = 1; k <= ADM_lall; k++) {
	rhog_split2[i][j][k] = rhog_split[i][j][k] + (grhog[i][j][k] + drhog_split[i][j][k]) * dt;
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  rhog_split2_pl[i][j][k] = rhog_split_pl[i][j][k] + (grhog_pl[i][j][k] + drhog_split_pl[i][j][k]) * dt;
	}
      }
    }
  }

  for (int i = 1; i <= ADM_gall; i++) {
    for (int j = 1; j <= ADM_kall; j++) {
      for (int k = 1; k <= ADM_lall; k++) {
	wk_rhog [i][j][k] = rhog [i][j][k] + rhog_split2 [i][j][k];
	wk_rhogvx[i][j][k] = rhogvx[i][j][k] + rhogvx_split2[i][j][k];
	wk_rhogvy[i][j][k] = rhogvy[i][j][k] + rhogvy_split2[i][j][k];
	wk_rhogvz[i][j][k] = rhogvz[i][j][k] + rhogvz_split2[i][j][k];
	wk_rhogw [i][j][k] = rhogw [i][j][k] + rhogw_split2 [i][j][k];
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  wk_rhog_pl [i][j][k] = rhog_pl [i][j][k] + rhog_split2_pl [i][j][k];
	  wk_rhogvx_pl[i][j][k] = rhogvx_pl[i][j][k] + rhogvx_split2_pl[i][j][k];
	  wk_rhogvy_pl[i][j][k] = rhogvy_pl[i][j][k] + rhogvy_split2_pl[i][j][k];
	  wk_rhogvz_pl[i][j][k] = rhogvz_pl[i][j][k] + rhogvz_split2_pl[i][j][k];
	  wk_rhogw_pl [i][j][k] = rhogw_pl [i][j][k] + rhogw_split2_pl [i][j][k];
	}
      }
    }
  }

  cnvvar_rhokin_ijkl (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
		      wk_rhog, wk_rhog_pl,
		      wk_rhogvx, wk_rhogvx_pl,
		      wk_rhogvy, wk_rhogvy_pl,
		      wk_rhogvz, wk_rhogvz_pl,
		      wk_rhogw, wk_rhogw_pl,
		      rhogkin2, rhogkin2_pl);

  for (int i = 1; i <= ADM_gall; i++) {
    for (int j = 1; j <= ADM_kall; j++) {
      for (int k = 1; k <= ADM_lall; k++) {
	ethtot[i][j][k] = eth[i][j][k] + rhogkin0[i][j][k] / rhog[i][j][k] + phi[i][j][k];
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  ethtot_pl[i][j][k] = eth_pl[i][j][k] + rhogkin0_pl[i][j][k] / rhog_pl[i][j][k] + phi_pl[i][j][k];
	}
      }
    }
  }

  src_advection_convergence (ADM_lall, ADM_lall_pl, ADM_gall_1d, ADM_gall, ADM_gall_pl, ADM_kall,
			     wk_rhogvx, wk_rhogvx_pl,
			     wk_rhogvy, wk_rhogvy_pl,
			     wk_rhogvz, wk_rhogvz_pl,
			     wk_rhogw, wk_rhogw_pl,
			     ethtot, ethtot_pl,
			     drhogetot_split, drhogetot_split_pl,
			     I_SRC_default);

  for (int i = 1; i <= ADM_gall; i++) {
    for (int j = 1; j <= ADM_kall; j++) {
      for (int k = 1; k <= ADM_lall; k++) {
	rhoge_split2[i][j][k] = rhoge_split [i][j][k] + (grhogetot [i][j][k] + drhogetot_split[i][j][k]) * dt + (rhogkin1 [i][j][k] - rhogkin2 [i][j][k]) + (rhog_split [i][j][k] - rhog_split2 [i][j][k]) * phi[i][j][k] ;
      }
    }
  }

  if (ADM_prc_me == ADM_prc_pl) {
    for (int i = 1; i <= ADM_gall_pl; i++) {
      for (int j = 1; j <= ADM_kall; j++) {
	for (int k = 1; k <= ADM_lall_pl; k++) {
	  rhoge_split2_pl[i][j][k] = rhoge_split_pl [i][j][k] + (grhogetot_pl [i][j][k] + drhogetot_split_pl[i][j][k]) * dt + (rhogkin1_pl [i][j][k] - rhogkin2_pl [i][j][k]) + (rhog_split_pl[i][j][k] - rhog_split2_pl [i][j][k]) * phi_pl[i][j][k];
	}
      }
    }
  }
  return;
}
