#include <stdlib.h>
#include <stdio.h>
#include "header.h"

#include "libHCQC.h"

extern int inv_pauli_dble(pauli_dble *m,pauli_dble *im);

int main (int argc, char *argv[])
{
  char *check_data_filename = NULL;
  int i;
  int result;
  pauli_dble m;
  pauli_dble im;

  if (argc != 2) {
    fprintf(stderr, "No check datafile\n");
    exit(1);
  }
  check_data_filename = argv[1];

  for (i = 0; i < 36; i++) {
    m.u[i] = 1.1 * i;
    im.u[i] = 0.0;
  }

  result = inv_pauli_dble(&m, &im);

#if 0
  printf ("%d\n", result);
  for (i = 0; i < 36; i++) {
    printf ("%e\n", im.u[i]);
  }
#endif

#ifdef GEN_CHECK_DATA_P
  {
    HCQC_save_begin (check_data_filename);
    HCQC_save_data_s ("result", &result, sizeof(int));
    HCQC_save_data_a ("im.u", (const void*)im.u, sizeof(double), (long long int)36);
    HCQC_save_end ();
  }
#else
  {
    int result_check;
    pauli_dble im_check;

    HCQC_load_begin (check_data_filename);
    HCQC_load_data_s ("result", &result_check);
    HCQC_load_data_a ("im.u", (void*)im_check.u);
    HCQC_load_end ();
    HCQC_check_data_s_i ("result", result_check, result);
    HCQC_check_data_a_d ("im.u", (double*)im_check.u, (double*)im.u, (long long int)36);
  }
#endif

  if (HCQC_error == 0)
    exit (0);
  else
    exit (1);
}
