#include <stdio.h>
#include <stdlib.h>

extern void kernel (int N, double A[N][N], double B[N][N], double C[N][N]);

static void init_data (int N, double A[N][N], double B[N][N])
{
  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      scanf ("%lf", &A[i][j]);
    }
  }
  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      scanf ("%lf", &B[i][j]);
    }
  }
}

static void print_data (int N, double C[N][N])
{
  int i, j;

  for (i = 0; i < N; i++) {
    for (j = 0; j < N; j++) {
      printf ("%e\n", C[i][j]);
    }
  }
}

static void run (int N)
{
  double A[N][N];
  double B[N][N];
  double C[N][N];

  init_data (N, A, B);

  kernel (N, A, B, C);

  print_data (N, C);
}

int main ()
{
  run (2);

  exit (0);
}
