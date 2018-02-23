#include <stdlib.h>
#include <stdio.h>
#include "header.h"

#include "libHCQC.h"

extern void mul_pauli_dble(pauli_dble *m,weyl_dble *s,weyl_dble *r);

int main (int argc, char *argv[])
{
  char *check_data_filename = NULL;
  int i;
  pauli_dble m;
  weyl_dble s;
  weyl_dble r;

  if (argc != 2) {
    fprintf(stderr, "No check datafile\n");
    exit(1);
  }
  check_data_filename = argv[1];

  for (i = 0; i < 36; i++) {
    m.u[i] = 1.1 * i;
  }
  s.c1.c1.re =  1.01;
  s.c1.c1.im =  2.02;
  s.c1.c2.re =  3.03;
  s.c1.c2.im =  4.04;
  s.c1.c3.re =  5.05;
  s.c1.c3.im =  6.06;
  s.c2.c1.re =  7.07;
  s.c2.c1.im =  8.08;
  s.c2.c2.re =  9.09;
  s.c2.c2.im = 10.10;
  s.c2.c3.re = 11.11;
  s.c2.c3.im = 12.12;

  mul_pauli_dble (&m, &s, &r);

#if 0
  printf ("%e\n", r.c1.c1.re);
  printf ("%e\n", r.c1.c1.im);
  printf ("%e\n", r.c1.c2.re);
  printf ("%e\n", r.c1.c2.im);
  printf ("%e\n", r.c1.c3.re);
  printf ("%e\n", r.c1.c3.im);
  printf ("%e\n", r.c2.c1.re);
  printf ("%e\n", r.c2.c1.im);
  printf ("%e\n", r.c2.c2.re);
  printf ("%e\n", r.c2.c2.im);
  printf ("%e\n", r.c2.c3.re);
  printf ("%e\n", r.c2.c3.im);
#endif

#ifdef GEN_CHECK_DATA_P
  {
    HCQC_save_begin (check_data_filename);
    HCQC_save_data_s ("r.c1.c1.re", &r.c1.c1.re, sizeof(double));
    HCQC_save_data_s ("r.c1.c1.im", &r.c1.c1.im, sizeof(double));
    HCQC_save_data_s ("r.c1.c2.re", &r.c1.c2.re, sizeof(double));
    HCQC_save_data_s ("r.c1.c2.im", &r.c1.c2.im, sizeof(double));
    HCQC_save_data_s ("r.c1.c3.re", &r.c1.c3.re, sizeof(double));
    HCQC_save_data_s ("r.c1.c3.im", &r.c1.c3.im, sizeof(double));
    HCQC_save_data_s ("r.c2.c1.re", &r.c2.c1.re, sizeof(double));
    HCQC_save_data_s ("r.c2.c1.im", &r.c2.c1.im, sizeof(double));
    HCQC_save_data_s ("r.c2.c2.re", &r.c2.c2.re, sizeof(double));
    HCQC_save_data_s ("r.c2.c2.im", &r.c2.c2.im, sizeof(double));
    HCQC_save_data_s ("r.c2.c3.re", &r.c2.c3.re, sizeof(double));
    HCQC_save_data_s ("r.c2.c3.im", &r.c2.c3.im, sizeof(double));
    HCQC_save_end ();
  }
#else
  {
    weyl_dble r_check;
    HCQC_load_begin (check_data_filename);
    HCQC_load_data_s ("r.c1.c1.re", &r_check.c1.c1.re);
    HCQC_load_data_s ("r.c1.c1.im", &r_check.c1.c1.im);
    HCQC_load_data_s ("r.c1.c2.re", &r_check.c1.c2.re);
    HCQC_load_data_s ("r.c1.c2.im", &r_check.c1.c2.im);
    HCQC_load_data_s ("r.c1.c3.re", &r_check.c1.c3.re);
    HCQC_load_data_s ("r.c1.c3.im", &r_check.c1.c3.im);
    HCQC_load_data_s ("r.c2.c1.re", &r_check.c2.c1.re);
    HCQC_load_data_s ("r.c2.c1.im", &r_check.c2.c1.im);
    HCQC_load_data_s ("r.c2.c2.re", &r_check.c2.c2.re);
    HCQC_load_data_s ("r.c2.c2.im", &r_check.c2.c2.im);
    HCQC_load_data_s ("r.c2.c3.re", &r_check.c2.c3.re);
    HCQC_load_data_s ("r.c2.c3.im", &r_check.c2.c3.im);
    HCQC_load_end ();
    HCQC_check_data_s_d ("r.c1.c1.re", r_check.c1.c1.re, r.c1.c1.re);
    HCQC_check_data_s_d ("r.c1.c1.im", r_check.c1.c1.im, r.c1.c1.im);
    HCQC_check_data_s_d ("r.c1.c2.re", r_check.c1.c2.re, r.c1.c2.re);
    HCQC_check_data_s_d ("r.c1.c2.im", r_check.c1.c2.im, r.c1.c2.im);
    HCQC_check_data_s_d ("r.c1.c3.re", r_check.c1.c3.re, r.c1.c3.re);
    HCQC_check_data_s_d ("r.c1.c3.im", r_check.c1.c3.im, r.c1.c3.im);
    HCQC_check_data_s_d ("r.c2.c1.re", r_check.c2.c1.re, r.c2.c1.re);
    HCQC_check_data_s_d ("r.c2.c1.im", r_check.c2.c1.im, r.c2.c1.im);
    HCQC_check_data_s_d ("r.c2.c2.re", r_check.c2.c2.re, r.c2.c2.re);
    HCQC_check_data_s_d ("r.c2.c2.im", r_check.c2.c2.im, r.c2.c2.im);
    HCQC_check_data_s_d ("r.c2.c3.re", r_check.c2.c3.re, r.c2.c3.re);
    HCQC_check_data_s_d ("r.c2.c3.im", r_check.c2.c3.im, r.c2.c3.im);
  }
#endif

  if (HCQC_error == 0)
    exit (0);
  else
    exit (1);
}
