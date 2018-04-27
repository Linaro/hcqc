typedef int     Index_type;
typedef double  Real_type;
#include <complex>
typedef std::complex<Real_type> Complex_type;
typedef Complex_type* Complex_ptr;

#define zabs2(z)    ( real(z)*real(z)+imag(z)*imag(z) )
