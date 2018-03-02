#define	PROBLEM_SIZE 162

#define	IMAX	PROBLEM_SIZE
#define	JMAX	PROBLEM_SIZE
#define	KMAX	PROBLEM_SIZE

extern int grid_points[3];	/* grid_ponts(1:3) */

extern double tx1, tx2, tx3, ty1, ty2, ty3, tz1, tz2, tz3;
extern double dssp, dt;
extern double ce[5][13];	/* ce(5,13) */
extern double xxcon1, xxcon2, xxcon3, xxcon4, xxcon5;
extern double dx1tx1, dx2tx1, dx3tx1, dx4tx1, dx5tx1;
extern double yycon1, yycon2, yycon3, yycon4, yycon5;
extern double dy1ty1, dy2ty1, dy3ty1, dy4ty1, dy5ty1;
extern double zzcon1, zzcon2, zzcon3, zzcon4, zzcon5;
extern double dz1tz1, dz2tz1, dz3tz1, dz4tz1, dz5tz1;
extern double conz1, c1, c2, c3, c4, c5, c4dssp, c5dssp, dtdssp;
extern double dnxm1, dnym1, dnzm1, c1c2, c1c5, c3c4, c1345;

extern double forcing[IMAX/2*2+1][JMAX/2*2+1][KMAX/2*2+1][5+1];

extern double cuf[PROBLEM_SIZE];
extern double q[PROBLEM_SIZE];
extern double ue[PROBLEM_SIZE][5];
extern double buf[PROBLEM_SIZE][5];
