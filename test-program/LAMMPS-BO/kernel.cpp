#include <cstdio>
#include <cmath>
#include "header.h"

void BO( reax_system *system, control_params *control, simulation_data *data,
         storage *workspace, reax_list **lists, output_controls *out_control )
{
  int i, j, pj, type_i, type_j;
  int start_i, end_i, sym_index;
  double val_i, Deltap_i, Deltap_boc_i;
  double val_j, Deltap_j, Deltap_boc_j;
  double f1, f2, f3, f4, f5, f4f5, exp_f4, exp_f5;
  double exp_p1i,        exp_p2i, exp_p1j, exp_p2j;
  double temp, u1_ij, u1_ji, Cf1A_ij, Cf1B_ij, Cf1_ij, Cf1_ji;
  double Cf45_ij, Cf45_ji, p_lp1; //u_ij, u_ji
  double A0_ij, A1_ij, A2_ij, A2_ji, A3_ij, A3_ji;
  double explp1, p_boc1, p_boc2;
  single_body_parameters *sbp_i, *sbp_j;
  two_body_parameters *twbp;
  bond_order_data *bo_ij, *bo_ji;
  reax_list *bonds = (*lists) + BONDS;

  p_boc1 = system->reax_param.gp.l[0];
  p_boc2 = system->reax_param.gp.l[1];

 /* Calculate Deltaprime, Deltaprime_boc values */
  for( i = 0; i < system->N; ++i ) {
    type_i = system->my_atoms[i].type;
    if (type_i < 0) continue;
    sbp_i = &(system->reax_param.sbp[type_i]);
    workspace->Deltap[i] = workspace->total_bond_order[i] - sbp_i->valency;
    workspace->Deltap_boc[i] =
      workspace->total_bond_order[i] - sbp_i->valency_val;

    workspace->total_bond_order[i] = 0;
  }

  /* Corrected Bond Order calculations */
  for( i = 0; i < system->N; ++i ) {
    type_i = system->my_atoms[i].type;
    if (type_i < 0) continue;
    sbp_i = &(system->reax_param.sbp[type_i]);
    val_i = sbp_i->valency;
    Deltap_i = workspace->Deltap[i];
    Deltap_boc_i = workspace->Deltap_boc[i];
    start_i = Start_Index(i, bonds);
    end_i = End_Index(i, bonds);

    for( pj = start_i; pj < end_i; ++pj ) {
      j = bonds->select.bond_list[pj].nbr;
      type_j = system->my_atoms[j].type;
      if (type_j < 0) continue;
      bo_ij = &( bonds->select.bond_list[pj].bo_data );
      // fprintf( stderr, "\tj:%d - ubo: %8.3f\n", j+1, bo_ij->BO );

      if( i < j || workspace->bond_mark[j] > 3 ) {
        twbp = &( system->reax_param.tbp[type_i][type_j] );

        if( twbp->ovc < 0.001 && twbp->v13cor < 0.001 ) {
          bo_ij->C1dbo = 1.000000;
          bo_ij->C2dbo = 0.000000;
          bo_ij->C3dbo = 0.000000;

          bo_ij->C1dbopi = bo_ij->BO_pi;
          bo_ij->C2dbopi = 0.000000;
          bo_ij->C3dbopi = 0.000000;
          bo_ij->C4dbopi = 0.000000;

          bo_ij->C1dbopi2 = bo_ij->BO_pi2;
          bo_ij->C2dbopi2 = 0.000000;
          bo_ij->C3dbopi2 = 0.000000;
          bo_ij->C4dbopi2 = 0.000000;

        }
        else {
          val_j = system->reax_param.sbp[type_j].valency;
          Deltap_j = workspace->Deltap[j];
          Deltap_boc_j = workspace->Deltap_boc[j];

          /* on page 1 */
          if( twbp->ovc >= 0.001 ) {
            /* Correction for overcoordination */
            exp_p1i = exp( -p_boc1 * Deltap_i );
            exp_p2i = exp( -p_boc2 * Deltap_i );
            exp_p1j = exp( -p_boc1 * Deltap_j );
            exp_p2j = exp( -p_boc2 * Deltap_j );

            f2 = exp_p1i + exp_p1j;
            f3 = -1.0 / p_boc2 * log( 0.5 * ( exp_p2i  + exp_p2j ) );
            f1 = 0.5 * ( ( val_i + f2 )/( val_i + f2 + f3 ) +
                         ( val_j + f2 )/( val_j + f2 + f3 ) );

            temp = f2 + f3;
            u1_ij = val_i + temp;
            u1_ji = val_j + temp;
            Cf1A_ij = 0.5 * f3 * (1.0 / SQR( u1_ij ) +
                                  1.0 / SQR( u1_ji ));
            Cf1B_ij = -0.5 * (( u1_ij - f3 ) / SQR( u1_ij ) +
                              ( u1_ji - f3 ) / SQR( u1_ji ));

            Cf1_ij = 0.50 * ( -p_boc1 * exp_p1i / u1_ij -
                              ((val_i+f2) / SQR(u1_ij)) *
                              ( -p_boc1 * exp_p1i +
                                exp_p2i / ( exp_p2i + exp_p2j ) ) +
                              -p_boc1 * exp_p1i / u1_ji -
                              ((val_j+f2) / SQR(u1_ji)) *
                              ( -p_boc1 * exp_p1i +
                                exp_p2i / ( exp_p2i + exp_p2j ) ));


            Cf1_ji = -Cf1A_ij * p_boc1 * exp_p1j +
              Cf1B_ij * exp_p2j / ( exp_p2i + exp_p2j );

          }
          else {
            /* No overcoordination correction! */
            f1 = 1.0;
            Cf1_ij = Cf1_ji = 0.0;
          }

          if( twbp->v13cor >= 0.001 ) {
            /* Correction for 1-3 bond orders */
            exp_f4 =exp(-(twbp->p_boc4 * SQR( bo_ij->BO ) -
                          Deltap_boc_i) * twbp->p_boc3 + twbp->p_boc5);
            exp_f5 =exp(-(twbp->p_boc4 * SQR( bo_ij->BO ) -
                          Deltap_boc_j) * twbp->p_boc3 + twbp->p_boc5);

            f4 = 1. / (1. + exp_f4);
            f5 = 1. / (1. + exp_f5);
            f4f5 = f4 * f5;

            /* Bond Order pages 8-9, derivative of f4 and f5 */
            Cf45_ij = -f4 * exp_f4;
            Cf45_ji = -f5 * exp_f5;
          }
          else {
            f4 = f5 = f4f5 = 1.0;
            Cf45_ij = Cf45_ji = 0.0;
          }

          /* Bond Order page 10, derivative of total bond order */
          A0_ij = f1 * f4f5;
          A1_ij = -2 * twbp->p_boc3 * twbp->p_boc4 * bo_ij->BO *
            (Cf45_ij + Cf45_ji);
          A2_ij = Cf1_ij / f1 + twbp->p_boc3 * Cf45_ij;
          A2_ji = Cf1_ji / f1 + twbp->p_boc3 * Cf45_ji;
          A3_ij = A2_ij + Cf1_ij / f1;
          A3_ji = A2_ji + Cf1_ji / f1;

          /* find corrected bond orders and their derivative coef */
          bo_ij->BO    = bo_ij->BO    * A0_ij;
          bo_ij->BO_pi = bo_ij->BO_pi * A0_ij *f1;
          bo_ij->BO_pi2= bo_ij->BO_pi2* A0_ij *f1;
          bo_ij->BO_s  = bo_ij->BO - ( bo_ij->BO_pi + bo_ij->BO_pi2 );

          bo_ij->C1dbo = A0_ij + bo_ij->BO * A1_ij;
          bo_ij->C2dbo = bo_ij->BO * A2_ij;
          bo_ij->C3dbo = bo_ij->BO * A2_ji;

          bo_ij->C1dbopi = f1*f1*f4*f5;
          bo_ij->C2dbopi = bo_ij->BO_pi * A1_ij;
          bo_ij->C3dbopi = bo_ij->BO_pi * A3_ij;
          bo_ij->C4dbopi = bo_ij->BO_pi * A3_ji;

          bo_ij->C1dbopi2 = f1*f1*f4*f5;
          bo_ij->C2dbopi2 = bo_ij->BO_pi2 * A1_ij;
          bo_ij->C3dbopi2 = bo_ij->BO_pi2 * A3_ij;
          bo_ij->C4dbopi2 = bo_ij->BO_pi2 * A3_ji;

        }

        /* neglect bonds that are < 1e-10 */
        if( bo_ij->BO < 1e-10 )
          bo_ij->BO = 0.0;
        if( bo_ij->BO_s < 1e-10 )
          bo_ij->BO_s = 0.0;
        if( bo_ij->BO_pi < 1e-10 )
          bo_ij->BO_pi = 0.0;
        if( bo_ij->BO_pi2 < 1e-10 )
          bo_ij->BO_pi2 = 0.0;

        workspace->total_bond_order[i] += bo_ij->BO; //now keeps total_BO

      }
      else {
        /* We only need to update bond orders from bo_ji
           everything else is set in uncorrected_bo calculations */
        sym_index = bonds->select.bond_list[pj].sym_index;
        bo_ji = &(bonds->select.bond_list[ sym_index ].bo_data);
        bo_ij->BO = bo_ji->BO;
        bo_ij->BO_s = bo_ji->BO_s;
        bo_ij->BO_pi = bo_ji->BO_pi;
        bo_ij->BO_pi2 = bo_ji->BO_pi2;

        workspace->total_bond_order[i] += bo_ij->BO;// now keeps total_BO
      }
    }

  }

  p_lp1 = system->reax_param.gp.l[15];
  for( j = 0; j < system->N; ++j ){
    type_j = system->my_atoms[j].type;
    if (type_j < 0) continue;
    sbp_j = &(system->reax_param.sbp[ type_j ]);

    workspace->Delta[j] = workspace->total_bond_order[j] - sbp_j->valency;
    workspace->Delta_e[j] = workspace->total_bond_order[j] - sbp_j->valency_e;
    workspace->Delta_boc[j] = workspace->total_bond_order[j] -
      sbp_j->valency_boc;
    workspace->Delta_val[j] = workspace->total_bond_order[j] -
      sbp_j->valency_val;

    workspace->vlpex[j] = workspace->Delta_e[j] -
      2.0 * (int)(workspace->Delta_e[j]/2.0);
    explp1 = exp(-p_lp1 * SQR(2.0 + workspace->vlpex[j]));
    workspace->nlp[j] = explp1 - (int)(workspace->Delta_e[j] / 2.0);
    workspace->Delta_lp[j] = sbp_j->nlp_opt - workspace->nlp[j];
    workspace->Clp[j] = 2.0 * p_lp1 * explp1 * (2.0 + workspace->vlpex[j]);
    workspace->dDelta_lp[j] = workspace->Clp[j];

    if( sbp_j->mass > 21.0 ) {
      workspace->nlp_temp[j] = 0.5 * (sbp_j->valency_e - sbp_j->valency);
      workspace->Delta_lp_temp[j] = sbp_j->nlp_opt - workspace->nlp_temp[j];
      workspace->dDelta_lp_temp[j] = 0.;
    }
    else {
      workspace->nlp_temp[j] = workspace->nlp[j];
      workspace->Delta_lp_temp[j] = sbp_j->nlp_opt - workspace->nlp_temp[j];
      workspace->dDelta_lp_temp[j] = workspace->Clp[j];
    }

  }

}
