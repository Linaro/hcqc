#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "header.hxx"

extern void runARawLoops_ENERGY_CALC (Index_type len, const Real_type rho0, const Real_type e_cut, const Real_type emin, const Real_type q_cut, Real_ptr e_new, Real_ptr e_old, Real_ptr delvc, Real_ptr p_new, Real_ptr p_old, Real_ptr q_new, Real_ptr q_old, Real_ptr work, Real_ptr compHalfStep, Real_ptr pHalfStep, Real_ptr bvc, Real_ptr pbvc, Real_ptr ql_old, Real_ptr qq_old, Real_ptr vnewc);

int main ()
{
#if 0
  runARawLoops_ENERGY_CALC (len, rho0, e_cut, emin, q_cut, e_new, e_old, delvc, p_new, p_old, q_new, q_old, work, compHalfStep, pHalfStep, bvc, pbvc, ql_old, qq_old, vnewc);
#endif
  exit (0);
}
