#define	PROBLEM_SIZE 162

#define ISIZ1 PROBLEM_SIZE
#define ISIZ2 PROBLEM_SIZE
#define ISIZ3 PROBLEM_SIZE

#define	C1		1.40e+00
#define	C2		0.40e+00
#define	C3		1.00e-01
#define	C4		1.00e+00
#define	C5		1.40e+00

extern int nx, ny, nz;
extern int nx0, ny0, nz0;
extern int ist, iend;
extern int jst, jend;
extern double tx1, tx2, tx3;
extern double ty1, ty2, ty3;
extern double tz1, tz2, tz3;
extern double dx1, dx2, dx3, dx4, dx5;
extern double dy1, dy2, dy3, dy4, dy5;
extern double dz1, dz2, dz3, dz4, dz5;
extern double dssp;

extern double ce[5][13];

extern double rsd[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];
extern double frct[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];
extern double flux[ISIZ1][ISIZ2/2*2+1][ISIZ3/2*2+1][5];
