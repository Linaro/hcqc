#include <stdio.h>
#include <stdlib.h>
#include "header.h"

#include "libHCQC.h"

SparseMatrix A;
Vector r;
Vector x;

extern int ComputeSYMGS_ref();

#define MAX_A_localNumberOfRows 2197
#define MAX_currentNumberOfNonzeros 11

#define DUMMY_DATA_1 1.1
#define DUMMY_DATA_2 2.2
#define DUMMY_DATA_3 3.3

static void init ()
{
  A.localNumberOfRows = MAX_A_localNumberOfRows;
  x.localLength = MAX_A_localNumberOfRows;

  A.nonzerosInRow = (char *)calloc (MAX_A_localNumberOfRows, sizeof(char));
  if (A.nonzerosInRow == NULL)
    abort ();
  A.matrixValues = (double**)calloc(MAX_A_localNumberOfRows, sizeof(double*));
  if (A.matrixValues == NULL)
    abort ();
  A.mtxIndL = (local_int_t **)calloc (MAX_A_localNumberOfRows, sizeof(local_int_t*));
  if (A.mtxIndL == NULL)
    abort ();
  A.matrixDiagonal = (double**)calloc(MAX_A_localNumberOfRows, sizeof(double*));
  if (A.matrixDiagonal == NULL)
    abort ();

  r.values = (double*)calloc(MAX_A_localNumberOfRows, sizeof(double));
  if (r.values == NULL)
    abort ();
  x.values = (double*)calloc(MAX_A_localNumberOfRows, sizeof(double));
  if (x.values == NULL)
    abort ();
  for (int i = 0; i < A.localNumberOfRows; i++) {
    int currentNumberOfNonzeros;
    if (i % 2 == 0) {
      currentNumberOfNonzeros = 7;
    } else {
      currentNumberOfNonzeros = 11;
    }
    A.nonzerosInRow[i] = (char)currentNumberOfNonzeros;
    A.matrixDiagonal[i] = (double*)malloc(sizeof(double));
    if (A.matrixDiagonal[i] == NULL)
      abort ();
    A.matrixDiagonal[i][0] = 1.1;
    {
      local_int_t *currentColIndices = NULL;
      local_int_t curCol = 0;
      local_int_t c1 = 0;
      local_int_t c2 = 1;
      double *currentValues = NULL;
      currentColIndices = (int *)calloc(currentNumberOfNonzeros, sizeof(local_int_t));
      if (currentColIndices == NULL)
	abort ();
      A.mtxIndL[i] = currentColIndices;
      currentValues = (double *)calloc(currentNumberOfNonzeros, sizeof(double));
      if (currentValues == NULL)
	abort ();
      A.matrixValues[i] = currentValues;
      for (int j = 0; j < currentNumberOfNonzeros; j++) {
	currentColIndices[j] = curCol;
	curCol = c1 + c2;
	c1 = c2;
	c2 = curCol;
	currentValues[j] = DUMMY_DATA_1 + (double)j;
      }
    }
    r.values[i] = DUMMY_DATA_2 + (double)i;
    x.values[i] = DUMMY_DATA_3 + (double)i;
  }
}

int main (int argc, char *argv[])
{
  char *check_data_filename = NULL;
  int i, j;

  if (argc != 2) {
    fprintf(stderr, "No check datafile\n");
    exit(1);
  }
  check_data_filename = argv[1];

  init ();

  ComputeSYMGS_ref();

#if 0
  for (int i = 0; i < MAX_A_localNumberOfRows; i++) {
    printf ("%f\n", x.values[i]);
  }
#endif

#ifdef GEN_CHECK_DATA_P
  {
    HCQC_save_begin (check_data_filename);
    HCQC_save_data_a ("x.values", (const void*)x.values, sizeof(double), (long long int)MAX_A_localNumberOfRows);
    HCQC_save_end ();
  }
#else
  {
    double x_values_check[MAX_A_localNumberOfRows];

    HCQC_load_begin (check_data_filename);
    HCQC_load_data_a ("x.values", (void*)x_values_check);
    HCQC_load_end ();
    HCQC_check_data_a_d ("x.values", (double*)x_values_check, (double*)x.values, (long long int)MAX_A_localNumberOfRows);
  }
#endif

  free (A.nonzerosInRow);
  for (int i = 0; i < MAX_A_localNumberOfRows; i++) {
    free (A.matrixDiagonal[i]);
    free (A.mtxIndL[i]);
    free (A.matrixValues[i]);
  }
  free (A.matrixValues);
  free (A.mtxIndL);
  free (A.matrixDiagonal);
  free (r.values);
  free (x.values);

  if (HCQC_error == 0)
    exit (0);
  else
    exit (1);
}
