#define MIMAX            33
#define MJMAX            33
#define MKMAX            65

STORAGE_CLASS float  p[MIMAX][MJMAX][MKMAX];
STORAGE_CLASS float  a[4][MIMAX][MJMAX][MKMAX],
                     b[3][MIMAX][MJMAX][MKMAX],
                     c[3][MIMAX][MJMAX][MKMAX];
STORAGE_CLASS float  bnd[MIMAX][MJMAX][MKMAX];
STORAGE_CLASS float  wrk1[MIMAX][MJMAX][MKMAX],
                     wrk2[MIMAX][MJMAX][MKMAX];

STORAGE_CLASS int imax, jmax, kmax;
STORAGE_CLASS float omega;
