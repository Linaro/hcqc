typedef struct
{
   double re,im;
} complex_dble;

typedef struct
{
   complex_dble c1,c2,c3;
} su3_vector_dble;

typedef struct
{
   su3_vector_dble c1,c2;
} weyl_dble;

typedef struct
{
   double u[36];
} pauli_dble;
