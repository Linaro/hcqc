void kernel (int AAA[3][5], int BBB[3][5], int CCC[3][5], int *i_)
{
  int i;

  for (i = 0; i < 3; i++) {
    for (int j = 0; j < 5; j++) {
      CCC[i][j] = AAA[i][j] + BBB[i][j];
    }
  }

  *i_ = i;
}
