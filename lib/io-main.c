#include <stdio.h>
#include <stdlib.h>

#include "libHCQC.h"

static int AAA[3][5];
static int BBB[3][5];
static int CCC[3][5];

extern void kernel (int AAA[3][5], int BBB[3][5], int CCC[3][5], int *i_);

int main ()
{
  int i, j;

  for (i = 0; i < 3; i++) {
    for (j = 0; j < 5; j++) {
      AAA[i][j] = i + j;
      BBB[i][j] = i - j;
    }
  }
  kernel (AAA, BBB, CCC, &i);
#ifdef INIT_PHASE_P
  {
    HCQC_save_begin ();
    HCQC_save_data_a ("CCC", (const void*)CCC, sizeof(int), 15LL);
    HCQC_save_data_s ("i", &i, sizeof(int));
    HCQC_save_end ();
  }
#else
  {
    int CCC_check[3][5];
    int i_check;

    HCQC_load_begin ();
    HCQC_load_data_a ("CCC", (void*)CCC_check);
    HCQC_load_data_s ("i", &i_check);
    HCQC_load_end ();
    HCQC_check_data_a_i ("CCC", (int*)CCC_check, (int*)CCC, 15LL);
    HCQC_check_data_s_i ("i", i_check, i);
  }
#endif
}
