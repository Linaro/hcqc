#include <stdlib.h>
#include <stdio.h>
#include "header.h"

extern double det_pauli_dble(pauli_dble *m);

int main ()
{
  int i;
  double result;
  pauli_dble m;

  for (i = 0; i < 36; i++) {
    m.u[i] = 1.1 * i;
  }

  result = det_pauli_dble(&m);

#if 0
  printf ("%e\n", result);
#endif

  if (6.138e+8 < result && result < 6.138e+9) {
    exit (0);
  } else {
    exit (1);
  }
}
