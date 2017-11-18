#include <stdlib.h>

void cnvvar_rhokin_ijkl (int ADM_lall,
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
			 double M6_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1])
{
  /* TODO */
}

void src_advection_convergence (int ADM_lall,
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
				int I_SRC_horizontal)
{
  /* TODO */
}

void BNDCND_rhow (int LLL,
		  int GGG,
		  int KKK,
		  int l,
		  double M1[GGG+1][   KKK+1][LLL   +1],
		  double M2[GGG+1][   KKK+1][LLL   +1],
		  double M3[GGG+1][   KKK+1][LLL   +1],
		  double M4[GGG+1][   KKK+1][LLL   +1],
		  int x)
{
  /* TODO */
}

void src_flux_convergence (int ADM_lall,
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
			   int I_SRC_horizontal)
{
  /* TODO */
}

void  vi_rhow (int ADM_lall,
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
	       double dt)
{
  /* TODO */
}


int main ()
{
#if 0
  /* TODO: */
  vi_path2 (ADM_lall,
	    ADM_lall_pl,
	    ADM_gall_1d,
	    ADM_gall,
	    ADM_gall_pl,
	    ADM_kall,
	    ADM_prc_me,
	    ADM_prc_pl,
	    I_SRC_horizontal,
	    I_SRC_default,
	    CNST_RAIR,
	    CNST_CV,
	    VMTR_C2Wfact,
	    VMTR_C2Wfact_pl,
	    rhog_split2     ;
	    rhog_split2_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvx_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvx_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvy_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvy_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvz_split2   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvz_split2_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogw_split2    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogw_split2_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhoge_split2    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhoge_split2_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	    phi    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    phi_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	    rhog_split      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhog_split_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvx_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvx_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvy_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvy_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvz_split    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvz_split_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogw_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogw_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhoge_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhoge_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    pregd_split     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    pregd_split_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	    rhog     [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhog_pl  [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvx   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvx_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvy   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvy_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogvz   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogvz_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    rhogw    [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    rhogw_pl [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    eth      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    eth_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	    grhog       [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    grhog_pl    [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    grhogw      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    grhogw_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    grhoge      [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    grhoge_pl   [ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],
	    grhogetot   [ADM_gall+1][   ADM_kall+1][ADM_lall   +1],
	    grhogetot_pl[ADM_gall_pl+1][ADM_kall+1][ADM_lall_pl+1],

	    dt);
#endif
  exit (0);
}
